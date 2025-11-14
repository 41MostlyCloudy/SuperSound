#pragma once



#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono> // Deals with time
#include <stdlib.h> // For rand()

// For searching file directories. New as of c++ 17.
#include <filesystem>
#include <cstdlib>



#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Floating-point 2d position
struct Vector2 { float x = 0; float y = 0; };



// A square of the screen GUI
struct UIElement
{
	Vector2 sprite;
	int textCol = 0; int bgCol = 0; // Colors for text and background.
};


struct RGBColor
{
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
};



struct RGBNorm
{
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
};





// An audio sample
struct Sample
{
	std::string sampleName = "NewSample";
	std::string filePath = "";
	std::vector <char> sampleData;
	std::vector <int> jumpPoints;
	bool looping = false;
	int loopStart = 0;
	int loopEnd = 0;
	
	//int sampleRateDivision = 1; // The division of a regular 48000 sample rate that the sample uses.


	// Envelope
	float attack = 0.0f;
	float decay = 0.0f;
	float sustain = 0.0f;
	float release = 0.0f;


	void LoadSampleData()
	{
		std::string fileName = filePath;



		
		std::ifstream sampleFile(fileName, std::ios::binary | std::ios::ate);

		if (sampleFile)
		{
			std::streamsize fileSize = sampleFile.tellg();
			sampleFile.seekg(0, std::ios::beg);



			sampleData.resize(fileSize);
			sampleFile.read(sampleData.data(), fileSize);
		}
		else
		{
			std::cout << "file not reached. ";
		}

		sampleFile.close();



		return;
	}

	
};



struct GUITheme
{
	RGBColor uiColors[9];
};


struct ScrollBar
{
	Vector2 topLeft;
	float position = 0.0f;
	float length = 10.0f;
	bool horizontal = false;
	bool drag = false;
};


struct Dial
{
	Vector2 position;
	float rotation = 0.0f; // The knobs position from 0 to 1.
	bool drag = false;
	int channel = -1; // The number of the channel it is attached to. (-1 if not attached to a channel.)
};


struct GUI
{
	// Scrollbars
	std::vector <ScrollBar> scrollBars =
	{
	{ { 6, 3 }, 0.0f, 8, false, false }, // frameMenuScroll
	{ { 91, 3 }, 0.0f, 8, false, false }, // sampleMenuScroll
	{ { 91, 17 }, 0.0f, 38, false, false }, // NoteScroll
	{ { 6, 56 }, 0.0f, 84, true, false }, // NoteScrollHorizontal
	{ { -1, -1 }, 0.0f, 17, false, false }, // fileScroll
	//{ { -1, -1 }, 0.0f, 49, true, false }, // sampleEditorScroll
	};

	std::vector <Dial> dials =
	{
		{}, {}, {}, {} // The sample editor ADSR filter.
	};


	bool clickingOnFloatingWind = false;

	bool drawScreen = false;
	// Tells the program to draw certain parts of the screen.
	bool drawUIThisFrame = true;
	bool drawFrameThisFrame = true;

	// 92 x 57 tile ui.
	UIElement activeUI[92][57]; // Active user interface elements on the screen.

	Vector2 hoveredTile;
	Vector2 floatHoveredTile;
	Vector2 selectedTile;

	int frameListScroll = 0;
	int fileListScroll = 0;
	int sampleListScroll = 0;
	Vector2 frameScroll;



	// GUI Themes:
	//		Red, Rust, Sand, Forest, Aquatic, Blue, Cosmic, Steel
	int uiColorTheme = 5;
	std::vector <GUITheme> themes = {};
	bool pianoMode = false;

	unsigned int uiTexture;

