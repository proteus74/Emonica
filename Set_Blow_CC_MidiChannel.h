#pragma once
/*
	This is the Setuppage for the MIDI CC Channel, when blowing
*/


void Set_Blow_CC_MidiChannel()
{
	int seltemp = -1;
	int temp = Presets[CurrentPreset].Blow_CC_MidiChannel;
	const int MinVal = 1;
	const int MaxVal = 16;
	display.clearDisplay();
	DisplayPresetNumber();
	DrawProgressbar(temp, MinVal, MaxVal);
	display.setFont(NULL);
	printCentered("CC Midi", 0);
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
	AdaptiveButton	ValueDown = AdaptiveButton(Pin_ValueDown, &temp, -1, MinVal, MaxVal);


	boolean exit = false;
	LastTimer = millis();
	while (exit == false)
	{
		if (ValueUp.Update() || ValueDown.Update())
		{
			flagSaveConfig = true;
			LastTimer = millis();
			display.fillRect(0, 20, 127, 38, 0);
			display.setFont(&FreeSansBold18pt7b);
			Presets[CurrentPreset].Blow_CC_MidiChannel = temp;
			printCentered(String(temp), 48);
			DrawProgressbar(temp, MinVal, MaxVal);
			display.display();
		}

		if (Prev.Update())
		{
			exit = true;
			currentScreen = Page_Set_Pressure_Max;
		}
		if (Next.Update())
		{
			exit = true;
			currentScreen = Page_Set_Blow_CC_Controller;
		}
		if (abs(millis() - LastTimer) > TIME_TO_RETURN_TO_MAIN_PAGE)
		{
			exit = true;
			currentScreen = Page_Main;
		}

	}
}
