#pragma once



#include "GlobalVariables.h"
#include "FileHandling.cpp"

 

//void AnswerQuestion(int question, int answer, GLFWwindow* wind);

void ChangeTheme(int theme);

void ClickFloatingWindow(FloatingWindow* wind, int windowIndex, Vector2 clickPos, GLFWwindow* windMain);

void RightClickFloatingWindow(FloatingWindow* wind, int windowIndex, Vector2 clickPos, GLFWwindow* windMain);

void HoldClickFloatingWindow(FloatingWindow* wind, int windowIndex, Vector2 clickPos, GLFWwindow* windMain);

void RightHoldFloatingWindow(FloatingWindow* wind, int windowIndex, Vector2 clickPos, GLFWwindow* windMain);






void ClickFloatingWindow(FloatingWindow* wind, int windowIndex, Vector2 clickPos, GLFWwindow* windMain)
{
	clickPos.x = int(clickPos.x);
	clickPos.y = int(clickPos.y);


	
	

	if (clickPos.y == 0)
	{
		if (clickPos.x == wind->size.x - 2 || clickPos.x == wind->size.x - 1) // Exit window.
		{
			if (wind->name == "Sample Editor")
				sampleDisplay.visible = false;
			windowController.windows.erase(windowController.windows.begin() + windowIndex);
			windowController.windows.shrink_to_fit();
			gui.drawUIThisFrame = true;
			gui.drawFrameThisFrame = true;
			gui.scrollBars[4].topLeft = { -1,-1 }; // Reset the position of the file scrollbar to hide it.
			gui.scrollBars[4].position = 0;
			gui.scrollBars[4].length = 0;
			return;
		}
		else // Drag window.
		{
			wind->dragWindow = true;
			wind->dragPoint = clickPos;
		}
		// Exit
	}
	
	{

		if (wind->name == "Themes")
		{
			if (clickPos.y > 1)
			{
				if (int(clickPos.y) % 2 == 0)
				{
					ChangeTheme((clickPos.y - 2) / 2);
				}
			}
			
		}
		else if (wind->name == "Save and Exit")
		{
			if (clickPos.y == 6)
			{
				SaveSong();
				glfwSetWindowShouldClose(windMain, true);
			}
			else if (clickPos.y == 8)
				glfwSetWindowShouldClose(windMain, true);
		}
		else if (wind->name == "Save and Load")
		{
			if (clickPos.y == 7)
			{
				SaveSong();
				LoadSong(editor.fileToLoad);
				windowController.windows.erase(windowController.windows.begin() + windowIndex);
				windowController.windows.shrink_to_fit();
				gui.drawUIThisFrame = true;
				gui.drawFrameThisFrame = true;
				return;
			}
			else if (clickPos.y == 9)

			{
				loadedSong.unsavedChanges = false;
				LoadSong(editor.fileToLoad);
				windowController.windows.erase(windowController.windows.begin() + windowIndex);
				windowController.windows.shrink_to_fit();
				gui.drawUIThisFrame = true;
				gui.drawFrameThisFrame = true;
				return;
			}
		}
		else if (wind->name == "Delete Frames")
		{
			if (clickPos.y == 7)
			{
				// Check for unused frames.
				std::vector <bool> frameUsed;
				for (int i = 0; i < loadedSong.frames.size(); i++)
					frameUsed.emplace_back(false);

				for (int i = 0; i < loadedSong.frameSequence.size(); i++)
				{
					frameUsed[loadedSong.frameSequence[i]] = true;
				}

				std::vector <Frame> newFrames;
				for (int i = 0; i < frameUsed.size(); i++)
				{
					if (frameUsed[i] == true)
						newFrames.emplace_back(loadedSong.frames[i]);
				}

				loadedSong.frames = newFrames;

				SaveSong();
				windowController.windows.erase(windowController.windows.begin() + windowIndex);
				windowController.windows.shrink_to_fit();
				gui.drawUIThisFrame = true;
				gui.drawFrameThisFrame = true;
				return;
			}
			else if (clickPos.y == 9)
			{
				SaveSong();
				windowController.windows.erase(windowController.windows.begin() + windowIndex);
				windowController.windows.shrink_to_fit();
				gui.drawUIThisFrame = true;
				gui.drawFrameThisFrame = true;
				return;
			}
		}
		else if (wind->name == "Overwrite Song")
		{
			if (clickPos.y == 7)
			{
				loadedSong.overWriteOldSong = true;
				SaveSong();
				loadedSong.overWriteOldSong = false;
				windowController.windows.erase(windowController.windows.begin() + windowIndex);
				windowController.windows.shrink_to_fit();
				gui.drawUIThisFrame = true;
				gui.drawFrameThisFrame = true;
				return;
			}
			else if (clickPos.y == 9)
			{
				windowController.windows.erase(windowController.windows.begin() + windowIndex);
				windowController.windows.shrink_to_fit();
				gui.drawUIThisFrame = true;
				gui.drawFrameThisFrame = true;
				return;
			}
		}
		else if (wind->name == "Load File")
		{
			gui.scrollBars[4].length = 17;
			if (clickPos.y == 1 && clickPos.x == 1)
			{
				fileNavigator.ExitFile();
				gui.drawUIThisFrame = true;
				gui.drawFrameThisFrame = true;
			}
			else if (clickPos.y == 19 && clickPos.x > 28 && clickPos.x < 35) // Load.
			{
				if (fileNavigator.fileNames[editor.selectedFile].at(0) == '1') // Load Song
				{
					gui.drawUIThisFrame = true;
					gui.drawFrameThisFrame = true;
					std::string fileN = fileNavigator.fileNames[editor.selectedFile];
					fileN.erase(0, 1);
					LoadSong(fileN);
				}
				else if (fileNavigator.fileNames[editor.selectedFile].at(0) == '2') // Load Sample
				{
					Sample newSample;
					std::string fileN = fileNavigator.fileNames[editor.selectedFile];
					fileN.erase(0, 1);
					newSample.sampleName = fileN;
					newSample.filePath = "C:/" + fileNavigator.currentFilePath.std::filesystem::path::string() + "/" + fileN;
					//LoadSampleData(&newSample);
					if (editor.selectedSample >= loadedSamples.size())
					{
						loadedSamples.emplace_back(newSample);
						loadedSamples[loadedSamples.size() - 1].LoadSampleData();
					}
					else
					{
						editor.selectedSample++;
						loadedSamples.emplace(loadedSamples.begin() + editor.selectedSample, newSample);
						loadedSamples[editor.selectedSample].LoadSampleData();
						//LoadSampleData(editor.selectedSample);
					}
					if (sampleDisplay.visible)
					{
						toDrawSampleDisplay = true;
					}
					loadedSong.unsavedChanges = true;
				}
			}
			else if (clickPos.y > 0 && clickPos.x > 0 && clickPos.x < 39)
			{
				if (clickPos.y - 2 + gui.fileListScroll < fileNavigator.fileNames.size())
				{
					if (clickPos.x == 1)
					{
						if (fileNavigator.fileNames[clickPos.y - 2 + gui.fileListScroll].at(0) == '0')
						{
							std::string fileN = fileNavigator.fileNames[clickPos.y - 2 + gui.fileListScroll];
							fileN.erase(0, 1);
							fileNavigator.EnterFile(fileN);
						}
					}
					else
					{
						editor.selectedFile = clickPos.y - 2 + gui.fileListScroll;
					}
				}
			}
		}
		else if (wind->name == "Save Song")
		{
			gui.scrollBars[4].length = 17;
			if (clickPos.y == 1 && clickPos.x == 1)
			{
				fileNavigator.ExitFile();
				gui.drawUIThisFrame = true;
				gui.drawFrameThisFrame = true;
			}
			else if (clickPos.y == 19 && clickPos.x > 28 && clickPos.x < 35) // Save.
			{
				//std::string fileN = fileNavigator.fileNames[editor.selectedFile];
				//fileN.erase(0, 1);
				SaveSong();
			}
			else if (clickPos.y > 0 && clickPos.x > 0 && clickPos.x < 39)
			{
				if (clickPos.y - 2 + gui.fileListScroll < fileNavigator.fileNames.size())
				{
					if (clickPos.x == 1)
					{
						if (fileNavigator.fileNames[clickPos.y - 2 + gui.fileListScroll].at(0) == '0')
						{
							std::string fileN = fileNavigator.fileNames[clickPos.y - 2 + gui.fileListScroll];
							fileN.erase(0, 1);
							fileNavigator.EnterFile(fileN);
						}
					}
					else
					{
						editor.selectedFile = clickPos.y - 2 + gui.fileListScroll;
					}
				}
			}
		}
		else if (wind->name == "Config")
		{
			if (clickPos.y == 2)
			{
				windowController.windows.erase(windowController.windows.begin() + windowIndex);
				windowController.windows.shrink_to_fit();
				windowController.InitializeWindow("Themes", wind->position, { 16, 34 });
			}
			else if (clickPos.y == 4)
			{
				editor.channelFocus = !editor.channelFocus;
				if (editor.channelFocus)
				{
					for (int ch = 0; ch < channels.size(); ch++)
						channels[ch].compressed = true;
				}
				SaveSettings();
			}
			if (clickPos.y == 6)
			{
				gui.pianoMode = !gui.pianoMode;
				SaveSettings();
			}

		}
		else if (wind->name == "Selection")
		{
			if (clickPos.y == 2)
			{
				copyNotes();
				windowController.windows.erase(windowController.windows.begin() + windowIndex);
				windowController.windows.shrink_to_fit();
			}
			else if (clickPos.y == 4)
			{
				pasteNotes();
				windowController.windows.erase(windowController.windows.begin() + windowIndex);
				windowController.windows.shrink_to_fit();
			}
			else if (clickPos.y == 6)
			{
				deleteNotes();
				windowController.windows.erase(windowController.windows.begin() + windowIndex);
				windowController.windows.shrink_to_fit();
			}
			else if (clickPos.y == 8)
			{
				windowController.windows.erase(windowController.windows.begin() + windowIndex);
				windowController.windows.shrink_to_fit();
				editor.transposeValue = 0;
				windowController.InitializeWindow("Transpose", wind->position, { 16, 10 });
			}
			else if (clickPos.y == 10)
			{
				setNoteSamples();
				windowController.windows.erase(windowController.windows.begin() + windowIndex);
				windowController.windows.shrink_to_fit();
			}
		}
		else if (wind->name == "Transpose")
		{
			if (clickPos.y == 3)
			{
				if (clickPos.x == 2)
					editor.transposeValue += 12;
				if (clickPos.x == 4)
					editor.transposeValue -= 12;
			}
			else if (clickPos.y == 5)
			{
				if (clickPos.x == 2)
					editor.transposeValue++;
				if (clickPos.x == 4)
					editor.transposeValue--;
			}
			else if (clickPos.y == 9)
			{
				if (clickPos.x > 7 && clickPos.x < 15)
				{
					transposeNotes();
				}
			}
		}
		else if (wind->name == "Help")
		{
			gui.scrollBars[4].length = 37;
		}
		else if (wind->name == "Export as .WAV")
		{
			gui.scrollBars[4].length = 17;
			if (clickPos.y == 1 && clickPos.x == 1)
			{
				fileNavigator.ExitFile();
				gui.drawUIThisFrame = true;
				gui.drawFrameThisFrame = true;
			}
			else if (clickPos.y == 19 && clickPos.x > 28 && clickPos.x < 37) // Save.
			{
				//std::string fileN = fileNavigator.fileNames[editor.selectedFile];
				//fileN.erase(0, 1);
				windowController.windows.erase(windowController.windows.begin() + windowIndex);
				windowController.windows.shrink_to_fit();
				for (int name = 0; name < fileNavigator.fileNames.size(); name++)
				{
					std::string indexName = fileNavigator.fileNames[name];
					indexName.erase(0, 1);
					if (indexName == loadedSong.songName + ".wav")
					{
						windowController.InitializeWindow("Overwrite WAV", { 22, 17 }, { 20, 16 });
						return;
					}
				}
				editor.recordingSong = true;
				//SaveSong();
			}
			else if (clickPos.y > 0 && clickPos.x > 0 && clickPos.x < 39)
			{
				if (clickPos.y - 2 + gui.fileListScroll < fileNavigator.fileNames.size())
				{
					if (clickPos.x == 1)
					{
						if (fileNavigator.fileNames[clickPos.y - 2 + gui.fileListScroll].at(0) == '0')
						{
							std::string fileN = fileNavigator.fileNames[clickPos.y - 2 + gui.fileListScroll];
							fileN.erase(0, 1);
							fileNavigator.EnterFile(fileN);
						}
					}
					else
					{
						editor.selectedFile = clickPos.y - 2 + gui.fileListScroll;
					}
				}
			}
		}
		else if (wind->name == "Overwrite WAV")
		{
			if (clickPos.y == 7)
			{
				editor.recordingSong = true;
				windowController.windows.erase(windowController.windows.begin() + windowIndex);
				windowController.windows.shrink_to_fit();
				gui.drawUIThisFrame = true;
				gui.drawFrameThisFrame = true;
				return;
			}
			else if (clickPos.y == 9)
			{
				windowController.windows.erase(windowController.windows.begin() + windowIndex);
				windowController.windows.shrink_to_fit();
				gui.drawUIThisFrame = true;
				gui.drawFrameThisFrame = true;
				return;
			}
		}
		else if (wind->name == "Sample Editor")
		{

			// Synth UI.
			if (clickPos.x > 0 && clickPos.x < 12)
			{
				if (clickPos.y == 2) // Change modulator type
				{
					sampleDisplay.modulatorType++;
					if (sampleDisplay.modulatorType > 4)
						sampleDisplay.modulatorType = -1;
					toGenerateAdditiveWave = true;
					return;
				}
				else if (clickPos.y == 4) // Change carrier type
				{
					sampleDisplay.carrierType++;
					if (sampleDisplay.carrierType > 4)
						sampleDisplay.carrierType = 0;
					return;
				}
			}

			// Envelope editing knobs.
			sampleDisplay.knobSelected = -1;
			if (clickPos.x > 2 && clickPos.x < 5)
			{
				if (clickPos.y > 5)
				{
					if (clickPos.y < 14)
					{
						sampleDisplay.knobSelected = (clickPos.y - 6) * 0.5;
					}
				}
			}
			


			if (editor.selectedSample < loadedSamples.size())
			{
				if (clickPos.y > 14 && clickPos.y < 28 && editor.selectedSample > -1 && editor.selectedSample < loadedSamples.size())
				{
					if (clickPos.y < 16) // Set jump points.
					{
						float posX = (gui.floatHoveredTile.x - windowController.windows[windowIndex].position.x) * 16.0f - 8.0f;
						int frameIndex = (posX / 800.0f) * (((sampleDisplay.pcmData.size() / sampleDisplay.zoomFactor) * 0.5f)) + sampleDisplay.offset;

						if (loadedSamples[editor.selectedSample].jumpPoints.size() == 0)
							loadedSamples[editor.selectedSample].jumpPoints.emplace_back(frameIndex);
						else
						{
							bool afterLastJump = false;
							int addPos = 0;
							while (!afterLastJump)
							{
								if (addPos >= loadedSamples[editor.selectedSample].jumpPoints.size())
									afterLastJump = true;
								else if (loadedSamples[editor.selectedSample].jumpPoints[addPos] <= frameIndex)
								{
									afterLastJump = true;
								}
								else
								{
									addPos++;
								}
							}

							loadedSamples[editor.selectedSample].jumpPoints.emplace(loadedSamples[editor.selectedSample].jumpPoints.begin() + addPos, frameIndex);
						}
					}
					else if (clickPos.y > 25) // Move loop points.
					{
						float posX = (gui.floatHoveredTile.x - windowController.windows[windowIndex].position.x) * 16.0f - 8.0f;
						int frameIndex = (posX / 800.0f) * (((sampleDisplay.pcmData.size() / sampleDisplay.zoomFactor) * 0.5f)) + sampleDisplay.offset;

						float distToStart = abs(loadedSamples[editor.selectedSample].loopStart - frameIndex);
						float distToEnd = abs(loadedSamples[editor.selectedSample].loopEnd - frameIndex);

						if (distToStart < distToEnd)
							loadedSamples[editor.selectedSample].loopStart = frameIndex;
						else
							loadedSamples[editor.selectedSample].loopEnd = frameIndex;
						toDrawSampleDisplay = true;
						return;
					}
					else
					{
						float posX = (gui.floatHoveredTile.x - windowController.windows[windowIndex].position.x) * 16.0f - 8.0f;
						int frameIndex = (posX / 800.0f) * (((sampleDisplay.pcmData.size() / sampleDisplay.zoomFactor) * 0.5f)) + sampleDisplay.offset;
						ma_decoder_seek_to_pcm_frame(&g_pDecoders[0], frameIndex);
						sampleDisplay.sampleStartPos = frameIndex;
					}
					toDrawSampleDisplay = true;
				}
				else if (clickPos.y == 28)
				{
					/*
					if (clickPos.x == 1) // Zoom in
					{
						sampleDisplay.zoomFactor *= 2.0f;
						if (sampleDisplay.zoomFactor > 2048.0f)
							sampleDisplay.zoomFactor = 2048.0f;
						DrawSampleDisplay();
					}
					else if (clickPos.x == 2) // Zoom out
					{
						sampleDisplay.zoomFactor *= 0.5f;
						if (sampleDisplay.zoomFactor < 1.0f)
							sampleDisplay.zoomFactor = 1.0f;
						DrawSampleDisplay();
					}
					else*/
					if (clickPos.x == 2) // Set sample to loop
					{
						loadedSamples[editor.selectedSample].looping = !loadedSamples[editor.selectedSample].looping;
						loadedSamples[editor.selectedSample].loopStart = 0;

						//float posX = (799) * 16.0f - 8.0f;

						//ma_decoder_seek_to_pcm_frame(&g_pDecoders[0], frameIndex);
						loadedSamples[editor.selectedSample].loopEnd = (sampleDisplay.pcmData.size() * 0.5f) - 1.0f;

						toDrawSampleDisplay = true;
					}
					else if (clickPos.x == 4) // Play sample
					{
						if (!editor.playingSong) // Play the note sound.
						{
							channels[0].volume = 64.0f;
							//channelCallbackToStopForNote[selectedChannel] = true;
							//channels[selectedChannel].noteToPlaySample = selectedSample;
							//channels[selectedChannel].noteToPlayPitch = noteNum;

							StartSample(0, editor.selectedSample, 48, sampleDisplay.sampleStartPos);
							//InitializeSample(0, editor.selectedSample, 48);
							//ma_decoder_seek_to_pcm_frame(&g_pDecoders[0], sampleDisplay.samplePos);

							//channels[ch].arpNotes[channels[ch].arpIndex]
							channels[0].arpNotes[0] = 48;
							channels[0].arpIndex = 0;
							DrawPiano();
							sampleDisplay.pauseSample = false;
							DrawPiano();

							sampleDisplay.playingSample = true;
						}
					}
					else if (clickPos.x == 5) // Return sample to start
					{
						ma_decoder_seek_to_pcm_frame(&g_pDecoders[0], 0);
						sampleDisplay.samplePos = 0;
						sampleDisplay.sampleStartPos = 0;
						toDrawSampleDisplay = true;
					}
					else if (clickPos.x == 6) // Pause sample
					{
						sampleDisplay.pauseSample = true;
						sampleDisplay.playingSample = false;
						toDrawSampleDisplay = true;
					}
				}
			}
		}
	}


	if (windowIndex > 0) // Move the dragged window to front.
	{
		FloatingWindow moveWind = windowController.windows[windowIndex];
		windowController.windows.erase(windowController.windows.begin() + windowIndex);
		windowController.windows.emplace(windowController.windows.begin(), moveWind);
		windowController.windows.shrink_to_fit();
		windowIndex = 0;
	}

	return;
}



