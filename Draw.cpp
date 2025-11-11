#pragma once





void DrawEverything();

void DrawTopUI();
void DrawKeyBoard();
void DrawPiano();

void DrawKey(int button, int key, Vector2 pos);
void DrawPianoKey(Vector2 pos, int key, bool blackKey);

void DrawMute();
void DrawChannelNumber(int channelNum, int offsetX);

void DrawFrameBorder();
void DrawVolumeBars();

void DrawChannel(int channelNum, int offsetX);
void DrawChannelCompressed(int channelNum, int offsetX);

void DrawText(std::string text, int textStart, int textEnd, int textY, int textCol, int bgCol);
void DrawText(std::string text, int textStart, int textEnd, int textY, int textCol, int bgCol, int selectedLetter, int buttonIndex);
void DrawText(std::string text, int textStart, int textEnd, int textY, int textCol, int bgCol, bool continueDown);

void DrawNum(int text, int textStart, int textEnd, int textY, int textCol, int bgCol);
void DrawNum(int text, int textStart, int textEnd, int textY, int textCol, int bgCol, int selectedLetter, int buttonIndex);

void DrawHex(uint8_t num, int textStart, int textY, int textCol, int bgCol);
void DrawHex(uint8_t num, int textStart, int textY, int textCol, int bgCol, int selectedLetter, int buttonIndex);

void DrawNote(uint8_t num, int textStart, int textY, int textCol, int bgCol, int selectedLetter, int buttonIndex);

void DrawFloatingWindow(FloatingWindow* wind);

void DrawWaveTypeButton(int type, int xVal, int yVal);

void DrawKnob(Vector2 pos, float val);



void DrawEverything()
{
	// Scroll vertically when playing the song.
	if (editor.playingSong)
	{
		gui.frameScroll.y = loadedSong.currentNote - 8;

		if (gui.frameScroll.y > float(loadedFrame.rows.size()) - 40)
			gui.frameScroll.y = float(loadedFrame.rows.size()) - 40;


		if (gui.frameScroll.y < 0)
			gui.frameScroll.y = 0;

		if (gui.frameScroll.y > float(loadedFrame.rows.size() - 1))
			return;
	}

	gui.drawScreen = true;

	if (gui.drawFrameThisFrame)
	{
		DrawMute();
		DrawFrameBorder();
		
		int xStart = 5;
		for (int i = 0; i < loadedSong.numberOfChannels; i++)
		{
			if (channels[i].compressed)
				DrawChannelCompressed(i, xStart - gui.frameScroll.x);
			else
				DrawChannel(i, xStart - gui.frameScroll.x);

			if (channels[i].compressed)
				xStart += 3;
			else
				xStart += 8 + channels[i].effectCountPerRow * 5;
		}

		DrawTopUI();
	}
	else if (gui.drawUIThisFrame)
	{
		DrawTopUI();
	}

	DrawVolumeBars();


	for (int wind = windowController.windows.size() - 1; wind > -1; wind--) // Draw windows.
		DrawFloatingWindow(&windowController.windows[wind]);


	return;
}



