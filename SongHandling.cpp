#pragma once


#include "GlobalVariables.h"










// Pitch lookup table
// To get different notes, the pitch of the sample is shifted. Samples should be a 'C4' note by default, and the pitch is scaled around that.
float pitchTable[85] =
{
    0.05, // C1

    0.066,
    0.07,
    0.074,
    0.0787,
    0.0834,
    0.0884,
    0.0936,
    0.0992,
    0.105,
    0.111,
    0.118,
    0.125, // C1

    0.132,
    0.14,
    0.149,
    0.157,
    0.167,
    0.177,
    0.187,
    0.198,
    0.21,
    0.223,
    0.236,
    0.25, // C2

    0.265,
    0.28,
    0.297,
    0.315,
    0.334,
    0.353,
    0.375,
    0.4,
    0.42,
    0.4454,
    0.472,
    0.5, // C3

    0.53,
    0.56,
    0.595,
    0.63,
    0.667,
    0.707,
    0.75,
    0.794,
    0.84,
    0.89,
    0.944,
    1.0, // C4

    1.06,
    1.122,
    1.19,
    1.26,
    1.335,
    1.414,
    1.5,
    1.587,
    1.68,
    1.78,
    1.888,
    2.0, // C5

    2.119,
    2.245,
    2.378,
    2.52,
    2.67,
    2.828,
    2.997,
    3.1748,
    3.364,
    3.564,
    3.775,
    4.0, // C6

    4.238,
    4.4897,
    4.757,
    5.04,
    5.34,
    5.66,
    5.99,
    6.35,
    6.727,
    7.127,
    7.55,
    8.0 // C7
};



void SetUpAudioEngine();

void StartSample(int channel, int sampleNumber, float pitch, int startFrame);

void StopSample(int channel);

void stepSongInCallback(float elapsedMS);

void stepSample(float elapsedMS);

void InitializeSample(int channel, int sampleNumber, float pitch);

void ResizeDecoders(int newSize); // When a new song is loaded, decoders need to be created or destroyed to match the number of channels.

void RecordSong();

void StartOrStopSong();

void DrawSampleDisplay();

void GenerateAdditiveWave();

std::vector <float> ConstructWave(int waveType, std::vector <float> modulator, float frequencies[30], float periodLen, float framesToWrite, float modPower);









////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// New Audio



ma_uint32 read_and_mix_pcm_frames_f32(ma_decoder* pDecoder, float* pOutputF32, float frameCount, int channel, float betweenFrames, ma_uint64 offset);

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);


// Decoders
std::vector <ma_decoder> g_pDecoders;
std::vector <bool> channelInitialized = { false };
ma_decoder_config decoderConfig;

//ma_decoder_config decoderConfig;
//std::vector <bool> channelInitialized = { false };

//std::vector <ma_audio_buffer> g_pBuffers;
//ma_audio_buffer_config bufferConfig;


// Device
ma_device_config deviceConfig;
ma_device device;


// Encoder
ma_encoder_config encoderConfig;
ma_encoder encoder;

bool toGenerateAdditiveWave = false;
bool toDrawSampleDisplay = false;






// Samples
//std::vector <void*> sampleAudioData;
//std::vector <float> sampleAudioData;
//std::vector <size_t> sampleSizes;

//void* pAudioData = ...;
//float pOut[7700] = { 0.0f };

//float* pOutputF32 = (float*)pOut;


//std::vector <ma_resource_manager_data_source> sampleData;




// The speed at which a note stops by fading out.
//float decoderNoteStopSpeed = 1.0f;




ma_uint32 read_and_mix_pcm_frames_f32(ma_decoder* pDecoder, float* pOutputF32, float frameCount, int channel, float betweenFrames, ma_uint64 offset)
{   
    //The way mixing works is that we just read into a temporary buffer, then take the contents of that buffer and mix it with the
    //contents of the output buffer by simply adding the samples together. You could also clip the samples to -1..+1, but I'm not
    //doing that in this example.
    float temp[7700];
    //float tempPitched[7700];
    ma_uint32 tempCapInFrames = ma_countof(temp) / 2;
    ma_uint32 totalFramesRead = 0;
    float leftoverFrame = channels[channel].startBetweenFrames;


    //std::cout << " start -> ";

    // Apply resampling before mixing.

    // Lowpass filter
    float alpha = channels[channel].alphaLow;
    float alpha2 = channels[channel].alphaHigh;
    //alpha = alpha / (alpha + 1.0f); // Simplified smoothing factor


    //float alpha2 = 2.0f * 3.14159265f * channels[channel].highPass / 48000.0f;
    //alpha2 = alpha2 / (alpha2 + 1.0f); // Simplified smoothing factor


    //float RC = 1.0f / (2.0f * 3.14159265f * channels[channel].highPass);
    //float dt = 1.0f / 48000.0f;
    //float alpha2 = RC / (RC + dt);



    //float RC = 1.0f / (2.0f * 3.14159265f * channels[channel].highPass);
    //float dt = 1.0f / 48000.0f;
    //float alpha2 = RC / (RC + dt);

    
    float frameResolution = int(frameCount) / frameCount;
    float scaledPitch = channels[channel].pitch + channels[channel].pitchWaveOffset;

    if (scaledPitch > 8)
        scaledPitch = 8;
    else if (scaledPitch < 0.05)
        scaledPitch = 0.05;

    scaledPitch *= frameResolution;


    float avgVolL = 0.0f;
    float avgVolR = 0.0f;
    

    // Apply Low-pass filter before mixing.

    // Apply High-Pass filter before mixing.
    

    while (totalFramesRead < ma_uint32(frameCount)) // While there are frames to read.
    {
        ma_uint64 framesReadThisIteration = 0;
        ma_uint32 totalFramesRemaining = frameCount - totalFramesRead;
        ma_uint32 framesToReadThisIteration = tempCapInFrames;


        if (framesToReadThisIteration > totalFramesRemaining) // Clamp the number of frames to read to the number of frames left.
        {
            framesToReadThisIteration = totalFramesRemaining;
        }

        // Set position for reversing.
        /*
        ma_uint64 soundPos1;
        ma_decoder_get_cursor_in_pcm_frames(pDecoder, &soundPos1);
        soundPos1 -= framesToReadThisIteration;
        if (soundPos1 < 0)
        {
            framesToReadThisIteration += soundPos1;
            soundPos1 = 0;
        }
        ma_decoder_seek_to_pcm_frame(pDecoder, soundPos1);
        */

        
        ma_decoder_read_pcm_frames(pDecoder, temp, framesToReadThisIteration, &framesReadThisIteration); // Read new frames to temp.


        if (loadedSamples[channels[channel].instrument].looping) // Loop
        {
            
            // Loop if at end of sample.
            while (framesReadThisIteration < framesToReadThisIteration)
            {
                ma_uint64 newFramesToRead = framesToReadThisIteration - framesReadThisIteration;
                ma_uint64 newFramesRead;
                ma_decoder_seek_to_pcm_frame(pDecoder, 0);
                float temp2[7700];
                ma_decoder_read_pcm_frames(pDecoder, temp2, newFramesToRead, &newFramesRead); // Read new frames to temp.
                for (int i = 0; i < newFramesRead * 2; i++)
                    temp[framesReadThisIteration * 2 + i] = temp2[i];
                framesReadThisIteration += newFramesRead;
            }
            /*
            else
            {
                // Loop if at end of loop point.
                ma_uint64 soundPos1;
                ma_decoder_get_cursor_in_pcm_frames(pDecoder, &soundPos1);
                if (soundPos1 > loadedSamples[channels[channel].instrument].loopEnd)
                {
                    int framesPastLoop = soundPos1 - loadedSamples[channels[channel].instrument].loopEnd;
                    ma_uint64 newPos = loadedSamples[channels[channel].instrument].loopStart + framesPastLoop;
                    ma_decoder_seek_to_pcm_frame(pDecoder, newPos);
                }
            }*/
        }


        

        if (framesReadThisIteration == 0)
        {
            return totalFramesRead;
        }
    

        //if (loadedSamples[editor.selectedSample].looping)
        //{
        //    if (x > loopStartPos && x < loopEndPos)
        //        brightness *= 0.25f;
        //}
    

        


        // Set again for reverse effect.
        //ma_decoder_seek_to_pcm_frame(pDecoder, soundPos1);
        
        
        float fr = float(framesReadThisIteration);

        // Reverse newly-read frames for the reverse effect.
        /*
        float temp2[7700];
        for (ma_uint32 i = 0; i < fr; i++)
        {
            temp2[i * 2] = temp[i * 2];
            temp2[i * 2 + 1] = temp[i * 2 + 1];

            temp[i * 2] = temp2[ma_uint32(fr - 1 - i) * 2];
            temp[i * 2 + 1] = temp2[ma_uint32(fr - 1 - i) * 2 + 1];
        }*/



        /////////////////////////////////////////////////


        float volumeLeft = 0;
        float volumeRight = 0;

        // Panning
        float pan = channels[channel].panValue * 2;
        volumeLeft += 1.0f + pan + channels[channel].volumeWaveOffset;
        volumeRight += 1.0f - pan + channels[channel].volumeWaveOffset;

        if (volumeLeft > 2.0f)
            volumeLeft = 2.0f;
        if (volumeRight > 2.0f)
            volumeRight = 2.0f;

        if (volumeLeft < 0.0f)
            volumeLeft = 0.0f;
        if (volumeRight < 0.0f)
            volumeRight = 0.0f;

        float totalVol = channels[channel].volume * channels[channel].envelopeVolume;
        if (totalVol < 0.0f)
            totalVol = 0.0f;
        else if (totalVol > 1.0f)
            totalVol = 1.0f;

        volumeLeft *= totalVol;
        volumeRight *= totalVol;

        

        
        //fr *= scaledPitch;

        for (ma_uint32 i = 0; i < fr; i++)
        {
            temp[i * 2] = temp[i * 2] * volumeLeft;
            temp[i * 2 + 1] = temp[i * 2 + 1] * volumeRight;
        }

        
        // Highpass
        /*
        for (ma_uint32 i = 0; i < fr; i++)
        {
            float input = temp[(i + totalFramesRead) * 2];
            float output = alpha2 * (channels[channel].prevHighPassSampleL + input - channels[channel].prevHighPassSampleLI);
            temp[(i + totalFramesRead) * 2] = output;
            channels[channel].prevHighPassSampleLI = input;
            channels[channel].prevHighPassSampleL = output;


            input = temp[(i + totalFramesRead) * 2 + 1];
            output = alpha2 * (channels[channel].prevHighPassSampleR + input - channels[channel].prevHighPassSampleRI);
            temp[(i + totalFramesRead) * 2 + 1] = output;
            channels[channel].prevHighPassSampleRI = input;
            channels[channel].prevHighPassSampleR = output;


            //if ((i + totalFramesRead) * 2 > 1)
            //{
                //channels[channel].prevHighPassSampleL = alpha2 * (channels[channel].prevHighPassSampleL + temp[(i + totalFramesRead) * 2] - temp[((i + totalFramesRead) * 2) - 2]);
                //temp[(i + totalFramesRead) * 2] = channels[channel].prevHighPassSampleL;

                //channels[channel].prevHighPassSampleR = alpha2 * (channels[channel].prevHighPassSampleR + temp[(i + totalFramesRead) * 2 + 1] - temp[((i + totalFramesRead) * 2) - 1]);
                //temp[((i + totalFramesRead) * 2) + 1] = channels[channel].prevHighPassSampleR;

            //}

        }*/


        float framesRead = float(framesReadThisIteration);

        if (scaledPitch > 1.0f)
            framesRead = framesRead / scaledPitch;


        


        float resampleIndex = leftoverFrame;
        for (ma_uint32 iSample = 0; iSample < framesRead; iSample += channels[channel].bitCrush + 1)
        {
            for (ma_uint32 x = 0; x < channels[channel].bitCrush + 1; x++)
            {
                if (iSample + x >= framesRead)
                    break;

                // Resample using linear interpolation
                ma_uint32 index1 = resampleIndex;
                ma_uint32 index2 = index1 + 1;


                
                


                float t = resampleIndex - index1;  // Fractional part
                float addSound = temp[index1 * 2] * (1.0f - t) + temp[index2 * 2] * t;
                float oldInput = 0;

                // Lowpass
                channels[channel].prevLowPassSampleL = alpha * addSound + (1.0f - alpha) * channels[channel].prevLowPassSampleL;
                addSound = channels[channel].prevLowPassSampleL;

                // High-pass
                oldInput = addSound;
                addSound = alpha2 * (channels[channel].prevHighPassSampleL + addSound - channels[channel].prevHighPassSampleLI);
                channels[channel].prevHighPassSampleLI = oldInput;
                channels[channel].prevHighPassSampleL = addSound;

                avgVolL += abs(addSound);
                pOutputF32[(iSample + x + totalFramesRead + offset) * 2] += addSound;
                addSound = temp[(index1 * 2) + 1] * (1.0f - t) + temp[(index2 * 2) + 1] * t;

                // Lowpass
                channels[channel].prevLowPassSampleR = alpha * addSound + (1.0f - alpha) * channels[channel].prevLowPassSampleR;
                addSound = channels[channel].prevLowPassSampleR;

                // High-pass
                oldInput = addSound;
                addSound = alpha2 * (channels[channel].prevHighPassSampleR + addSound - channels[channel].prevHighPassSampleRI);
                channels[channel].prevHighPassSampleRI = oldInput;
                channels[channel].prevHighPassSampleR = addSound;

                avgVolR += abs(addSound);
                pOutputF32[(iSample + x + totalFramesRead + offset) * 2 + 1] += addSound;

                
            }
            resampleIndex += scaledPitch * (channels[channel].bitCrush + 1);  // Adjust pitch
        }


        

        //if (channels[channel].bitCrush > 0)
        //{
        //    for (ma_uint32 iSample = 0; iSample < framesRead; iSample++)
        //    {

        //    }
        //}

        
        
        leftoverFrame = resampleIndex - ma_uint32(resampleIndex);

        //resampleIndex = (ma_uint32)resampleIndex;

        


        if (scaledPitch < 1.0f)
        {
            channels[channel].startBetweenFrames = leftoverFrame;


            ma_uint64 soundPos;
            ma_decoder_get_cursor_in_pcm_frames(pDecoder, &soundPos);
            ma_uint64 moveTo = soundPos - framesToReadThisIteration + ma_uint32(resampleIndex);
            while (moveTo < 0)
            {
                ma_uint64 sampleLen;
                ma_decoder_get_length_in_pcm_frames(pDecoder, &sampleLen);
                moveTo = sampleLen + moveTo;
            }
            ma_decoder_seek_to_pcm_frame(pDecoder, moveTo);
            //channels[channel].startBetweenFrames = leftoverFrame;
        }
        else
        {
            channels[channel].startBetweenFrames = 0.0f;
        }

        


        totalFramesRead += (ma_uint32)framesReadThisIteration; // Increment the number of frames read up.


        
        if (framesReadThisIteration < (ma_uint32)framesToReadThisIteration) // Break if at the end of the sample.
        {
            return totalFramesRead; // Reached EOF.
        }
    }


    
    

    

    //std::cout << "  " << avgVolL;
        
    if (totalFramesRead > 0)
    {
        float framesR = (500.0f / float(totalFramesRead));
        if (scaledPitch > 1.0f)
        {
            framesR *= scaledPitch;
        }

        avgVolL *= framesR;
        avgVolR *= framesR;

        

        float interp = 0.2f;

        channels[channel].averageVolL = channels[channel].averageVolL * (1 - interp) + avgVolL * interp;
        channels[channel].averageVolR = channels[channel].averageVolR * (1 - interp) + avgVolR * interp;
    }
    

    //totalFramesRead = iSample;
    

    //LowPassFilter* filter = (LowPassFilter*)pDecoder->pUserData;

    //for (int i = 0; i < totalFramesRead * 2; i += 2)
    //{
    //    temp[i] = resampleTemp[i];
    //    temp[i + 1] = resampleTemp[i + 1];
    //}

    /*
    for (int i = 2; i < totalFramesRead * 2; i += 2)
    {
        

        //previousSampleR = alpha * temp[i] + (1.0f - alpha) * previousSampleR;
        //temp[i] = previousSampleR;
        //previousSampleL = alpha * temp[i + 1] + (1.0f - alpha) * previousSampleL;
        //temp[i + 1] = previousSampleL;

        previousSampleR = temp[i] - temp[i - 2] + alpha * previousSampleR;
        temp[i] = previousSampleR;
        previousSampleL = temp[i + 1] - temp[i - 1] + alpha * previousSampleL;
        temp[i + 1] = previousSampleL;

        

    }
    */
    
    
    

    //totalFramesRead *= channels[channel].realPitch;
    

    //std::cout << " <- end ";


    



    return totalFramesRead;
}