	float uiColors[54] =
	{
		0.0, 0.0, 0.0, // Blue/Gray\n"
		30.0 / 255.0, 30.0 / 255.0, 60.0 / 255.0,
		50.0 / 255.0, 50.0 / 255.0, 100.0 / 255.0,
		90.0 / 255.0, 90.0 / 255.0, 140.0 / 255.0,
		150.0 / 255.0, 150.0 / 255.0, 200.0 / 255.0,
		1.0, 1.0, 1.0,

		63.0 / 255.0, 100.0 / 255.0, 100.0 / 255.0, // Green/Yellow
		127.0 / 255.0, 190.0 / 255.0, 0.0,
		1.0, 1.0, 0.0,

		100.0 / 255.0, 0.0, 60.0 / 255.0, // Red
		160.0 / 255.0, 0.0, 40.0 / 255.0,
		1.0, 0.0, 0.0,

		0.0, 0.0, 120.0 / 255.0, // Blue
		0.0, 90.0 / 255.0, 180.0 / 255.0,
		0.0, 180.0 / 255.0, 1.0,

		63.0 / 255.0, 100.0 / 255.0, 100.0 / 255.0, // Accent colors
		127.0 / 255.0, 190.0 / 255.0, 0.0,
		1.0, 1.0, 0.0,
	};


	
};


struct Screen
{
	GLFWwindow* window;

	Vector2 screenSize;
	float windowRatio = 1.0f;

	Vector2 mouseCoords;
	bool mouseDown = false;
	bool rightMouseDown = false;
	float mouseHoldTime = 0.0f; // The amount of time that the mouse has been held down.
	bool keyDown = false;
};


struct Editor
{
	bool playingSong = false;
	bool recordingSong = false;
	bool focusOnNotes = false; // Whether pressing keys will create notes.
	bool channelFocus = false; // When enabled, only one channel will be uncompressed at a time.


	// When a channel is being dragged to a new location.
	int channelBeingMoved = -1;
	int channelMoveTo = -1;

	// Editing settings
	int selectedOctave = 3;
	// Selectable Buttons:
	// 0 = Song name, 1 = BPM, 2 = TPB, 3 = OCT, 4 = ROW, 5 = BEAT, 6 = Artist name, 7 = notes window
	int selectedButton = -1;
	int selectedSample = 0;
	int selectedKey = -1;
	int selectedEffect = -1;
	int selectedFile = 0;

	Vector2 noteSelectionStart;
	Vector2 noteSelectionEnd;
	int transposeValue = 0; // The number of semitones to transpose by if the transpose window is used.

	std::string fileToLoad = "";

	std::string songNotes[20] = {};



	int findNotePlayed(int input)
	{
		int noteNum = -1;

		if (input == 122) noteNum += 1; // z (C 1)
		else if (input == 115) noteNum += 2; // s (C# 1)	
		else if (input == 120) noteNum += 3; // x (D 1)
		else if (input == 100) noteNum += 4; // d (D# 1)
		else if (input == 99) noteNum += 5; // c (E 1)
		else if (input == 118) noteNum += 6; // v (F 1)
		else if (input == 103) noteNum += 7; // g (F# 1)
		else if (input == 98) noteNum += 8; // b (G 1)
		else if (input == 104) noteNum += 9; // h (G# 1)
		else if (input == 110) noteNum += 10; // n (A 2)
		else if (input == 106) noteNum += 11; // j (A# 2)
		else if (input == 109) noteNum += 12; // m (B 2)
		else if (input == 44) noteNum += 13; // , (C 2)
		else if (input == 108) noteNum += 14; // l (C# 2)
		else if (input == 46) noteNum += 15; // . (D 2)
		else if (input == 59) noteNum += 16; // ; (D# 2)
		else if (input == 47) noteNum += 17; // / (E 2)

		selectedKey = noteNum;


		if (input == 113) noteNum += 13; // q (C 2)
		else if (input == 50) noteNum += 14; // 2 (C# 2)
		else if (input == 119) noteNum += 15; // w (D 2)
		else if (input == 51) noteNum += 16; // 3 (D# 2)
		else if (input == 101) noteNum += 17; // e (E 2)
		else if (input == 114) noteNum += 18; // r (F 2)
		else if (input == 53) noteNum += 19; // 5 (F# 2)
		else if (input == 116) noteNum += 20; // t (G 2)
		else if (input == 54) noteNum += 21; // 6 (G# 2)
		else if (input == 121) noteNum += 22; // y (A 3)
		else if (input == 55) noteNum += 23; // 7 (A# 3)
		else if (input == 117) noteNum += 24; // u (B 3)
		else if (input == 105) noteNum += 25; // i (C 3)
		else if (input == 57) noteNum += 26; // 9 (C# 3)
		else if (input == 111) noteNum += 27; // o (D 3)
		else if (input == 48) noteNum += 28; // 0 (D# 3)
		else if (input == 112) noteNum += 29; // p (E 3)
		else if (input == 91) noteNum += 30; // [ (F 3)
		else if (input == 61) noteNum += 31; // = (F# 3)
		else if (input == 93) noteNum += 32; // ] (A 3)

		if (selectedKey == -1)
		{
			if (noteNum == -1)
				selectedKey = -1;
			else
				selectedKey = noteNum + 5;
		}
		if (noteNum != -1)
		{
			noteNum += selectedOctave * 12;

			if (noteNum > 84)
				noteNum = 84;

		}

		return(noteNum);
	}
};


