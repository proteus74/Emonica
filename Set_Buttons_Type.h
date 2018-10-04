#pragma once
#pragma once

void Set_Button_Type()
{

	String EnumButtonTypes[5] = { "Off","Pitch Bend","Transpose","CC Control","Midi Note" };

	int seltemp = -1;
	int temp = Presets[CurrentPreset].Buttons[ButtonToConfigure].Type;

	display.clearDisplay();
	DisplayPresetNumber();
	display.setFont(NULL);
	printCentered("Button" , 0);
	printCentered("Type ", 8);
	display.drawBitmap(0, 0, PushButton, 16, 16, 1);
	display.fillRect(0, 18, 127, 38, 0);
	display.setFont(&FreeSansBold9pt7b);
	printCentered(String(EnumButtonTypes[temp]), 44);
	DisplayButtonNumber();
	display.display();
	delay(PAUSE_BETWEEN_SCREENS);

	AdaptiveButton Next = AdaptiveButton(Pin_NextScreen, &seltemp, 1, 0, 2);
	AdaptiveButton Prev = AdaptiveButton(Pin_PrevScreen, &seltemp, -1, 0, 2);
	AdaptiveButton	ValueUp = AdaptiveButton(Pin_ValueUp, &temp, 1, 0, 4);
	AdaptiveButton	ValueDown = AdaptiveButton(Pin_ValueDown, &temp, -1, 0,4);


	boolean exit = false;
	LastTimer = millis();
	while (exit == false)
	{
		if (ValueUp.Update() || ValueDown.Update())
		{
			flagSaveConfig = true;
			LastTimer = millis();
			display.fillRect(0, 20, 127, 38, 0);
			display.setFont(&FreeSansBold9pt7b);
			Presets[CurrentPreset].Buttons[ButtonToConfigure].Type = temp;
			printCentered(String(EnumButtonTypes[temp]), 44);
			display.display();
		}

		if (Prev.Update())
		{
			

			if (ButtonToConfigure > 0)
			{
				ButtonToConfigure--;
				

				if (Presets[CurrentPreset].Buttons[ButtonToConfigure].Type == 1)
				{
					exit = true;
					currentScreen = Page_Set_Button_Type_Pitch_Bend_To;
				}
				else if (Presets[CurrentPreset].Buttons[ButtonToConfigure].Type == 2)
				{
					exit = true;
					currentScreen = Page_Set_Button_Type_Transpose_To;
				}
				else if (Presets[CurrentPreset].Buttons[ButtonToConfigure].Type == 3)
				{
					exit = true;
					currentScreen = Page_Set_Button_Type_CC_Max_Value;
				}
				else if (Presets[CurrentPreset].Buttons[ButtonToConfigure].Type == 4)
				{
					exit = true;
					currentScreen = Page_Set_Button_Type_Midi_Expression;
				}
				else
				{
					exit = true;
					currentScreen = Page_Set_Button_Type;
				}
			}
			else
			{

				if (Presets[CurrentPreset].Key_AdaptiveExpression)
				{
					exit = true;
					currentScreen = Page_Set_Key_Adaptive_Expression_Max;
				}
				else
				{
					exit = true;
					currentScreen = Page_Set_Key_Expression;
				}
				
			}

		}
		if (Next.Update())
		{

			if (temp == 0)  // Off
			{

				if (ButtonToConfigure < 5)
				{
					ButtonToConfigure++;
				}
				exit = true;
				currentScreen = Page_Set_Button_Type;
			}
			else if (temp == 1)  // Slide To
			{
				exit = true;
				currentScreen = Page_Set_Button_Type_Pitch_Bend_To;
			}
			else if (temp == 2)  // Transpose
			{
				exit = true;
				currentScreen = Page_Set_Button_Type_Transpose_To;
			}
			else if (temp == 3)  // CC
			{
				exit = true;
				currentScreen = Page_Set_Button_Type_CC_Midi_Channel;
			}
			else if (temp == 4)  // Play Note
			{
				exit = true;
				currentScreen = Page_Set_Button_Type_Midi_Note;
			}

		}
		if (abs(millis() - LastTimer) > TIME_TO_RETURN_TO_MAIN_PAGE)
		{
			exit = true;
			currentScreen = Page_Main;
		}
	}
}