void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    /*
    if (editor.recordingSong) // Don't play when recording.
    {
        RecordSong();
        return;
    }*/

    if (toGenerateAdditiveWave)
    {
        GenerateAdditiveWave();
        return;
    }

    if (sampleDisplay.pauseSample) // Don't play sounds if the sample is stopped. (This is only applied if playing a sample in the sample menu.
        return;
    else if (sampleDisplay.playingSample || toDrawSampleDisplay)
        DrawSampleDisplay();

    if (editor.recordingSong)
    {
        RecordSong();
        return;
    }

    if (editor.playingSong)
        stepSongInCallback(float(frameCount) / 48000.0f * 1000.0f);
    else // if (sampleDisplay.playingSample)
    {
        stepSample(float(frameCount) / 48000.0f * 1000.0f);
    }

    /*
    else
    {
        if (editor.recordingSong) // Stop recording.
        {
            ma_encoder_uninit(&encoder);
            editor.recordingSong = false;
        }
    }
    */

    float* pOutputF32 = (float*)pOutput;

    // This example assumes the device was configured to use ma_format_f32.
    for (int channel = 0; channel < loadedSong.numberOfChannels; channel++)
    {
        
        if (channels[channel].toUninitialize)
        {
            channels[channel].volume = 0;
            StopSample(channel);
        }
        else if (channelInitialized[channel] && !channels[channel].muted)
        {
            float channelFrameCount = frameCount;
            if (channels[channel].pitch + channels[channel].pitchWaveOffset > 1.0f)
                channelFrameCount *= channels[channel].pitch + channels[channel].pitchWaveOffset;
            ma_uint64 framesRead = 0;
            framesRead = read_and_mix_pcm_frames_f32(&g_pDecoders[channel], pOutputF32, channelFrameCount, channel, channels[channel].startBetweenFrames, framesRead);

            if (framesRead < frameCount)
            {
                /*
                if (loadedSamples[channels[channel].instrument].looping)
                {
                    while (framesRead < frameCount)
                    {
                        //float* pOutput2 = (float*)pOutput;

                        float channelFrameCount2 = frameCount - framesRead;
                        
                        if(channels[channel].pitch + channels[channel].pitchWaveOffset > 1.0f)
                            channelFrameCount2 *= channels[channel].pitch + channels[channel].pitchWaveOffset;

                        ma_decoder_seek_to_pcm_frame(&g_pDecoders[channel], 0);

                        framesRead += read_and_mix_pcm_frames_f32(&g_pDecoders[channel], pOutputF32, channelFrameCount2, channel, channels[channel].startBetweenFrames, framesRead);
                    }
                }
                else
                */
                StopSample(channel);
            }

        }
        else
        {
            channels[channel].averageVolL *= 0.9;
            channels[channel].averageVolR *= 0.9;
        }
    }
    /*
    if (editor.playingSong && editor.recordingSong)
    {
        ma_uint64 framesWritten;
        ma_encoder_write_pcm_frames(&encoder, pOutputF32, frameCount, &framesWritten); // Write frames to file if recording.
    }*/
    
    
    
    (void)pInput;
    (void)pDevice;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








void SetUpAudioEngine()
{
    // In this example, all decoders need to have the same output format.
    decoderConfig = ma_decoder_config_init(ma_format_f32, 2, 48000);

    /* Create only a single device. The decoders will be mixed together in the callback. In this example the data format needs to be the same as the decoders. */
    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = ma_format_f32;
    deviceConfig.playback.channels = 2;
    deviceConfig.sampleRate = 48000;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = NULL;

    ma_device_init(NULL, &deviceConfig, &device);

    // Now we start playback and wait for the audio thread to tell us to stop.
    ma_device_start(&device);
    

    // Set the number of channels.
    g_pDecoders.resize(loadedSong.numberOfChannels);

    // Set up the encoder
    encoderConfig = ma_encoder_config_init(ma_encoding_format_wav, ma_format_f32, 2, 48000);
    


    // Set up the resource manager.
    //ManagerConfig = ma_resource_manager_config_init();
    //ManagerConfig.decodedFormat = device.playback.format;
    //ManagerConfig.decodedChannels = device.playback.channels;
    //ManagerConfig.decodedSampleRate = device.sampleRate;
    //ma_resource_manager_init(&ManagerConfig, &resourceManager);


    // Set up node graph
    //nodeGraphConfig = ma_node_graph_config_init(2);
    //ma_node_graph_init(&nodeGraphConfig, NULL, &nodeGraph);

    //lpfNodeConfig = ma_lpf_node_config_init(2, 48000, 48000 / 80, 8);
    //ma_lpf_node_init(&nodeGraph, &lpfNodeConfig, NULL, &lpfNode);

    /* Connect the output bus of the low pass filter node to the input bus of the endpoint. */
    //ma_node_attach_output_bus(&lpfNode, 0, ma_node_graph_get_endpoint(&nodeGraph), 0);

    /* Set the volume of the low pass filter to make it more of less impactful. */
    //ma_node_set_output_bus_volume(&lpfNode, 0, 0.9f);

    // Set up splitter.
    //splitterNodeConfig = ma_splitter_node_config_init(2);
    //ma_splitter_node_init(&nodeGraph, &splitterNodeConfig, NULL, &splitter);
    //ma_node_attach_output_bus(&splitter, 0, &lpfNode, 0);


    //decoderNodes.resize(g_pDecoders.size());

    /*
    for (int i = 0; i < g_pDecoders.size(); i++)
    {
        dataSourceNodeConfig = ma_data_source_node_config_init(&g_pDecoders[i]);
        
        ma_result result = ma_data_source_node_init(&nodeGraph, &dataSourceNodeConfig, NULL, &decoderNodes[i]);
        if (result != MA_SUCCESS) {
            std::cout << "a";
            // Failed to create data source node.
        }
        else
        {
            std::cout << "b";
        }
        //ma_node_attach_output_bus(&decoderNodes[i], 0, &nodeGraph, 0);
        
    }*/


    return;
}