// Audio channel
struct Channel
{
	bool muted = false;
	bool compressed = false;
	bool loopNote = true;
	bool toUninitialize = false;

	// Synthesis
	bool isSynth = false;
	bool isModulator = false;
	bool fmSynth = false; // AM or FM synthesis if used as a modulator.
	float modulatorStrength = 0.0f;

	float startBetweenFrames = 0.0f;

	float panValue = 0; // -/1 (0/80)
	float panSlide = 0; // -1/1 (0/8   0/8)

	float volume = 1.0f; // 0/1 (0/80)
	float volumeSlide = 0.0f; // -1/1 (0/8   0/8)
	float volumeWaveDepth = 0.0f; // 0/1
	float volumeWaveSpeed = 0.0f;
	float volumeWavePoint = 0.0f;
	float volumeWaveOffset = 0.0f;

	float pitch = 0.0f;
	float pitchSlide = 0.0f; // -1/1 (0/8   0/8)
	float pitchWaveDepth = 0.0f; // 0/1
	float pitchWaveSpeed = 0.0f; // 0/1
	float pitchWavePoint = 0.0f; // 0/1
	float pitchWaveOffset = 0.0f; // 0/1

	std::vector <int> arpNotes = {0};
	std::vector <float> arpPitches = {0};
	int arpIndex = 0;

	bool rocketEffect1 = false;
	bool rocketEffect2 = false;
	int rocketAdd = 0;
	int rocketSub = 0;

	int bitCrush = 0;

	bool delayNote = false;
	int delayLength = 0;

	int retriggerLength = 0;
	int retriggerIndex = 0;
	
	int jumpToFrame = 0;
	
	float instrument = 0;


	// Envelope
	float envelopeVolume = 0.0f;
	float envelopeTime = 0.0f;

	

	float averageVolL = 0.0f; // The volume of the channel averaging frames.
	float averageVolR = 0.0f; // The volume of the channel averaging frames.


	//std::vector <int> loopNoteIndex = {};
	//std::vector <int> loopVolumeIndex = {};
	//std::vector <int> loopEffectIndex = {};


	int noteToPlaySample = 0;
	float noteToPlayPitch = 0;

	int effectCountPerRow = 1; // The number of effects displayed on each line.


	float lowPass = 6400.0f;
	float lowPassSlide = 0.0f;
	float alphaLow = 1.0f;
	float highPass = 1.0f;
	float highPassSlide = 0.0f;
	float alphaHigh = 1.0f;



	float prevLowPassSampleL = 0.0f;      // Previous output sample (for filtering)
	float prevLowPassSampleR = 0.0f;      // Previous output sample (for filtering)
	float prevHighPassSampleL = 0.0f;      // Previous output sample (for filtering)
	float prevHighPassSampleR = 0.0f;      // Previous output sample (for filtering)
	float prevHighPassSampleLI = 0.0f;      // Previous output sample (for filtering)
	float prevHighPassSampleRI = 0.0f;      // Previous output sample (for filtering)
};



// 1 channel of a frame.
struct FrameChannel
{
	std::vector <int> notes;
	std::vector <int> volumes;
	std::vector <int> effects;
};



// Song frame object
struct Frame
{
	uint8_t rows = 32; // Length of the frame in beats.
	int beatsPerMeasure = 4;
	std::vector <FrameChannel> channels;
};