void  DrawTopUI()
{
	for (int y = 0; y < 12; y++)
	{
		for (int x = 0; x < 92; x++)
		{
			gui.activeUI[x][y].sprite = { 15, 0 };
			gui.activeUI[x][y].bgCol = 0;
		}
	}


	// Song editor
	DrawText("Song", 1, 5, 0, 3, 0);
	DrawText("Num", 7, 10, 4, 3, 0);
	DrawText("Add", 7, 10, 7, 3, 0);

	// Frame list
	gui.activeUI[0][1].sprite = { 0, 3 };
	gui.activeUI[1][1].sprite = { 2, 3 };
	gui.activeUI[2][1].sprite = { 2, 3 };
	gui.activeUI[3][1].sprite = { 3, 3 };
	gui.activeUI[4][1].sprite = { 2, 3 };
	gui.activeUI[5][1].sprite = { 2, 3 };
	gui.activeUI[6][1].sprite = { 1, 3 };

	gui.activeUI[0][12].sprite = { 0, 4 };
	gui.activeUI[1][12].sprite = { 2, 3 };
	gui.activeUI[2][12].sprite = { 2, 3 };
	gui.activeUI[3][12].sprite = { 4, 4 };
	gui.activeUI[4][12].sprite = { 3, 3 };
	gui.activeUI[6][12].sprite = { 4, 4 };

	for (int y = 2; y < 12; y++)
	{
		gui.activeUI[0][y].sprite = { 2, 4 };
		gui.activeUI[3][y].sprite = { 2, 4 };
	}
	for (int y = 3; y < 10; y++)
		gui.activeUI[6][y].sprite = { 31, 3 };

	gui.activeUI[6][3].sprite = { 31, 2 };
	gui.activeUI[6][10].sprite = { 31, 4 };

	for (int y = 0; y < 10; y++)
	{
		if (y + gui.frameListScroll < loadedSong.frameSequence.size())
		{
			
			if (y + gui.frameListScroll == loadedSong.currentFrame)
			{
				DrawHex(loadedSong.frameSequence[y + gui.frameListScroll], 4, 2 + y, 5, 3, -1, -1);
				DrawHex(y + int(gui.frameListScroll), 1, 2 + y, 4, 3, -1, -1);
				gui.activeUI[3][y + 2].sprite = { 5, 4 };
			}
			else
			{
				DrawHex(loadedSong.frameSequence[y + gui.frameListScroll], 4, 2 + y, 5, 0, -1, -1);
				DrawHex(y + int(gui.frameListScroll), 1, 2 + y, 3, 0, -1, -1);
			}
		}
		else
		{
			gui.activeUI[4][y + 2].sprite = { 15, 0 };
			gui.activeUI[4][y + 2].bgCol = 0;
			gui.activeUI[5][y + 2].sprite = { 15, 0 };
			gui.activeUI[5][y + 2].bgCol = 0;
			DrawHex(y + int(gui.frameListScroll), 1, 2 + y, 3, 0, -1, -1);
		}
	}

	

	// Edit button box
	for (int x = 7; x < 10; x++)
	{
		gui.activeUI[x][3].sprite = { 2, 3 };
		gui.activeUI[x][9].sprite = { 2, 3 };
	}
	for (int y = 4; y < 9; y++)
	{
		gui.activeUI[10][y].sprite = { 2, 4 };
	}
	gui.activeUI[10][3].sprite = { 1, 3 };
	gui.activeUI[10][9].sprite = { 1, 4 };


	// Song/Frame settings box
	DrawText("Settings", 11, 21, 0, 3, 0);
	gui.activeUI[10][1].sprite = { 0, 3 };
	gui.activeUI[18][1].sprite = { 1, 3 };

	
	for (int x = 11; x < 18; x++)
	{
		gui.activeUI[x][1].sprite = { 2, 3 };
	}
	for (int y = 2; y < 12; y++)
	{
		gui.activeUI[10][y].sprite = { 2, 4 };
		gui.activeUI[18][y].sprite = { 2, 4 };
	}

	gui.activeUI[10][3].sprite = { 4, 3 };
	gui.activeUI[10][9].sprite = { 4, 3 };

	// Buttons
	DrawText("BPM", 11, 14, 2, 3, 0);
	DrawNum(loadedSong.bpm, 15, 18, 2, 4, 0, -1, 1);
	DrawText("TPB", 11, 14, 4, 3, 0);
	DrawNum(loadedSong.ticksPerBeat, 15, 17, 4, 4, 0, -1, 2);
	DrawText("OCT", 11, 14, 6, 3, 0);
	DrawNum(editor.selectedOctave, 15, 16, 6, 4, 0, -1, 3);
	DrawText("ROW", 11, 14, 8, 3, 0);
	DrawNum(loadedFrame.rows.size(), 15, 18, 8, 4, 0, -1, 4);
	DrawText("BEAT", 11, 15, 10, 3, 0);
	DrawNum(loadedFrame.beatsPerMeasure, 15, 17, 10, 4, 0, -1, 5);



	// Song name box
	DrawText("Song Name", 46, 21 + 45, 0, 3, 0);
	

	
	
	gui.activeUI[44][1].sprite = { 0, 3 };
	gui.activeUI[44][2].sprite = { 2, 4 };
	gui.activeUI[44][3].sprite = { 2, 4 };
	gui.activeUI[44][4].sprite = { 0, 4 };

	gui.activeUI[21 + 44][1].sprite = { 1, 3 };
	gui.activeUI[21 + 44][2].sprite = { 2, 4 };
	gui.activeUI[21 + 44][3].sprite = { 2, 4 };
	gui.activeUI[21 + 44][4].sprite = { 1, 4 };
	for (int x = 0; x < 20; x++)
	{
		gui.activeUI[45 + x][1].sprite = { 2, 3 };
		gui.activeUI[45 + x][4].sprite = { 2, 3 };
	}
	DrawText(loadedSong.songName, 45, 21 + 44, 2, 5, 0, -1, 0);
	DrawText("BY:", 45, 21 + 44, 3, 5, 0, -1, -1);
	DrawText(loadedSong.artistName, 48, 21 + 44, 3, 5, 0, -1, 6);

	// Effect menu
	DrawText("Effects", 36, 43, 0, 3, 0);

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			gui.activeUI[36 + x * 2][2 + y].sprite = { float(y + 8 * x), float(19) };
			gui.activeUI[36 + x * 2][2 + y].bgCol = 0;

			gui.activeUI[37 + x * 2][2 + y].sprite = { float(y + 8 * x), float(20) };
			gui.activeUI[37 + x * 2][2 + y].bgCol = 0;

			if (y + 8 * x - 1 == editor.selectedEffect)
			{
				gui.activeUI[36 + x * 2][2 + y].textCol = 5;
				gui.activeUI[37 + x * 2][2 + y].textCol = 5;
			}
			else
			{
				gui.activeUI[36 + x * 2][2 + y].textCol = 2;
				gui.activeUI[37 + x * 2][2 + y].textCol = 2;
			}
		}
	}
	

	

	

	DrawText("Samples", 70, 77, 0, 3, 0);
	

	// Draw keyboard
	if (gui.pianoMode)
		DrawPiano();
	else
		DrawKeyBoard();


	
	


	// File select box
	for (int y = 3; y < 11; y++)
	{
		gui.activeUI[68][y].sprite = { 2, 4 };
		gui.activeUI[71][y].sprite = { 2, 4 };
		gui.activeUI[91][y].sprite = { 31, 3 };
	}
	for (int x = 68; x < 91; x++)
	{
		gui.activeUI[x][1].sprite = { 2, 3 };
	}
	gui.activeUI[68][2].sprite = { 2, 4 };
	gui.activeUI[68][11].sprite = { 2, 4 };
	gui.activeUI[71][2].sprite = { 2, 4 };
	gui.activeUI[71][11].sprite = { 2, 4 };

	gui.activeUI[91][3].sprite = { 31, 2 };
	gui.activeUI[91][10].sprite = { 31, 4 };

	for (int y = 2; y < 12; y++)
	{
		if (y - 2 + gui.sampleListScroll < loadedSamples.size()) // Draw samples
		{
			if (y - 2 + gui.sampleListScroll == editor.selectedSample)
			{
				DrawText(loadedSamples[y - 2 + gui.sampleListScroll].sampleName, 72, 91, y, 5, 3);
				gui.activeUI[71][y].sprite = { 5, 4 };
				DrawHex(y - 2 + int(gui.sampleListScroll), 69, y, 4, 3);
				gui.activeUI[90][y].sprite = { 31, 9 }; // Delete button
			}
			else
			{
				DrawText(loadedSamples[y - 2 + gui.sampleListScroll].sampleName, 72, 91, y, 4, 0);
				DrawHex(y - 2 + int(gui.sampleListScroll), 69, y, 3, 0);
			}
		}
		else
		{
			DrawText("", 72, 91, y, 4, 0);
			DrawHex(y - 2 + int(gui.sampleListScroll), 69, y, 2, 0);
		}
	}


	gui.activeUI[88][0].sprite = { 29, 10 }; // Minimize program button
	gui.activeUI[89][0].sprite = { 30, 10 };
	gui.activeUI[90][0].sprite = { 29, 9 }; // Exit program button
	gui.activeUI[91][0].sprite = { 30, 9 };
	
	
	

	gui.activeUI[68][1].sprite = { 0, 3 };
	gui.activeUI[71][1].sprite = { 3, 3 };
	gui.activeUI[91][1].sprite = { 1, 3 };


	// Logo
	int logoX = 20;
	int logoY = 1;
	
	gui.activeUI[logoX][logoY].sprite = { 17, 3 };		gui.activeUI[logoX][logoY + 1].sprite = { 22, 3 };		gui.activeUI[logoX][logoY + 2].sprite = { 17, 4 };
	gui.activeUI[logoX + 1][logoY].sprite = { 19, 3 };	gui.activeUI[logoX + 1][logoY + 1].sprite = { 23, 3 };	gui.activeUI[logoX + 1][logoY + 2].sprite = { 19, 4 };
	gui.activeUI[logoX + 2][logoY].sprite = { 18, 3 };	gui.activeUI[logoX + 2][logoY + 1].sprite = { 24, 3 };	gui.activeUI[logoX + 2][logoY + 2].sprite = { 18, 4 };

	gui.activeUI[logoX + 3][logoY].sprite = { 20, 3 };	gui.activeUI[logoX + 3][logoY + 1].sprite = { 20, 4 };	gui.activeUI[logoX + 3][logoY + 2].sprite = { 17, 4 };
																												gui.activeUI[logoX + 4][logoY + 2].sprite = { 19, 4 };
	gui.activeUI[logoX + 5][logoY].sprite = { 21, 3 };	gui.activeUI[logoX + 5][logoY + 1].sprite = { 21, 4 };	gui.activeUI[logoX + 5][logoY + 2].sprite = { 18, 4 };

	gui.activeUI[logoX + 6][logoY].sprite = { 17, 3 };	gui.activeUI[logoX + 6][logoY + 1].sprite = { 20, 4 };	gui.activeUI[logoX + 6][logoY + 2].sprite = { 20, 3 };
	gui.activeUI[logoX + 7][logoY].sprite = { 19, 3 };	gui.activeUI[logoX + 7][logoY + 1].sprite = { 19, 4 };
	gui.activeUI[logoX + 8][logoY].sprite = { 18, 3 };	gui.activeUI[logoX + 8][logoY + 1].sprite = { 18, 4 };

	gui.activeUI[logoX + 9][logoY].sprite = { 17, 3 };	gui.activeUI[logoX + 9][logoY + 1].sprite = { 20, 4 };	gui.activeUI[logoX + 9][logoY + 2].sprite = { 17, 4 };
	gui.activeUI[logoX + 10][logoY].sprite = { 19, 3 };	gui.activeUI[logoX + 10][logoY + 1].sprite = { 19, 4 };	gui.activeUI[logoX + 10][logoY + 2].sprite = { 19, 4 };
	gui.activeUI[logoX + 11][logoY].sprite = { 18, 3 };	gui.activeUI[logoX + 11][logoY + 1].sprite = { 18, 4 };	gui.activeUI[logoX + 11][logoY + 2].sprite = { 18, 4 };

	gui.activeUI[logoX + 12][logoY].sprite = { 17, 3 };	gui.activeUI[logoX + 12][logoY + 1].sprite = { 20, 4 };	gui.activeUI[logoX + 12][logoY + 2].sprite = { 20, 3 };
	gui.activeUI[logoX + 13][logoY].sprite = { 19, 3 };	gui.activeUI[logoX + 13][logoY + 1].sprite = { 19, 4 };
	gui.activeUI[logoX + 14][logoY].sprite = { 18, 3 };	gui.activeUI[logoX + 14][logoY + 1].sprite = { 18, 4 };	gui.activeUI[logoX + 14][logoY + 2].sprite = { 18, 4 };
	
	/*
	gui.activeUI[logoX][logoY].sprite = { 26, 7 };		gui.activeUI[logoX][logoY + 1].sprite = { 28, 8 };		gui.activeUI[logoX][logoY + 2].sprite = { 26, 8 };
	gui.activeUI[logoX + 1][logoY].sprite = { 27, 7 };	gui.activeUI[logoX + 1][logoY + 1].sprite = { 29, 7 };	gui.activeUI[logoX + 1][logoY + 2].sprite = { 27, 8 };

	gui.activeUI[logoX + 2][logoY].sprite = { 26, 7 };	gui.activeUI[logoX + 2][logoY + 1].sprite = { 30, 7 };	gui.activeUI[logoX + 2][logoY + 2].sprite = { 26, 8 };
	gui.activeUI[logoX + 3][logoY].sprite = { 27, 7 };	gui.activeUI[logoX + 3][logoY + 1].sprite = { 29, 8 };

	gui.activeUI[logoX + 4][logoY].sprite = { 26, 7 };	gui.activeUI[logoX + 4][logoY + 1].sprite = { 30, 7 };	gui.activeUI[logoX + 4][logoY + 2].sprite = { 30, 8 };
	gui.activeUI[logoX + 5][logoY].sprite = { 27, 7 };	gui.activeUI[logoX + 5][logoY + 1].sprite = { 31, 7 };  gui.activeUI[logoX + 5][logoY + 2].sprite = { 27, 8 };

	gui.activeUI[logoX + 6][logoY].sprite = { 26, 7 };	gui.activeUI[logoX + 6][logoY + 1].sprite = { 30, 7 };	gui.activeUI[logoX + 6][logoY + 2].sprite = { 30, 7 };
	gui.activeUI[logoX + 7][logoY].sprite = { 27, 7 };	gui.activeUI[logoX + 7][logoY + 1].sprite = { 29, 8 };  gui.activeUI[logoX + 7][logoY + 2].sprite = { 29, 7 };

	gui.activeUI[logoX + 8][logoY].sprite = { 30, 7 };	gui.activeUI[logoX + 8][logoY + 1].sprite = { 30, 7 };	gui.activeUI[logoX + 8][logoY + 2].sprite = { 30, 8 };
	gui.activeUI[logoX + 9][logoY].sprite = { 29, 8 };	gui.activeUI[logoX + 9][logoY + 1].sprite = { 29, 7 };  gui.activeUI[logoX + 9][logoY + 2].sprite = { 27, 8 };

	gui.activeUI[logoX + 10][logoY].sprite = { 30, 8 };	gui.activeUI[logoX + 10][logoY + 1].sprite = { 30, 8 };	gui.activeUI[logoX + 10][logoY + 2].sprite = { 26, 8 };
														gui.activeUI[logoX + 11][logoY + 1].sprite = { 29, 7 }; gui.activeUI[logoX + 11][logoY + 2].sprite = { 27, 8 };

	gui.activeUI[logoX + 12][logoY].sprite = { 30, 8 };	gui.activeUI[logoX + 12][logoY + 1].sprite = { 28, 8 };
	gui.activeUI[logoX + 13][logoY].sprite = { 27, 7 }; gui.activeUI[logoX + 13][logoY + 1].sprite = { 31, 7 }; gui.activeUI[logoX + 13][logoY + 2].sprite = { 27, 8 };
	*/
	logoY += 3;

	gui.activeUI[logoX][logoY].sprite = { 17, 3 };		gui.activeUI[logoX][logoY + 1].sprite = { 22, 3 };		gui.activeUI[logoX][logoY + 2].sprite = { 17, 4 };
	gui.activeUI[logoX + 1][logoY].sprite = { 19, 3 };	gui.activeUI[logoX + 1][logoY + 1].sprite = { 23, 3 };	gui.activeUI[logoX + 1][logoY + 2].sprite = { 19, 4 };
	gui.activeUI[logoX + 2][logoY].sprite = { 18, 3 };	gui.activeUI[logoX + 2][logoY + 1].sprite = { 24, 3 };	gui.activeUI[logoX + 2][logoY + 2].sprite = { 18, 4 };

	gui.activeUI[logoX + 3][logoY].sprite = { 17, 3 };	gui.activeUI[logoX + 3][logoY + 1].sprite = { 20, 4 };	gui.activeUI[logoX + 3][logoY + 2].sprite = { 17, 4 };
	gui.activeUI[logoX + 4][logoY].sprite = { 19, 3 };															gui.activeUI[logoX + 4][logoY + 2].sprite = { 19, 4 };
	gui.activeUI[logoX + 5][logoY].sprite = { 18, 3 };	gui.activeUI[logoX + 5][logoY + 1].sprite = { 21, 4 };	gui.activeUI[logoX + 5][logoY + 2].sprite = { 18, 4 };

	gui.activeUI[logoX + 6][logoY].sprite = { 20, 3 };	gui.activeUI[logoX + 6][logoY + 1].sprite = { 20, 4 };	gui.activeUI[logoX + 6][logoY + 2].sprite = { 17, 4 };
																												gui.activeUI[logoX + 7][logoY + 2].sprite = { 19, 4 };
	gui.activeUI[logoX + 8][logoY].sprite = { 21, 3 };	gui.activeUI[logoX + 8][logoY + 1].sprite = { 21, 4 };	gui.activeUI[logoX + 8][logoY + 2].sprite = { 18, 4 };

	gui.activeUI[logoX + 9][logoY].sprite = { 20, 3 };	gui.activeUI[logoX + 9][logoY + 1].sprite = { 20, 4 };	gui.activeUI[logoX + 9][logoY + 2].sprite = { 17, 4 };
														gui.activeUI[logoX + 10][logoY + 1].sprite = { 23, 3 };
	gui.activeUI[logoX + 11][logoY].sprite = { 18, 3 };	gui.activeUI[logoX + 11][logoY + 1].sprite = { 21, 4 };	gui.activeUI[logoX + 11][logoY + 2].sprite = { 21, 3 };

	gui.activeUI[logoX + 12][logoY].sprite = { 20, 3 };	gui.activeUI[logoX + 12][logoY + 1].sprite = { 20, 4 };	gui.activeUI[logoX + 12][logoY + 2].sprite = { 20, 3 };
	gui.activeUI[logoX + 13][logoY].sprite = { 19, 3 };															gui.activeUI[logoX + 13][logoY + 2].sprite = { 19, 4 };
	gui.activeUI[logoX + 14][logoY].sprite = { 18, 3 };	gui.activeUI[logoX + 14][logoY + 1].sprite = { 21, 4 };	gui.activeUI[logoX + 14][logoY + 2].sprite = { 18, 4 };

	logoY += 3;

	gui.activeUI[logoX][logoY].sprite = { 22, 4 };		gui.activeUI[logoX + 1][logoY].sprite = { 23, 4 };		gui.activeUI[logoX + 2][logoY].sprite = { 24, 4 };
	gui.activeUI[logoX + 3][logoY].sprite = { 22, 4 };	gui.activeUI[logoX + 4][logoY].sprite = { 23, 4 };		gui.activeUI[logoX + 5][logoY].sprite = { 24, 4 };
	gui.activeUI[logoX + 6][logoY].sprite = { 22, 4 };	gui.activeUI[logoX + 7][logoY].sprite = { 23, 4 };		gui.activeUI[logoX + 8][logoY].sprite = { 24, 4 };
	gui.activeUI[logoX + 9][logoY].sprite = { 22, 4 };															gui.activeUI[logoX + 11][logoY].sprite = { 25, 4 };
	gui.activeUI[logoX + 12][logoY].sprite = { 25, 3 };	gui.activeUI[logoX + 13][logoY].sprite = { 23, 4 };		gui.activeUI[logoX + 14][logoY].sprite = { 24, 4 };


	// UI Buttons.
	if (loadedSong.unsavedChanges)
		DrawText("SAVE", 20, 26, 8, 4, 2);
	else
		DrawText("SAVE", 20, 26, 8, 1, 2);
	gui.activeUI[19][8].sprite = { 24, 0 };
	gui.activeUI[26][8].sprite = { 25, 0 };

	DrawText("LOAD", 20, 26, 9, 4, 2);
	gui.activeUI[19][9].sprite = { 24, 0 };
	gui.activeUI[26][9].sprite = { 25, 0 };

	DrawText("NEW", 20, 26, 10, 4, 2);
	gui.activeUI[19][10].sprite = { 24, 0 };
	gui.activeUI[26][10].sprite = { 25, 0 };

	DrawText("EXPORT", 20, 26, 11, 4, 2);
	gui.activeUI[19][11].sprite = { 24, 0 };
	gui.activeUI[26][11].sprite = { 25, 0 };
	//gui.activeUI[24][11].sprite = { 23 + float(editor.recordingSong), 5 };

	DrawText("NOTES", 28, 35, 8, 4, 2);
	gui.activeUI[27][8].sprite = { 24, 0 };
	gui.activeUI[34][8].sprite = { 25, 0 };

	DrawText("HELP", 28, 35, 9, 4, 2);
	gui.activeUI[27][9].sprite = { 24, 0 };
	gui.activeUI[34][9].sprite = { 25, 0 };

	DrawText("CONFIG", 28, 35, 10, 4, 2);
	gui.activeUI[27][10].sprite = { 24, 0 };
	gui.activeUI[34][10].sprite = { 25, 0 };

	DrawText("SAMPLE", 28, 35, 11, 4, 2);
	gui.activeUI[27][11].sprite = { 24, 0 };
	gui.activeUI[34][11].sprite = { 25, 0 };

	// Eye
	/*
	logoY -= 2;
	activeUI[logoX + 15][logoY].sprite = { 26, 1 }; activeUI[logoX + 16][logoY].sprite = { 27, 1 };
	activeUI[logoX + 15][logoY + 1].sprite = { 26, 2 }; activeUI[logoX + 16][logoY + 1].sprite = { 27, 2 };
	activeUI[logoX + 15][logoY + 2].sprite = { 28, 2 }; activeUI[logoX + 16][logoY + 2].sprite = { 29, 2 };
	*/



	// Draw the play and pause buttons.
	if (editor.playingSong)
	{
		gui.activeUI[0][13].sprite = { 14, 7 };
		gui.activeUI[0][14].sprite = { 14, 8 };
		gui.activeUI[1][13].sprite = { 15, 7 };
		gui.activeUI[1][14].sprite = { 15, 8 };

		gui.activeUI[2][13].sprite = { 16, 7 };
		gui.activeUI[2][14].sprite = { 16, 8 };
		gui.activeUI[3][13].sprite = { 17, 7 };
		gui.activeUI[3][14].sprite = { 17, 8 };
	}
	else
	{
		gui.activeUI[0][13].sprite = { 12, 7 };
		gui.activeUI[0][14].sprite = { 12, 8 };
		gui.activeUI[1][13].sprite = { 13, 7 };
		gui.activeUI[1][14].sprite = { 13, 8 };

		gui.activeUI[2][13].sprite = { 18, 7 };
		gui.activeUI[2][14].sprite = { 18, 8 };
		gui.activeUI[3][13].sprite = { 19, 7 };
		gui.activeUI[3][14].sprite = { 19, 8 };
	}



	// Buttons
	for (int y = 0; y < 5; y++) // Revert buttons to unpressed.
	{
		gui.activeUI[11][3 + 2 * y].sprite = { 8, 3 };
		gui.activeUI[12][3 + 2 * y].sprite = { 9, 3 };
		gui.activeUI[13][3 + 2 * y].sprite = { 10, 3 };
	}
	gui.activeUI[7][5].sprite = { 8, 3 };
	gui.activeUI[8][5].sprite = { 9, 3 };
	gui.activeUI[9][5].sprite = { 10, 3 };
	gui.activeUI[7][8].sprite = { 8, 3 };
	gui.activeUI[8][8].sprite = { 9, 3 };
	gui.activeUI[9][8].sprite = { 10, 3 };
	// Frame menu scroll arrows
	gui.activeUI[6][2].sprite = { 6, 3 };
	gui.activeUI[6][11].sprite = { 6, 4 };
	// Frame scroll arrows
	gui.activeUI[91][16].sprite = { 6, 3 };
	gui.activeUI[91][55].sprite = { 6, 4 };
	gui.activeUI[5][56].sprite = { 30, 4 };
	gui.activeUI[90][56].sprite = { 30, 3 };
	// Sample scroll arrows
	gui.activeUI[91][2].sprite = { 6, 3 };
	gui.activeUI[91][11].sprite = { 6, 4 };
	

	return;
}



void DrawKeyBoard()
{
	gui.activeUI[44][7].sprite = { 24, 9 };
	gui.activeUI[44][8].sprite = { 25, 9 };

	for (int x = 45; x < 60; x++)
	{
		gui.activeUI[x][8].sprite = { 26, 9 };
	}
	gui.activeUI[60][8].sprite = { 27, 9 };
	gui.activeUI[60][9].sprite = { 28, 9 };

	DrawKey(17, 0, { 44, 6 });
	DrawKey(18, 1, { 45, 5 });
	DrawKey(19, 2, { 46, 6 });
	DrawKey(20, 3, { 47, 5 });
	DrawKey(21, 4, { 48, 6 });
	DrawKey(22, 5, { 50, 6 });
	DrawKey(23, 6, { 51, 5 });
	DrawKey(24, 7, { 52, 6 });
	DrawKey(25, 8, { 53, 5 });
	DrawKey(26, 9, { 54, 6 });
	DrawKey(27, 10, { 55, 5 });
	DrawKey(28, 11, { 56, 6 });

	DrawKey(29, 0, { 58, 6 });
	DrawKey(30, 1, { 59, 5 });
	DrawKey(31, 2, { 60, 6 });
	DrawKey(32, 3, { 61, 5 });
	DrawKey(33, 4, { 62, 6 });
	DrawKey(34, 5, { 64, 6 });
	DrawKey(35, 6, { 65, 5 });
	DrawKey(36, 7, { 66, 6 });


	DrawKey(0, 0, { 46, 10 });
	DrawKey(1, 1, { 47, 9 });
	DrawKey(2, 2, { 48, 10 });
	DrawKey(3, 3, { 49, 9 });
	DrawKey(4, 4, { 50, 10 });
	DrawKey(5, 5, { 52, 10 });
	DrawKey(6, 6, { 53, 9 });
	DrawKey(7, 7, { 54, 10 });
	DrawKey(8, 8, { 55, 9 });
	DrawKey(9, 9, { 56, 10 });
	DrawKey(10, 10, { 57, 9 });
	DrawKey(11, 11, { 58, 10 });

	DrawKey(12, 0, { 60, 10 });
	DrawKey(13, 1, { 61, 9 });
	DrawKey(14, 2, { 62, 10 });
	DrawKey(15, 3, { 63, 9 });
	DrawKey(16, 4, { 64, 10 });

	return;
}