void StartSample(int channel, int sampleNumber, float pitch, int startFrame)
{
    channels[channel].noteToPlaySample = sampleNumber;
    channels[channel].noteToPlayPitch = pitch;

    // Filter test
    channels[channel].lowPass = 500;
    channels[channel].highPass = 500;

    InitializeSample(channel, channels[channel].noteToPlaySample, channels[channel].noteToPlayPitch);

    

    ma_uint64 sampleLen;
    ma_decoder_get_length_in_pcm_frames(&g_pDecoders[channel], &sampleLen);
    ma_uint64 frameNum = channels[channel].jumpToFrame;
    ma_decoder_seek_to_pcm_frame(&g_pDecoders[channel], frameNum);

    ma_uint64 soundPos;
    ma_decoder_get_cursor_in_pcm_frames(&g_pDecoders[channel], &soundPos);
    ma_decoder_seek_to_pcm_frame(&g_pDecoders[channel], soundPos + startFrame);


    // Start sample in reverse.
    //ma_decoder_get_cursor_in_pcm_frames(&g_pDecoders[channel], &soundPos);
    
    //ma_decoder_get_available_frames(&g_pDecoders[channel], &sampleLen);
    //ma_decoder_get_length_in_pcm_frames(&g_pDecoders[channel], &sampleLen);
    //ma_decoder_seek_to_pcm_frame(&g_pDecoders[channel], sampleLen - 1 - soundPos);

    return;
}



void InitializeSample(int channel, int sampleNumber, float pitch)
{
    if (sampleNumber >= loadedSamples.size()) // Don't play samples that are not loaded.
        return;



    pitch = pitchTable[int(pitch)];
    channels[channel].pitch = pitch;



    StopSample(channel);


    if (sampleNumber > -1) // Don't play notes with samples below zero. (-1 is used to stop notes.)
    {
        //ma_decoder_init_file(name, &decoderConfig, &g_pDecoders[channel]);

        //float* pOutput = (float*)loadedSamples[sampleNumber].sampleAudioData;
        
        void* sampleAudioData;
        sampleAudioData = loadedSamples[sampleNumber].sampleData.data();
        
        
        ma_decoder_init_memory(sampleAudioData, loadedSamples[sampleNumber].sampleData.size(), &decoderConfig, &g_pDecoders[channel]);

        //ma_result result = ma_decoder_init_memory(loadedSamples[sampleNumber].sampleAudioData, loadedSamples[sampleNumber].sampleSize, NULL, &g_pDecoders[channel]);
        /*
        if (result != MA_SUCCESS)
        {
            printf("Failed to initialize decoder from memory.\n");
            return;
        }*/

        channelInitialized[channel] = true;

        if (channels[channel].volume > 1)
            channels[channel].volume = 1;
        else if (channels[channel].volume < 0)
            channels[channel].volume = 0;


        channels[channel].envelopeTime = 0.0f; // Restart the ADSR envelope.
        channels[channel].envelopeVolume = 0.0f;
    }

    

    return;
}



void StopSample(int channel)
{
    if (channelInitialized[channel]) // Don't uninitialized uninitialized devices.
    {
        channelInitialized[channel] = false;
        ma_decoder_uninit(&g_pDecoders[channel]);
    }

    channels[channel].toUninitialize = false;

    sampleDisplay.playingSample = false; // This flag is used to stop the sample played in the sample menu if any sample is stopped.
    sampleDisplay.pauseSample = false;
    sampleDisplay.samplePos = 0;


    //channels[channel].highPass = 100.0f; // Highpass test.


    
    return;
}








void ResizeDecoders(int newSize) // When a new song is loaded, decoders need to be created or destroyed to match the number of channels.
{
    // Set the number of channels.
    for (int ch = 0; ch < loadedSong.numberOfChannels; ch++)
        StopSample(ch);

    // Resize sound objects
    g_pDecoders.resize(newSize);
    channelInitialized.resize(newSize);
    for (int i = loadedSong.numberOfChannels; i < newSize; i++)
        channelInitialized[i] = false;

    // Resize frame
    for (int i = 0; i < loadedSong.frames.size(); i++)
    {
        loadedSong.frames[i].channels.resize(newSize); // Resize frame to match the number of channels.
        for (int j = loadedSong.numberOfChannels; j < newSize; j++)
        {
            loadedSong.frames[i].channels[j].notes = { loadedSong.frames[i].rows };
            loadedSong.frames[i].channels[j].volumes = { loadedSong.frames[i].rows };
            loadedSong.frames[i].channels[j].effects = { loadedSong.frames[i].rows };
        }
    }
    for (int i = 0; i < loadedFrame.rows.size(); i++)
    {
        loadedFrame.rows[i].note.resize(newSize);
        loadedFrame.rows[i].instrument.resize(newSize);
        loadedFrame.rows[i].volume.resize(newSize);
        loadedFrame.rows[i].effects.resize(newSize);

        for (int j = loadedSong.numberOfChannels; j < newSize; j++)
        {
            loadedFrame.rows[i].note[j] = -1;
            loadedFrame.rows[i].instrument[j] = -1;
            loadedFrame.rows[i].volume[j] = -1;
        }
    }

    // Resize song objects
    loadedSong.noteChannelIndex.resize(newSize);
    loadedSong.volumeChannelIndex.resize(newSize);
    loadedSong.effectChannelIndex.resize(newSize);
    loadedSong.toNextChannelNote.resize(newSize);
    loadedSong.toNextChannelVolume.resize(newSize);
    loadedSong.toNextChannelEffect.resize(newSize);

    for (int i = 0; i < newSize; i++)
    {
        loadedSong.noteChannelIndex[i] = 0;
        loadedSong.volumeChannelIndex[i] = 0;
        loadedSong.effectChannelIndex[i] = 0;
        loadedSong.toNextChannelNote[i] = 0;
        loadedSong.toNextChannelVolume[i] = 0;
        loadedSong.toNextChannelEffect[i] = 0;
    }


    channels.resize(newSize);


    loadedSong.unsavedChanges = false;

    loadedSong.numberOfChannels = newSize;
    gui.drawFrameThisFrame = true;

    return;
}



