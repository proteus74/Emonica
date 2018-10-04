/*
	------------------------------------------------------------------------------------------------------
	Emonica - the MIDI Harmonica
	Copyright (c) 2018 (john.lassen at gmx.de)
	------------------------------------------------------------------------------------------------------
	This software is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This software is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	------------------------------------------------------------------------------------------------------
*/

#define MaxPreset 8  // Maximum Presets
#define IR0 22		 // Light breaker 1
#define IR1 21		 // Light breaker 2
#define PRESSURE A9	 // Analog Input for the pressure sensor



#define L1 15   // Input pin for the left top button
#define L2 14   // Input pin for the left middle button
#define L3 13	// Input pin for the left bottom button


#define R1 12	// Input pin for the right top button
#define R2 11   // Input pin for the right middle button
#define R3 10	// Input pin for the right bottom button 

//#define R3 12	// Input pin for the right top button			// SCHELLER
//#define R2 11   // Input pin for the right middle button		// SCHELLER
//#define R1 10	// Input pin for the right bottom button		// SCHELLER


#define Pin_PrevScreen L2
#define Pin_NextScreen L1
#define Pin_ValueUp R1
#define Pin_ValueDown R2



#define SCALECOUNTS 10  // Number of Scale-Types (N+1)


#define CoreIntervall 5000   // Process Slider and pressure everey 5ms
// byte ButtonPinNumbers[6] = { L1,L2,L3,R1,R2,R3 };  // SCHELLER
byte ButtonPinNumbers[6] = { L1,L2,L3,R1,R2,R3 };  // JOHN


int CurrentNote;
int CurrentPressureRaw;
int CurrentPressureMidiValue;
int OldNote;
byte CurrentPreset = 0;


boolean RefreshDisplay = false;
boolean JumpToSetup = false;

#include "i2c_t3.h"
#include <Adafruit_GFX.h>
#include "Adafruit_SSD1306.h"  // Don't forget to set the DEFINE to #define SSD1306_128_64 for the display
Adafruit_SSD1306 display(4);
#include "Bounce.h"
#include "AdaptiveButton.h"
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <EEPROM.h>
#include "Preset.h"
Preset Presets[MaxPreset];
#include "ScaleIntervals.h"
#include "Defines.h"
#include <Encoder.h>


boolean flagSaveConfig = false;
int currentScreen = 0;
Encoder Slider(IR1, IR0);

#include "Helpers.h"   // Some helper function (e.g. EEPROM reading/writing)
#include "Logo.h"
#include "CalculateNote.h"
#include "ButtonProcessing.h"
#include "Main.h"
#include "Set_Select_Preset.h"
#include "MoveSliderToCalibrate.h"
#include "Set_Blow_CC_MidiChannel.h"
#include "Set_Blow_CC_Controller.h"
#include "Set_Blow_Pressure_Threshold.h"
#include "Set_Blow_Max_Pressure.h"
#include "Set_Blow_Min_Value.h"
#include "Set_Blow_Max_Value.h"
#include "Set_Key_Midi_Channel.h"
#include "Set_Key_Static_Expression_Value.h"
#include "Set_Key_Low_Note.h"
#include "Set_Key_High_Note.h"
#include "Set_Key_Scale.h"
#include "Set_Key_Adaptive_Expression.h"
#include "Set_Key_Adaptive_Expression_Min_Value.h"
#include "Set_Key_Adaptive_Expression_Max_Value.h"
#include "Set_Buttons_Type.h"
#include "Set_Button_CC_Controller.h"
#include "Set_Button_CC_Max_Value.h"
#include "Set_Button_CC_Min_Value.h"
#include "Set_Button_CC_Midi_Channel.h"
#include "Set_Button_Slide_Speed.h"
#include "Set_Button_Slide_To.h"
#include "Set_Button_Transpose_To.h"
#include "Set_Button_Midi_Note.h"
#include "Set_Button_Midi_Channel.h"
#include "Set_Button_Type_Midi_Expression.h"
#include "Welcome.h"
#include "CoreProcessing.h"

IntervalTimer CoreTimer;