struct UnrolledEffects
{
	std::vector <int> cEffect = { -1 };
	std::vector <int> cEffectValue = { -1 };
};



// 1 row of an unrolled frame.
struct FrameRow
{
	std::vector <int> note = { };
	std::vector <int> instrument = { };
	std::vector <int> volume = { };
	std::vector <UnrolledEffects> effects = { };
	//std::vector <int> effect = { };
	//std::vector <int> effectValue = { };
};



struct UnrolledFrame
{
	// Frame format
	std::vector <FrameRow> rows;
	int beatsPerMeasure = 4;
};



struct Song
{
	std::string songName = "NewSong1";
	std::string artistName = "Me";
	std::string notes = "";

	float startingBPM = 120;
	float bpm = 120;
	float bpmSlide = 0.0f;
	float startingTPB = 6;
	float ticksPerBeat = 6;

	std::vector <int> frameSequence;

	std::vector <Frame> frames;



	int currentFrame = 0; // Current frame in frameSequence.
	int currentNote = 0;
	float timeInNote = 0;
	float timeInTick = 0;
	float timeInSong = 0;

	int numberOfChannels = 1;

	std::vector <float> toNextChannelNote = { };
	std::vector <float> toNextChannelVolume = { };
	std::vector <float> toNextChannelEffect = { };

	std::vector <float> noteChannelIndex = { };
	std::vector <float> volumeChannelIndex = { };
	std::vector <float> effectChannelIndex = { };


	bool unsavedChanges = false;
	bool overWriteOldSong = false;
	bool unusedFrames = false;
};



struct FloatingWindow
{
	std::string name = "Floating Window";
	Vector2 position;
	Vector2 size;
	bool dragWindow = false;
	Vector2 dragPoint;
};



struct WindowController
{
	std::vector <FloatingWindow> windows;


	

	bool HasWindow(std::string windName)
	{
		bool exists = false;

		for (int i = 0; i < windows.size(); i++)
		{
			if (windows[i].name == windName)
				exists = true;
		}

		return exists;
	};


	void InitializeWindow(std::string windowName, Vector2 windPos, Vector2 windSize)
	{
		if (HasWindow(windowName)) // Don't create copies of existing windows.
			return;

		FloatingWindow newWindow;

		for (int i = 0; i < windows.size(); i++) // Move the window so that it doesn't fall on the exact same spot as old ones.
		{
			if (windows[i].position.x == windPos.x)
				windPos.x++;
			if (windows[i].position.y == windPos.y)
				windPos.y++;
		}

		if (windPos.x < 0) windPos.x = 0;
		if (windPos.x + windSize.x > 91) windPos.x = 91 - windSize.x;
		if (windPos.y < 0) windPos.y = 0;
		if (windPos.y + windSize.y > 56)windPos.y = 56 - windSize.y;

		if (windSize.x < windowName.length() + 6) windSize.x = windowName.length() + 6;
		if (windSize.y < 3)  windSize.y = 3;
		if (windSize.y > 56)  windSize.y = 56;




		newWindow.name = windowName;
		newWindow.position = windPos;
		newWindow.size = windSize;


		windows.emplace(windows.begin(), newWindow);

		return;
	}
};


struct SampleDisplay
{
	bool visible = false;

	Vector2 position;
	RGBColor* pixelData = new RGBColor[800 * 192]; // Screen data (608 x 160) pixels.
	std::vector <float> pcmData;

	float zoomFactor = 1.0f; // Increase to zoom in on the sample.
	int offset = 0;

	bool playingSample = false;
	bool pauseSample = false;
	int samplePos = 0;
	int sampleStartPos = 0;

	float frequencies[32];
	float modFrequencies[32];


	int knobSelected = -1; // 0=A, 1=D, 2=S, 3=R
	


	// Wave types: Sine, Square, Triangle, Saw, Noise
	int modulatorType = -1;
	int carrierType = 0; // -1: Constantly open (so that the modulator can be used as a standalone wave.)

	bool modulatorSelected = false;

