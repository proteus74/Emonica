#pragma once

void Set_Breath_CC_Controller()
{
	int seltemp = -1;
	int temp = Presets[CurrentPreset].Breath_CC_Controller;
	const int MinVal = 0;
	const int MaxVal = 127;
	display.clearDisplay();
	DisplayPresetNumber();
	DrawProgressbar(temp, MinVal, MaxVal);
	display.setFont(NULL);
	printCentered("CC", 0);
	printCentered("Channel", 9);
	display.drawBitmap(0, 0, Mouth_Icon, 27, 16, 1);
	display.fillRect(0, 18, 127, 38, 0);
	display.setFont(&FreeSansBold18pt7b);
	printCentered(String(temp), 48);
	display.display();
	delay(PAUSE_BETWEEN_SCREENS);

	AdaptiveButton Next = AdaptiveButton(Pin_NextScreen, &seltemp, 1, 0, 2);
	AdaptiveButton Prev = AdaptiveButton(Pin_PrevScreen, &seltemp, -1, 0, 2);
	AdaptiveButton	ValueUp = AdaptiveButton(Pin_ValueUp, &temp, 1, MinVal, MaxVal);
	AdaptiveButton	ValueDown = AdaptiveButton(Pin_ValueDown, &temp, -1, MinVal,MaxVal);


	boolean exit = false;
	LastTimer = millis();
	while (exit == false)
	{
		if (ValueUp.Update() || ValueDown.Update())
		{
			LastTimer = millis();
			display.fillRect(0, 20, 127, 38, 0);
			display.setFont(&FreeSansBold18pt7b);
			Presets[CurrentPreset].Breath_CC_Controller = temp;
			printCentered(String(temp), 48);
			DrawProgressbar(temp, MinVal, MaxVal);
			flagSaveConfig = true;
			display.display();
		}

		if (Prev.Update())
		{
			exit = true;
			currentScreen = Page_Set_Breath_CC_Midi_Channel;
		}
		  if (Next.Update())
		{
			exit = true;
			currentScreen = Page_Set_Breath_CC_Min_Value;
		}

		  if (abs(millis() - LastTimer) > TIME_TO_MAIN_PAGE)
		  {
			  exit = true;
			  currentScreen = Page_Main;
		  }
	}
}
