#define Pin_ValueUp 12
#define Pin_ValueDown 11
#define Pin_PrevScreen 14
#define Pin_NextScreen 15
#define MaxPreset 8
#define IR0 22
#define IR1 21
#define PRESSURE A9

#define R3 10
#define R2 11
#define R1 12
#define L3 13
#define L2 14
#define L1 15

#define CoreIntervall 5000   // Process Slider and pressure everey 5ms
byte ButtonPinNumbers[6] = { 15,14,13,12,11,10 };
int CurrentNote;
int CurrentPressureRaw;
int CurrentPressureMidiValue;
int OldNote;

boolean RefreshDisplay = false;
boolean JumpToSetup = false;

#include "i2c_t3.h"
#include <Adafruit_GFX.h>
#include "Adafruit_SSD1306.h"
Adafruit_SSD1306 display(4);
#include "Bounce.h"
#include "AdaptiveButton.h"
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <EEPROM.h>

byte CurrentPreset = 0;

#include "Preset.h"
Preset Presets[MaxPreset];
#include "ScaleIntervals.h"
#include "Defines.h"

#include <Encoder.h>


boolean flagSaveConfig = false;
int currentScreen = 0;
Encoder Slider(IR1, IR0);

#include "Helpers.h"
#include "Logo.h"
#include "CalculateNote.h"
#include "ButtonProcessing.h"
#include "Main.h"
#include "MoveSliderToCalibrate.h"
#include "Set_Breath_CC_MidiChannel.h"
#include "Set_Breath_CC_Controller.h"
#include "Set_Breath_Pressure_Threshold.h"
#include "Set_Breath_Max_Pressure.h"
#include "Set_Select_Preset.h"
#include "Set_Breath_Min_Value.h"
#include "Set_Breath_Max_Value.h"
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
	CurrentPreset = EEPROM.read(0);  // the first Byte in the eeprom contains the current selected preset
	

	//
	// Comment this out for initial preset initialisation.
	//


	/*
	
	for (byte t = 0; t < 8;t++)
	{
		Presets[t].Breath_CC_Controller = 1;
		Presets[t].Breath_CC_Max = 127;
		Presets[t].Breath_CC_MidiChannel = 1;
		Presets[t].Breath_CC_Min = 0;
		Presets[t].Breath_Max_Pressure = 255;
		Presets[t].Breath_Pressure_Threshold = 10;
		Presets[t].Buttons[0].Type = 0;
		Presets[t].Key_AdaptiveExpression = false;
		Presets[t].Key_AdaptiveExpressionMax = 127;
		Presets[t].Key_AdaptiveExpressionMin = 0;
		Presets[t].Key_Expression = 127;
		Presets[t].Key_HighNote = 48;
		Presets[t].Key_LowNote = 40;
		Presets[t].Key_MidiChannel = 1;
		Presets[t].Key_Scale = 0;
	}
	EEPROM_writeAnything(1, Presets);
	*/
	
	
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


	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display.clearDisplay();
	display.display();
	display.setTextColor(WHITE);
	Wire.setClock(400000);
	Serial.begin(115200);
	FillScaleIntervals();

 

	//Serial.println("Calc Steps:" + String(CalculateScaleSteps()));
	//Serial.println("Calc Note: " + String(GetNoteFromScale(48)));
	
	CalculatedScaleSteps = CalculateScaleSteps();
	Calibrate();
	CoreTimer.begin(CoreProcessing, CoreIntervall);
	usbMIDI.begin();

}



void loop()
{

	WelcomeScreen();
	MoveSliderToCalibrate();
	
	while(1)
	{
		switch (currentScreen)
		{
			case Page_Main: MainScreen(); break;
			case Page_Select_Preset: Set_Select_Preset(); break;
			case Page_Set_Pressure_Threshold: Set_Breath_Pressure_Threshold(); break;
			case Page_Set_Pressure_Max: Set_Breath_Max_Pressure (); break;
			case Page_Set_Breath_CC_Midi_Channel: Set_Breath_CC_MidiChannel(); break;
			case Page_Set_Breath_CC_Controller:	Set_Breath_CC_Controller(); break;
			case Page_Set_Breath_CC_Min_Value: Set_Breath_Min_Value(); break;
			case Page_Set_Breath_CC_Max_Value: Set_Breath_Max_Value(); break;

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
			case Page_Set_Button_Type_Slide_To: Set_Button_Slide_To(); break;
			case Page_Set_Button_Type_Slide_Speed: Set_Button_Slide_Speed(); break;
			case Page_Set_Button_Type_Transpose_To: Set_Button_Transpose_To(); break;
			case Page_Set_Button_Type_Midi_Note: Set_Button_Midi_Note(); break;

			case Page_Set_Button_Type_Midi_Channel: Set_Button_Midi_Channel(); break;
			case Page_Set_Button_Type_Midi_Expression:  Set_Button_Type_Midi_Expression(); break;
			default:
			break;
			}
	}

}
 
 