

enum ButtonType
{
	Off = 0,
	Slide = 1,
	Transpose = 2,
	Controller =3
	
} ButtonTypes;
struct ButtonAssign
{
	byte Type;
	byte CC_MidiChannel;			// Midi-Kanal für CC
	byte CC_Controller;				// Midi-CC
	byte CC_Min;					// CC-Wert Min (Bei Pressure Threshold)
	byte CC_Max;					// CC-Wert Max (Bei Max_Pressure)
	int TransposeTo;
	int SlideTo;
	byte Note_MidiChannel;
	byte Note_Note;
	byte Note_Expression;
	int SlideSpeed;
};

struct Preset
{
	int Breath_Pressure_Threshold;
	int Breath_Max_Pressure;

	byte Breath_CC_MidiChannel;			// Midi-Kanal für CC
	byte Breath_CC_Controller;				// Midi-CC
	byte Breath_CC_Min;					// CC-Wert Min (Bei Pressure Threshold)
	byte Breath_CC_Max;					// CC-Wert Max (Bei Max_Pressure)

	byte Key_MidiChannel;			// Midi-Kanal für Key
	byte Key_Expression;			// Volume bei Note On, wenn nicht Adaptiv
	byte Key_Scale;				// Tonscala
	byte Key_LowNote;				// Unterste Note
	byte Key_HighNote;				// Oberste Note
	byte Key_AdaptiveExpression;		// Wenn An, so wird die Lautstärke abhängig vom Luftdruck gemacht
	byte Key_AdaptiveExpressionMin;	// Lautstärke Min, beim Luftdruck
	byte Key_AdaptiveExpressionMax;	// Lautstärke Max, beim Luftdruck
	ButtonAssign Buttons[6];	// Tasten-Zuweisung
};