void stepSongInCallback(float elapsedMS)
{

    // 1B = 1/BPM;

    // 1B = 60/BPS;

    // 1B = 60,000/BPMS;

    // 1B = 1ms
    // 1000D / 1s


    loadedSong.timeInNote += elapsedMS;
    loadedSong.timeInTick += elapsedMS;
    loadedSong.timeInSong += elapsedMS;






    if (loadedSong.timeInNote >= (60000.0f / (loadedSong.bpm * 4.0f)))
    {
        // The interface has changed, and must be redrawn.
        gui.drawFrameThisFrame = true;

        loadedSong.currentNote++;



        loadedSong.timeInNote -= (60000.0f / (loadedSong.bpm * 4.0f));


        int frameToJumpTo = -1; // The frame to jump to if jumping to a frame this cycle. This allows all effects to be played before a frame jump happens.

        //loadedSong.timeInNote = 0;


        //if (loadedSong.currentFrame >= loadedSong.frames.size())
        //    loadedSong.currentFrame = loadedSong.frames.size() - 1;

        //std::cout << "Start -> ";

        if (loadedSong.currentNote >= loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].rows) // Start new row
        {

            loadedSong.currentFrame++;
            loadedSong.currentNote = 0;

            if (loadedSong.currentFrame >= loadedSong.frameSequence.size()) // Restart song.
            {
                loadedSong.currentFrame = 0;
                loadedSong.timeInSong = 0;
                if (editor.recordingSong) // Stop recording.
                {
                    editor.recordingSong = false;
                }
            }


            loadCurrentFrame();



            for (int ch = 0; ch < loadedSong.numberOfChannels; ch++)
            {
                loadedSong.noteChannelIndex[ch] = 0;
                loadedSong.volumeChannelIndex[ch] = 0;
                loadedSong.effectChannelIndex[ch] = 0;
                loadedSong.toNextChannelNote[ch] = 0;
                loadedSong.toNextChannelVolume[ch] = 0;
                loadedSong.toNextChannelEffect[ch] = 0;
            }

            //std::cout << " <- end ";

        }


        //std::cout << " <- End ";


        // Read note data an display notes.
        for (int ch = 0; ch < loadedSong.numberOfChannels; ch++)
        {
            



            // Find distance to first note, volume and effect.
            int noteIndex = loadedSong.noteChannelIndex[ch];
            int volumeIndex = loadedSong.volumeChannelIndex[ch];
            int effectIndex = loadedSong.effectChannelIndex[ch];

            if (noteIndex == 0) // Initial "to next note" at the start of each frame.
            {
                loadedSong.toNextChannelNote[ch] = loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].notes[0];
                loadedSong.noteChannelIndex[ch]++;
                noteIndex++;
                loadedSong.toNextChannelVolume[ch] = loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].volumes[0];
                loadedSong.volumeChannelIndex[ch]++;
                volumeIndex++;
                loadedSong.toNextChannelEffect[ch] = loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].effects[0];
                loadedSong.effectChannelIndex[ch]++;
                effectIndex++;
                loadedSong.currentNote = 0;
            }


            loadedSong.toNextChannelNote[ch]--;
            loadedSong.toNextChannelVolume[ch]--;
            loadedSong.toNextChannelEffect[ch]--;




            if (loadedSong.toNextChannelVolume[ch] < 0) // Read next volume.
            {
                if (volumeIndex < loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].volumes.size())
                {
                    int volume = loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].volumes[volumeIndex];
                    loadedSong.volumeChannelIndex[ch]++;
                    volumeIndex++;

                    channels[ch].volume = float(volume) / 80.0f;


                    // Set distance to next note.
                    if (volumeIndex < loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].volumes.size())
                    {
                        loadedSong.toNextChannelVolume[ch] = loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].volumes[volumeIndex];
                        loadedSong.volumeChannelIndex[ch]++;
                    }
                    else
                        loadedSong.toNextChannelVolume[ch] = 255; // No more notes in this channel in the frame.
                }
            }

            bool readingEffects = true;
            int effectNum = 0;

            while (readingEffects)
            {
                effectIndex = loadedSong.effectChannelIndex[ch];
                if (loadedSong.toNextChannelEffect[ch] < 0) // Read next effect.
                {
                    if (effectIndex < loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].effects.size())
                    {
                        int effect = loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].effects[effectIndex];
                        loadedSong.effectChannelIndex[ch]++;
                        effectIndex++;
                        int effectValue = loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].effects[effectIndex];
                        loadedSong.effectChannelIndex[ch]++;
                        effectIndex++;


                        if (effect == 0) // Set pan.
                        {
                            channels[ch].panValue = float(effectValue / 40.0f) - 1;
                        }
                        else if (effect == 1) // Pan slide.
                        {
                            //int p1 = effectValue / 10;
                            //int p2 = effectValue % 10;
                            channels[ch].panSlide = float(effectValue) / 3200.0f;
                        }
                        else if (effect == 2) // Volume slide.
                        {
                            channels[ch].volumeSlide = float(effectValue) / 3200.0f;
                        }
                        else if (effect == 3) // Volume wave.
                        {
                            channels[ch].volumeWaveDepth = float(effectValue / 10) / 16.0f;
                            channels[ch].volumeWaveSpeed = float(effectValue % 10);
                        }
                        else if (effect == 4) // Set pitch.
                        {
                            /*
                            int p1 = effectValue / 10;
                            int p2 = effectValue % 10;

                            effectValue = channels[ch].arpNotes[0] + p1 - p2;

                            if (effectValue > 84)
                                effectValue = 84;
                            else if (effectValue < 0)
                                effectValue = 0;

                            channels[ch].pitch = pitchTable[effectValue];
                            channels[ch].arpNotes[0] = effectValue;


                            int arpOffset = 0;
                            for (int arp = 0; arp < channels[ch].arpNotes.size(); arp++)
                            {
                                arpOffset += channels[ch].arpNotes[arp];
                                if (arpOffset > 84)
                                    arpOffset = 84;
                                else if (arpOffset < 0)
                                    arpOffset = 0;
                                channels[ch].arpPitches[arp] = pitchTable[arpOffset];
                            }
                            */

                            channels[ch].pitch = pitchTable[effectValue];
                            int arpOffset = 0;
                            //channels[ch].arpNotes[0] = effectValue;
                            for (int arp = 0; arp < channels[ch].arpNotes.size(); arp++)
                            {
                                arpOffset += channels[ch].arpNotes[arp];
                                if (arpOffset > 84)
                                    arpOffset = 84;
                                else if (arpOffset < 0)
                                    arpOffset = 0;

                                channels[ch].arpNotes[arp] = arpOffset;
                                channels[ch].arpPitches[arp] = pitchTable[arpOffset];
                            }
                        }
                        else if (effect == 5) // Pitch slide.
                        {
                            int p1 = effectValue / 10;
                            int p2 = effectValue % 10;
                            channels[ch].pitchSlide = float(p1 - p2) / 2000.0f;
                        }
                        else if (effect == 6) // Pitch slide to note.
                        {
                            int p1 = effectValue / 10;
                            int p2 = effectValue % 10;
                            channels[ch].pitchSlide = float(p1 - p2) / 2000.0f;
                        }
                        else if (effect == 7) // Pitch wave.
                        {
                            channels[ch].pitchWaveDepth = float(effectValue / 10) / 16.0f;
                            channels[ch].pitchWaveSpeed = float(effectValue % 10); 
                        }
                        else if (effect == 8) // Set pitch (fine).
                        {
                            int p1 = effectValue / 10;
                            int p2 = effectValue % 10;
                            channels[ch].pitch += float(p1 - p2) / 120.0f;
                            for (int arp = 0; arp < channels[ch].arpNotes.size(); arp++)
                                channels[ch].arpPitches[arp] += float(p1 - p2) / 120.0f;
                        }
                        else if (effect == 9) // Pitch slide (fine).
                        {
                            int p1 = effectValue / 10;
                            int p2 = effectValue % 10;
                            channels[ch].pitchSlide = float(p1 - p2) / 20000.0f;
                        }
                        else if (effect == 10) // Pitch wave (fine).
                        {
                            channels[ch].pitchWaveDepth = float(effectValue / 10) / 160.0f;
                            channels[ch].pitchWaveSpeed = float(effectValue % 10);
                        }
                        else if (effect == 11) // Lowpass filter.
                        {
                            if (effectValue >= 80)
                                channels[ch].lowPass = 1.0f;
                            else
                                channels[ch].lowPass = 80.0f * float(80 - effectValue);
                            channels[ch].alphaLow = 2.0f * 3.14159265f * channels[ch].lowPass / 48000.0f;
                        }
                        else if (effect == 12) // Lowpass slide.
                        {
                            int p1 = effectValue / 10;
                            int p2 = effectValue % 10;
                            channels[ch].lowPassSlide = float(p1 - p2) / 10.0f;
                        }
                        else if (effect == 13) // High-pass filter.
                        {
                            if (effectValue <= 0)
                                channels[ch].highPass = 1.0f;
                            else
                                channels[ch].highPass = 80.0f * float(effectValue);
                            float RC = 1.0f / (2.0f * 3.14159265f * channels[ch].highPass);
                            channels[ch].alphaHigh = RC / (RC + (1.0f / 48000.0f));
                        }
                        else if (effect == 14) // High-pass slide.
                        {
                            int p1 = effectValue / 10;
                            int p2 = effectValue % 10;
                            channels[ch].highPassSlide = float(p1 - p2) / 10.0f;
                        }
                        else if (effect == 15) // Delay note.
                        {
                            channels[ch].delayNote = true;
                            channels[ch].delayLength = effectValue + 1;
                        }
                        else if (effect == 16) // Set tempo.
                        {
                            loadedSong.bpm = effectValue;
                            if (loadedSong.bpm < 1)
                                loadedSong.bpm = 1;
                        }
                        else if (effect == 17) // Change tempo.
                        {
                            loadedSong.bpm += effectValue;
                            if (loadedSong.bpm > 255)
                                loadedSong.bpm = 255;
                        }
                        else if (effect == 18) // Arpeggio.
                        {
                            if (effectValue == 0)
                            {
                                channels[ch].arpPitches.clear();
                                channels[ch].arpPitches.emplace_back(0);
                                channels[ch].arpPitches.shrink_to_fit();
                                channels[ch].arpNotes.clear();
                                channels[ch].arpNotes.emplace_back(0);
                                channels[ch].arpNotes.shrink_to_fit();
                            }
                            else
                            {
                                if (effectNum == 0)
                                {
                                    channels[ch].arpPitches.clear();
                                    channels[ch].arpPitches.emplace_back(0);
                                    channels[ch].arpPitches.shrink_to_fit();
                                    channels[ch].arpNotes.clear();
                                    channels[ch].arpNotes.emplace_back(0);
                                    channels[ch].arpNotes.shrink_to_fit();
                                }

                                int p1 = effectValue / 10;
                                int p2 = effectValue % 10;

                                if (p1 > 0)
                                {
                                    channels[ch].arpNotes.emplace_back(p1);
                                    channels[ch].arpPitches.emplace_back(0);
                                }
                                if (p2 > 0)
                                {
                                    channels[ch].arpNotes.emplace_back(p2);
                                    channels[ch].arpPitches.emplace_back(0);
                                }
                            }

                            /*
                            if (effectValue == 0)
                            {
                                channels[ch].arpPitches.clear();
                                channels[ch].arpPitches.emplace_back(0);
                                channels[ch].arpPitches.shrink_to_fit();
                                channels[ch].arpNotes.clear();
                                channels[ch].arpNotes.emplace_back(0);
                                channels[ch].arpNotes.shrink_to_fit();
                            }
                            else
                            {
                                if (effectNum == 0)
                                {
                                    channels[ch].arpPitches.clear();
                                    channels[ch].arpPitches.emplace_back(0);
                                    channels[ch].arpPitches.shrink_to_fit();
                                    channels[ch].arpNotes.clear();
                                    channels[ch].arpNotes.emplace_back(0);
                                    channels[ch].arpNotes.shrink_to_fit();
                                }

                                int p1 = effectValue / 10;
                                int p2 = effectValue % 10;

                                if (p1 > 0)
                                {
                                    channels[ch].arpNotes.emplace_back(-p1);
                                    channels[ch].arpPitches.emplace_back(0);
                                }
                                if (p2 > 0)
                                {
                                    channels[ch].arpNotes.emplace_back(-p2);
                                    channels[ch].arpPitches.emplace_back(0);
                                }
                            }*/
                        }
                        else if (effect == 19) // Retrigger Arpeggio.
                        {

                        }
                        else if (effect == 20) // Rocket Effect.
                        {
                            if (effectValue == 0)
                                channels[ch].rocketEffect1 = false;
                            else
                            {
                                int p1 = effectValue / 10;
                                int p2 = effectValue % 10;
                                channels[ch].rocketEffect1 = true;
                                channels[ch].rocketEffect2 = false;
                                channels[ch].rocketAdd = p1;
                                channels[ch].rocketSub = p2;
                            }

                            /*
                            if (effectValue == 0)
                                channels[ch].rocketEffect2 = false;
                            else
                            {
                                int p1 = effectValue / 10;
                                int p2 = effectValue % 10;
                                channels[ch].rocketEffect2 = true;
                                channels[ch].rocketEffect1 = false;
                                channels[ch].rocketAdd = p1;
                                channels[ch].rocketSub = p2;
                            }*/
                        }
                        else if (effect == 21) // Retrigger (Every x ticks)
                        {
                            channels[ch].retriggerLength = effectValue;
                            channels[ch].retriggerIndex = 0;
                        }
                        else if (effect == 22) // Bit Crush
                        {
                            channels[ch].bitCrush = effectValue;
                        }
                        else if (effect == 23) // Sample jump
                        {
                            ma_uint64 sampleLen;

                            if (effectValue - 1 < loadedSamples[channels[ch].instrument].jumpPoints.size())
                            {
                                channels[ch].jumpToFrame = loadedSamples[channels[ch].instrument].jumpPoints[effectValue - 1];
                                ma_decoder_seek_to_pcm_frame(&g_pDecoders[ch], channels[ch].jumpToFrame);
                            }
                        }
                        else if (effect == 24) // Reverse
                        {
                            loadedSong.ticksPerBeat = effectValue;
                        }
                        else if (effect == 25) // Set ticks
                        {
                            loadedSong.ticksPerBeat = effectValue;
                        }
                        else if (effect == 26) // Jump  to frame
                        {
                            frameToJumpTo = effectValue;
                        }
                        else if (effect == 27) // Toggle mute
                        {
                            if (effectValue > 0)
                                channels[ch].muted = true;
                            else
                                channels[ch].muted = false;
                        }
                        /*
                        else if (effect == 5) // Set volume slide.
                        {
                            channels[ch].volumeSlide = (effectValue / 16) - (effectValue % 16);
                        }
                        else if (effect == 1 || effect == 2 || effect == 3 || effect == 6 || effect == 7) // 2-parameter effects.
                        {
                            channels[ch].effectX = effectValue / 16;
                            channels[ch].effectY = effectValue % 16;
                        }
                        else if (effect == 8) // Jump to frame.
                        {
                            if (effectValue >= loadedSong.frameSequence.size())
                                effectValue = loadedSong.frameSequence.size() - 1;
                            loadedSong.currentFrame = effectValue;
                            loadedSong.currentNote = 0;

                            loadCurrentFrame();

                            // Reset frame values.
                            loadedSong.timeInNote = 0;
                            loadedSong.timeInTick = 0;
                            loadedSong.timeInSong = 0;
                            gui.frameScroll.y = 0.0f;

                            for (int ch2 = 0; ch2 < loadedSong.numberOfChannels; ch2++)
                            {
                                loadedSong.noteChannelIndex[ch2] = 0;
                                loadedSong.volumeChannelIndex[ch2] = 0;
                                loadedSong.effectChannelIndex[ch2] = 0;
                                loadedSong.toNextChannelNote[ch2] = 0;
                                loadedSong.toNextChannelVolume[ch2] = 0;
                                loadedSong.toNextChannelEffect[ch2] = 0;
                            }

                            loadedSong.toNextChannelNote[ch] = loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].notes[0];
                            loadedSong.noteChannelIndex[ch]++;
                            noteIndex++;
                            loadedSong.toNextChannelVolume[ch] = loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].volumes[0];
                            loadedSong.volumeChannelIndex[ch]++;
                            volumeIndex++;
                            loadedSong.toNextChannelEffect[ch] = loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].effects[0];
                            loadedSong.effectChannelIndex[ch]++;
                            effectIndex++;
                            loadedSong.currentNote = 0;
                        }
                        else if (effect == 10) // Set sample position.
                        {
                            //ma_sound_seek_to_pcm_frame(&channelAudio[ch], float(effectValue) * 100000.0f * (1000.0f / ma_engine_get_sample_rate(&engine)));
                            channels[ch].effectX = effectValue;
                        }
                        else if (effect == 11) // Set BPM.
                        {
                            loadedSong.bpm = effectValue;
                        }
                        else if (effect == 12) // Set TPB.
                        {
                            loadedSong.ticksPerBeat = effectValue;
                        }
                        else // 1-parameter effects.
                            channels[ch].effectX = effectValue;

                         */

                        if (effectIndex < loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].effects.size())
                        {
                            loadedSong.toNextChannelEffect[ch] = loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].effects[effectIndex];
                            loadedSong.effectChannelIndex[ch]++;
                            if (loadedSong.toNextChannelEffect[ch] == 255) // Read multiple effects on one beat.
                            {
                                loadedSong.toNextChannelEffect[ch] = -1;
                                effectNum++;
                                //loadedSong.effectChannelIndex[ch]--;
                            }
                            else
                                readingEffects = false;
                        }
                        else
                        {
                            loadedSong.toNextChannelEffect[ch] = 256; // No more notes in this channel in the frame.
                            readingEffects = false;
                        }
                    }
                    else
                        readingEffects = false;
                }
                else
                    readingEffects = false;
            }

            if (loadedSong.toNextChannelNote[ch] < 0) // Read next note.
            {
                if (noteIndex < loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].notes.size())
                {
                    int note = loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].notes[noteIndex];
                    loadedSong.noteChannelIndex[ch]++;
                    noteIndex++;
                    //channels[ch].pitch = note;

                    int instrument = loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].notes[noteIndex];
                    channels[ch].instrument = instrument;
                    loadedSong.noteChannelIndex[ch]++;
                    noteIndex++;

                    // Set distance to next note.
                    if (noteIndex < loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].notes.size())
                    {
                        float pastNote = loadedSong.toNextChannelNote[ch];
                        loadedSong.toNextChannelNote[ch] = loadedSong.frames[loadedSong.frameSequence[loadedSong.currentFrame]].channels[ch].notes[noteIndex];
                        loadedSong.toNextChannelNote[ch] += pastNote + 1;
                        loadedSong.noteChannelIndex[ch]++;
                    }
                    else
                        loadedSong.toNextChannelNote[ch] = 255; // No more notes in this channel in the frame.

                    if (note == 255) // Stop note.
                    {
                        channels[ch].envelopeTime = (loadedSamples[channels[ch].instrument].attack * 1000.0f) + (loadedSamples[channels[ch].instrument].sustain * 1000.0f);
                        //StopSample(ch); // Start a blank note to stop the current one.
                    }
                    else if (channels[ch].delayNote) // Delayed note.
                    {
                        StopSample(ch); // Stop the current note.
                        channels[ch].arpIndex = 0;
                        int arpOffset = 0;
                        channels[ch].arpNotes[0] = note;
                        for (int arp = 0; arp < channels[ch].arpNotes.size(); arp++)
                        {
                            arpOffset += channels[ch].arpNotes[arp];
                            if (arpOffset > 84)
                                arpOffset = 84;
                            else if (arpOffset < 0)
                                arpOffset = 0;
                            channels[ch].arpPitches[arp] = pitchTable[arpOffset];
                        }
                    }
                    else
                    {
                        channels[ch].arpIndex = 0;
                        int arpOffset = 0;
                        channels[ch].arpNotes[0] = note;
                        for (int arp = 0; arp < channels[ch].arpNotes.size(); arp++)
                        {
                            arpOffset += channels[ch].arpNotes[arp];
                            if (arpOffset > 84)
                                arpOffset = 84;
                            else if (arpOffset < 0)
                                arpOffset = 0;
                            channels[ch].arpPitches[arp] = pitchTable[arpOffset];
                        }
                        StartSample(ch, instrument, note, loadedSong.timeInNote);
                    }
                    /*
                    else if (channels[ch].effect != 15) // Don't play delayed notes.
                    {
                        StartSample(ch, instrument, note, loadedSong.timeInNote);


                        if (channels[ch].effect == 24) // Set sample position.
                        {
                            //ma_sound_seek_to_pcm_frame(&channelAudio[ch], channels[ch].effectX * 100000.0f * (1000.0f / ma_engine_get_sample_rate(&engine)));
                        }
                    }*/


                }
            }




            /*
            channels[ch].soundVolume = 4.0f - ma_sound_get_time_in_milliseconds(&channelAudio[ch]) * 0.01;

            if (channels[ch].soundVolume < 0)
                channels[ch].soundVolume = 0.0f;

            if (channels[ch].soundVolume > 5.0)
                channels[ch].soundVolume = 5.0f;
            */
            //channels[ch].soundVolume = ma_sound_get_data_source(&channelAudio[ch]);
        }

        /*
        for (int ch = 0; ch < 8; ch++) // Play notes.
        {
            if (loadedSong.currentNote % 4 == 0)
                StartSample(ch, 0, 32);
        }*/

        if (frameToJumpTo > -1)
        {
            if (frameToJumpTo >= loadedSong.frameSequence.size())
                frameToJumpTo = loadedSong.frameSequence.size() - 1;
            loadedSong.currentFrame = frameToJumpTo;
            loadedSong.currentNote = 0;

            loadCurrentFrame();

            // Reset frame values.
            loadedSong.timeInNote = 0;
            loadedSong.timeInTick = 0;
            loadedSong.timeInSong = 0;
            gui.frameScroll.y = 0.0f;


            for (int ch2 = 0; ch2 < loadedSong.numberOfChannels; ch2++)
            {
                loadedSong.noteChannelIndex[ch2] = 0;
                loadedSong.volumeChannelIndex[ch2] = 0;
                loadedSong.effectChannelIndex[ch2] = 0;
                loadedSong.toNextChannelNote[ch2] = 0;
                loadedSong.toNextChannelVolume[ch2] = 0;
                loadedSong.toNextChannelEffect[ch2] = 0;
            }
        }
    }




    if (loadedSong.timeInTick > (60000.0f / (loadedSong.ticksPerBeat * 4.0f * loadedSong.bpm))) // New tick
    {
        loadedSong.timeInTick -= (60000.0f / (loadedSong.ticksPerBeat * 4.0f * loadedSong.bpm));
        for (int ch = 0; ch < loadedSong.numberOfChannels; ch++)
        {
            if (channels[ch].rocketEffect1)
            {
                channels[ch].pitch += channels[ch].rocketAdd * 0.1f;
                channels[ch].rocketAdd = channels[ch].rocketSub - channels[ch].rocketAdd;
                channels[ch].rocketSub = -channels[ch].rocketSub;
            }
            else if (channels[ch].rocketEffect2)
            {
                channels[ch].pitch -= channels[ch].rocketAdd * 0.1f;
                channels[ch].rocketAdd -= channels[ch].rocketSub;
                channels[ch].rocketSub = -channels[ch].rocketSub;
            }

            if (channels[ch].arpPitches.size() > 1) // Arpeggio
            {
                channels[ch].arpIndex++;
                if (channels[ch].arpIndex > channels[ch].arpPitches.size() - 1)
                    channels[ch].arpIndex = 0;
                channels[ch].pitch = channels[ch].arpPitches[channels[ch].arpIndex];
            }

            if (channels[ch].delayNote)
            {
                channels[ch].delayLength--;
                if (channels[ch].delayLength <= 0)
                {
                    channels[ch].delayNote = false;
                    channels[ch].delayLength = 0;


                    if (channels[ch].arpNotes.size() > 0)
                    {
                        int arpOffset = 0;
                        channels[ch].arpIndex = 0;
                    }
                    StartSample(ch, channels[ch].instrument, channels[ch].arpNotes[0], loadedSong.timeInNote);
                }
            }

            if (channels[ch].retriggerLength > 0)
            {
                channels[ch].retriggerIndex++;
                if (channels[ch].retriggerIndex > channels[ch].retriggerLength)
                {
                    StartSample(ch, channels[ch].instrument, channels[ch].arpNotes[0], loadedSong.timeInNote);
                    channels[ch].retriggerIndex = 0;
                }
            }
            
            /*
            
            else if (channels[ch].effect == 9) // Delayed note
            {
                channels[ch].effectX--;

                if (channels[ch].effectX <= 0)
                {
                    StartSample(ch, channels[ch].instrument, channels[ch].pitch, loadedSong.timeInNote);
                    channels[ch].effect = -1;
                }
            }*/
        }
    }
    

    stepSample(elapsedMS); // Apply the ADSR filter.

    for (int ch = 0; ch < loadedSong.numberOfChannels; ch++) // Constant checks
    {


        if (!channels[ch].delayNote)
        {
            // Pan Slide
            channels[ch].panValue += channels[ch].panSlide * elapsedMS * loadedSong.bpm / 120;
            if (channels[ch].panValue > 1)
                channels[ch].panValue = 1;
            else if (channels[ch].panValue < -1)
                channels[ch].panValue = -1;

            // Volume Slide
            channels[ch].volume += channels[ch].volumeSlide * elapsedMS * loadedSong.bpm / 120;
            if (channels[ch].volume > 1)
                channels[ch].volume = 1;
            else if (channels[ch].volume < 0)
                channels[ch].volume = 0;

            // Volume Wave
            channels[ch].volumeWavePoint += elapsedMS / (120000.0f / (loadedSong.bpm)) * channels[ch].volumeWaveSpeed;
            if (channels[ch].volumeWavePoint > 1)
                channels[ch].volumeWavePoint--;
            channels[ch].volumeWaveOffset = sin(channels[ch].volumeWavePoint * 3.1416 * 2) * channels[ch].volumeWaveDepth;

            // Pitch Slide
            channels[ch].pitch += channels[ch].pitchSlide * elapsedMS * loadedSong.bpm / 120;
            for (int arp = 0; arp < channels[ch].arpNotes.size(); arp++)
            {
                channels[ch].arpPitches[arp] += channels[ch].pitchSlide * elapsedMS * loadedSong.bpm / 120;
            }

            // Pitch Wave
            channels[ch].pitchWavePoint += elapsedMS / (120000.0f / (loadedSong.bpm)) * channels[ch].pitchWaveSpeed;
            if (channels[ch].pitchWavePoint > 1)
                channels[ch].pitchWavePoint--;
            channels[ch].pitchWaveOffset = sin(channels[ch].pitchWavePoint * 3.1416 * 2) * channels[ch].pitchWaveDepth;

            // Lowpass Slide
            if (channels[ch].lowPassSlide != 0)
            {
                channels[ch].lowPass += channels[ch].lowPassSlide * elapsedMS * loadedSong.bpm / 120;
                if (channels[ch].lowPass > 6400)
                    channels[ch].lowPass = 6400;
                else if (channels[ch].lowPass < 1.0f)
                    channels[ch].lowPass = 1.0f;
                channels[ch].alphaLow = 2.0f * 3.14159265f * channels[ch].lowPass / 48000.0f;
            }

            // High-pass Slide
            if (channels[ch].highPassSlide != 0)
            {
                channels[ch].highPass += channels[ch].highPassSlide * elapsedMS * loadedSong.bpm / 120;
                if (channels[ch].highPass > 6400)
                    channels[ch].highPass = 6400;
                else if (channels[ch].highPass < 1.0f)
                    channels[ch].highPass = 1.0f;
                float RC = 1.0f / (2.0f * 3.14159265f * channels[ch].highPass);
                channels[ch].alphaHigh = RC / (RC + (1.0f / 48000.0f));
            }

            // BPM Slide
            if (loadedSong.bpmSlide != 0)
            {
                loadedSong.bpm += loadedSong.bpmSlide * elapsedMS * loadedSong.bpm / 120;
                if (loadedSong.bpm > 255.0f)
                    loadedSong.bpm = 255.0f;
                else if (loadedSong.bpm < 1.0f)
                    loadedSong.bpm = 1.0f;
            }
        }


        
        
        //channels[channel].envelopeVolume
        
        // Filter test
        //channels[ch].lowPass += 10.0f * delta;
        //channels[ch].highPass -= 10.0f * delta;
        //channels[ch].highPass += 100.0f * delta;
        // Pitch bent test.
        /*
        channels[ch].realPitch -= 0.0001 * delta;
        if (channels[ch].realPitch < 0.05)
            channels[ch].realPitch = 0.05;
        else if (channels[ch].realPitch > 8)
            channels[ch].realPitch = 8;
        */

        
        /*
        if (channels[ch].effect == 2) // Vibrato
        {
            float volOffset = sin((loadedSong.timeInSong / (60000.0f / (loadedSong.bpm * 4.0f))) * 3.14f * 0.125f * channels[ch].effectX) * channels[ch].effectY * 0.01;

            //ma_sound_set_pitch(&channelAudio[ch], pitchTable[int(channels[ch].pitch)] + volOffset);
        }
        else if (channels[ch].effect == 3) // Tremolo
        {
            float volOffset = sin((loadedSong.timeInSong / (60000.0f / (loadedSong.bpm * 4.0f))) * 3.14f * 0.125f * channels[ch].effectX) * channels[ch].effectY * 0.1;
            //channels[ch].volume += volOffset;
            //ma_sound_set_volume(&channelAudio[ch], channels[ch].volume + volOffset);
        }
        if (channels[ch].volumeSlide != 0.0f) // Volume Slide
        {
            float vol = float(channels[ch].volumeSlide) * 0.0002f * elapsedMS;
            channels[ch].volume += vol;

            if (channels[ch].volume < 0)
                channels[ch].volume = 0;
            else if (channels[ch].volume > 1)
                channels[ch].volume = 1;

            //ma_sound_set_volume(&channelAudio[ch], float(channels[ch].volume));
        }
        else if (channels[ch].effect == 6) // Pitch Slide
        {
            float vol = float(channels[ch].effectX - channels[ch].effectY) * 0.001f * elapsedMS;
            channels[ch].realPitch += vol;

            //ma_sound_set_pitch(&channelAudio[ch], channels[ch].realPitch);
        }
        else if (channels[ch].effect == 7) // Pitch Slide (fine)
        {
            float vol = float(channels[ch].effectX - channels[ch].effectY) * 0.0001f * elapsedMS;
            channels[ch].realPitch += vol;

            //ma_sound_set_pitch(&channelAudio[ch], channels[ch].realPitch);
        }
        */


        //if (channels[ch].muted)
        //    channels[ch].volume = 0;

        
    }




    return;
}



