#pragma once
#pragma once

void Set_Select_Preset()
{
	int seltemp = -1;
	int temp = CurrentPreset;

	display.clearDisplay();
	display.setFont(NULL);
	printCentered("Select Preset", 4);
	
	display.setFont(&FreeSansBold18pt7b);
	printCentered(String(temp + 1), 48);
	display.display();
	delay(PAUSE_BETWEEN_SCREENS);

	AdaptiveButton Next = AdaptiveButton(Pin_NextScreen, &seltemp, 1, 0, 2);
	AdaptiveButton Prev = AdaptiveButton(Pin_PrevScreen, &seltemp, -1, 0, 2);
	AdaptiveButton	ValueUp = AdaptiveButton(Pin_ValueUp, &temp, 1, 0, 7);
	AdaptiveButton	ValueDown = AdaptiveButton(Pin_ValueDown, &temp, -1, 0, 7);

	while (!digitalRead(L1) || !digitalRead(L2) || !digitalRead(L3) || !digitalRead(R1) || !digitalRead(R2) || !digitalRead(R3))
	{
		delay(50);
		Serial.print(digitalRead(L1));
		Serial.print(digitalRead(L2));
		Serial.print(digitalRead(L3));
		Serial.print(digitalRead(R1));
		Serial.print(digitalRead(R2));
		Serial.println(digitalRead(R3));
	}

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
			CurrentPreset = temp;
			printCentered(String(temp + 1), 48);
			display.display();
		}

		if (Prev.Update())
		{
			exit = true;
			EEPROM.write(0,CurrentPreset);
			currentScreen = Page_Main;
		}
		else if (Next.Update())
		{
			exit = true;
			EEPROM.write(0, CurrentPreset);
			currentScreen = Page_Set_Pressure_Threshold;
		}

		if (abs(millis() - LastTimer) > TIME_TO_MAIN_PAGE)
		{
			exit = true;
			EEPROM.write(0, CurrentPreset);
			currentScreen = Page_Main;
		}

	}
}