void DrawPiano()
{


	for (int oct = 0; oct < 7; oct++)
	{
		DrawPianoKey({ 49, 11 - float(oct) }, oct * 12, false);
		DrawPianoKey({ 50, 11 - float(oct) }, oct * 12 + 1, true);
		DrawPianoKey({ 51, 11 - float(oct) }, oct * 12 + 2, false);
		DrawPianoKey({ 52, 11 - float(oct) }, oct * 12 + 3, true);
		DrawPianoKey({ 53, 11 - float(oct) }, oct * 12 + 4, false);

		DrawPianoKey({ 54, 11 - float(oct) }, oct * 12 + 5, false);
		DrawPianoKey({ 55, 11 - float(oct) }, oct * 12 + 6, true);
		DrawPianoKey({ 56, 11 - float(oct) }, oct * 12 + 7, false);
		DrawPianoKey({ 57, 11 - float(oct) }, oct * 12 + 8, true);
		DrawPianoKey({ 58, 11 - float(oct) }, oct * 12 + 9, false);
		DrawPianoKey({ 59, 11 - float(oct) }, oct * 12 + 10, true);
		DrawPianoKey({ 60, 11 - float(oct) }, oct * 12 + 11, false);
	}

	return;
}



void DrawMute()
{
	for (int y = 12; y < 17; y++)
	{
		for (int x = 0; x < 92; x++)
		{
			gui.activeUI[x][y].sprite = { 15, 0 };
			gui.activeUI[x][y].bgCol = 0;
		}
	}


	int xPos = 5 - gui.frameScroll.x;

	for (int i = 0; i < loadedSong.numberOfChannels; i++)
	{
		// Border
		for (int x = 0; x < 7 + channels[i].effectCountPerRow * 5; x++)
		{
			if (xPos + x > 4 && xPos + x < 91)
			{
				gui.activeUI[xPos + x][12].sprite = { 2, 3 };
				gui.activeUI[xPos + x][15].sprite = { 2, 3 };
			}
		}
		int muteBorderEnd = xPos + 7 + channels[i].effectCountPerRow * 5;
		if (muteBorderEnd > 4 && muteBorderEnd < 91)
		{
			gui.activeUI[muteBorderEnd][12].sprite = { 3, 3 };
			gui.activeUI[muteBorderEnd][15].sprite = { 5, 3 };
		}

		float muteOffset = 0;

		if (channels[i].muted)
			muteOffset = 3;

		int color = 1;


		
		if (channels[i].compressed)
		{
			if (xPos > 4)
			{
				if (channels[i].muted)
					DrawNum(i + 1, xPos, xPos + 2, 13, 2, 0);
				else
					DrawNum(i + 1, xPos, xPos + 2, 13, 16, 0);
			}
			xPos += 2;
			if (xPos > 90) return;

			if (xPos > 4)
			{
				gui.activeUI[xPos][13].sprite = { 27, 10 }; gui.activeUI[xPos][14].sprite = { 27, 11 };
			}
			xPos++;

			if (xPos > 90) return;
		}
		else
		{
			// Glass
			if (xPos > 4)
			{
				gui.activeUI[xPos][13].sprite = { 11 + muteOffset, 3 }; gui.activeUI[xPos][14].sprite = { 11 + muteOffset, 4 };
			}
			xPos++;
			if (xPos > 90) return;

			if (xPos > 4)
			{
				gui.activeUI[xPos][13].sprite = { 12 + muteOffset, 3 }; gui.activeUI[xPos][14].sprite = { 12 + muteOffset, 4 };
			}
			xPos++;
			if (xPos > 90) return;

			if (xPos > 4)
			{
				gui.activeUI[xPos][13].sprite = { 12 + muteOffset, 3 }; gui.activeUI[xPos][14].sprite = { 12 + muteOffset, 4 };
			}
			xPos++;
			if (xPos > 90) return;

			if (xPos > 4)
			{
				gui.activeUI[xPos][13].sprite = { 12 + muteOffset, 3 }; gui.activeUI[xPos][14].sprite = { 12 + muteOffset, 4 };
			}
			xPos++;
			if (xPos > 90) return;

			DrawChannelNumber(i + 1, xPos);
			xPos += 2;



			if (xPos > 90) return;

			if (xPos > 4)
			{
				gui.activeUI[xPos][13].sprite = { 6 + muteOffset, 12 }; gui.activeUI[xPos][14].sprite = { 6 + muteOffset, 13 };
			}
			xPos++;

			// Draw the channel effects
			if (channels[i].effectCountPerRow > 1)
			{
				for (int ef = 0; ef < channels[i].effectCountPerRow - 1; ef++)
				{
					if (xPos > 90) return;

					if (xPos > 4)
					{
						gui.activeUI[xPos][13].sprite = { 5 + muteOffset, 12 }; gui.activeUI[xPos][14].sprite = { 5 + muteOffset, 13 };
					}
					xPos++;
					if (xPos > 90) return;

					if (xPos > 4)
					{
						gui.activeUI[xPos][13].sprite = { 12 + muteOffset, 3 }; gui.activeUI[xPos][14].sprite = { 12 + muteOffset, 4 };
					}
					xPos++;
					if (xPos > 90) return;

					if (xPos > 4)
					{
						gui.activeUI[xPos][13].sprite = { 12 + muteOffset, 3 }; gui.activeUI[xPos][14].sprite = { 12 + muteOffset, 4 };
					}
					xPos++;
					if (xPos > 90) return;

					if (xPos > 4)
					{
						gui.activeUI[xPos][13].sprite = { 12 + muteOffset, 3 }; gui.activeUI[xPos][14].sprite = { 12 + muteOffset, 4 };
					}
					xPos++;
					if (xPos > 90) return;

					if (xPos > 4)
					{
						gui.activeUI[xPos][13].sprite = { 6 + muteOffset, 12 }; gui.activeUI[xPos][14].sprite = { 6 + muteOffset, 13 };
					}
					xPos++;
				}

				if (xPos > 90) return;

				if (xPos > 4)
				{
					if (channels[i].effectCountPerRow < 8)
					{
						gui.activeUI[xPos][13].sprite = { 0 + muteOffset, 12 }; gui.activeUI[xPos][14].sprite = { 0 + muteOffset, 13 };
					}
					else
					{
						gui.activeUI[xPos][13].sprite = { 5 + muteOffset, 12 }; gui.activeUI[xPos][14].sprite = { 5 + muteOffset, 13 };
					}
				}
				xPos++;
				if (xPos > 90) return;

				if (xPos > 4)
				{
					gui.activeUI[xPos][13].sprite = { 12 + muteOffset, 3 }; gui.activeUI[xPos][14].sprite = { 12 + muteOffset, 4 };
				}
				xPos++;
				if (xPos > 90) return;

				if (xPos > 4)
				{
					gui.activeUI[xPos][13].sprite = { 12 + muteOffset, 3 }; gui.activeUI[xPos][14].sprite = { 12 + muteOffset, 4 };
				}
				xPos++;
				if (xPos > 90) return;


				if (xPos > 4)
				{
					gui.activeUI[xPos][13].sprite = { 1 + muteOffset, 12 }; gui.activeUI[xPos][14].sprite = { 1 + muteOffset, 13 };
				}
				xPos++;
				if (xPos > 90) return;

				if (xPos > 4)
				{
					gui.activeUI[xPos][13].sprite = { 13 + muteOffset, 3 }; gui.activeUI[xPos][14].sprite = { 13 + muteOffset, 4 };
				}
				xPos++;
			}
			else
			{
				if (xPos > 90) return;

				if (xPos > 4)
				{
					gui.activeUI[xPos][13].sprite = { 0 + muteOffset, 12 }; gui.activeUI[xPos][14].sprite = { 0 + muteOffset, 13 };
				}
				xPos++;
				if (xPos > 90) return;

				if (xPos > 4)
				{
					gui.activeUI[xPos][13].sprite = { 12 + muteOffset, 3 }; gui.activeUI[xPos][14].sprite = { 12 + muteOffset, 4 };
				}
				xPos++;
				if (xPos > 90) return;

				if (xPos > 4)
				{
					gui.activeUI[xPos][13].sprite = { 12 + muteOffset, 3 }; gui.activeUI[xPos][14].sprite = { 12 + muteOffset, 4 };
				}
				xPos++;
				if (xPos > 90) return;

				if (xPos > 4)
				{
					gui.activeUI[xPos][13].sprite = { 12 + muteOffset, 3 }; gui.activeUI[xPos][14].sprite = { 12 + muteOffset, 4 };
				}
				xPos++;
				if (xPos > 90) return;

				if (xPos > 4)
				{
					gui.activeUI[xPos][13].sprite = { 13 + muteOffset, 3 }; gui.activeUI[xPos][14].sprite = { 13 + muteOffset, 4 };
				}

				xPos++;
			}



			if (xPos > 90) return;
			if (xPos > 4)
			{
				gui.activeUI[xPos][13].sprite = { 28, 10 }; gui.activeUI[xPos][14].sprite = { 28, 11 };
			}
			xPos++;
		}
	}

	// Draw add/remove frame button.
	if (xPos > 90) return;
	if (xPos > 4)
	{
		gui.activeUI[xPos][13].sprite = { 11, 3 }; gui.activeUI[xPos][14].sprite = { 11, 4 };
	}
	xPos++;
	if (xPos > 90) return;
	if (xPos > 4)
	{
		gui.activeUI[xPos][13].sprite = { 26, 3 }; gui.activeUI[xPos][14].sprite = { 26, 4 };
	}
	xPos++;
	if (xPos > 90) return;
	if (xPos > 4)
	{
		gui.activeUI[xPos][13].sprite = { 27, 3 }; gui.activeUI[xPos][14].sprite = { 27, 4 };
	}
	xPos++;
	if (xPos > 90) return;
	if (xPos > 4)
	{
		gui.activeUI[xPos][13].sprite = { 28, 3 }; gui.activeUI[xPos][14].sprite = { 28, 4 };
	}
	xPos++;
	if (xPos > 90) return;
	if (xPos > 4)
	{
		gui.activeUI[xPos][13].sprite = { 13, 3 }; gui.activeUI[xPos][14].sprite = { 13, 4 };
	}

	return;
}



void DrawChannelNumber(int channelNum, int offsetX)
{
	int channelNum2 = 0;
	int channelNumOg = channelNum - 1;
	if (channelNum > 9)
	{
		channelNum2 = int(channelNum * 0.1);
		channelNum = channelNum % 10;

		offsetX -= 2;
		if (offsetX > 4 && offsetX < 91)
		{
			if (channels[channelNumOg].muted)
			{
				gui.activeUI[offsetX][13].sprite = { 2.0f * channelNum2, 10 };
				gui.activeUI[offsetX][14].sprite = { 2.0f * channelNum2, 11 };
			}
			else
			{
				gui.activeUI[offsetX][13].sprite = { 2.0f * channelNum2, 5 };
				gui.activeUI[offsetX][14].sprite = { 2.0f * channelNum2, 6 };
			}
		}
		else if (offsetX > 90)
			return;

		offsetX++;
		if (offsetX > 4 && offsetX < 91)
		{
			if (channels[channelNumOg].muted)
			{
				gui.activeUI[offsetX][13].sprite = { 2.0f * channelNum2 + 1, 10 };
				gui.activeUI[offsetX][14].sprite = { 2.0f * channelNum2 + 1, 11 };
			}
			else
			{
				gui.activeUI[offsetX][13].sprite = { 2.0f * channelNum2 + 1, 5 };
				gui.activeUI[offsetX][14].sprite = { 2.0f * channelNum2 + 1, 6 };
			}
		}
		offsetX++;
	}

	if (offsetX > 4 && offsetX < 91)
	{
		if (channels[channelNumOg].muted)
		{
			gui.activeUI[offsetX][13].sprite = { 2.0f * channelNum, 10 };
			gui.activeUI[offsetX][14].sprite = { 2.0f * channelNum, 11 };
		}
		else
		{
			gui.activeUI[offsetX][13].sprite = { 2.0f * channelNum, 5 };
			gui.activeUI[offsetX][14].sprite = { 2.0f * channelNum, 6 };
		}
	}
	else if (offsetX > 90)
		return;

	offsetX++;
	if (offsetX > 4 && offsetX < 91)
	{
		if (channels[channelNumOg].muted)
		{
			gui.activeUI[offsetX][13].sprite = { 2.0f * channelNum + 1, 10 };
			gui.activeUI[offsetX][14].sprite = { 2.0f * channelNum + 1, 11 };
		}
		else
		{
			gui.activeUI[offsetX][13].sprite = { 2.0f * channelNum + 1, 5 };
			gui.activeUI[offsetX][14].sprite = { 2.0f * channelNum + 1, 6 };
		}
	}

	return;
}