void stepSample(float elapsedMS)
{
    
    // 1B = 1/BPM;

    // 1B = 60/BPS;

    // 1B = 60,000/BPMS;

    // 1B = 1ms
    // 1000D / 1s


    for (int ch = 0; ch < loadedSong.numberOfChannels; ch++) // Constant checks
    {
        if (channels[ch].instrument >= loadedSamples.size())
            break;

        // Apply the ADSR filter.
        channels[ch].envelopeTime += elapsedMS * 1.0f;
        float inpterpVol = 1.0f;

        float attackEnd = loadedSamples[channels[ch].instrument].attack * 1000.0f;
        float sustainEnd = attackEnd + loadedSamples[channels[ch].instrument].sustain * 1000.0f;

        if (channels[ch].envelopeTime < attackEnd)
        {
            inpterpVol = channels[ch].envelopeTime / attackEnd;
            channels[ch].envelopeVolume = inpterpVol;
        }
        else if (loadedSamples[channels[ch].instrument].sustain > 0 && channels[ch].envelopeTime < sustainEnd)
        {
            channels[ch].envelopeVolume -= loadedSamples[channels[ch].instrument].decay * 0.1f;
        }
        else if (loadedSamples[channels[ch].instrument].sustain > 0)
        {
            float inpterpVol = loadedSamples[channels[ch].instrument].release;
            channels[ch].envelopeVolume = channels[ch].envelopeVolume * (inpterpVol);
        }

        
    }

    return;
}