void RightClickFloatingWindow(FloatingWindow* wind, int windowIndex, Vector2 clickPos, GLFWwindow* windMain)
{
	clickPos.x = int(clickPos.x);
	clickPos.y = int(clickPos.y);





	if (clickPos.y > 0)
	{

		if (wind->name == "Sample Editor")
		{
			if (editor.selectedSample < loadedSamples.size())
			{
				


				if (clickPos.y > 14 && clickPos.y < 28 && editor.selectedSample > -1 && editor.selectedSample < loadedSamples.size())
				{
					if (clickPos.y < 16) // Delete jump points.
					{
						float posX = (gui.floatHoveredTile.x - windowController.windows[windowIndex].position.x) * 16.0f - 8.0f;
						for (int i = 0; i < loadedSamples[editor.selectedSample].jumpPoints.size(); i++)
						{
							int iPos = (loadedSamples[editor.selectedSample].jumpPoints[i]) * (1.0f / sampleDisplay.pcmData.size()) * 2.0f * 800.0f;
							if (abs(iPos - posX) < 8)
							{
								loadedSamples[editor.selectedSample].jumpPoints.erase(loadedSamples[editor.selectedSample].jumpPoints.begin() + i);
								loadedSamples[editor.selectedSample].jumpPoints.shrink_to_fit();
								toDrawSampleDisplay = true;
								return;
							}
						}
					}
					else // Open the selection menu.
					{
						
					}
					toDrawSampleDisplay = true;
				}
				else if (clickPos.y == 28)
				{
					/*
					if (clickPos.x == 1) // Zoom in
					{
						sampleDisplay.zoomFactor *= 2.0f;
						if (sampleDisplay.zoomFactor > 2048.0f)
							sampleDisplay.zoomFactor = 2048.0f;
						DrawSampleDisplay();
					}
					else if (clickPos.x == 2) // Zoom out
					{
						sampleDisplay.zoomFactor *= 0.5f;
						if (sampleDisplay.zoomFactor < 1.0f)
							sampleDisplay.zoomFactor = 1.0f;
						DrawSampleDisplay();
					}
					else*/
					if (clickPos.x == 2) // Set sample to loop
					{
						loadedSamples[editor.selectedSample].looping = !loadedSamples[editor.selectedSample].looping;
						loadedSamples[editor.selectedSample].loopStart = 0;

						//float posX = (799) * 16.0f - 8.0f;

						//ma_decoder_seek_to_pcm_frame(&g_pDecoders[0], frameIndex);
						loadedSamples[editor.selectedSample].loopEnd = (sampleDisplay.pcmData.size() * 0.5f) - 1.0f;

						toDrawSampleDisplay = true;
					}
					else if (clickPos.x == 4) // Play sample
					{
						if (!editor.playingSong) // Play the note sound.
						{
							channels[0].volume = 64.0f;
							//channelCallbackToStopForNote[selectedChannel] = true;
							//channels[selectedChannel].noteToPlaySample = selectedSample;
							//channels[selectedChannel].noteToPlayPitch = noteNum;

							StartSample(0, editor.selectedSample, 48, sampleDisplay.sampleStartPos);
							//InitializeSample(0, editor.selectedSample, 48);
							//ma_decoder_seek_to_pcm_frame(&g_pDecoders[0], sampleDisplay.samplePos);

							//channels[ch].arpNotes[channels[ch].arpIndex]
							channels[0].arpNotes[0] = 48;
							channels[0].arpIndex = 0;
							DrawPiano();
							sampleDisplay.pauseSample = false;
							DrawPiano();

							sampleDisplay.playingSample = true;
						}
					}
					else if (clickPos.x == 5) // Return sample to start
					{
						ma_decoder_seek_to_pcm_frame(&g_pDecoders[0], 0);
						sampleDisplay.samplePos = 0;
						sampleDisplay.sampleStartPos = 0;
						toDrawSampleDisplay = true;
					}
					else if (clickPos.x == 6) // Pause sample
					{
						sampleDisplay.pauseSample = true;
						sampleDisplay.playingSample = false;
						toDrawSampleDisplay = true;
					}
				}
			}
		}
	}


	if (windowIndex > 0) // Move the dragged window to front.
	{
		FloatingWindow moveWind = windowController.windows[windowIndex];
		windowController.windows.erase(windowController.windows.begin() + windowIndex);
		windowController.windows.emplace(windowController.windows.begin(), moveWind);
		windowController.windows.shrink_to_fit();
		windowIndex = 0;
	}

	return;
}