void DrawFrameBorder()
{
	gui.activeUI[4][12].sprite = { 0, 3 };
	gui.activeUI[4][13].sprite = { 2, 4 };
	gui.activeUI[4][14].sprite = { 2, 4 };
	gui.activeUI[4][15].sprite = { 5, 3 };

	gui.activeUI[91][12].sprite = { 4, 3 };
	gui.activeUI[91][13].sprite = { 2, 4 };
	gui.activeUI[91][14].sprite = { 2, 4 };
	gui.activeUI[91][15].sprite = { 4, 3 };

	gui.activeUI[0][15].sprite = { 0, 3 };
	gui.activeUI[1][15].sprite = { 2, 3 };
	gui.activeUI[2][15].sprite = { 2, 3 };
	gui.activeUI[3][15].sprite = { 2, 3 };




	for (int x = 0; x < 4; x++)
		gui.activeUI[x][56].sprite = { 2, 3 };
	for (int x = 7; x < 89; x++)
		gui.activeUI[x][56].sprite = { 28, 2 };
	
	gui.activeUI[6][56].sprite = { 27, 2 };
	gui.activeUI[89][56].sprite = { 29, 2 };


	gui.activeUI[4][56].sprite = { 4, 4 };

	// Frame borders.
	int posX = 4 - gui.frameScroll.x;
	for (int i = 0; i < loadedSong.numberOfChannels; i++)
	{
		if (channels[i].compressed)
			posX += 3;
		else
			posX += 8 + channels[i].effectCountPerRow * 5;
	}
	if (posX < 91)
	{
		if (posX < -1)
			posX = -1;
		for (int x = posX + 6; x < 91; x++)
		{
			for (int y = 13; y < 15; y++)
			{
				gui.activeUI[x][y].sprite = { 15, 0 };
				gui.activeUI[x][y].bgCol = 0;
			}
		}
		if (posX < 4)
			posX = 4;

		// Fill in after frames.
		for (int x = posX + 1; x < 91; x++)
		{
			gui.activeUI[x][12].sprite = { 2, 3 };
			for (int y = 16; y < 56; y++)
			{
				gui.activeUI[x][y].sprite = { 15, 0 };
				gui.activeUI[x][y].bgCol = 0;
			}
			gui.activeUI[x][15].sprite = { 2, 3 };
		}
	}




	
	for (int y = 18; y < 54; y++)
	{
		//gui.activeUI[91][y].sprite = { 2, 4 };
		gui.activeUI[91][y].sprite = { 31, 3 };
	}
	gui.activeUI[91][17].sprite = { 31, 2 };
	gui.activeUI[91][54].sprite = { 31, 4 };

	for (int y = 16; y < 56; y++)
	{
		gui.activeUI[0][y].sprite = { 2, 4 };
		gui.activeUI[4][y].sprite = { 2, 4 };

		// Beat numbering.
		if (loadedFrame.beatsPerMeasure > 0 && (y - 16 + int(gui.frameScroll.y)) % loadedFrame.beatsPerMeasure == 0)
			DrawNum(y - 16 + gui.frameScroll.y, 1, 4, y, 4, 0);
		else
			DrawNum(y - 16 + gui.frameScroll.y, 1, 4, y, 2, 0);

		if (y - 16 + gui.frameScroll.y == loadedSong.currentNote)
		{
			gui.activeUI[1][y].bgCol = 2;
			gui.activeUI[2][y].bgCol = 2;
			gui.activeUI[3][y].bgCol = 2;
			gui.activeUI[1][y].textCol++;
			gui.activeUI[2][y].textCol++;
			gui.activeUI[3][y].textCol++;
			gui.activeUI[0][y].sprite = { 5, 4 };
			gui.activeUI[4][y].sprite = { 5, 4 };
		}
	}

	gui.activeUI[0][16].sprite = { 2, 4 };
	gui.activeUI[0][55].sprite = { 2, 4 };




	gui.activeUI[0][56].sprite = { 0, 4 };
	gui.activeUI[91][56].sprite = { 1, 4 };

	// Where the top UI and frames meet.
	gui.activeUI[10][12].sprite = { 4, 4 };
	gui.activeUI[18][12].sprite = { 4, 4 };

	gui.activeUI[68][12].sprite = { 4, 4 };
	gui.activeUI[71][12].sprite = { 4, 4 };
	gui.activeUI[91][12].sprite = { 4, 3 };

	return;
}



void DrawVolumeBars()
{
	// Frame borders.
	int posX = 4 - gui.frameScroll.x;

	for (int i = 0; i < loadedSong.numberOfChannels; i++)
	{
		if (channels[i].compressed)
			posX += 3;
		else
			posX += 8 + channels[i].effectCountPerRow * 5;

		if (posX > 3 && posX < 91)
		{
			if (editor.channelBeingMoved == i)
			{
				gui.activeUI[posX][12].sprite = { 26, 6 };
			}
			else
			{
				gui.activeUI[posX][12].sprite = { 25, 6 };
			}

			gui.activeUI[posX][16].sprite = { 2, 4 };
			gui.activeUI[posX][17].sprite = { 4, 21 };


			for (int y = 0; y < 36; y++)
			{
				if (editor.channelMoveTo == i) // Draw selected channel move-to point.
				{
					gui.activeUI[posX][y + 18].sprite = { 27, 6 };
				}
				else
				{
					if (36 - y < channels[i].averageVolR)
					{
						if (36 - y < channels[i].averageVolL)
						{
							gui.activeUI[posX][y + 18].sprite = { 1, 21 }; // Yellow
						}
						else
							gui.activeUI[posX][y + 18].sprite = { 0, 21 }; // Red
					}
					else if (36 - y < channels[i].averageVolL)
					{
						gui.activeUI[posX][y + 18].sprite = { 2, 21 }; // Blue
					}
					else
						gui.activeUI[posX][y + 18].sprite = { 3, 21 }; // Grey

					gui.activeUI[posX][y + 18].bgCol = 0;
				}
			}
			gui.activeUI[posX][54].sprite = { 5, 21 };
			gui.activeUI[posX][55].sprite = { 2, 4 };
		}
	}

	

	return;
}



void DrawChannel(int channelNum, int offsetX)
{
	if (loadedSong.currentNote >= loadedFrame.rows.size())
		return;

	int intScrollY = int(gui.frameScroll.y);


	for (int y = 0; y < 40; y++)
	{
		int bgColor = 0;
		int textColor = 5;

		if (channels[channelNum].muted || y >= loadedFrame.rows.size())
		{
			bgColor--;
			if (bgColor < 0)
				bgColor = 0;
			textColor -= 2;
		}

		if (y + intScrollY == loadedSong.currentNote) // Note position marker.
		{
			bgColor = 2;
		}
		else if (loadedFrame.beatsPerMeasure > 0 && y + intScrollY < loadedFrame.rows.size() && (y + intScrollY) % loadedFrame.beatsPerMeasure == 0)
			bgColor = 1;
		

		


		if (offsetX > 4 && offsetX < 91)
		{
			gui.activeUI[offsetX][y + 16].sprite = { 16, 0 };
			gui.activeUI[offsetX][y + 16].bgCol = bgColor;
			gui.activeUI[offsetX][y + 16].textCol = bgColor + 1;
			if (channels[channelNum].muted || y >= loadedFrame.rows.size())
				gui.activeUI[offsetX][y + 16].textCol = 0;
		}
		if (offsetX > 3 && offsetX < 90)
		{
			gui.activeUI[1 + offsetX][y + 16].sprite = { 16, 0 };
			gui.activeUI[1 + offsetX][y + 16].bgCol = bgColor;
			gui.activeUI[1 + offsetX][y + 16].textCol = bgColor + 1;
			if (channels[channelNum].muted || y >= loadedFrame.rows.size())
				gui.activeUI[offsetX + 1][y + 16].textCol = 0;
		}
		if (offsetX > 2 && offsetX < 89)
		{
			gui.activeUI[2 + offsetX][y + 16].sprite = { 12, 0 };
			gui.activeUI[2 + offsetX][y + 16].bgCol = bgColor;
			gui.activeUI[2 + offsetX][y + 16].textCol = bgColor + 1;
			if (channels[channelNum].muted || y >= loadedFrame.rows.size())
				gui.activeUI[offsetX + 2][y + 16].textCol = 0;
		}
		if (offsetX > 1 && offsetX < 88)
		{
			gui.activeUI[3 + offsetX][y + 16].sprite = { 16, 0 };
			gui.activeUI[3 + offsetX][y + 16].bgCol = bgColor;
			gui.activeUI[3 + offsetX][y + 16].textCol = bgColor + 1;
			if (channels[channelNum].muted || y >= loadedFrame.rows.size())
				gui.activeUI[offsetX + 3][y + 16].textCol = 0;
		}
		if (offsetX > 0 && offsetX < 87)
		{
			gui.activeUI[4 + offsetX][y + 16].sprite = { 12, 0 };
			gui.activeUI[4 + offsetX][y + 16].bgCol = bgColor;
			gui.activeUI[4 + offsetX][y + 16].textCol = bgColor + 1;
			if (channels[channelNum].muted || y >= loadedFrame.rows.size())
				gui.activeUI[offsetX + 4][y + 16].textCol = 0;
		}
		if (offsetX > -1 && offsetX < 86)
		{
			gui.activeUI[5 + offsetX][y + 16].sprite = { 16, 0 };
			gui.activeUI[5 + offsetX][y + 16].bgCol = bgColor;
			gui.activeUI[5 + offsetX][y + 16].textCol = bgColor + 1;
			if (channels[channelNum].muted || y >= loadedFrame.rows.size())
				gui.activeUI[offsetX + 5][y + 16].textCol = 0;
		}
		if (offsetX > -2 && offsetX < 85)
		{
			gui.activeUI[6 + offsetX][y + 16].sprite = { 12, 0 };
			gui.activeUI[6 + offsetX][y + 16].bgCol = bgColor;
			gui.activeUI[6 + offsetX][y + 16].textCol = bgColor + 1;
			if (channels[channelNum].muted || y >= loadedFrame.rows.size())
				gui.activeUI[offsetX + 6][y + 16].textCol = 0;
		}
		for (int ef = 0; ef < channels[channelNum].effectCountPerRow; ef++)
		{
			int effectOffset = offsetX + ef * 5;
			if (effectOffset > -3 && effectOffset < 84)
			{
				gui.activeUI[7 + effectOffset][y + 16].sprite = { 0, 19 };
				gui.activeUI[7 + effectOffset][y + 16].bgCol = bgColor;
				gui.activeUI[7 + effectOffset][y + 16].textCol = bgColor + 1;
				if (channels[channelNum].muted || y >= loadedFrame.rows.size())
					gui.activeUI[effectOffset + 7][y + 16].textCol = 0;
			}
			if (effectOffset > -4 && effectOffset < 83)
			{
				gui.activeUI[8 + effectOffset][y + 16].sprite = { 0, 20 };
				gui.activeUI[8 + effectOffset][y + 16].bgCol = bgColor;
				gui.activeUI[8 + effectOffset][y + 16].textCol = bgColor + 1;
				if (channels[channelNum].muted || y >= loadedFrame.rows.size())
					gui.activeUI[effectOffset + 8][y + 16].textCol = 0;
			}
			if (effectOffset > -5 && effectOffset < 82)
			{
				gui.activeUI[9 + effectOffset][y + 16].sprite = { 16, 0 };
				gui.activeUI[9 + effectOffset][y + 16].bgCol = bgColor;
				gui.activeUI[9 + effectOffset][y + 16].textCol = bgColor + 1;
				if (channels[channelNum].muted || y >= loadedFrame.rows.size())
					gui.activeUI[effectOffset + 9][y + 16].textCol = 0;
			}
			if (effectOffset > -6 && effectOffset < 81)
			{
				gui.activeUI[10 + effectOffset][y + 16].sprite = { 16, 0 };
				gui.activeUI[10 + effectOffset][y + 16].bgCol = bgColor;
				gui.activeUI[10 + effectOffset][y + 16].textCol = bgColor + 1;
				if (channels[channelNum].muted || y >= loadedFrame.rows.size())
					gui.activeUI[effectOffset + 10][y + 16].textCol = 0;
			}
			if (effectOffset > -7 && effectOffset < 80)
			{
				gui.activeUI[11 + effectOffset][y + 16].sprite = { 16, 0 };
				gui.activeUI[11 + effectOffset][y + 16].bgCol = bgColor;
				gui.activeUI[11 + effectOffset][y + 16].textCol = bgColor + 1;
				if (channels[channelNum].muted || y >= loadedFrame.rows.size())
					gui.activeUI[effectOffset + 11][y + 16].textCol = 0;
			}
		}

		

		if (y + intScrollY < loadedFrame.rows.size())
		{
			if (loadedFrame.rows[y + intScrollY].note[channelNum] > -1) // Note
			{
				if (offsetX > 4)
				{
					DrawNote(uint8_t(loadedFrame.rows[y + intScrollY].note[channelNum]), offsetX, y + 16, textColor, bgColor, -1, -1);
				}
			}

			if (loadedFrame.rows[y + intScrollY].instrument[channelNum] > -1) // Instrument
			{
				if (offsetX > 1)
				{
					DrawHex(uint8_t(loadedFrame.rows[y + intScrollY].instrument[channelNum]), 3 + offsetX, y + 16, textColor + 3, bgColor, -1, -1);
				}
			}

			// Draw stop note
			if (loadedFrame.rows[y + intScrollY].note[channelNum] == 255) // Stop Note
			{
				if (offsetX > 4 && offsetX < 91)
					gui.activeUI[offsetX][y + 16].sprite = { 18, 0 };
				if (offsetX > 3 && offsetX < 90)
					gui.activeUI[1 + offsetX][y + 16].sprite = { 19, 0 };
				if (offsetX > 2 && offsetX < 89)
					gui.activeUI[2 + offsetX][y + 16].sprite = { 19, 0 };
				if (offsetX > 1 && offsetX < 88)
					gui.activeUI[3 + offsetX][y + 16].sprite = { 19, 0 };
				if (offsetX > 0 && offsetX < 87)
					gui.activeUI[4 + offsetX][y + 16].sprite = { 20, 0 };
			}


			if (loadedFrame.rows[y + intScrollY].volume[channelNum] > -1) // Volume
			{
				int xStart = 5 + offsetX;
				if (xStart > 3)
				{
					if (xStart == 4)
						xStart++;
					DrawNum(int(loadedFrame.rows[y + intScrollY].volume[channelNum]), xStart, 7 + offsetX, y + 16, textColor + 6, bgColor);
				}
			}

			for (int ef = 0; ef < channels[channelNum].effectCountPerRow; ef++)
			{
				int effectVal = loadedFrame.rows[y + intScrollY].effects[channelNum].cEffectValue[ef];

				if (loadedFrame.rows[y + intScrollY].effects[channelNum].cEffect[ef] > -1) // Effect
				{
					int effectNum = loadedFrame.rows[y + intScrollY].effects[channelNum].cEffect[ef];
					int offset = 0;

					if (effectVal < 0) // Special decreasing effects.
						offset = -2;

					if (offsetX + ef * 5 > -2 && offsetX + ef * 5 < 84)
					{
						gui.activeUI[7 + offsetX + ef * 5][y + 16].sprite = { float(effectNum) + 1, 19 + float(offset) };
						gui.activeUI[7 + offsetX + ef * 5][y + 16].textCol = textColor;
					}
					if (offsetX + 1 + ef * 5 > -2 && offsetX + 1 + ef * 5 < 84)
					{
						gui.activeUI[8 + offsetX + ef * 5][y + 16].sprite = { float(effectNum) + 1, 20 + float(offset) };
						gui.activeUI[8 + offsetX + ef * 5][y + 16].textCol = textColor;
					}



					if (effectNum > -1) // Effect Value
					{
						int xStart = 9 + offsetX + ef * 5;
						

						if (xStart > 3)
						{
							if (xStart == 4)
								xStart++;

							if (effectNum == 3 || effectNum == 6 || effectNum == 7 || effectNum == 10 || effectNum == 18 || effectNum == 19 || effectNum == 20) // 2-parameter effects.
							{
								
								int onesPlace = effectVal % 10;
								int tensPlace = (effectVal - onesPlace) * 0.1;

								DrawText(std::to_string(tensPlace) + "&" + std::to_string(onesPlace), xStart, xStart + 3, y + 16, textColor + 3, bgColor);
							}
							else if (effectNum == 4) // Note effects.
							{
								DrawNote(effectVal, xStart, y + 16, textColor + 3, bgColor, -1, -1);
							}
							else
								DrawNum(effectVal, xStart, xStart + 2, y + 16, textColor + 3, bgColor);
						}
					}
				}
			}
		}


		if (y + intScrollY >= editor.noteSelectionStart.y && y + intScrollY <= editor.noteSelectionEnd.y) // Draw selected notes.
		{
			int xStart = editor.noteSelectionStart.x + 5 - gui.frameScroll.x;
			int xEnd = editor.noteSelectionEnd.x + 5 - gui.frameScroll.x;

			if (xStart < 5)
				xStart = 5;
			else if (xStart > 91)
				xStart = 91;
			if (xEnd < 5)
				xEnd = 5;
			else if (xEnd > 91)
				xEnd = 91;

			for (int x = xStart; x < xEnd; x++)
			{
				gui.activeUI[x][y + 16].bgCol = 4;
				gui.activeUI[x][y + 16].textCol = 0;
			}
		}
	}

	return;
}