void RecordSong()
{
    


    //ma_device_stop(&device);
    

    


    saveCurrentFrame();
    if (editor.playingSong)
        StartOrStopSong();
    loadedSong.currentFrame = 0;
    loadCurrentFrame();


    StartOrStopSong();


    std::string fileName = "C:/" + fileNavigator.currentFilePath.std::filesystem::path::string() + "/" + loadedSong.songName + ".wav";
    //std::string fileName = "Export/" + loadedSong.songName + ".wav";
    const char* name = &fileName[0];
    ma_encoder_init_file(name, &encoderConfig, &encoder);

    
    

    ma_uint64 stepSize = 480;

    while (editor.recordingSong)
    {
        stepSongInCallback(float(stepSize) / 48000.0f * 1000.0f);

        float pOut[7700] = { 0.0f };

        float* pOutputF32 = (float*)pOut;

        //float pOutputF32[7700];

        //float* pOutputF32 = (float*)pOutput;

        // This example assumes the device was configured to use ma_format_f32.
        for (int channel = 0; channel < loadedSong.numberOfChannels; channel++)
        {
            if (channels[channel].toUninitialize)
            {
                channels[channel].volume = 0;
                StopSample(channel);
            }
            else if (channelInitialized[channel])
            {
                float channelFrameCount = stepSize;
                if (channels[channel].pitch + channels[channel].pitchWaveOffset > 1.0f)
                    channelFrameCount *= channels[channel].pitch + channels[channel].pitchWaveOffset;
                ma_uint64 framesRead = 0;
                framesRead = read_and_mix_pcm_frames_f32(&g_pDecoders[channel], pOutputF32, channelFrameCount, channel, channels[channel].startBetweenFrames, framesRead);
                if (framesRead < stepSize)
                {
                    StopSample(channel);
                }
            }
        }




        ma_uint64 framesWritten;
        ma_encoder_write_pcm_frames(&encoder, pOutputF32, stepSize, &framesWritten); // Write frames to file if recording.


        //(void)pInput;
        //(void)pDevice;
    }

    
    
    StartOrStopSong();

    //ma_device_start(&device);

    ma_encoder_uninit(&encoder);

    

    return;
}




void StartOrStopSong()
{

    sampleDisplay.pauseSample = false;

    /*
    if (editor.recordingSong && !editor.playingSong) // Set up recording.
    {
        std::string fileName = "Export/" + loadedSong.songName + ".wav";
        const char* name = &fileName[0];
        ma_encoder_init_file(name, &encoderConfig, &encoder);
    }*/

    editor.playingSong = !editor.playingSong;
    loadedSong.bpm = loadedSong.startingBPM;
    loadedSong.ticksPerBeat = loadedSong.startingTPB;


    //editor.editing = !editor.playingSong;
    saveCurrentFrame();

    for (int ch = 0; ch < loadedSong.numberOfChannels; ch++)
    {
        channels[ch].toUninitialize = true;
    }

    if (editor.playingSong) // Restart the frame when playing the song.
    {
        loadedSong.currentNote = 0;
        loadedSong.timeInNote = (60000.0f / (loadedSong.bpm * 4.0f));
        loadedSong.timeInSong = 0.0f;
        loadedSong.timeInTick = 0.0f;
        for (int ch = 0; ch < loadedSong.numberOfChannels; ch++)
        {
            StopSample(ch);
            loadedSong.noteChannelIndex[ch] = 0;
            loadedSong.volumeChannelIndex[ch] = 0;
            loadedSong.effectChannelIndex[ch] = 0;
            loadedSong.toNextChannelNote[ch] = 0;
            loadedSong.toNextChannelVolume[ch] = 0;
            loadedSong.toNextChannelEffect[ch] = 0;


            channels[ch].panValue = 0.0f;
            channels[ch].panSlide = 0.0f;

            channels[ch].pitchSlide = 0.0f;
            channels[ch].pitchWaveDepth = 0.0f;
            channels[ch].pitchWaveSpeed = 0.0f;
            channels[ch].pitchWavePoint = 0.0f;

            channels[ch].volume = 1.0f;
            channels[ch].volumeSlide = 0.0f;

            channels[ch].rocketEffect1 = false;
            channels[ch].rocketEffect2 = false;

            channels[ch].arpPitches.clear();
            channels[ch].arpPitches.emplace_back(0);
            channels[ch].arpPitches.shrink_to_fit();
            channels[ch].arpNotes.clear();
            channels[ch].arpNotes.emplace_back(0);
            channels[ch].arpNotes.shrink_to_fit();

            channels[ch].arpIndex = 0;
            channels[ch].bitCrush = 0;

            channels[ch].lowPass = 6400.0f;
            channels[ch].alphaLow = 2.0f * 3.14159265f * channels[ch].lowPass / 48000.0f;
            channels[ch].lowPassSlide = 0.0f;

            channels[ch].highPass = 1.0f;
            float RC = 1.0f / (2.0f * 3.14159265f * channels[ch].highPass);
            channels[ch].alphaHigh = RC / (RC + (1.0f / 48000.0f));
            channels[ch].highPassSlide = 0.0f;
            channels[ch].retriggerLength = 0;
            channels[ch].jumpToFrame = 0;
        }


    }
}