void HoldClickFloatingWindow(FloatingWindow* wind, int windowIndex, Vector2 clickPos, GLFWwindow* windMain)
{
	//clickPos.x = int(clickPos.x);
	//clickPos.y = int(clickPos.y);


	if (loadedSamples.size() > 0 && sampleDisplay.knobSelected > -1) // Turn envelope knobs.
	{
		if (sampleDisplay.knobSelected == 0)
		{
			float angle = atan2(3.0f - clickPos.x, 6.0f - clickPos.y);
			loadedSamples[editor.selectedSample].attack = 0.5f - (angle / (6.283f));
		}
		else if (sampleDisplay.knobSelected == 1)
		{
			float angle = atan2(3.0f - clickPos.x, 8.0f - clickPos.y);
			loadedSamples[editor.selectedSample].decay = 0.5f - (angle / (6.283f));
		}
		else if (sampleDisplay.knobSelected == 2)
		{
			float angle = atan2(3.0f - clickPos.x, 10.0f - clickPos.y);
			loadedSamples[editor.selectedSample].sustain = 0.5f - (angle / (6.283f));
		}
		else if (sampleDisplay.knobSelected == 3)
		{
			float angle = atan2(3.0f - clickPos.x, 12.0f - clickPos.y);
			loadedSamples[editor.selectedSample].release = 0.5f - (angle / (6.283f));
		}

		DrawSampleDisplay();
	}


	if (clickPos.y > 0)
	{

		if (wind->name == "Sample Editor")
		{
			if (clickPos.x > 19 && clickPos.x < 50 && clickPos.y > 1 && clickPos.y < 15) // Change frequencies.
			{
				sampleDisplay.frequencies[int(clickPos.x) - 20] = 14.0f - float(int(clickPos.y * 4.0f)) * 0.25f;

				toGenerateAdditiveWave = true;
				//GenerateAdditiveWave();
				//DrawSampleDisplay();
			}
		}
	}



	return;
}