void DrawChannelCompressed(int channelNum, int offsetX)
{
	if (loadedSong.currentNote >= loadedFrame.rows.size())
		return;

	int intScrollY = int(gui.frameScroll.y);


	for (int y = 0; y < 40; y++)
	{
		int bgColor = 0;
		int textColor = 5;

		if (channels[channelNum].muted || y >= loadedFrame.rows.size())
		{
			bgColor--;
			if (bgColor < 0)
				bgColor = 0;
			textColor -= 2;
		}

		if (y + intScrollY == loadedSong.currentNote) // Note position marker.
		{
			bgColor = 2;
		}
		else if (loadedFrame.beatsPerMeasure > 0 && y + intScrollY < loadedFrame.rows.size() && (y + intScrollY) % loadedFrame.beatsPerMeasure == 0)
			bgColor = 1;





		if (offsetX > 4 && offsetX < 91)
		{
			gui.activeUI[offsetX][y + 16].sprite = { 16, 0 };
			gui.activeUI[offsetX][y + 16].bgCol = bgColor;
			gui.activeUI[offsetX][y + 16].textCol = bgColor + 1;
			if (channels[channelNum].muted || y >= loadedFrame.rows.size())
				gui.activeUI[offsetX][y + 16].textCol = 0;
		}
		if (offsetX > 3 && offsetX < 90)
		{
			gui.activeUI[1 + offsetX][y + 16].sprite = { 16, 0 };
			gui.activeUI[1 + offsetX][y + 16].bgCol = bgColor;
			gui.activeUI[1 + offsetX][y + 16].textCol = bgColor + 1;
			if (channels[channelNum].muted || y >= loadedFrame.rows.size())
				gui.activeUI[offsetX + 1][y + 16].textCol = 0;
		}
		




		if (y + intScrollY < loadedFrame.rows.size())
		{
			if (loadedFrame.rows[y + intScrollY].note[channelNum] > -1) // Note
			{
				if (offsetX > 4)
				{
					if (loadedFrame.rows[y + intScrollY].volume[channelNum] > -1) // Volume
					{
						if (channels[channelNum].muted)
							gui.activeUI[offsetX][y + 16].sprite = { 23, 11 };
						else
							gui.activeUI[offsetX][y + 16].sprite = { 25, 11 };
					}
					else
					{
						if (channels[channelNum].muted)
							gui.activeUI[offsetX][y + 16].sprite = { 23, 10 };
						else
							gui.activeUI[offsetX][y + 16].sprite = { 25, 10 };
					}
					
					//DrawNote(uint8_t(loadedFrame.rows[y + intScrollY].note[channelNum]), offsetX, y + 16, textColor, bgColor, -1, -1);
				}
				else if (loadedFrame.rows[y + intScrollY].volume[channelNum] > -1) // Volume
				{
					if (channels[channelNum].muted)
						gui.activeUI[offsetX][y + 16].sprite = { 23, 12 };
					else
						gui.activeUI[offsetX][y + 16].sprite = { 25, 12 };
				}
			}


			if (offsetX > 3)
			{
				bool effectExists = false;

				for (int ef = 0; ef < channels[channelNum].effectCountPerRow; ef++)
				{
					if (loadedFrame.rows[y + intScrollY].effects[channelNum].cEffect[ef] > -1) // Effect
					{
						effectExists = true;
					}
				}

				if (effectExists)
				{
					if (loadedFrame.rows[y + intScrollY].volume[channelNum] > -1) // Volume
					{
						if (channels[channelNum].muted)
							gui.activeUI[offsetX + 1][y + 16].sprite = { 24, 11 };
						else
							gui.activeUI[offsetX + 1][y + 16].sprite = { 26, 11 };
					}
					else
					{
						if (channels[channelNum].muted)
							gui.activeUI[offsetX + 1][y + 16].sprite = { 24, 10 };
						else
							gui.activeUI[offsetX + 1][y + 16].sprite = { 26, 10 };
					}
				}
				else if (loadedFrame.rows[y + intScrollY].volume[channelNum] > -1) // Volume
				{
					if (channels[channelNum].muted)
						gui.activeUI[offsetX + 1][y + 16].sprite = { 24, 12 };
					else
						gui.activeUI[offsetX + 1][y + 16].sprite = { 26, 12 };
				}
			}


			// Draw stop note
			if (loadedFrame.rows[y + intScrollY].note[channelNum] == 255) // Stop Note
			{
				if (offsetX > 4 && offsetX < 91)
					gui.activeUI[offsetX][y + 16].sprite = { 22, 10 };
			}




			
			
		}


		if (y + intScrollY >= editor.noteSelectionStart.y && y + intScrollY <= editor.noteSelectionEnd.y) // Draw selected notes.
		{
			int xStart = editor.noteSelectionStart.x + 5 - gui.frameScroll.x;
			int xEnd = editor.noteSelectionEnd.x + 5 - gui.frameScroll.x;

			if (xStart < 5)
				xStart = 5;
			else if (xStart > 91)
				xStart = 91;
			if (xEnd < 5)
				xEnd = 5;
			else if (xEnd > 91)
				xEnd = 91;

			for (int x = xStart; x < xEnd; x++)
			{
				gui.activeUI[x][y + 16].bgCol = 4;
				gui.activeUI[x][y + 16].textCol = 0;
			}
		}
	}

	return;
}



void DrawNum(int text, int textStart, int textEnd, int textY, int textCol, int bgCol, int selectedLetter, int buttonIndex)
{
	if (textEnd > 90)
		textEnd = 90;

	int selectedChar = -1; // Selected letter.
	if (buttonIndex > -1 && editor.selectedButton == buttonIndex)
	{
		selectedChar = gui.selectedTile.x - textStart;
	}

	std::string textS = std::to_string(int(text));
	for (int i = 0; i < textEnd - textStart; i++)
	{
		int textI = i - (textEnd - textStart) + textS.length();
		Vector2 textSprite = { 17, 0 };

		if (textI > -1 && textI < textS.length())
		{
			if (textS.at(textI) > 47 && textS.at(textI) < 58) // 0-9
				textSprite = { float(textS.at(textI) - 48), 0.0f };
		}
		else
			textSprite = { 0.0f, 0.0f };

		gui.activeUI[textStart + i][textY].sprite = textSprite;
		gui.activeUI[textStart + i][textY].textCol = textCol;
		gui.activeUI[textStart + i][textY].bgCol = bgCol;

		if (i == selectedChar)
			gui.activeUI[textStart + i][textY].bgCol = 3;
	}

	return;
}



void DrawNum(int text, int textStart, int textEnd, int textY, int textCol, int bgCol)
{
	if (textEnd > 90)
		textEnd = 90;

	std::string textS = std::to_string(int(text));
	for (int i = 0; i < textEnd - textStart; i++)
	{
		int textI = i - (textEnd - textStart) + textS.length();
		Vector2 textSprite = { 17, 0 };

		if (textI > -1 && textI < textS.length())
		{
			if (textS.at(textI) > 47 && textS.at(textI) < 58) // 0-9
				textSprite = { float(textS.at(textI) - 48), 0.0f };
		}
		else
			textSprite = { 0.0f, 0.0f };

		gui.activeUI[textStart + i][textY].sprite = textSprite;
		gui.activeUI[textStart + i][textY].textCol = textCol;
		gui.activeUI[textStart + i][textY].bgCol = bgCol;
	}

	return;
}



void DrawText(std::string text, int textStart, int textEnd, int textY, int textCol, int bgCol)
{

	for (int i = 0; i < textEnd - textStart; i++)
	{
		if (i > textEnd)
			return;

		Vector2 textSprite = { 17, 0 };

		if (i < text.length())
		{
			if (text.at(i) > 39 && text.at(i) < 42) // (-)
				textSprite = { float(text.at(i) - 40) + 13, 0.0f };
			else if (text.at(i) > 47 && text.at(i) < 58) // 0-9
				textSprite = { float(text.at(i) - 48), 0.0f };
			else if (text.at(i) > 64 && text.at(i) < 91) // A-Z
				textSprite = { float(text.at(i) - 65), 1.0f };
			else if (text.at(i) > 96 && text.at(i) < 123) // a-z
				textSprite = { float(text.at(i) - 97), 2.0f };
			else if (text.at(i) == 44) // ,
				textSprite = { 23.0f, 0.0f };
			else if (text.at(i) == 45) // -
				textSprite = { 11.0f, 0.0f };
			else if (text.at(i) == 46) // .
				textSprite = { 22.0f, 0.0f };
			else if (text.at(i) == 33) // !
				textSprite = { 27.0f, 0.0f };
			else if (text.at(i) == 63) // ?
				textSprite = { 26.0f, 0.0f };
			else if (text.at(i) == 58) // :
				textSprite = { 30.0f, 0.0f };
			else if (text.at(i) == 38) // &
				textSprite = { 28.0f, 0.0f };
		}

		gui.activeUI[textStart + i][textY].sprite = textSprite;
		gui.activeUI[textStart + i][textY].textCol = textCol;
		gui.activeUI[textStart + i][textY].bgCol = bgCol;
	}

	return;
}



void DrawText(std::string text, int textStart, int textEnd, int textY, int textCol, int bgCol, int selectedLetter, int buttonIndex)
{
	int selectedChar = -1; // Selected letter.
	if (buttonIndex > -1 && editor.selectedButton == buttonIndex)
	{
		selectedChar = gui.selectedTile.x - textStart;
	}

	for (int i = 0; i < textEnd - textStart; i++)
	{
		if (i > textEnd)
			return;

		Vector2 textSprite = { 17, 0 };

		if (i < text.length())
		{
			if (text.at(i) > 39 && text.at(i) < 42) // (-)
				textSprite = { float(text.at(i) - 40) + 13, 0.0f };
			else if (text.at(i) > 47 && text.at(i) < 58) // 0-9
				textSprite = { float(text.at(i) - 48), 0.0f };
			else if (text.at(i) > 64 && text.at(i) < 91) // A-Z
				textSprite = { float(text.at(i) - 65), 1.0f };
			else if (text.at(i) > 96 && text.at(i) < 123) // a-z
				textSprite = { float(text.at(i) - 97), 2.0f };
			else if (text.at(i) == 44) // ,
				textSprite = { 23.0f, 0.0f };
			else if (text.at(i) == 45) // -
				textSprite = { 11.0f, 0.0f };
			else if (text.at(i) == 46) // .
				textSprite = { 22.0f, 0.0f };
			else if (text.at(i) == 33) // !
				textSprite = { 27.0f, 0.0f };
			else if (text.at(i) == 63) // ?
				textSprite = { 26.0f, 0.0f };
			else if (text.at(i) == 58) // :
				textSprite = { 30.0f, 0.0f };
			else if (text.at(i) == 38) // &
				textSprite = { 28.0f, 0.0f };
		}

		gui.activeUI[textStart + i][textY].sprite = textSprite;
		gui.activeUI[textStart + i][textY].textCol = textCol;
		gui.activeUI[textStart + i][textY].bgCol = bgCol;

		if (i == selectedChar)
			gui.activeUI[textStart + i][textY].bgCol = 3;
	}

	if (buttonIndex > -1 && editor.selectedButton == buttonIndex)
	{
		if (loadedSong.songName.length() == 0)
			gui.activeUI[textStart][textY].bgCol = 3;
	}

	return;
}



void DrawText(std::string text, int textStart, int textEnd, int textY, int textCol, int bgCol, bool continueDown)
{
	int yPos = textY;
	int xPos = textStart;

	for (int i = 0; i < text.length(); i++)
	{
		if (xPos > textEnd)
		{
			yPos++;
			xPos = textStart;

			if (yPos > 55)
				return;
		}

		Vector2 textSprite = { 17, 0 };

		if (i < text.length())
		{
			if (text.at(i) > 39 && text.at(i) < 42) // (-)
				textSprite = { float(text.at(i) - 40) + 13, 0.0f };
			else if (text.at(i) > 47 && text.at(i) < 58) // 0-9
				textSprite = { float(text.at(i) - 48), 0.0f };
			else if (text.at(i) > 64 && text.at(i) < 91) // A-Z
				textSprite = { float(text.at(i) - 65), 1.0f };
			else if (text.at(i) > 96 && text.at(i) < 123) // a-z
				textSprite = { float(text.at(i) - 97), 2.0f };
			else if (text.at(i) == 44) // ,
				textSprite = { 23.0f, 0.0f };
			else if (text.at(i) == 45) // -
				textSprite = { 11.0f, 0.0f };
			else if (text.at(i) == 46) // .
				textSprite = { 22.0f, 0.0f };
			else if (text.at(i) == 33) // !
				textSprite = { 27.0f, 0.0f };
			else if (text.at(i) == 63) // ?
				textSprite = { 26.0f, 0.0f };
			else if (text.at(i) == 58) // :
				textSprite = { 30.0f, 0.0f };
			else if (text.at(i) == 38) // &
				textSprite = { 28.0f, 0.0f };
		}

		gui.activeUI[xPos][yPos].sprite = textSprite;
		gui.activeUI[xPos][yPos].textCol = textCol;
		gui.activeUI[xPos][yPos].bgCol = bgCol;

		xPos++;
	}

	return;
}