	float carrierVolume = 0.0f; // This is a value from 0 to 1.
	float carrierPitch = 1.0f; // This value is a pitch multiplier from the pitch table, obtained through a note input.
};



struct FileNavigator
{
	std::filesystem::path currentFilePath;

	std::string filePathName = "";

	std::vector <std::string> fileNames = {};



	// Currently, samples are saved with absolute file paths, making transferring songs between computers difficult.


	void NavigateToFile()
	{
		std::string pathName = "C:/" + currentFilePath.std::filesystem::path::string();

		filePathName = " " + currentFilePath.std::filesystem::path::filename().generic_string();

		fileNames.clear();
		for (auto const& dir_entry : std::filesystem::directory_iterator(pathName))
		{
			std::string sampleName;
			if (!dir_entry.path().std::filesystem::path::has_extension())
				sampleName = "0" + dir_entry.path().std::filesystem::path::filename().generic_string();
			else if (dir_entry.path().std::filesystem::path::extension() == ".song")
				sampleName = "1" + dir_entry.path().std::filesystem::path::filename().generic_string();
			else if (dir_entry.path().std::filesystem::path::extension() == ".wav")
				sampleName = "2" + dir_entry.path().std::filesystem::path::filename().generic_string();
			else
				sampleName = "3" + dir_entry.path().std::filesystem::path::filename().generic_string();

			fileNames.emplace_back(sampleName);
		}

		



		

		return;
	}

	void ExitFile()
	{
		currentFilePath = currentFilePath.std::filesystem::path::parent_path();
		
		NavigateToFile();

		return;
	}

	void EnterFile(std::string fileName)
	{
		currentFilePath = currentFilePath.std::filesystem::path::append(fileName);

		NavigateToFile();

		return;
	}

	void NavigateToSamplesFile()
	{
		currentFilePath = std::filesystem::relative("Samples", "C:/");

		NavigateToFile();

		return;
	}

};






// Effects:

// -1 - No effect:

// 0 - Cosmic Arp: (Not Implemented)
//		Play alternating notes every tick, between the base note and the two offsets.
//		-x = semitone offset 1
//		-y = semitone offset 2

// 2 - Vibrato: (Not Implemented)
//		Change the note pitch over time.
//		-x = speed (1 cycle in x loops)
//		-y = depth (y/4 semitones)

// 3 - Tremolo: (Not Implemented)
//		Change the note volume over time.
//		-x = speed (1 cycle in x loops)
//		-y = depth (y/4 semitones)

// 4 - Set Panning:
//		Set the stereo panning position from 00(Left) to FF(Right).
//		-xy = position

// 5 - Volume Slide:
//		Slide the volume up by x speed or down by y speed.
//		-x = volume up (x/4 semitones per beat)
//		-y = volume down (x/4 semitones per beat)

// 6 - Pitch Slide: (Not Implemented)
//		Slide the pitch up by x speed or down by y speed.
//		-x = pitch up (x semitones per beat)
//		-y = pitch down (x semitones per beat)

// 7 - Pitch Slide (fine): (Not Implemented)
//		Slide the pitch up by x speed or down by y speed.
//		-x = speed up (x/4 semitones per beat)
//		-y = speed down (x/4 semitones per beat)

// 8 - Jump to frame:
//		Jump the the specified frame.
//		-xx = frame

// 9 - Note delay:
//		Start the note late, after x ticks.
//		-xx = ticks

// 10 - Sample offset: (Not Implemented)
//		Offset the sample starting on the beat by x ticks.
//		-xx = ticks

// 11 - Set BPM:
//		Set the song BPM to x.
//		-xx = BPM

// 12 - Set TPB:
//		Set the song TPB to x.
//		-xx = TPB


/////////////////////////////////////////// FUTURE



// 13 - Low Pass:
//		A Low pass filter.
//		-xx size.
// 
// 14 - High Pass:
//		A High pass filter.
//		-xx size.
// 
// 15 - Loop section:
//		Loop the next x beats y times.
//		Other notes, instruments, volumes and effects play over the repeated beats and take priority.
//		The loop ends when another loop effect is encountered. (A loop of length 0 stops loops entirely).
//		-xx = Number of beats.

// 16 - (To be decided)