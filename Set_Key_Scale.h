#pragma once

void Set_Key_Scale()
{
	int seltemp = -1;
	int temp = Presets[CurrentPreset].Key_Scale;

	display.clearDisplay();
	DisplayPresetNumber();
	DisplayPresetNumber();
	display.setFont(NULL);
	
	printCentered("Scale", 5);


	display.drawBitmap(0, 0, Harmonic_Icon, 26, 16, 1);
	display.fillRect(0, 18, 127, 38, 0);
	display.setFont(&FreeSans9pt7b);
	if (ScaleIntervals[temp].Name2 == "")
	{
		printCentered(ScaleIntervals[temp].Name, 44);

	}
	else
	{
		printCentered(ScaleIntervals[temp].Name, 34);
		printCentered(ScaleIntervals[temp].Name2, 54);
	}
	display.display();
	delay(PAUSE_BETWEEN_SCREENS);

	AdaptiveButton Next = AdaptiveButton(Pin_NextScreen, &seltemp, 1, 0, 2);
	AdaptiveButton Prev = AdaptiveButton(Pin_PrevScreen, &seltemp, -1, 0, 2);
	AdaptiveButton	ValueUp = AdaptiveButton(Pin_ValueUp, &temp, 1, 0, SCALECOUNTS -1 );
	AdaptiveButton	ValueDown = AdaptiveButton(Pin_ValueDown, &temp, -1, 0, SCALECOUNTS - 1);


	boolean exit = false;
	LastTimer = millis();
	while (exit == false)
	{
		if (ValueUp.Update() || ValueDown.Update())
		{
			flagSaveConfig = true;
			LastTimer = millis();
			display.fillRect(0, 20, 127, 38, 0);
			display.setFont(&FreeSans9pt7b);
			Presets[CurrentPreset].Key_Scale = temp;
			CalculatedScaleSteps = CalculateScaleSteps();
			if (ScaleIntervals[temp].Name2 == "")
			{
				printCentered(ScaleIntervals[temp].Name, 44);
				
			}
			else
			{
				printCentered(ScaleIntervals[temp].Name, 34);
				printCentered(ScaleIntervals[temp].Name2, 54);
			}
			display.display();
		}

		if (Prev.Update())
		{
			exit = true;
			currentScreen = Page_Set_Key_High_Note;
		}
		if (Next.Update())
		{
			exit = true;
			currentScreen = Page_Set_Key_Adaptive_Expression;
		}
		if (abs(millis() - LastTimer) > TIME_TO_RETURN_TO_MAIN_PAGE)
		{
			exit = true;
			currentScreen = Page_Main;
		}
	}
}