void DrawHex(uint8_t num, int textStart, int textY, int textCol, int bgCol)
{

	Vector2 textSprite = { 15, 0 };

	int value1 = num / 16;
	int value2 = num % 16;

	//std::cout << "  " << value1 << "  " << value2;

	if (value1 < 10)
		textSprite = { float(value1), 0.0f };
	else
		textSprite = { float(value1 - 10), 1.0f };

	gui.activeUI[textStart][textY].sprite = textSprite;
	gui.activeUI[textStart][textY].textCol = textCol;
	gui.activeUI[textStart][textY].bgCol = bgCol;

	if (value2 < 10)
		textSprite = { float(value2), 0.0f };
	else
		textSprite = { float(value2 - 10), 1.0f };

	gui.activeUI[textStart + 1][textY].sprite = textSprite;
	gui.activeUI[textStart + 1][textY].textCol = textCol;
	gui.activeUI[textStart + 1][textY].bgCol = bgCol;

	return;
}



void DrawHex(uint8_t num, int textStart, int textY, int textCol, int bgCol, int selectedLetter, int buttonIndex)
{
	if (textStart > 90)
		return;

	int selectedChar = -1; // Selected letter.
	if (buttonIndex > -1 && editor.selectedButton == buttonIndex)
	{
		selectedChar = gui.selectedTile.x - textStart;
	}

	Vector2 textSprite = { 15, 0 };

	int value1 = num / 16;
	int value2 = num % 16;

	//std::cout << "  " << value1 << "  " << value2;

	if (value1 < 10)
		textSprite = { float(value1), 0.0f };
	else
		textSprite = { float(value1 - 10), 1.0f };

	gui.activeUI[textStart][textY].sprite = textSprite;
	gui.activeUI[textStart][textY].textCol = textCol;
	gui.activeUI[textStart][textY].bgCol = bgCol;

	if (textStart > 89)
		return;

	if (value2 < 10)
		textSprite = { float(value2), 0.0f };
	else
		textSprite = { float(value2 - 10), 1.0f };

	gui.activeUI[textStart + 1][textY].sprite = textSprite;
	gui.activeUI[textStart + 1][textY].textCol = textCol;
	gui.activeUI[textStart + 1][textY].bgCol = bgCol;


	if (selectedChar == 0)
		gui.activeUI[textStart][textY].bgCol = 3;
	else if (selectedChar == 1)
		gui.activeUI[textStart + 1][textY].bgCol = 3;

	return;
}


void DrawNote(uint8_t num, int textStart, int textY, int textCol, int bgCol, int selectedLetter, int buttonIndex)
{
	if (textStart > 90)
		return;

	// A, A#, B, C, C#, D, D#, E, F, F#, G, G#
	int letter = num % 12;
	int octave = int(float(num) / 12.0f);

	switch (letter)
	{
		case 0: // C
		{
			gui.activeUI[textStart][textY].sprite = { 2, 1 };
			if (textStart < 90)
				gui.activeUI[textStart + 1][textY].sprite = { 11, 0 };
			break;
		}
		case 1: // C#
		{
			gui.activeUI[textStart][textY].sprite = { 2, 1 };
			if (textStart < 90)
				gui.activeUI[textStart + 1][textY].sprite = { 10, 0 };
			break;
		}

		case 2: // D
		{
			gui.activeUI[textStart][textY].sprite = { 3, 1 };
			if (textStart < 90)
				gui.activeUI[textStart + 1][textY].sprite = { 11, 0 };
			break;
		}

		case 3: // D#
		{
			gui.activeUI[textStart][textY].sprite = { 3, 1 };
			if (textStart < 90)
				gui.activeUI[textStart + 1][textY].sprite = { 10, 0 };
			break;
		}

		case 4: // E
		{
			gui.activeUI[textStart][textY].sprite = { 4, 1 };
			if (textStart < 89)
				gui.activeUI[textStart + 1][textY].sprite = { 11, 0 };
			break;
		}

		case 5: // F
		{
			gui.activeUI[textStart][textY].sprite = { 5, 1 };
			if (textStart < 90)
				gui.activeUI[textStart + 1][textY].sprite = { 11, 0 };
			break;
		}

		case 6: // F#
		{
			gui.activeUI[textStart][textY].sprite = { 5, 1 };
			if (textStart < 90)
				gui.activeUI[textStart + 1][textY].sprite = { 10, 0 };
			break;
		}

		case 7: // G
		{
			gui.activeUI[textStart][textY].sprite = { 6, 1 };
			if (textStart < 90)
				gui.activeUI[textStart + 1][textY].sprite = { 11, 0 };
			break;
		}

		case 8: // G#
		{
			gui.activeUI[textStart][textY].sprite = { 6, 1 };
			if (textStart < 90)
				gui.activeUI[textStart + 1][textY].sprite = { 10, 0 };
			break;
		}

		case 9: // A
		{
			gui.activeUI[textStart][textY].sprite = { 0, 1 };
			if (textStart < 90)
			gui.activeUI[textStart + 1][textY].sprite = { 11, 0 };
			break;
		}

		case 10: // A#
		{
			gui.activeUI[textStart][textY].sprite = { 0, 1 };
			if (textStart < 90)
				gui.activeUI[textStart + 1][textY].sprite = { 10, 0 };
			break;
		}

		default: // B
		{
			gui.activeUI[textStart][textY].sprite = { 1, 1 };
			if (textStart < 90)
				gui.activeUI[textStart + 1][textY].sprite = { 11, 0 };
			break;
		}
	}

	if (textStart > 90)
		return;
	gui.activeUI[textStart][textY].textCol = textCol;
	gui.activeUI[textStart][textY].bgCol = bgCol;
	if (textStart > 89)
		return;
	gui.activeUI[textStart + 1][textY].textCol = textCol;
	gui.activeUI[textStart + 1][textY].bgCol = bgCol;
	DrawNum(octave, textStart + 2, textStart + 3, textY, textCol, bgCol);

	return;
}


void DrawKey(int button, int key, Vector2 pos)
{
	if (button == editor.selectedKey) // Key selected
	{
		gui.activeUI[int(pos.x)][int(pos.y)].textCol = 8;
		gui.activeUI[int(pos.x)][int(pos.y)].bgCol = 6;
		gui.activeUI[int(pos.x + 1)][int(pos.y)].textCol = 8;
		gui.activeUI[int(pos.x + 1)][int(pos.y)].bgCol = 6;
	}
	else
	{
		gui.activeUI[int(pos.x)][int(pos.y)].textCol = 3;
		gui.activeUI[int(pos.x)][int(pos.y)].bgCol = 0;
		gui.activeUI[int(pos.x + 1)][int(pos.y)].textCol = 3;
		gui.activeUI[int(pos.x + 1)][int(pos.y)].bgCol = 0;
	}

	gui.activeUI[int(pos.x)][int(pos.y)].sprite = { float(key * 2), 9 };
	gui.activeUI[int(pos.x + 1)][int(pos.y)].sprite = { float(key * 2) + 1, 9 };

	return;
}



void DrawPianoKey(Vector2 pos, int key, bool blackKey)
{
	bool pressed = false;

	for (int ch = 0; ch < channels.size(); ch++)
	{
		if (!channels[ch].muted && channels[ch].volume > 0 && channels[ch].arpNotes[channels[ch].arpIndex] == key)
		{
			pressed = true;
			break;
		}
	}

	if (blackKey)
	{
		if (pressed)
			gui.activeUI[int(pos.x)][int(pos.y)].sprite = { 21, 8 };
		else
			gui.activeUI[int(pos.x)][int(pos.y)].sprite = { 21, 7 };
	}
	else
	{
		if (pressed)
			gui.activeUI[int(pos.x)][int(pos.y)].sprite = { 20, 8 };
		else
			gui.activeUI[int(pos.x)][int(pos.y)].sprite = { 20, 7 };
	}

	return;
}




/*
void DrawPromptWindow()
{
	// Draw box edges.
	gui.activeUI[22][16].sprite = { 20, 5 };
	gui.activeUI[69][16].sprite = { 22, 5 };
	gui.activeUI[22][36].sprite = { 20, 6 };
	gui.activeUI[69][36].sprite = { 22, 6 };


	for (int x = 23; x < 69; x++)
	{
		gui.activeUI[x][16].sprite = { 21, 5 };
		gui.activeUI[x][17].sprite = { 24, 5 };
		gui.activeUI[x][35].sprite = { 24, 6 };
		gui.activeUI[x][36].sprite = { 21, 6 };
	}
	for (int y = 17; y < 36; y++)
	{
		gui.activeUI[22][y].sprite = { 23, 5 };
		gui.activeUI[69][y].sprite = { 23, 5 };
	}

	gui.activeUI[67][16].sprite = { 29, 9 }; // Exit button
	gui.activeUI[68][16].sprite = { 30, 9 };

	// Draw interior
	for (int y = 18; y < 35; y++)
	{
		for (int x = 23; x < 69; x++)
		{
			gui.activeUI[x][y].sprite = { 17, 0 };
			gui.activeUI[x][y].bgCol = 0;
		}
	}

	// Draw speech line
	gui.activeUI[26][16].sprite = { 27, 6 };
	gui.activeUI[27][16].sprite = { 29, 5 };
	gui.activeUI[28][16].sprite = { 27, 5 };

	gui.activeUI[26][15].sprite = { 25, 6 };
	gui.activeUI[27][15].sprite = { 29, 5 };
	gui.activeUI[28][15].sprite = { 26, 6 };

	gui.activeUI[26][14].sprite = { 25, 5 };
	gui.activeUI[27][14].sprite = { 26, 5 };
	gui.activeUI[28][14].sprite = { 26, 6 };

	gui.activeUI[27][13].sprite = { 25, 6 };
	gui.activeUI[28][13].sprite = { 26, 6 };

	gui.activeUI[27][12].sprite = { 25, 5 };
	gui.activeUI[28][12].sprite = { 29, 6 };

	gui.activeUI[28][11].sprite = { 28, 6 };
	gui.activeUI[28][10].sprite = { 28, 5 };


	// Draw prompt text.
	DrawText(promptWind.text, 24, 68, 18, 8, 0, true);

	// Draw text answers.
	if (promptWind.optionText.size() > 12)
	{
		int midPoint = ceil(promptWind.optionText.size() / 2.0f);
		for (int i = 0; i < midPoint; i++)
		{
			DrawText(promptWind.optionText[i], 25, 45, 20 + i * 2, 4, 2);
			gui.activeUI[24][20 + i * 2].sprite = { 24, 0 };
			gui.activeUI[45][20 + i * 2].sprite = { 25, 0 };
		}
		for (int i = midPoint; i < promptWind.optionText.size(); i++)
		{
			DrawText(promptWind.optionText[i], 47, 67, 20 + (i - midPoint) * 2, 4, 2);
			gui.activeUI[46][20 + (i - midPoint) * 2].sprite = { 24, 0 };
			gui.activeUI[67][20 + (i - midPoint) * 2].sprite = { 25, 0 };
		}
	}
	else if (promptWind.optionText.size() > 6)
	{
		int midPoint = ceil(promptWind.optionText.size() / 2.0f);
		for (int i = 0; i < midPoint; i++)
		{
			DrawText(promptWind.optionText[i], 25, 45, 24 + i * 2, 4, 2);
			gui.activeUI[24][24 + i * 2].sprite = { 24, 0 };
			gui.activeUI[45][24 + i * 2].sprite = { 25, 0 };
		}
		for (int i = midPoint; i < promptWind.optionText.size(); i++)
		{
			DrawText(promptWind.optionText[i], 47, 67, 24 + (i - midPoint) * 2, 4, 2);
			gui.activeUI[46][24 + (i - midPoint) * 2].sprite = { 24, 0 };
			gui.activeUI[67][24 + (i - midPoint) * 2].sprite = { 25, 0 };
		}
	}
	else
	{
		for (int i = 0; i < promptWind.optionText.size(); i++)
		{
			DrawText(promptWind.optionText[i], 25, 67, 24 + i * 2, 4, 2);
			gui.activeUI[24][24 + i * 2].sprite = { 24, 0 };
			gui.activeUI[67][24 + i * 2].sprite = { 25, 0 };
		}
	}

	return;
}*/