void setup()
{

	Serial.begin(115200);
	delay(500);
	Serial.print("Emonica starting...");
	// Uncomment this part for the first time, to initialze the Presets in the EEPROM 
	//InitDefaultEepromValues();
	
	CurrentPreset = EEPROM.read(0);  // the first Byte in the eeprom contains the current selected preset

	// Read presets
	EEPROM_readAnything(1, Presets);

	pinMode(Pin_ValueUp, INPUT_PULLUP);
	pinMode(Pin_ValueDown, INPUT_PULLUP);
	pinMode(Pin_PrevScreen, INPUT_PULLUP);
	pinMode(Pin_NextScreen, INPUT_PULLUP);

	pinMode(R1, INPUT_PULLUP);
	pinMode(R2, INPUT_PULLUP);
	pinMode(R3, INPUT_PULLUP);
	pinMode(L1, INPUT_PULLUP);
	pinMode(L2, INPUT_PULLUP);
	pinMode(L3, INPUT_PULLUP);

	Serial.print("1");
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	Serial.print("...2");
	display.clearDisplay();
	display.display();
	display.setTextColor(WHITE);
	Serial.print("...3");
	Wire.setClock(400000);


	FillScaleIntervals();
	Serial.print("...4");
 

	Serial.println("Calc Steps:" + String(CalculateScaleSteps()));
	Serial.println("Calc Note: " + String(GetNoteFromScale(48)));
	
	CalculatedScaleSteps = CalculateScaleSteps();
	Calibrate();
	CoreTimer.begin(CoreProcessing, CoreIntervall);
	usbMIDI.begin();

}



void loop()
{

	WelcomeScreen();  // Show the Welcomescreen
	MoveSliderToCalibrate();  // Calibrate the slider (move left/right)
	
	while(1)
	{
		switch (currentScreen)
		{
			case Page_Main: MainScreen(); break;
			case Page_Select_Preset: Set_Select_Preset(); break;
			case Page_Set_Pressure_Threshold: Set_Blow_Pressure_Threshold(); break;
			case Page_Set_Pressure_Max: Set_Blow_Max_Pressure (); break;
			case Page_Set_Blow_CC_Midi_Channel: Set_Blow_CC_MidiChannel(); break;
			case Page_Set_Blow_CC_Controller:	Set_Blow_CC_Controller(); break;
			case Page_Set_Blow_CC_Min_Value: Set_Blow_Min_Value(); break;
			case Page_Set_Blow_CC_Max_Value: Set_Blow_Max_Value(); break;

			case Page_Set_Key_Midi_Channel: Set_Key_Midi_Channel(); break;
			case Page_Set_Key_Low_Note: Set_Key_Low_Note();  break;
			case Page_Set_Key_High_Note: Set_Key_High_Note();  break;
			case Page_Set_Key_Scale: Set_Key_Scale(); break;
			case Page_Set_Key_Adaptive_Expression: Set_Key_Adaptive_Expression(); break;
			case Page_Set_Key_Expression: Set_Key_Static_Expression_Value(); break;
			case Page_Set_Key_Adaptive_Expression_Min: Set_Key_Adaptive_Expression_Min(); break;
			case Page_Set_Key_Adaptive_Expression_Max: Set_Key_Adaptive_Expression_Max(); break;

			case Page_Set_Button_Type: Set_Button_Type(); break;
			case Page_Set_Button_Type_CC_Controller: Set_Button_CC_Controller(); break;
			case Page_Set_Button_Type_CC_Max_Value: Set_Button_CC_Max_Value(); break;
			case Page_Set_Button_Type_CC_Min_Value: Set_Button_CC_Min_Value(); break;
			case Page_Set_Button_Type_CC_Midi_Channel: Set_Button_CC_Midi_Channel(); break;
			case Page_Set_Button_Type_Pitch_Bend_To: Set_Button_Pitch_Bend_To(); break;
			case Page_Set_Button_Type_Pitch_Speed: Set_Button_Pitch_Bend_Speed(); break;
			case Page_Set_Button_Type_Transpose_To: Set_Button_Transpose_To(); break;
			case Page_Set_Button_Type_Midi_Note: Set_Button_Midi_Note(); break;

			case Page_Set_Button_Type_Midi_Channel: Set_Button_Midi_Channel(); break;
			case Page_Set_Button_Type_Midi_Expression:  Set_Button_Type_Midi_Expression(); break;
			default:
			break;
			}
	}

}
 
 