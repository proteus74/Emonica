#pragma once
void Set_Key_Adaptive_Expression()
{
	int seltemp = -1;
	int temp = Presets[CurrentPreset].Key_AdaptiveExpression;
	const int MinVal = 0;
	const int MaxVal = 1;
	display.clearDisplay();
	DisplayPresetNumber();
	display.setFont(NULL);
	printCentered("Adaptive", 0);
	printCentered("Expression", 8);
	display.drawBitmap(0, 0, Mundharmonic_Icon, 26, 16, 1);
	display.fillRect(0, 18, 127, 38, 0);
	display.setFont(&FreeSansBold18pt7b);
	printCentered((temp == 0 ? "off" : "on"), 48);
	
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
			Presets[CurrentPreset].Key_AdaptiveExpression = temp;


			printCentered((temp == 0 ? "off" : "on"), 48);
			display.display();
		}

		if (Prev.Update())
		{
			exit = true;
			currentScreen = Page_Set_Key_Scale;
		}
		if (Next.Update())
		{
			exit = true;
			if (temp == 0)
			{
				currentScreen = Page_Set_Key_Expression;
			}
			else
			{
				currentScreen = Page_Set_Key_Adaptive_Expression_Min;
			}
		}
		if (abs(millis() - LastTimer) > TIME_TO_MAIN_PAGE)
		{
			exit = true;
			currentScreen = Page_Main;
		}
	}
}