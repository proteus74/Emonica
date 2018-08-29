#pragma once

void MainScreen()
{


	if (flagSaveConfig == true)
	{
		Serial.print("Save Conig\n");
		EEPROM_writeAnything(1,Presets);
		Serial.print("Finsih\n");
		flagSaveConfig = false;
	}

	int seltemp = -1;
	int temp = Presets[CurrentPreset].Breath_Max_Pressure;
	
	display.clearDisplay();
	DisplayPresetNumber();
 
	display.drawBitmap(30, 46, Logo_Klein, 76, 18, 1);
//	display.display();
	//delay(PAUSE_BETWEEN_SCREENS);
	



	JumpToSetup = false;
	while (JumpToSetup == false)
	{
		delay(50);
		RefreshDisplay = true;
		if (RefreshDisplay)
		{
			
			display.fillRect(0, 0, 113, 40, 0);
			display.setFont(&FreeSansBold24pt7b);
			printCentered( MidiToNote(CurrentNote), 34);
			
			int Test = map(CurrentPressureRaw, Presets[CurrentPreset].Breath_Pressure_Threshold, Presets[CurrentPreset].Breath_Max_Pressure, 0,36);
			Test = constrain(Test, 0,36);

			double factor = ((double)125 / (double)127)  * (double)Test;
			//Serial.print(Test);

			display.drawRect(4, 0, 12, 38, 1);
			display.fillRect(5, 1, 10, 36, 1);
			display.fillRect(5, 1, 10, 36 - factor, 0);

			
	 
			display.display();
			RefreshDisplay = false;
		//	Serial.println(CurrentPressureRaw);
		}
	}
	currentScreen = Page_Select_Preset;
}