void DrawFloatingWindow(FloatingWindow* wind)
{
	//std::cout << wind->name;
	// Top
	gui.activeUI[int(wind->position.x)][int(wind->position.y)].sprite = { 20, 5 };
	for (int i = wind->position.x + 1; i < wind->position.x + wind->size.x; i++)
		gui.activeUI[i][int(wind->position.y)].sprite = { 21, 5 };
	DrawText(wind->name, int(wind->position.x + 2), int(wind->position.x + 2 + wind->name.length()), int(wind->position.y), 5, 16);


	gui.activeUI[int(wind->position.x + wind->size.x - 2)][int(wind->position.y)].sprite = { 29, 9 }; // Exit button
	gui.activeUI[int(wind->position.x + wind->size.x - 1)][int(wind->position.y)].sprite = { 30, 9 };
	gui.activeUI[int(wind->position.x + wind->size.x - 2)][int(wind->position.y)].bgCol = 16;
	gui.activeUI[int(wind->position.x + wind->size.x - 1)][int(wind->position.y)].bgCol = 16;


	gui.activeUI[int(wind->position.x + wind->size.x)][int(wind->position.y)].sprite = { 22, 5 };

	// Body
	for (int y = wind->position.y + 1; y < wind->position.y + wind->size.y - 1; y++)
	{
		gui.activeUI[int(wind->position.x)][y].sprite = {20, 6};
		for (int x = wind->position.x + 1; x < wind->position.x + wind->size.x; x++)
		{
			gui.activeUI[x][y].sprite = { 21, 6 };
		}
		gui.activeUI[int(wind->position.x + wind->size.x)][y].sprite = { 22, 6 };
	}

	// Bottom
	gui.activeUI[int(wind->position.x)][int(wind->position.y + wind->size.y - 1)].sprite = { 11, 4 };
	for (int i = wind->position.x + 1; i < wind->position.x + wind->size.x; i++)
		gui.activeUI[i][int(wind->position.y + wind->size.y - 1)].sprite = { 12, 4 };
	gui.activeUI[int(wind->position.x + wind->size.x)][int(wind->position.y + wind->size.y - 1)].sprite = { 13, 4 };


	// Draw contents
	if (wind->name == "Themes")
	{
		std::string themeText[16] = {
			"Red", "Leather", "Rust", "Burnt Orange", "Sand",
			"Olive", "Slightly Green", "Forest", "Sea Foam",
			"Aquatic", "Standard", "Too Blue",
			"Steel", "Cosmic", "Neutral Grey", "Bone White",
		};
		for (int i = 0; i < 16; i++)
		{
			DrawText(themeText[i], wind->position.x + 2, wind->position.x + 15, wind->position.y + 2 + i * 2, 4, 2);
			gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + 2 + i * 2)].sprite = {24, 0};
			gui.activeUI[int(wind->position.x + 15)][int(wind->position.y + 2 + i * 2)].sprite = { 25, 0 };
		}
	}
	else if (wind->name == "Save and Exit")
	{
		DrawText("Would you like to", wind->position.x + 2, wind->position.x + 19, wind->position.y + 2, 4, 1);
		DrawText("save your song", wind->position.x + 2, wind->position.x + 16, wind->position.y + 3, 4, 1);
		DrawText("before exiting?", wind->position.x + 2, wind->position.x + 17, wind->position.y + 4, 4, 1);

		gui.activeUI[int(wind->position.x + 7)][int(wind->position.y + 6)].sprite = { 24, 0 };
		DrawText("YES", wind->position.x + 8, wind->position.x + 12, wind->position.y + 6, 4, 2);
		gui.activeUI[int(wind->position.x + 12)][int(wind->position.y + 6)].sprite = { 25, 0 };

		gui.activeUI[int(wind->position.x + 7)][int(wind->position.y + 8)].sprite = { 24, 0 };
		DrawText(" NO", wind->position.x + 8, wind->position.x + 12, wind->position.y + 8, 4, 2);
		gui.activeUI[int(wind->position.x + 12)][int(wind->position.y + 8)].sprite = { 25, 0 };
	}
	else if (wind->name == "Save and Load")
	{
		DrawText("Would you like to", wind->position.x + 2, wind->position.x + 19, wind->position.y + 2, 4, 1);
		DrawText("save your current", wind->position.x + 2, wind->position.x + 16, wind->position.y + 3, 4, 1);
		DrawText("song before loading", wind->position.x + 2, wind->position.x + 17, wind->position.y + 4, 4, 1);
		DrawText("a new one?", wind->position.x + 2, wind->position.x + 17, wind->position.y + 5, 4, 1);

		gui.activeUI[int(wind->position.x + 7)][int(wind->position.y + 7)].sprite = { 24, 0 };
		DrawText("YES", wind->position.x + 8, wind->position.x + 12, wind->position.y + 7, 4, 2);
		gui.activeUI[int(wind->position.x + 12)][int(wind->position.y + 7)].sprite = { 25, 0 };

		gui.activeUI[int(wind->position.x + 7)][int(wind->position.y + 9)].sprite = { 24, 0 };
		DrawText(" NO", wind->position.x + 8, wind->position.x + 12, wind->position.y + 9, 4, 2);
		gui.activeUI[int(wind->position.x + 12)][int(wind->position.y + 9)].sprite = { 25, 0 };
	}
	else if (wind->name == "Delete Frames")
	{
		DrawText("This song contains", wind->position.x + 2, wind->position.x + 19, wind->position.y + 2, 4, 1);
		DrawText("unused frames.", wind->position.x + 2, wind->position.x + 16, wind->position.y + 3, 4, 1);
		DrawText("Would you like ", wind->position.x + 2, wind->position.x + 17, wind->position.y + 4, 4, 1);
		DrawText("to keep them?", wind->position.x + 2, wind->position.x + 17, wind->position.y + 5, 4, 1);

		gui.activeUI[int(wind->position.x + 7)][int(wind->position.y + 7)].sprite = { 24, 0 };
		DrawText("YES", wind->position.x + 8, wind->position.x + 12, wind->position.y + 7, 4, 2);
		gui.activeUI[int(wind->position.x + 12)][int(wind->position.y + 7)].sprite = { 25, 0 };

		gui.activeUI[int(wind->position.x + 7)][int(wind->position.y + 9)].sprite = { 24, 0 };
		DrawText(" NO", wind->position.x + 8, wind->position.x + 12, wind->position.y + 9, 4, 2);
		gui.activeUI[int(wind->position.x + 12)][int(wind->position.y + 9)].sprite = { 25, 0 };
	}
	else if (wind->name == "Overwrite Song")
	{
		DrawText("This song already", wind->position.x + 2, wind->position.x + 19, wind->position.y + 2, 4, 1);
		DrawText("exists. Would", wind->position.x + 2, wind->position.x + 16, wind->position.y + 3, 4, 1);
		DrawText("you like to", wind->position.x + 2, wind->position.x + 17, wind->position.y + 4, 4, 1);
		DrawText("Replace it?", wind->position.x + 2, wind->position.x + 17, wind->position.y + 5, 4, 1);

		gui.activeUI[int(wind->position.x + 7)][int(wind->position.y + 7)].sprite = { 24, 0 };
		DrawText("YES", wind->position.x + 8, wind->position.x + 12, wind->position.y + 7, 4, 2);
		gui.activeUI[int(wind->position.x + 12)][int(wind->position.y + 7)].sprite = { 25, 0 };

		gui.activeUI[int(wind->position.x + 7)][int(wind->position.y + 9)].sprite = { 24, 0 };
		DrawText(" NO", wind->position.x + 8, wind->position.x + 12, wind->position.y + 9, 4, 2);
		gui.activeUI[int(wind->position.x + 12)][int(wind->position.y + 9)].sprite = { 25, 0 };
	}
	else if (wind->name == "Load File")
	{
		gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + 1)].sprite = { 25, 5 };
		DrawText(fileNavigator.filePathName, wind->position.x + 2, wind->position.x + 40, wind->position.y + 1, 4, 2);

		for (int y = 2; y < 19; y++)
		{
			if (y - 2 + gui.fileListScroll < fileNavigator.fileNames.size())
			{
				if (y - 2 + gui.fileListScroll == editor.selectedFile)
					DrawText(fileNavigator.fileNames[y - 2 + gui.fileListScroll], wind->position.x + 1, wind->position.x + 39, wind->position.y + y, 5, 1);
				else
					DrawText(fileNavigator.fileNames[y - 2 + gui.fileListScroll], wind->position.x + 1, wind->position.x + 39, wind->position.y + y, 3, 0);

				if (fileNavigator.fileNames[y - 2 + gui.fileListScroll].at(0) == '0')
					gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + y)].sprite = { 26, 5 };
				else if (fileNavigator.fileNames[y - 2 + gui.fileListScroll].at(0) == '1')
					gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + y)].sprite = { 27, 5 };
				else if (fileNavigator.fileNames[y - 2 + gui.fileListScroll].at(0) == '2')
					gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + y)].sprite = { 28, 5 };
				else
					gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + y)].sprite = { 29, 5 };
			}
			else
				DrawText("", wind->position.x + 1, wind->position.x + 39, wind->position.y + y, 3, 0);

			gui.activeUI[int(wind->position.x + 39)][int(wind->position.y + y)].sprite = { 31, 3 };
		}

		gui.activeUI[int(wind->position.x + 39)][int(wind->position.y + 2)].sprite = { 31, 2 };
		gui.activeUI[int(wind->position.x + 39)][int(wind->position.y + 18)].sprite = { 31, 4 };


		gui.scrollBars[4].topLeft.x = wind->position.x + 39;
		gui.scrollBars[4].topLeft.y = wind->position.y + 2;

		DrawText("LOAD", wind->position.x + 30, wind->position.x + 34, wind->position.y + 19, 4, 2);
		gui.activeUI[int(wind->position.x + 29)][int(wind->position.y + 19)].sprite = { 24, 0 };
		gui.activeUI[int(wind->position.x + 34)][int(wind->position.y + 19)].sprite = { 25, 0 };
	}
	else if (wind->name == "Save Song")
	{
		gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + 1)].sprite = { 25, 5 };
		DrawText(fileNavigator.filePathName, wind->position.x + 2, wind->position.x + 40, wind->position.y + 1, 4, 2);

		for (int y = 2; y < 19; y++)
		{
			if (y - 2 + gui.fileListScroll < fileNavigator.fileNames.size())
			{
				if (y - 2 + gui.fileListScroll == editor.selectedFile)
					DrawText(fileNavigator.fileNames[y - 2 + gui.fileListScroll], wind->position.x + 1, wind->position.x + 39, wind->position.y + y, 5, 1);
				else
					DrawText(fileNavigator.fileNames[y - 2 + gui.fileListScroll], wind->position.x + 1, wind->position.x + 39, wind->position.y + y, 3, 0);

				if (fileNavigator.fileNames[y - 2 + gui.fileListScroll].at(0) == '0')
					gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + y)].sprite = { 26, 5 };
				else if (fileNavigator.fileNames[y - 2 + gui.fileListScroll].at(0) == '1')
					gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + y)].sprite = { 27, 5 };
				else if (fileNavigator.fileNames[y - 2 + gui.fileListScroll].at(0) == '2')
					gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + y)].sprite = { 28, 5 };
				else
					gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + y)].sprite = { 29, 5 };
			}
			else
				DrawText("", wind->position.x + 1, wind->position.x + 39, wind->position.y + y, 3, 0);

			gui.activeUI[int(wind->position.x + 39)][int(wind->position.y + y)].sprite = { 31, 3 };
		}

		gui.activeUI[int(wind->position.x + 39)][int(wind->position.y + 2)].sprite = { 31, 2 };
		gui.activeUI[int(wind->position.x + 39)][int(wind->position.y + 18)].sprite = { 31, 4 };


		gui.scrollBars[4].topLeft.x = wind->position.x + 39;
		gui.scrollBars[4].topLeft.y = wind->position.y + 2;

		DrawText("SAVE", wind->position.x + 30, wind->position.x + 34, wind->position.y + 19, 4, 2);
		gui.activeUI[int(wind->position.x + 29)][int(wind->position.y + 19)].sprite = { 24, 0 };
		gui.activeUI[int(wind->position.x + 34)][int(wind->position.y + 19)].sprite = { 25, 0 };

	}
	else if (wind->name == "Config")
	{
		std::string settingsText[3] = {
			"Themes", "Channel Focus", "Piano View"
		};
		for (int i = 0; i < 3; i++)
		{
			DrawText(settingsText[i], wind->position.x + 2, wind->position.x + 15, wind->position.y + 2 + i * 2, 4, 2);
			gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + 2 + i * 2)].sprite = { 24, 0 };
			gui.activeUI[int(wind->position.x + 15)][int(wind->position.y + 2 + i * 2)].sprite = { 25, 0 };
		}

		if (editor.channelFocus)
			gui.activeUI[int(wind->position.x + 16)][int(wind->position.y + 4)].sprite = { 24, 6 };
		else
			gui.activeUI[int(wind->position.x + 16)][int(wind->position.y + 4)].sprite = { 23, 6 };
	}
	else if (wind->name == "Notes")
	{
		
	}
	else if (wind->name == "Selection")
	{
		std::string settingsText[5] = {
			"Copy", "Paste", "Delete", "Transpose", "Set Sample"
		};
		for (int i = 0; i < 5; i++)
		{
			DrawText(settingsText[i], wind->position.x + 2, wind->position.x + 15, wind->position.y + 2 + i * 2, 4, 2);
			gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + 2 + i * 2)].sprite = { 24, 0 };
			gui.activeUI[int(wind->position.x + 15)][int(wind->position.y + 2 + i * 2)].sprite = { 25, 0 };
		}
	}
	else if (wind->name == "Transpose")
	{
		DrawText("Octaves", wind->position.x + 2, wind->position.x + 12, wind->position.y + 2, 4, 1);
		gui.activeUI[int(wind->position.x + 2)][int(wind->position.y + 3)].sprite = { 8, 3 };
		gui.activeUI[int(wind->position.x + 3)][int(wind->position.y + 3)].sprite = { 9, 3 };
		gui.activeUI[int(wind->position.x + 4)][int(wind->position.y + 3)].sprite = { 10, 3 };
		DrawText("Semitones", wind->position.x + 2, wind->position.x + 12, wind->position.y + 4, 4, 1);
		gui.activeUI[int(wind->position.x + 2)][int(wind->position.y + 5)].sprite = { 8, 3 };
		gui.activeUI[int(wind->position.x + 3)][int(wind->position.y + 5)].sprite = { 9, 3 };
		gui.activeUI[int(wind->position.x + 4)][int(wind->position.y + 5)].sprite = { 10, 3 };
		DrawText("C-4 to ", wind->position.x + 2, wind->position.x + 12, wind->position.y + 7, 4, 1);
		DrawNote(editor.transposeValue + 48, wind->position.x + 9, wind->position.y + 7, 4, 1, -1, -1);

		DrawText("APPLY", wind->position.x + 9, wind->position.x + 14, wind->position.y + 9, 4, 2);
		gui.activeUI[int(wind->position.x + 8)][int(wind->position.y + 9)].sprite = { 24, 0 };
		gui.activeUI[int(wind->position.x + 14)][int(wind->position.y + 9)].sprite = { 25, 0 };
	}
	else if (wind->name == "Help")
	{

		int helpPageScroll = ((helpPageText.size() + 40) / 39) * gui.scrollBars[4].position;
		
		for (int y = 2; y < 39; y++)
		{
			if (y - 2 + helpPageScroll < helpPageText.size())
			{
				if (helpPageText[y - 2 + helpPageScroll][0] == '#')
				{
					for (int x = 1; x < 39; x++)
					{
						gui.activeUI[int(wind->position.x + x)][int(wind->position.y + y)].sprite = { 21, 0 };
					}
				}
				else
					DrawText(helpPageText[y - 2 + helpPageScroll], wind->position.x + 1, wind->position.x + 39, wind->position.y + y, 3, 0);
			}
			else
				DrawText("", wind->position.x + 1, wind->position.x + 39, wind->position.y + y, 3, 0);
			gui.activeUI[int(wind->position.x + 39)][int(wind->position.y + y)].sprite = { 31, 3 };
		}

		gui.activeUI[int(wind->position.x + 39)][int(wind->position.y + 2)].sprite = { 31, 2 };
		gui.activeUI[int(wind->position.x + 39)][int(wind->position.y + 38)].sprite = { 31, 4 };


		gui.scrollBars[4].topLeft.x = wind->position.x + 39;
		gui.scrollBars[4].topLeft.y = wind->position.y + 2;
	}
	else if (wind->name == "Export as .WAV")
	{
		gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + 1)].sprite = { 25, 5 };
		DrawText(fileNavigator.filePathName, wind->position.x + 2, wind->position.x + 40, wind->position.y + 1, 4, 2);

		for (int y = 2; y < 19; y++)
		{
			if (y - 2 + gui.fileListScroll < fileNavigator.fileNames.size())
			{
				if (y - 2 + gui.fileListScroll == editor.selectedFile)
					DrawText(fileNavigator.fileNames[y - 2 + gui.fileListScroll], wind->position.x + 1, wind->position.x + 39, wind->position.y + y, 5, 1);
				else
					DrawText(fileNavigator.fileNames[y - 2 + gui.fileListScroll], wind->position.x + 1, wind->position.x + 39, wind->position.y + y, 3, 0);

				if (fileNavigator.fileNames[y - 2 + gui.fileListScroll].at(0) == '0')
					gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + y)].sprite = { 26, 5 };
				else if (fileNavigator.fileNames[y - 2 + gui.fileListScroll].at(0) == '1')
					gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + y)].sprite = { 27, 5 };
				else if (fileNavigator.fileNames[y - 2 + gui.fileListScroll].at(0) == '2')
					gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + y)].sprite = { 28, 5 };
				else
					gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + y)].sprite = { 29, 5 };
			}
			else
				DrawText("", wind->position.x + 1, wind->position.x + 39, wind->position.y + y, 3, 0);

			gui.activeUI[int(wind->position.x + 39)][int(wind->position.y + y)].sprite = { 31, 3 };
		}

		gui.activeUI[int(wind->position.x + 39)][int(wind->position.y + 2)].sprite = { 31, 2 };
		gui.activeUI[int(wind->position.x + 39)][int(wind->position.y + 18)].sprite = { 31, 4 };


		gui.scrollBars[4].topLeft.x = wind->position.x + 39;
		gui.scrollBars[4].topLeft.y = wind->position.y + 2;

		DrawText("EXPORT", wind->position.x + 30, wind->position.x + 36, wind->position.y + 19, 4, 2);
		gui.activeUI[int(wind->position.x + 29)][int(wind->position.y + 19)].sprite = { 24, 0 };
		gui.activeUI[int(wind->position.x + 36)][int(wind->position.y + 19)].sprite = { 25, 0 };

	}
	else if (wind->name == "Overwrite WAV")
	{
		DrawText("This file already", wind->position.x + 2, wind->position.x + 19, wind->position.y + 2, 4, 1);
		DrawText("exists. Would", wind->position.x + 2, wind->position.x + 16, wind->position.y + 3, 4, 1);
		DrawText("you like to", wind->position.x + 2, wind->position.x + 17, wind->position.y + 4, 4, 1);
		DrawText("Replace it?", wind->position.x + 2, wind->position.x + 17, wind->position.y + 5, 4, 1);

		gui.activeUI[int(wind->position.x + 7)][int(wind->position.y + 7)].sprite = { 24, 0 };
		DrawText("YES", wind->position.x + 8, wind->position.x + 12, wind->position.y + 7, 4, 2);
		gui.activeUI[int(wind->position.x + 12)][int(wind->position.y + 7)].sprite = { 25, 0 };

		gui.activeUI[int(wind->position.x + 7)][int(wind->position.y + 9)].sprite = { 24, 0 };
		DrawText(" NO", wind->position.x + 8, wind->position.x + 12, wind->position.y + 9, 4, 2);
		gui.activeUI[int(wind->position.x + 12)][int(wind->position.y + 9)].sprite = { 25, 0 };
	}
	else if (wind->name == "Sample Editor")
	{
		sampleDisplay.position.x = wind->position.x + 25;
		sampleDisplay.position.y = wind->position.y + 20.5;

		//gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + 28)].sprite = { 10, 7 };
		//gui.activeUI[int(wind->position.x + 2)][int(wind->position.y + 28)].sprite = { 11, 7 };


		// 14 x 30 tile additive synth
		//gui.activeUI[int(wind->position.x + 9)][int(wind->position.y + 2)].sprite = { 24, 0 };
		//DrawText("GENERATE", wind->position.x + 10, wind->position.x + 18, wind->position.y + 2, 4, 2);
		//gui.activeUI[int(wind->position.x + 18)][int(wind->position.y + 2)].sprite = { 25, 0 };


		// Synth parameter interface.
		DrawText("MODULATOR", wind->position.x + 3, wind->position.x + 12, wind->position.y + 1, 3, 0);
		DrawWaveTypeButton(sampleDisplay.modulatorType, int(wind->position.x) + 3, int(wind->position.y) + 2);

		DrawText("CARRIER", wind->position.x + 3, wind->position.x + 12, wind->position.y + 3, 3, 0);
		DrawWaveTypeButton(sampleDisplay.carrierType, int(wind->position.x) + 3, int(wind->position.y) + 4);


		// Envelope editor
		//DrawText("ADSR", wind->position.x + 3, wind->position.x + 7, wind->position.y + 6, 3, 0);
		if (loadedSamples.size() > 0)
			DrawText("A  " + std::to_string(loadedSamples[editor.selectedSample].attack), wind->position.x + 2, wind->position.x + 12, wind->position.y + 6, 3, 0);
		DrawKnob({ wind->position.x + 3, wind->position.y + 6 }, loadedSamples[editor.selectedSample].attack);

		if (loadedSamples.size() > 0)
			DrawText("D  " + std::to_string(loadedSamples[editor.selectedSample].decay), wind->position.x + 2, wind->position.x + 12, wind->position.y + 8, 3, 0);
		DrawKnob({ wind->position.x + 3, wind->position.y + 8 }, loadedSamples[editor.selectedSample].decay);

		if (loadedSamples.size() > 0)
			DrawText("S  " + std::to_string(loadedSamples[editor.selectedSample].sustain), wind->position.x + 2, wind->position.x + 12, wind->position.y + 10, 3, 0);
		DrawKnob({ wind->position.x + 3, wind->position.y + 10 }, loadedSamples[editor.selectedSample].sustain);

		if (loadedSamples.size() > 0)
			DrawText("R  " + std::to_string(loadedSamples[editor.selectedSample].release), wind->position.x + 2, wind->position.x + 12, wind->position.y + 12, 3, 0);
		DrawKnob({ wind->position.x + 3, wind->position.y + 12 }, loadedSamples[editor.selectedSample].release);

		/*
		if (sampleDisplay.modulatorSelected)
		{
			gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + 1)].sprite = { 5, 4 };
			gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + 3)].sprite = { 5, 3 };
		}
		else
		{
			gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + 1)].sprite = { 5, 3 };
			gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + 3)].sprite = { 5, 4 };
		}
		gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + 2)].sprite = { 2, 4 };
		gui.activeUI[int(wind->position.x + 1)][int(wind->position.y + 4)].sprite = { 2, 4 };
		*/


		//gui.activeUI[int(wind->position.x + 11)][int(wind->position.y + 3)].sprite = { 25, 0 };
		//gui.activeUI[int(wind->position.x + 20 + x)][int(wind->position.y + 14 - y)].sprite = { 13, 21 };



		for (int x = 0; x < 30; x++)
		{

			float offsetSprite = 0;
			if (x == 0 || x == 1 || x == 3 || x == 7 || x == 15 || x == 31 || x == 63)
				offsetSprite++;
			for (int y = 0; y < 14; y++)
			{
				if (y < sampleDisplay.frequencies[x])
				{
					if (sampleDisplay.frequencies[x] - y == 0.25)
						gui.activeUI[int(wind->position.x + 20 + x)][int(wind->position.y + 14 - y)].sprite = { 7, 21 + offsetSprite };
					else if (sampleDisplay.frequencies[x] - y == 0.5)
						gui.activeUI[int(wind->position.x + 20 + x)][int(wind->position.y + 14 - y)].sprite = { 8, 21 + offsetSprite };
					else if (sampleDisplay.frequencies[x] - y == 0.75)
						gui.activeUI[int(wind->position.x + 20 + x)][int(wind->position.y + 14 - y)].sprite = { 9, 21 + offsetSprite };
					else
						gui.activeUI[int(wind->position.x + 20 + x)][int(wind->position.y + 14 - y)].sprite = { 10, 21 + offsetSprite };


					//gui.activeUI[int(wind->position.x + 20 + x)][int(wind->position.y + 14 - y)].sprite = { 6.0f + int(y * 0.5), 21 };
				}
				else
				{
					gui.activeUI[int(wind->position.x + 20 + x)][int(wind->position.y + 14 - y)].sprite = { 6, 21 + offsetSprite };
					//gui.activeUI[int(wind->position.x + 20 + x)][int(wind->position.y + 14 - y)].sprite = { 13, 21 };
				}
			}
		}
		/*
		for (int x = 0; x < 30; x++)
		{
			for (int y = 0; y < 14; y++)
			{
				if (y < sampleDisplay.frequencies[x])
				{
					if (y < sampleDisplay.modFrequencies[x])
					{
						gui.activeUI[int(wind->position.x + 20 + x)][int(wind->position.y + 14 - y)].sprite = { 6.0f + int(y * 0.5), 22 };
					}
					else
					{
						gui.activeUI[int(wind->position.x + 20 + x)][int(wind->position.y + 14 - y)].sprite = { 6.0f + int(y * 0.5), 21 };
					}
				}
				else
				{
					if (y < sampleDisplay.modFrequencies[x])
					{
						gui.activeUI[int(wind->position.x + 20 + x)][int(wind->position.y + 14 - y)].sprite = { 6.0f + int(y * 0.5), 23 };
					}
					else
					{
						gui.activeUI[int(wind->position.x + 20 + x)][int(wind->position.y + 14 - y)].sprite = { 13, 21 };
					}
				}
			}
		}*/


		if (editor.selectedSample < loadedSamples.size() && loadedSamples[editor.selectedSample].looping)
			gui.activeUI[int(wind->position.x + 2)][int(wind->position.y + 28)].sprite = { 8, 7 };
		else
			gui.activeUI[int(wind->position.x + 2)][int(wind->position.y + 28)].sprite = { 8, 8 };

		gui.activeUI[int(wind->position.x + 4)][int(wind->position.y + 28)].sprite = { 9, 8 };
		gui.activeUI[int(wind->position.x + 5)][int(wind->position.y + 28)].sprite = { 10, 8 };
		gui.activeUI[int(wind->position.x + 6)][int(wind->position.y + 28)].sprite = { 11, 8 };
		


		


		//gui.scrollBars[5].topLeft.x = wind->position.x + 1;
		//gui.scrollBars[5].topLeft.y = wind->position.y + 27;

		

		//if (sampleDisplay.offset >= sampleDisplay.pcmData.size() * 0.5f - (960 / sampleDisplay.zoomFactor))
		//	sampleDisplay.offset = sampleDisplay.pcmData.size() * 0.5f - (960 / sampleDisplay.zoomFactor) - 1;
		//if (sampleDisplay.offset < 0)
		//	sampleDisplay.offset = 0;
		

		//DrawSampleDisplay();
	}
	


	return;
}