void DrawSampleDisplay()
{
    toDrawSampleDisplay = false;
    int loopStartPos = 0;
    int loopEndPos = 0;
    if (editor.selectedSample < loadedSamples.size())
    {
        loopStartPos = (loadedSamples[editor.selectedSample].loopStart) * (1.0f / sampleDisplay.pcmData.size()) * 2.0f * 800.0f;
        loopEndPos = (loadedSamples[editor.selectedSample].loopEnd) * (1.0f / sampleDisplay.pcmData.size()) * 2.0f * 800.0f;
    }


    for (int x = 0; x < 800; x++)
    {

        float brightness = 1.0f;

        if (editor.selectedSample < loadedSamples.size())
        {
            if (loadedSamples[editor.selectedSample].looping)
            {
                if (x > loopStartPos && x < loopEndPos)
                    brightness *= 0.25f;
            }
        }
        
        for (int y = 0; y < 192 - 16; y++)
        {
            

            sampleDisplay.pixelData[x + 800 * y].r = gui.uiColors[3] * y * brightness;
            sampleDisplay.pixelData[x + 800 * y].g = gui.uiColors[4] * y * 1;
            sampleDisplay.pixelData[x + 800 * y].b = gui.uiColors[5] * y * 1;
        }
        for (int y = 192 - 16; y < 192; y++)
        {
            sampleDisplay.pixelData[x + 800 * y].r = gui.uiColors[3] * (y - 192 + 16) * 16 * brightness;
            sampleDisplay.pixelData[x + 800 * y].g = gui.uiColors[4] * (y - 192 + 16) * 16;
            sampleDisplay.pixelData[x + 800 * y].b = gui.uiColors[5] * (y - 192 + 16) * 16;
        }
        for (int y = 0; y < 16; y++)
        {
            sampleDisplay.pixelData[x + 800 * y].r = gui.uiColors[3] * (y) * 16 * brightness;
            sampleDisplay.pixelData[x + 800 * y].g = gui.uiColors[4] * (y) * 16;
            sampleDisplay.pixelData[x + 800 * y].b = gui.uiColors[5] * (y) * 16;
        }
    }

    if (editor.selectedSample < 0 || editor.selectedSample >= loadedSamples.size())
        return;


    // Load the sample data from memory.
    
    ma_decoder loadingDecoder;
    void* sampleAudioData;
    sampleAudioData = loadedSamples[editor.selectedSample].sampleData.data();
    ma_decoder_init_memory(sampleAudioData, loadedSamples[editor.selectedSample].sampleData.size(), &decoderConfig, &loadingDecoder);


    


    
    
    
    if (sampleDisplay.playingSample)
    {
        ma_uint64 soundPos;
        ma_decoder_get_cursor_in_pcm_frames(&g_pDecoders[0], &soundPos);
        sampleDisplay.samplePos = soundPos;
    }
    else
    {
        bool reading = true;

        sampleDisplay.pcmData.clear();
        sampleDisplay.pcmData.shrink_to_fit();

        while (reading)
        {
            float pcmFrames[2048]; // For stereo f32 output, this would be 2048 samples
            ma_uint64 framesRead;

            ma_result result;

            result = ma_decoder_read_pcm_frames(&loadingDecoder, pcmFrames, 1024, &framesRead);



            for (int i = 0; i < framesRead * 2; i++)
                sampleDisplay.pcmData.emplace_back(pcmFrames[i]);

            if (framesRead < 1024)
                reading = false;
        }

        ma_decoder_uninit(&loadingDecoder);
    }

    


    if (sampleDisplay.pcmData.size() > 0)
    {

        float lastFrameVal = 0;
        float lastFrameVal2 = 0;


        for (int x = 0; x < 800; x++)
        {
            int frameIndex = (float(x) / 800.0f) * (((sampleDisplay.pcmData.size() / sampleDisplay.zoomFactor) * 0.5f) * 2.0f) + sampleDisplay.offset * 2.0f;

            if (frameIndex + 1 >= sampleDisplay.pcmData.size())
                break;

            int frameVal = int(sampleDisplay.pcmData[frameIndex] * 112.0f) + 112.0f;

            if (frameVal > 191)
                frameVal = 191;
            else if (frameVal < 0)
                frameVal = 0;

            int frameVal2 = int(sampleDisplay.pcmData[frameIndex + 1] * 112.0f) + 112.0f;

            if (frameVal2 > 191)
                frameVal2 = 191;
            else if (frameVal2 < 0)
                frameVal2 = 0;

            float brightness = 127.0f;

            if (loadedSamples[editor.selectedSample].looping)
            {
                if (x > loopStartPos && x < loopEndPos)
                    brightness *= 0.5f;
            }

            if (frameVal > lastFrameVal)
            {
                for (int i = lastFrameVal; i < frameVal; i++)
                {
                    sampleDisplay.pixelData[x + 800 * i].r = gui.uiColors[12] * brightness;
                    sampleDisplay.pixelData[x + 800 * i].g = gui.uiColors[13] * 127;
                    sampleDisplay.pixelData[x + 800 * i].b = gui.uiColors[14] * 127;
                }
            }
            else
            {
                for (int i = frameVal; i <= lastFrameVal; i++)
                {
                    sampleDisplay.pixelData[x + 800 * i].r = gui.uiColors[12] * brightness;
                    sampleDisplay.pixelData[x + 800 * i].g = gui.uiColors[13] * 127;
                    sampleDisplay.pixelData[x + 800 * i].b = gui.uiColors[14] * 127;
                }
            }

            if (frameVal2 > lastFrameVal2)
            {
                for (int i = lastFrameVal2; i < frameVal2; i++)
                {
                    sampleDisplay.pixelData[x + 800 * i].r += gui.uiColors[12] * brightness;
                    sampleDisplay.pixelData[x + 800 * i].g += gui.uiColors[13] * brightness;
                    sampleDisplay.pixelData[x + 800 * i].b += gui.uiColors[14] * brightness;
                }
            }
            else
            {
                for (int i = frameVal2; i <= lastFrameVal2; i++)
                {
                    sampleDisplay.pixelData[x + 800 * i].r += gui.uiColors[12] * brightness;
                    sampleDisplay.pixelData[x + 800 * i].g += gui.uiColors[13] * brightness;
                    sampleDisplay.pixelData[x + 800 * i].b += gui.uiColors[14] * brightness;
                }
            }

            lastFrameVal = frameVal;
            lastFrameVal2 = frameVal2;
        }


        // Draw the current position in the sample.
        //ma_uint64 soundPos;
        //ma_decoder_get_cursor_in_pcm_frames(&g_pDecoders[0], &soundPos);
        //int markerPos = (float(x) / 800.0f) * float(int((sampleDisplay.pcmData.size() / sampleDisplay.zoomFactor) * 0.5f) * 2.0f) + sampleDisplay.offset * 2.0f;

        //int frameIndex = (float(x) / 800.0f) * float(int((sampleDisplay.pcmData.size() / sampleDisplay.zoomFactor) * 0.5f)) + sampleDisplay.offset;
        //frameIndex - sampleDisplay.offset = (float(x) / 800.0f) * float(int((sampleDisplay.pcmData.size() / sampleDisplay.zoomFactor) * 0.5f));
        //(frameIndex - sampleDisplay.offset) * float(int((sampleDisplay.zoomFactor / sampleDisplay.pcmData.size()) * 0.5f)) = (float(x) / 800.0f);
        //(frameIndex - sampleDisplay.offset) * float(int((sampleDisplay.zoomFactor / sampleDisplay.pcmData.size()) * 0.5f)) * 800.0f = (float(x));

        //int markerPos = (soundPos - sampleDisplay.offset) * float(int((sampleDisplay.zoomFactor / sampleDisplay.pcmData.size()) * 0.5f)) * 800.0f;

        int markerPos = (sampleDisplay.samplePos) * (1.0f / sampleDisplay.pcmData.size()) * 2.0f * 800.0f;

        if (markerPos > 0 && markerPos < 800)
        {
            for (int y = 0; y < 192; y++)
            {
                sampleDisplay.pixelData[markerPos + 800 * y].r = gui.uiColors[12] * y;
                sampleDisplay.pixelData[markerPos + 800 * y].g = gui.uiColors[13] * y;
                sampleDisplay.pixelData[markerPos + 800 * y].b = gui.uiColors[14] * y;
            }
        }



        // Draw jump points.
        for (int i = 0; i < loadedSamples[editor.selectedSample].jumpPoints.size(); i++)
        {
            int samplePos = (loadedSamples[editor.selectedSample].jumpPoints[i]) * (1.0f / sampleDisplay.pcmData.size()) * 2.0f * 800.0f;
            for (int y = 0; y < 16; y++)
            {
                for (int x = samplePos - abs(y * 0.5); x < samplePos + abs(y * 0.5) + 1; x++)
                {
                    if (x > 0 && x < 800)
                        sampleDisplay.pixelData[x + 800 * (y + 175)] = { 255, 255, 0 };
                }
            }
            for (int y = 0; y < 175; y++)
            {
                sampleDisplay.pixelData[samplePos + 800 * y] = { 255, 255, 0 };
            }
        }

        // Draw loop points.
        if (loadedSamples[editor.selectedSample].looping)
        {
            for (int y = 0; y < 16; y++)
            {
                for (int x = loopStartPos - abs(y * 0.5); x < loopStartPos + abs(y * 0.5) + 1; x++)
                {
                    if (x > 0 && x < 800)
                        sampleDisplay.pixelData[x + 800 * (15 - y)] = { 0, 255, 255 };
                }
            }
            if (loopStartPos < 800)
            {
                for (int y = 0; y < 175; y++)
                {
                    sampleDisplay.pixelData[loopStartPos + 800 * y] = { 0, 255, 255 };
                }
            }

            for (int y = 0; y < 16; y++)
            {
                for (int x = loopEndPos - abs(y * 0.5); x < loopEndPos + abs(y * 0.5) + 1; x++)
                {
                    if (x > 0 && x < 800)
                        sampleDisplay.pixelData[x + 800 * (15 - y)] = { 0, 255, 255 };
                }
            }
            if (loopEndPos < 800)
            {
                for (int y = 0; y < 175; y++)
                {
                    sampleDisplay.pixelData[loopEndPos + 800 * y] = { 0, 255, 255 };
                }
            }
        }


        // Draw the position marker.
        int startPos = (sampleDisplay.sampleStartPos) * (1.0f / sampleDisplay.pcmData.size()) * 2.0f * 800.0f;

        //if (startPos > 0 && startPos < 800)
        {
            for (int y = 0; y < 8; y++)
            {
                for (int x = startPos - abs(y * 0.5); x < startPos + abs(y * 0.5) + 1; x++)
                {
                    if (x > 0 && x < 800)
                    {
                        sampleDisplay.pixelData[x + 800 * (y + 175)] = { 255, 255, 255 };
                        sampleDisplay.pixelData[x + 800 * (15 - y)] = { 255, 255, 255 };
                    }
                }
            }
            for (int y = 0; y < 192; y++)
            {
                sampleDisplay.pixelData[startPos + 800 * y] = { 255, 255, 255 };
            }
        }
    }

    else
    {
        for (int x = 0; x < 800; x++)
        {
            for (int y = 192 - 16; y < 192; y++)
            {
                sampleDisplay.pixelData[x + 800 * y].r = gui.uiColors[3] * (y - 192 + 16) * 16;
                sampleDisplay.pixelData[x + 800 * y].g = 0;
                sampleDisplay.pixelData[x + 800 * y].b = 0;
            }
        }
    }

    return;
}



