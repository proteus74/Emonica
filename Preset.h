

enum ButtonType
{
	Off = 0,
	Slide = 1,
	Transpose = 2,
	Controller = 3,
	Note = 4
	
} ButtonTypes;
struct ButtonAssign
{
	byte Type;						// What happens when you push the button (Off, Slide, Transpose, Send Controller Value, Send Note Value)

	// If Type is Controller 
	byte CC_MidiChannel;			// MIDI Channel for the CC
	byte CC_Controller;				// MIDI Controllervalue 
	byte CC_Value_Pressed;			// CC Value when the Button is pressed
	byte CC_Value_Released;			// CC Value when the Button is released
	
	int TransposeTo;				// Transpose note to this steps, when button is pressed
	int SlideTo;					// Slide note to this value, when button is pressed
	int SlideSpeed;					// The speed of the slide

	byte Note_Note;					// Send a note, when a button is pressed
	byte Note_MidiChannel;			// MIDI Channel of the Note
	byte Note_Expression;			// Expression od the Note
	
};

struct Preset
{
	int Blow_Pressure_Threshold;		// The threshold at which level the emonica sends a note
	int Blow_Max_Pressure;				// The maximum level (cutoff) of the blow
	byte Blow_CC_MidiChannel;			// MIDI-Channel for the CC when blowing
	byte Blow_CC_Controller;			// MIDI-Controller number when blowing
	byte Blow_CC_Min;					// MIDI-Value for the lowest blow when hitting the threshold
	byte Blow_CC_Max;					// MIDI-Value for the highest blow when hitting the Blow_Max_Pressure

	byte Key_MidiChannel;				// MIDI Channel for the note
	byte Key_Expression;				// Volume value when sending a note and adaptive expression is disabled
	byte Key_Scale;						// Tone scale
	byte Key_LowNote;					// (Root) Note on the left side of the emonica
	byte Key_HighNote;					// (Last) Note on the right side of the emonica
	byte Key_AdaptiveExpression;		// If enabled the volume of the note is in relation to the blow
	byte Key_AdaptiveExpressionMin;		// Minimum Value for the Volume, when in adaptive mode
	byte Key_AdaptiveExpressionMax;		// Maximum Value for the Volume, when in adaptive mode

	ButtonAssign Buttons[6];			// Button assignments
};

