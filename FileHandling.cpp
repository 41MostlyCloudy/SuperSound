#pragma once


#include "GlobalVariables.h"
#include "FrameHandling.cpp"
#include "SongHandling.cpp"

// For searching file directories. New as of c++ 17.
#include <filesystem>




#include <cstdlib>






void SaveSong(); // Save the currently loaded song.

void LoadSong(std::string name); // Load the song file with the given name.

void LoadGUIThemes();

//void SwapLightDarkMode();

void SaveSettings(); // Save the setting for the SuperSound program.

void LoadSettings(); // Load the setting for the SuperSound program.




void SaveSong() // Save the currently loaded song.
{
    if (!loadedSong.unusedFrames)
    {
        // Check for unused frames.
        std::vector <bool> frameUsed;
        for (int i = 0; i < loadedSong.frames.size(); i++)
            frameUsed.emplace_back(false);

        for (int i = 0; i < loadedSong.frameSequence.size(); i++)
        {
            frameUsed[loadedSong.frameSequence[i]] = true;
        }


        for (int i = 0; i < frameUsed.size(); i++)
        {
            if (frameUsed[i] == false)
                loadedSong.unusedFrames = true;
        }

        if (loadedSong.unusedFrames)
        {
            windowController.InitializeWindow("Delete Frames", { 22, 17 }, { 20, 16 });
            return;
        }
    }
    else
        loadedSong.unusedFrames = false;

    std::string songFilePath = "C:/" + fileNavigator.currentFilePath.std::filesystem::path::string() + "/";

    

    // See if any songs have the same name.
    if (!loadedSong.overWriteOldSong)
    {
        //std::string pathName = "C:/" + fileNavigator.currentFilePath.std::filesystem::path::string();

        //std::string songName = fileNavigator.fileNames[editor.selectedFile];
        //songName.erase(0, 1);
        
        for (int name = 0; name < fileNavigator.fileNames.size(); name++)
        {
            std::string indexName = fileNavigator.fileNames[name];
            indexName.erase(0, 1);
            if (indexName == loadedSong.songName + ".song")
            {
                windowController.InitializeWindow("Overwrite Song", { 22, 17 }, { 20, 16 });
                return;
            }
        }
    }




    loadedSong.unsavedChanges = false;

    // Song file format.

    // Samples:
    // 1 byte: # of samples.
    // For each sample:
    //      1 byte: Length of name.
    //      For each character:
    //          1 byte value.

    // Song Body:
    //      1 byte: # Of channels.
    //      1 byte: BPM
    //      1 byte: TPB
    //      1 byte: # Of frames in frame sequence.
    //      For each index in the frame sequence:
    //          1 byte: Frame index number.
    //      1 byte: # Of frames in song.
    //      For each frame:
    //          1 byte: # of rows.
    //          1 byte: beats per measure.
    //          For each channel:
    //              1 byte: # of notes.
    //              For each note:
    //                  1 byte: note value.
    //              1 byte: # of volumes.
    //              For each volume:
    //                  1 byte: volume value.
    //              1 byte: # of effects.
    //              For each effect:
    //                  1 byte: note effect.

    

    saveCurrentFrame();

    std::ofstream songFile(songFilePath + loadedSong.songName + ".song", std::ios::binary | std::ios::out);


    if (songFile.is_open())
    {
        // Artist Name
        std::string artistName = loadedSong.artistName;

        uint8_t artistNameNum = artistName.length();
        songFile.write((char*)&artistNameNum, 1);

        for (int j = 0; j < artistName.length(); j++)
        {
            uint8_t artistChar = artistName.at(j);
            songFile.write((char*)&artistChar, 1);
        }


        // Samples
        uint8_t sampleNum = loadedSamples.size();
        songFile.write((char*)&sampleNum, 1); // # of samples.
        
        for (int i = 0; i < loadedSamples.size(); i++) // Sample
        {
            std::string sampleName = loadedSamples[i].sampleName;

            uint8_t sampleNameNum = sampleName.length();
            songFile.write((char*)&sampleNameNum, 1);

            for (int j = 0; j < sampleName.length(); j++)
            {
                uint8_t sampleChar = sampleName.at(j);
                songFile.write((char*)&sampleChar, 1);
            }

            std::string samplePath = loadedSamples[i].filePath;

            uint8_t samplePathNum = samplePath.length();
            songFile.write((char*)&samplePathNum, 1);

            for (int j = 0; j < samplePath.length(); j++)
            {
                uint8_t sampleChar = samplePath.at(j);
                songFile.write((char*)&sampleChar, 1);
            }

            uint64_t sampleDataLen = loadedSamples[i].sampleData.size();
            songFile.write((char*)&sampleDataLen, 8);

            for (int j = 0; j < sampleDataLen; j++)
            {
                uint8_t sampleChar = loadedSamples[i].sampleData[j];
                songFile.write((char*)&sampleChar, 1);
            }
        }

        
        
        
       

        // Song Body
        uint8_t channelCount = int(loadedSong.numberOfChannels);
        songFile.write((char*)&channelCount, 1);
        for (int ch = 0; ch < loadedSong.numberOfChannels; ch++)
        {
            uint8_t compressed = 0;
            if (channels[ch].compressed)
                compressed = 1;
            songFile.write((char*)&compressed, 1);
        }

        uint8_t bpm = int(loadedSong.startingBPM);
        songFile.write((char*)&bpm, 1);
        uint8_t tpb = int(loadedSong.startingTPB);
        songFile.write((char*)&tpb, 1);
        uint8_t seqSize = loadedSong.frameSequence.size();
        songFile.write((char*)&seqSize, 1);

        for (int i = 0; i < loadedSong.frameSequence.size(); i++) // Frame sequence.
        {
            uint8_t seqId = loadedSong.frameSequence[i];
            songFile.write((char*)&seqId, 1);
        }


        uint8_t frameNum = loadedSong.frames.size();
        songFile.write((char*)&frameNum, 1);

        for (int i = 0; i < loadedSong.frames.size(); i++) // Frame.
        {
            uint8_t frameRows = loadedSong.frames[i].rows;
            songFile.write((char*)&frameRows, 1);

            uint8_t frameMeasureBeats = loadedSong.frames[i].beatsPerMeasure;
            songFile.write((char*)&frameMeasureBeats, 1);

            for (int ch = 0; ch < loadedSong.numberOfChannels; ch++)
            {
                uint8_t noteSize = loadedSong.frames[i].channels[ch].notes.size();
                songFile.write((char*)&noteSize, 1);
                for (int j = 0; j < noteSize; j++)
                {
                    uint8_t note = loadedSong.frames[i].channels[ch].notes[j];
                    songFile.write((char*)&note, 1);
                }

                uint8_t volumeSize = loadedSong.frames[i].channels[ch].volumes.size();
                songFile.write((char*)&volumeSize, 1);
                for (int j = 0; j < volumeSize; j++)
                {
                    uint8_t volume = loadedSong.frames[i].channels[ch].volumes[j];
                    songFile.write((char*)&volume, 1);
                }

                uint8_t effectSize = loadedSong.frames[i].channels[ch].effects.size();
                songFile.write((char*)&effectSize, 1);
                for (int j = 0; j < effectSize; j++)
                {
                    uint8_t effect = loadedSong.frames[i].channels[ch].effects[j];
                    songFile.write((char*)&effect, 1);
                }
            }
        }

        songFile.close();
    }

    

    return;
}