void RightHoldFloatingWindow(FloatingWindow* wind, int windowIndex, Vector2 clickPos, GLFWwindow* windMain)
{
	clickPos.x = int(clickPos.x);
	clickPos.y = int(clickPos.y);





	if (clickPos.y > 0)
	{

		if (wind->name == "Sample Editor")
		{
			if (clickPos.x > 19 && clickPos.x < 50 && clickPos.y > 0 && clickPos.y < 15) // Return sample to start
			{
				sampleDisplay.modFrequencies[int(clickPos.x) - 20] = (14 - clickPos.y);

				toGenerateAdditiveWave = true;
				//GenerateAdditiveWave();
				//DrawSampleDisplay();
			}
		}
	}



	return;
}




void ChangeTheme(int theme)
{
	int sizeX, sizeY, comps;
	glBindTexture(GL_TEXTURE_2D, gui.uiTexture);
	unsigned char* data = stbi_load("Tiles.png", &sizeX, &sizeY, &comps, 3);

	GUITheme currentTheme = gui.themes[theme];
	/*
	if (gui.uiBrightMode)
	{
		for (int i = 0; i < 6; i++)
			currentTheme.uiColors[i] = gui.themes[theme].uiColors[5 - i];
	}*/

	for (int i = 0; i < 512 * 512; i++)
	{
		unsigned char* colPos = data + i * 3;
		if (colPos[0] == 0 && colPos[1] == 0 && colPos[2] == 0)
		{
			data[i * 3] = currentTheme.uiColors[0].r;
			data[i * 3 + 1] = currentTheme.uiColors[0].g;
			data[i * 3 + 2] = currentTheme.uiColors[0].b;
		}
		else if (colPos[0] == 0 && colPos[1] == 0 && colPos[2] == 40)
		{
			data[i * 3] = gui.themes[theme].uiColors[1].r * 0.5f + gui.themes[theme].uiColors[0].r * 0.5f;
			data[i * 3 + 1] = gui.themes[theme].uiColors[1].g * 0.5f + gui.themes[theme].uiColors[0].g * 0.5f;
			data[i * 3 + 2] = gui.themes[theme].uiColors[1].b * 0.5f + gui.themes[theme].uiColors[0].b * 0.5f;
		}
		else if (colPos[0] == 30)
		{
			data[i * 3] = currentTheme.uiColors[1].r;
			data[i * 3 + 1] = currentTheme.uiColors[1].g;
			data[i * 3 + 2] = currentTheme.uiColors[1].b;
		}
		else if (colPos[0] == 50)
		{
			data[i * 3] = currentTheme.uiColors[2].r;
			data[i * 3 + 1] = currentTheme.uiColors[2].g;
			data[i * 3 + 2] = currentTheme.uiColors[2].b;
		}
		else if (colPos[0] == 90)
		{
			data[i * 3] = currentTheme.uiColors[3].r;
			data[i * 3 + 1] = currentTheme.uiColors[3].g;
			data[i * 3 + 2] = currentTheme.uiColors[3].b;
		}
		else if (colPos[0] == 150)
		{
			data[i * 3] = currentTheme.uiColors[4].r;
			data[i * 3 + 1] = currentTheme.uiColors[4].g;
			data[i * 3 + 2] = currentTheme.uiColors[4].b;
		}
		else if (colPos[0] == 255 && colPos[1] == 255 && colPos[2] == 255)
		{
			data[i * 3] = currentTheme.uiColors[5].r;
			data[i * 3 + 1] = currentTheme.uiColors[5].g;
			data[i * 3 + 2] = currentTheme.uiColors[5].b;
		}
		// Accent colors
		else if (colPos[0] == 63 && colPos[1] == 100 && colPos[2] == 100)
		{
			data[i * 3] = currentTheme.uiColors[6].r;
			data[i * 3 + 1] = currentTheme.uiColors[6].g;
			data[i * 3 + 2] = currentTheme.uiColors[6].b;
		}
		else if (colPos[0] == 127 && colPos[1] == 190 && colPos[2] == 0)
		{
			data[i * 3] = currentTheme.uiColors[7].r;
			data[i * 3 + 1] = currentTheme.uiColors[7].g;
			data[i * 3 + 2] = currentTheme.uiColors[7].b;
		}
		else if (colPos[0] == 255 && colPos[1] == 255 && colPos[2] == 0)
		{
			data[i * 3] = currentTheme.uiColors[8].r;
			data[i * 3 + 1] = currentTheme.uiColors[8].g;
			data[i * 3 + 2] = currentTheme.uiColors[8].b;
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	// Set the ui colors.
	gui.uiColors[0] = gui.themes[theme].uiColors[0].r / 255.0f;
	gui.uiColors[1] = gui.themes[theme].uiColors[0].g / 255.0f;
	gui.uiColors[2] = gui.themes[theme].uiColors[0].b / 255.0f;

	gui.uiColors[3] = gui.themes[theme].uiColors[1].r / 255.0f;
	gui.uiColors[4] = gui.themes[theme].uiColors[1].g / 255.0f;
	gui.uiColors[5] = gui.themes[theme].uiColors[1].b / 255.0f;

	gui.uiColors[6] = gui.themes[theme].uiColors[2].r / 255.0f;
	gui.uiColors[7] = gui.themes[theme].uiColors[2].g / 255.0f;
	gui.uiColors[8] = gui.themes[theme].uiColors[2].b / 255.0f;

	gui.uiColors[9] = gui.themes[theme].uiColors[3].r / 255.0f;
	gui.uiColors[10] = gui.themes[theme].uiColors[3].g / 255.0f;
	gui.uiColors[11] = gui.themes[theme].uiColors[3].b / 255.0f;

	gui.uiColors[12] = gui.themes[theme].uiColors[4].r / 255.0f;
	gui.uiColors[13] = gui.themes[theme].uiColors[4].g / 255.0f;
	gui.uiColors[14] = gui.themes[theme].uiColors[4].b / 255.0f;

	gui.uiColors[15] = gui.themes[theme].uiColors[5].r / 255.0f;
	gui.uiColors[16] = gui.themes[theme].uiColors[5].g / 255.0f;
	gui.uiColors[17] = gui.themes[theme].uiColors[5].b / 255.0f;


	gui.uiColors[45] = gui.themes[theme].uiColors[7].r / 255.0f;
	gui.uiColors[46] = gui.themes[theme].uiColors[7].g / 255.0f;
	gui.uiColors[47] = gui.themes[theme].uiColors[7].b / 255.0f;

	gui.uiColors[48] = gui.themes[theme].uiColors[7].r / 255.0f;
	gui.uiColors[49] = gui.themes[theme].uiColors[7].g / 255.0f;
	gui.uiColors[50] = gui.themes[theme].uiColors[7].b / 255.0f;

	gui.uiColors[51] = gui.themes[theme].uiColors[8].r / 255.0f;
	gui.uiColors[52] = gui.themes[theme].uiColors[8].g / 255.0f;
	gui.uiColors[53] = gui.themes[theme].uiColors[8].b / 255.0f;

	gui.uiColorTheme = theme;
	SaveSettings();

	if (sampleDisplay.visible)
	{
		toDrawSampleDisplay = true;
	}
}