void GenerateAdditiveWave()
{
    sampleDisplay.pcmData.clear();

    

    ma_uint64 periodLen = 480.0f;

    ma_uint64 framesToWrite = periodLen * 1.0f;


    periodLen *= 1000.0f;


    // 480 samples for low C.

    // 240 samples for middle C.

    std::vector <float> modData;
    modData.assign(framesToWrite * 2, 1.0f);
    modData = ConstructWave(sampleDisplay.modulatorType, modData, sampleDisplay.modFrequencies, periodLen, framesToWrite, 0.5f);

    periodLen = 480.0f;

    sampleDisplay.pcmData = ConstructWave(sampleDisplay.carrierType, modData, sampleDisplay.frequencies, periodLen, framesToWrite, 0.5f);

    

    



    /*
    if (false)
    {
        for (int x = 0; x < framesToWrite; x++)
        {
            sampleDisplay.pcmData[x * 2] *= carrierData[x * 2];
            sampleDisplay.pcmData[x * 2 + 1] *= carrierData[x * 2 + 1];
        }
    }
    else
    {

    }*/

    /*
    for (int x = 0; x < framesToWrite; x++)
    {
        float vol = 0;

        for (int i = 0; i < 30; i++)
        {
            vol += sin(float(x) * float(i + 1) * 6.28312 / periodLen) * sampleDisplay.frequencies[i] * 0.02f;
            //sampleDisplay.frequencies
        }

        sampleDisplay.pcmData.emplace_back(vol);
        sampleDisplay.pcmData.emplace_back(vol);


        
    }*/


    // FM
    /*
    srand(0);
    for (int x = 0; x < framesToWrite; x++)
    {
        //float wave = float((rand() % 100) - 50) * 0.01;
        float vol = 0;

        for (int i = 0; i < 30; i++)
        {
            vol += sin(float(x) * float(i + 1) * 6.28312 / periodLen) * sampleDisplay.frequencies[i] * 0.02f;
            //sampleDisplay.frequencies
        }

        float wave = sin(float(x) * vol * float(3 + 1) * 6.28312 / periodLen) * 5.0f * 0.02f;

        vol = wave;

        sampleDisplay.pcmData.emplace_back(vol);
        sampleDisplay.pcmData.emplace_back(vol);



    }*/
    
    /*
    srand(0);
    for (int x = 0; x < framesToWrite; x++)
    {
        //float wave = float((rand() % 100) - 50) * 0.01;
        float vol = 0;

        float wave = 1;

        if (int(float(x) * 0.01) % 2 == 0)
            wave = -1;

        vol = wave;

        sampleDisplay.pcmData.emplace_back(vol);
        sampleDisplay.pcmData.emplace_back(vol);



    }*/

    srand(0);

    // Triangle
    /*
    for (int x = 0; x < framesToWrite; x++)
    {
        //float wave = float((rand() % 100) - 50) * 0.01;
        float vol = 0;
        

        for (int i = 0; i < 30; i++)
        {
            float waveLen = float(periodLen) / float(i + 1);
            float periodPos = float(x % int(waveLen)) / waveLen;

            if (periodPos > 0.5)
                vol += float(periodPos - 0.5f) * sampleDisplay.frequencies[i] * 0.02f;
            else
                vol += (1.0f - float(periodPos) - 0.5f) * sampleDisplay.frequencies[i] * 0.02f;

            //vol += sin(float(x) * float(i + 1) * 6.28312 / periodLen) * sampleDisplay.frequencies[i] * 0.02f;
            //sampleDisplay.frequencies
        }

        //float wave = sin(float(x) * vol * float(3 + 1) * 6.28312 / periodLen) * 5.0f * 0.02f;

        //vol = wave;

        sampleDisplay.pcmData.emplace_back(vol);
        sampleDisplay.pcmData.emplace_back(vol);



    }*/

    // Square
    /*
    for (int x = 0; x < framesToWrite; x++)
    {
        //float wave = float((rand() % 100) - 50) * 0.01;
        float vol = 0;


        for (int i = 0; i < 30; i++)
        {
            float waveLen = float(periodLen * 2.0f) / float(i + 1);
            float periodPos = float(x % int(waveLen)) / waveLen;

            float dutyCycle = 0.2f;

            if (periodPos >= dutyCycle)
                vol += 1.0f * sampleDisplay.frequencies[i] * 0.02f;
            else
                vol += -1.0f * sampleDisplay.frequencies[i] * 0.02f;

            //vol += sin(float(x) * float(i + 1) * 6.28312 / periodLen) * sampleDisplay.frequencies[i] * 0.02f;
            //sampleDisplay.frequencies
        }

        //float wave = sin(float(x) * vol * float(3 + 1) * 6.28312 / periodLen) * 5.0f * 0.02f;

        //vol = wave;

        sampleDisplay.pcmData.emplace_back(vol);
        sampleDisplay.pcmData.emplace_back(vol);



    }*/

    // Saw
    /*
    for (int x = 0; x < framesToWrite; x++)
    {
        //float wave = float((rand() % 100) - 50) * 0.01;
        float vol = 0;


        for (int i = 0; i < 30; i++)
        {
            float waveLen = float(periodLen) / float(i + 1);
            float periodPos = float(x % int(waveLen)) / waveLen;

            vol += float(periodPos - 0.5f) * sampleDisplay.frequencies[i] * 0.02f;

            //vol += sin(float(x) * float(i + 1) * 6.28312 / periodLen) * sampleDisplay.frequencies[i] * 0.02f;
            //sampleDisplay.frequencies
        }

        //float wave = sin(float(x) * vol * float(3 + 1) * 6.28312 / periodLen) * 5.0f * 0.02f;

        //vol = wave;

        sampleDisplay.pcmData.emplace_back(vol);
        sampleDisplay.pcmData.emplace_back(vol);



    }*/





    // Encode the new waveform to the selected sample.
    // I was going to save samples directly to memory, but I spent an inordinate amount of time trying to figure it out. It's infuriating.
    ///////////////////////////////////////////////////////////////


    // Encode the waveform to a .bin file.
    // Make very sure that a file with the temporary file's name does not already exist.
    std::string fileName = "newSample";
    while (std::filesystem::exists(fileName + ".wav"))
        fileName += "1";

    fileName += ".wav";
    const char* name = &fileName[0];
    ma_encoder_init_file(name, &encoderConfig, &encoder);

    //void* sampleAudioData;
    const void* sampleAudioData = static_cast<const void*>(sampleDisplay.pcmData.data());
    //sampleAudioData = loadedSamples[editor.selectedSample].sampleData.data();

    ma_uint64 framesWritten;
    
    ma_encoder_write_pcm_frames(&encoder, sampleAudioData, framesToWrite, &framesWritten); // Write frames to file if recording.
    ma_encoder_uninit(&encoder);


    // Save the .bin file as a sample.
    Sample newSample;
    newSample.sampleName = loadedSamples[editor.selectedSample].sampleName;
    newSample.filePath = fileName;
    newSample.looping = loadedSamples[editor.selectedSample].looping;
    newSample.loopStart = loadedSamples[editor.selectedSample].loopStart;
    newSample.loopEnd = loadedSamples[editor.selectedSample].loopEnd;
    newSample.attack = loadedSamples[editor.selectedSample].attack;
    newSample.decay = loadedSamples[editor.selectedSample].decay;
    newSample.sustain = loadedSamples[editor.selectedSample].sustain;
    newSample.release = loadedSamples[editor.selectedSample].release;


    loadedSamples.erase(loadedSamples.begin() + editor.selectedSample);
    loadedSamples.shrink_to_fit();
    loadedSamples.emplace(loadedSamples.begin() + editor.selectedSample, newSample);
    loadedSamples[editor.selectedSample].LoadSampleData();

    // Delete the .bin file.
    remove(name);
    
        

    //ma_encoder_init(my_write_proc(), NULL, )

    /*
    void* sampleAudioData;
    sampleAudioData = loadedSamples[editor.selectedSample].sampleData.data();
    ma_uint64 framesWritten;
    ma_uint64 framesToWrite = ma_uint64(sampleDisplay.pcmData.size());
    float* pInputF32 = (float*)sampleDisplay.pcmData.data();
    unsigned long long framesWritten2;

    //ma_result result = ma_encoder_init(onWrite(&encoder, sampleAudioData, framesToWrite, &framesWritten2), onSeek, sampleAudioData, &encoderConfig, &encoder);

    ma_result result = ma_encoder_init(onWrite, onSeek, sampleAudioData, &encoderConfig, &encoder);

    //ma_encoder_init(loadedSamples[editor.selectedSample].my_write_proc(), NULL, sampleAudioData, &encoderConfig, &encoder);
    
    //ma_encoder_init(my_write_proc(sampleAudioData, pOutputF32, framesToWrite * 2, &framesWritten2), NULL, sampleAudioData, &encoderConfig, &encoder);
    //ma_result result = ma_encoder_init(NULL, NULL, sampleAudioData, &encoderConfig, &encoder);

    if (result != MA_SUCCESS) {
        std::cout << "Not yet";
    }
    
    ma_encoder_write_pcm_frames(&encoder, pInputF32, framesToWrite, &framesWritten); // Write frames to file if recording.
    ma_encoder_uninit(&encoder);
    */

    //read_and_mix_pcm_frames_f32(&g_pDecoders[channel], pOutputF32, channelFrameCount, channel, channels[channel].startBetweenFrames);

    //std::string fileName = "C:/" + fileNavigator.currentFilePath.std::filesystem::path::string() + "/" + loadedSong.songName + ".wav";
    //std::string fileName = "Export/" + loadedSong.songName + ".wav";
    //const char* name = &fileName[0];
    //ma_encoder_init_file(name, &encoderConfig, &encoder);
    //ma_encoder_init()


    //void* sampleAudioData;
    //sampleAudioData = loadedSamples[editor.selectedSample].sampleData.data();


    //ma_decoder_init_memory(sampleAudioData, loadedSamples[editor.selectedSample].sampleData.size(), &decoderConfig, &g_pDecoders[channel]);

    
    
    
    DrawSampleDisplay();


    toGenerateAdditiveWave = false;

    return;
}



std::vector <float> ConstructWave(int waveType, std::vector <float> modulator, float frequencies[30], float periodLen, float framesToWrite, float modPower)
{
    

    std::vector <float> carrierData;
    //carrierData.assign(framesToWrite * 2, 1.0f);

    // 480 samples for low C.

    // 240 samples for middle C.

    

    srand(0);

    for (int x = 0; x < framesToWrite; x++)
    {
        float modPeriod = float(periodLen) * (modulator[x] * modPower + 1.0f * (1.0f - modPower));
        if (abs(modPeriod) < 0.01f)
        {
            if (modPeriod > 0.0f)
                modPeriod = 0.01f;
            else
                modPeriod = -0.01f;
        }

        if (waveType == -1) // Empty wave
        {

            carrierData.emplace_back(1.0f);
            carrierData.emplace_back(1.0f);
        }
        else if (waveType == 0) // Sine wave
        {
            float vol = 0;

            for (int i = 0; i < 30; i++)
            {
                vol += sin(float(x) * float(i + 1) * 6.28312 / modPeriod) * frequencies[i] * 0.02f;
                //sampleDisplay.frequencies
            }

            carrierData.emplace_back(vol);
            carrierData.emplace_back(vol);
        }
        else if (waveType == 1) // Square wave
        {
            //float wave = float((rand() % 100) - 50) * 0.01;
            float vol = 0;


            for (int i = 0; i < 30; i++)
            {
                float waveLen = float(modPeriod * 2.0f) / float(i + 1);

                float periodPos = float(x);

                while (periodPos > waveLen)
                    periodPos -= waveLen;

                periodPos /= waveLen;
                
                
                //float(x % int(waveLen)) / waveLen;

                float dutyCycle = 0.5f;

                if (periodPos >= dutyCycle)
                    vol += 1.0f * frequencies[i] * 0.02f;
                else
                    vol += -1.0f * frequencies[i] * 0.02f;

                //vol += sin(float(x) * float(i + 1) * 6.28312 / periodLen) * sampleDisplay.frequencies[i] * 0.02f;
                //sampleDisplay.frequencies
            }

            //float wave = sin(float(x) * vol * float(3 + 1) * 6.28312 / periodLen) * 5.0f * 0.02f;

            //vol = wave;

            carrierData.emplace_back(vol);
            carrierData.emplace_back(vol);
        }
        else if (waveType == 2) // Triangle wave
        {
            //float wave = float((rand() % 100) - 50) * 0.01;
            float vol = 0;


            for (int i = 0; i < 30; i++)
            {
                float waveLen = float(modPeriod) / float(i + 1);
                float periodPos = float(x % int(waveLen)) / waveLen;

                if (periodPos > 0.5)
                    vol += float(periodPos - 0.5f) * frequencies[i] * 0.02f;
                else
                    vol += (0.5f - float(periodPos)) * frequencies[i] * 0.02f;

                //vol += sin(float(x) * float(i + 1) * 6.28312 / periodLen) * sampleDisplay.frequencies[i] * 0.02f;
                //sampleDisplay.frequencies
            }

            //float wave = sin(float(x) * vol * float(3 + 1) * 6.28312 / periodLen) * 5.0f * 0.02f;

            //vol = wave;

            carrierData.emplace_back(vol);
            carrierData.emplace_back(vol);
        }
        else if (waveType == 3) // Saw wave
        {
            //float wave = float((rand() % 100) - 50) * 0.01;
            float vol = 0;


            for (int i = 0; i < 30; i++)
            {
                float waveLen = float(modPeriod) / float(i + 1);
                float periodPos = float(x % int(waveLen)) / waveLen;

                vol += float(periodPos - 0.5f) * frequencies[i] * 0.02f;

                //vol += sin(float(x) * float(i + 1) * 6.28312 / periodLen) * sampleDisplay.frequencies[i] * 0.02f;
                //sampleDisplay.frequencies
            }

            //float wave = sin(float(x) * vol * float(3 + 1) * 6.28312 / periodLen) * 5.0f * 0.02f;

            //vol = wave;

            carrierData.emplace_back(vol);
            carrierData.emplace_back(vol);
        }
        else if (waveType == 4) // Noise
        {
            float vol = 0;


            for (int i = 0; i < 30; i++)
            {
                float wave = float((rand() % 100) - 50) * 0.01;


                vol += wave * sampleDisplay.frequencies[i] * 0.02f;

                carrierData.emplace_back(vol);
                carrierData.emplace_back(vol);
            }
        }

    }


    return carrierData;
}