void LoadSong(std::string name) // Load the song file with the given name.
{
    if (loadedSong.unsavedChanges)
    {
        editor.fileToLoad = name;
        windowController.InitializeWindow("Save and Load", { 22, 17 }, { 20, 16 });
        //promptWind.AskSaveBeforeLoadingSong();
        return;
    }
    

    editor.playingSong = false;
    loadedSong.currentFrame = 0;
    loadedSong.currentNote = 0;

    std::ifstream songFile("C:/" + fileNavigator.currentFilePath.std::filesystem::path::string() + "/" + name, std::ios::binary | std::ios::in);

    name.erase(name.length() - 5, 6);
    loadedSong.songName = name;

    if (songFile.is_open())
    {
        // Artist Name
        //std::string artistName = loadedSong.artistName;

        uint8_t artistNameNum;
        songFile.read((char*)&artistNameNum, 1);

        std::string artistName = "";

        for (int j = 0; j < artistNameNum; j++)
        {
            //uint8_t artistChar = artistName.at(j);
            //songFile.write((char*)&artistChar, 1);
            uint8_t newChar;
            songFile.read((char*)&newChar, 1);
            artistName = artistName + char(newChar);
        }
        loadedSong.artistName = artistName;


        // Samples
        uint8_t sampleSize;
        songFile.read((char*)&sampleSize, 1);

        loadedSamples.clear();

        for (int i = 0; i < sampleSize; i++)
        {
            Sample newSample;
            newSample.sampleName = "";

            uint8_t nameSize;
            songFile.read((char*)&nameSize, 1);

            for (int j = 0; j < nameSize; j++)
            {
                uint8_t newChar;
                songFile.read((char*)&newChar, 1);
                newSample.sampleName = newSample.sampleName + char(newChar);
            }

            newSample.filePath = "";

            uint8_t pathSize;
            songFile.read((char*)&pathSize, 1);

            for (int j = 0; j < pathSize; j++)
            {
                uint8_t newChar;
                songFile.read((char*)&newChar, 1);
                newSample.filePath = newSample.filePath + char(newChar);
            }



            uint64_t sampleDataLen;
            songFile.read((char*)&sampleDataLen, 8);
            newSample.sampleData.resize(sampleDataLen);

            for (int j = 0; j < sampleDataLen; j++)
            {
                uint8_t sampleChar;
                songFile.read((char*)&sampleChar, 1);
                newSample.sampleData[j] = sampleChar;
            }

            loadedSamples.emplace_back(newSample);
        }


        


        

        // Song Body

        uint8_t channelCount;
        songFile.read((char*)&channelCount, 1);
        ResizeDecoders(channelCount); // Resize decoder array to match channels.

        for (int ch = 0; ch < loadedSong.numberOfChannels; ch++)
        {
            uint8_t compressed;
            songFile.read((char*)&compressed, 1);
            if (compressed == 0)
                channels[ch].compressed = false;
            else
                channels[ch].compressed = true;
        }

        uint8_t bpm;
        songFile.read((char*)&bpm, 1);
        loadedSong.startingBPM = bpm;
        loadedSong.bpm = loadedSong.startingBPM;
        uint8_t tpb;
        songFile.read((char*)&tpb, 1);
        loadedSong.startingTPB = tpb;
        loadedSong.ticksPerBeat = loadedSong.startingTPB;

        // Frame sequence
        uint8_t frameSeqNum;
        songFile.read((char*)&frameSeqNum, 1);
        loadedSong.frameSequence.clear();
        loadedSong.frameSequence.resize(frameSeqNum);
        for (int i = 0; i < frameSeqNum; i++)
        {
            songFile.read((char*)&loadedSong.frameSequence[i], 1);
        }

        // Frames
        uint8_t frameNum;
        songFile.read((char*)&frameNum, 1);
        loadedSong.frames.clear();
        loadedSong.frames.resize(frameNum);
        for (int i = 0; i < frameNum; i++)
        {
            loadedSong.frames[i].channels.resize(loadedSong.numberOfChannels); // Resize frame to match the number of channels.

            uint8_t rows;
            songFile.read((char*)&rows, 1);
            loadedSong.frames[i].rows = rows;

            uint8_t measureBeats;
            songFile.read((char*)&measureBeats, 1);
            loadedSong.frames[i].beatsPerMeasure = measureBeats;

            for (int ch = 0; ch < loadedSong.numberOfChannels; ch++)
            {
                uint8_t noteNum;
                songFile.read((char*)&noteNum, 1);
                loadedSong.frames[i].channels[ch].notes.clear();
                loadedSong.frames[i].channels[ch].notes.resize(noteNum);
                for (int j = 0; j < noteNum; j++)
                {
                    uint8_t note;
                    songFile.read((char*)&note, 1);
                    loadedSong.frames[i].channels[ch].notes[j] = note;
                }

                uint8_t volumeNum;
                songFile.read((char*)&volumeNum, 1);
                loadedSong.frames[i].channels[ch].volumes.clear();
                loadedSong.frames[i].channels[ch].volumes.resize(volumeNum);
                for (int j = 0; j < volumeNum; j++)
                {
                    uint8_t volume;
                    songFile.read((char*)&volume, 1);
                    loadedSong.frames[i].channels[ch].volumes[j] = volume;
                }

                uint8_t effectNum;
                songFile.read((char*)&effectNum, 1);
                loadedSong.frames[i].channels[ch].effects.clear();
                loadedSong.frames[i].channels[ch].effects.resize(effectNum);
                for (int j = 0; j < effectNum; j++)
                {
                    uint8_t effect;
                    songFile.read((char*)&effect, 1);
                    loadedSong.frames[i].channels[ch].effects[j] = effect;
                }
            }
        }

        songFile.close();
    }
    else // Create new song.
    {
        loadedSamples.clear();
        loadedSong.artistName = "Me";
        loadedSong.bpm = 120;
        loadedSong.ticksPerBeat = 6;
        loadedSong.frameSequence.clear();
        loadedSong.frames.clear();
        loadedSong.numberOfChannels = 1;

        // Initialize channels.
        channels.resize(loadedSong.numberOfChannels);
        // Initialize decoders.
        ResizeDecoders(loadedSong.numberOfChannels);

        Frame firstFrame;
        firstFrame.channels.resize(loadedSong.numberOfChannels);
        loadedSong.frames.emplace_back(firstFrame);
        loadedSong.frameSequence.emplace_back(0);


        loadedFrame = resizeUnrolledFrameRows(loadedFrame, 32);

        loadCurrentFrame();
        saveCurrentFrame();
    }


    loadedSong.currentFrame = 0; // Current frame in frameSequence.
    loadedSong.currentNote = 0;
    loadedSong.timeInNote = 0;
    loadedSong.timeInTick = 0;
    loadedSong.timeInSong = 0;


    for (int i = 0; i < loadedSong.numberOfChannels; i++)
    {
        loadedSong.toNextChannelNote[i] = 0;
        loadedSong.toNextChannelVolume[i] = 0;
        loadedSong.toNextChannelEffect[i] = 0;
    }

    // Load the sample data
    //LoadSampleData();


    loadCurrentFrame();


    toDrawSampleDisplay = true;

    return;
}





