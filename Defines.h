#pragma once


#define Page_Main 0
#define Page_Select_Preset 1
#define Page_Set_Pressure_Threshold 2
#define Page_Set_Pressure_Max 3
#define Page_Set_Blow_CC_Midi_Channel 4
#define Page_Set_Blow_CC_Controller 5
#define Page_Set_Blow_CC_Min_Value 6
#define Page_Set_Blow_CC_Max_Value 7
#define Page_Set_Key_Midi_Channel 8
#define Page_Set_Key_Low_Note 9
#define Page_Set_Key_High_Note 10
#define Page_Set_Key_Scale 11
#define Page_Set_Key_Adaptive_Expression 12
#define Page_Set_Key_Expression 13
#define Page_Set_Key_Adaptive_Expression_Min 14
#define Page_Set_Key_Adaptive_Expression_Max 15
#define Page_Set_Button_Type 16
#define Page_Set_Button_Type_Pitch_Bend_To 17
#define Page_Set_Button_Type_Pitch_Speed 18
#define Page_Set_Button_Type_Transpose_To 19
#define Page_Set_Button_Type_CC_Midi_Channel 20
#define Page_Set_Button_Type_CC_Controller 21
#define Page_Set_Button_Type_CC_Min_Value 22
#define Page_Set_Button_Type_CC_Max_Value 23
#define Page_Set_Button_Type_Midi_Note 24
#define Page_Set_Button_Type_Midi_Channel 25
#define Page_Set_Button_Type_Midi_Expression 26

#define PAUSE_BETWEEN_SCREENS 150
#define TIME_TO_RETURN_TO_MAIN_PAGE 56000

String NoteNames[12] = { "C","C#","D","D#","E","F","F#","G","G#","A","A#","B" };
int CalibationValue = 0;


long LastTimer = 0;

byte ButtonToConfigure = 0;
int CalculatedScaleSteps = 0;