void DrawWaveTypeButton(int type, int xVal, int yVal)
{
	gui.activeUI[int(xVal - 1)][int(yVal)].sprite = { 24, 0 };
	switch (type)
	{
		case -1:
		{
			DrawText("---------", xVal, xVal + 9, yVal, 4, 2);
			break;
		}
		case 1:
		{
			DrawText("SQUARE", xVal, xVal + 9, yVal, 4, 2);
			break;
		}
		case 2:
		{
			DrawText("TRIANGLE", xVal, xVal + 9, yVal, 4, 2);
			break;
		}
		case 3:
		{
			DrawText("SAW", xVal, xVal + 9, yVal, 4, 2);
			break;
		}
		case 4:
		{
			DrawText("NOISE", xVal, xVal + 9, yVal, 4, 2);
			break;
		}

		default:
		{
			DrawText("SINE", xVal, xVal + 9, yVal, 4, 2);
			break;
		}
	}
	gui.activeUI[int(xVal + 9)][int(yVal)].sprite = { 25, 0 };


	return;
}



void DrawKnob(Vector2 pos, float val)
{
	// 0 - 0.125

	// 0.125 - 0.25

	// 0.25 - 0.375

	// 0.375 - 0.5

	// 0.5 - 0.625
	
	// 0.625 - 0.75

	// 0.75 - 0.875

	// 0.875 - 1.0

	val += 0.125 * 0.25f;


	if (val < 0.125)
	{
		gui.activeUI[int(pos.x)][int(pos.y)].sprite = { 0, 7 };
		gui.activeUI[int(pos.x + 1)][int(pos.y)].sprite = { 1, 7 };

		gui.activeUI[int(pos.x)][int(pos.y + 1)].sprite = { 4, 8 };
		gui.activeUI[int(pos.x + 1)][int(pos.y + 1)].sprite = { 5, 8 };
	}
	else if (val < 0.25)
	{
		gui.activeUI[int(pos.x)][int(pos.y)].sprite = { 0, 7 };
		gui.activeUI[int(pos.x + 1)][int(pos.y)].sprite = { 1, 7 };

		gui.activeUI[int(pos.x)][int(pos.y + 1)].sprite = { 6, 8 };
		gui.activeUI[int(pos.x + 1)][int(pos.y + 1)].sprite = { 1, 8 };
	}
	else if (val < 0.375)
	{
		gui.activeUI[int(pos.x)][int(pos.y)].sprite = { 2, 7 };
		gui.activeUI[int(pos.x + 1)][int(pos.y)].sprite = { 1, 7 };

		gui.activeUI[int(pos.x)][int(pos.y + 1)].sprite = { 2, 8 };
		gui.activeUI[int(pos.x + 1)][int(pos.y + 1)].sprite = { 1, 8 };
	}
	else if (val < 0.5)
	{
		gui.activeUI[int(pos.x)][int(pos.y)].sprite = { 6, 7 };
		gui.activeUI[int(pos.x + 1)][int(pos.y)].sprite = { 1, 7 };

		gui.activeUI[int(pos.x)][int(pos.y + 1)].sprite = { 0, 8 };
		gui.activeUI[int(pos.x + 1)][int(pos.y + 1)].sprite = { 1, 8 };
	}
	else if (val < 0.625)
	{
		gui.activeUI[int(pos.x)][int(pos.y)].sprite = { 4, 7 };
		gui.activeUI[int(pos.x + 1)][int(pos.y)].sprite = { 5, 7 };

		gui.activeUI[int(pos.x)][int(pos.y + 1)].sprite = { 0, 8 };
		gui.activeUI[int(pos.x + 1)][int(pos.y + 1)].sprite = { 1, 8 };
	}
	else if (val < 0.75)
	{
		gui.activeUI[int(pos.x)][int(pos.y)].sprite = { 0, 7 };
		gui.activeUI[int(pos.x + 1)][int(pos.y)].sprite = { 7, 7 };

		gui.activeUI[int(pos.x)][int(pos.y + 1)].sprite = { 0, 8 };
		gui.activeUI[int(pos.x + 1)][int(pos.y + 1)].sprite = { 1, 8 };
	}
	else if (val < 0.875)
	{
		gui.activeUI[int(pos.x)][int(pos.y)].sprite = { 0, 7 };
		gui.activeUI[int(pos.x + 1)][int(pos.y)].sprite = { 3, 7 };

		gui.activeUI[int(pos.x)][int(pos.y + 1)].sprite = { 0, 8 };
		gui.activeUI[int(pos.x + 1)][int(pos.y + 1)].sprite = { 3, 8 };
	}
	else
	{
		gui.activeUI[int(pos.x)][int(pos.y)].sprite = { 0, 7 };
		gui.activeUI[int(pos.x + 1)][int(pos.y)].sprite = { 1, 7 };

		gui.activeUI[int(pos.x)][int(pos.y + 1)].sprite = { 0, 8 };
		gui.activeUI[int(pos.x + 1)][int(pos.y + 1)].sprite = { 7, 8 };
	}



	return;
}