void LoadGUIThemes()
{
    int sizeX, sizeY, comps;
    unsigned char* data = stbi_load("ThemeColors.png", &sizeX, &sizeY, &comps, 3);

    if (data)
    {
        gui.themes.clear();

        for (int y = 0; y < sizeY; y++)
        {
            GUITheme newTheme;
            for (int x = 0; x < sizeX; x++)
            {
                unsigned char* pixelOffset = data + ((9 * y) + x) * 3;
                //if (gui.uiBrightMode)
                //    newTheme.uiColors[8 - x] = { pixelOffset[0], pixelOffset[1], pixelOffset[2] };
                //else
                newTheme.uiColors[x] = { pixelOffset[0], pixelOffset[1], pixelOffset[2] };
            }
            
            

            gui.themes.emplace_back(newTheme);
        }
    }

    

    stbi_image_free(data);

    return;
}

/*
void SwapLightDarkMode()
{
    gui.uiBrightMode = !gui.uiBrightMode;
    LoadGUIThemes();
    SaveSettings();

    return;
}*/


void SaveSettings()
{
    std::ofstream settingsFile("SuperSound Settings.bin", std::ios::binary | std::ios::out);

    if (settingsFile.is_open())
    {
        // Samples
        uint8_t themeNum = gui.uiColorTheme;
        settingsFile.write((char*)&themeNum, 1); // Theme number.

        uint8_t chFoc = editor.channelFocus;
        settingsFile.write((char*)&chFoc, 1); // Channel Focus.

        uint8_t piano = gui.pianoMode;
        settingsFile.write((char*)&piano, 1); // Piano Mode.

        settingsFile.close();
    }

    return;
}


void LoadSettings()
{
    std::ifstream settingsFile("SuperSound Settings.bin", std::ios::binary | std::ios::out);

    if (settingsFile.is_open())
    {
        uint8_t themeNum;
        settingsFile.read((char*)&themeNum, 1); // Theme number.
        gui.uiColorTheme = themeNum;

        uint8_t chFoc;
        settingsFile.read((char*)&chFoc, 1); // Channel Focus.
        editor.channelFocus = bool(chFoc);

        uint8_t piano;
        settingsFile.read((char*)&piano, 1); // Piano Mode.
        gui.pianoMode = bool(piano);

        settingsFile.close();
    }

    return;
}





