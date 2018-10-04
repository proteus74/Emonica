#pragma once


template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
	const byte* p = (const byte*)(const void*)&value;
	unsigned int i;
	for (i = 0; i < sizeof(value); i++)
		EEPROM.write(ee++, *p++);
	return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
	byte* p = (byte*)(void*)&value;
	unsigned int i;
	for (i = 0; i < sizeof(value); i++)
		*p++ = EEPROM.read(ee++);
	return i;
}
void printCentered(String text, int yPos)
{

	int16_t  x1, y1;
	uint16_t w, h;
	char Buffer[50];
	text.toCharArray(Buffer, 50);
	display.getTextBounds(Buffer, 0, 0, &x1, &y1, &w, &h);
	display.setCursor(64 - (w / 2), yPos);
	display.print(text);
}


void DrawProgressbar(int value, int min, int max)
{
	double factor = ((double)125 / (double)((double)max - (double)min))  * (double)value;
	display.drawRect(0, 60, 127, 4, 1);
	display.fillRect(1, 61, 125, 2, 0);
	display.fillRect(1, 61, factor, 2, 1);
}
void DisplayPresetNumber()
{

	//display.fillRect(115, 0, 13, 13, 1);
	display.fillCircle(120, 7, 7, 1);
	display.setFont(NULL);
	display.setCursor(118, 4);
	display.setTextColor(0);
	display.print(CurrentPreset + 1);
	display.setTextColor(1);
	//display.drawLine(112, 0, 112, 15,1);



}
void DisplayButtonNumber()
{

	display.setFont(&FreeSansBold9pt7b);
	display.setCursor(17, 13);
	display.setTextColor(1);
	display.print(ButtonToConfigure + 1);



}
String MidiToNote(byte value)
{
	byte Octave = value / 12;
	byte Note = value % 12;
	return NoteNames[Note] + String(Octave);
}

void Calibrate()
{
	long  temp = 0;
	for (int t = 0; t < 1000; t++)
	{
		temp += analogRead(PRESSURE);
	}
	CalibationValue = temp / 1000;
}

void InitDefaultEepromValues()
{
	
	for (byte t = 0; t < 8;t++)
	{
		Presets[t].Blow_CC_Controller = 1;
		Presets[t].Blow_CC_Max = 127;
		Presets[t].Blow_CC_MidiChannel = 1;
		Presets[t].Blow_CC_Min = 0;
		Presets[t].Blow_Max_Pressure = 255;
		Presets[t].Blow_Pressure_Threshold = 10;
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
	
}