#pragma once



boolean LastButtonStats[6];
boolean isSliding[6];
boolean DoSlide[6];
boolean DoSlide_Return[6];

boolean DoButtonProcessing()
{

	if (!digitalRead(L1) && !digitalRead(L2) && !digitalRead(L3) && !digitalRead(R1) && !digitalRead(R2) && !digitalRead(R3))
	{
		return true;
	}

	for (byte t = 0; t < 6; t++)
	{
		if (Presets[CurrentPreset].Buttons[t].Type == 1)  // Slide
		{

			boolean ButtonState = !digitalRead(ButtonPinNumbers[t]);
			if (LastButtonStats[t] != ButtonState)
			{
				if (ButtonState)
				{
					DoSlide[t] = true;
				}
				else
				{
					DoSlide[t] = false;

				}
				LastButtonStats[t] = ButtonState;
			}

 

		}
		else if (Presets[CurrentPreset].Buttons[t].Type == 2)  // Transpose
		{
			if (!digitalRead(ButtonPinNumbers[t]))
			{
				CurrentNote += Presets[CurrentPreset].Buttons[t].TransposeTo;
				CurrentNote = constrain(CurrentNote, 0, 127);
			}
			
		}
		else if (Presets[CurrentPreset].Buttons[t].Type == 3)  // CC
		{
			boolean ButtonState = !digitalRead(ButtonPinNumbers[t]);
			if (LastButtonStats[t] != ButtonState)
			{
				if (ButtonState)
				{
					// Sende CC Wert HIGH
					Serial.print("CC CH:" + (String)Presets[CurrentPreset].Buttons[t].CC_MidiChannel + "  ");
					Serial.print("CC Con:" + (String)Presets[CurrentPreset].Buttons[t].CC_Controller + "  ");
					Serial.println("CC Val:" + (String)+Presets[CurrentPreset].Buttons[t].CC_Max);
					usbMIDI.sendControlChange(Presets[CurrentPreset].Buttons[t].CC_Controller, Presets[CurrentPreset].Buttons[t].CC_Max, Presets[CurrentPreset].Buttons[t].CC_MidiChannel);
				}
				else
				{
					// Sende CC Wert Low
					Serial.print("CC CH:" + (String)Presets[CurrentPreset].Buttons[t].CC_MidiChannel + "  ");
					Serial.print("CC Con:" + (String)Presets[CurrentPreset].Buttons[t].CC_Controller + "  ");
					Serial.println("CC Val:" + (String)+Presets[CurrentPreset].Buttons[t].CC_Min);
					usbMIDI.sendControlChange(Presets[CurrentPreset].Buttons[t].CC_Controller, Presets[CurrentPreset].Buttons[t].CC_Min, Presets[CurrentPreset].Buttons[t].CC_MidiChannel);
				}
				LastButtonStats[t] = ButtonState;
			}
		}
		else if (Presets[CurrentPreset].Buttons[t].Type == 4)  // Midi Note
		{
			boolean ButtonState = !digitalRead(ButtonPinNumbers[t]);
			if (LastButtonStats[t] != ButtonState)
			{
				if (ButtonState)
				{
					// Sende CC Wert HIGH
					Serial.print("Send Midi On Button");
					Serial.print("CH:" + (String)Presets[CurrentPreset].Buttons[t].Note_MidiChannel + "  ");
					Serial.print("Exp:" + (String)Presets[CurrentPreset].Buttons[t].Note_Expression + "  ");
					Serial.println("Val:" + (String)+Presets[CurrentPreset].Buttons[t].Note_Note);
					usbMIDI.sendNoteOn(Presets[CurrentPreset].Buttons[t].Note_Note, Presets[CurrentPreset].Buttons[t].Note_Expression, Presets[CurrentPreset].Buttons[t].Note_MidiChannel);
				}
				else
				{
					Serial.print("Send Midi Off Button");
					Serial.print("CH:" + (String)Presets[CurrentPreset].Buttons[t].Note_MidiChannel + "  ");
					Serial.print("Exp: 0");
					Serial.println("Val:" + (String)+Presets[CurrentPreset].Buttons[t].Note_Note);
					usbMIDI.sendNoteOff(Presets[CurrentPreset].Buttons[t].Note_Note, 0, Presets[CurrentPreset].Buttons[t].Note_MidiChannel);
				}
				LastButtonStats[t] = ButtonState;
			}

		}

	}
	return false;
}
