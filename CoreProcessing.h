#pragma once


long SetupCounter = 0;
boolean NoteIsPlaying = false;
int LastNote = 0;
void CoreProcessing()
{

	CurrentNote = GetNoteFromSlider(); // Returns the note from slider-position

	if (JumpToSetup == false)
	{
		if (DoButtonProcessing())
		{
			SetupCounter += CoreIntervall;
			if (SetupCounter > 500000)
			{
				JumpToSetup = true;
				Serial.print("Enter Setup");
			}
		}
		else
		{
			SetupCounter = 0;
		}
	}

	if (OldNote != CurrentNote)
	{
	//	Serial.println(MidiToNote(CurrentNote));
		RefreshDisplay = true;
		OldNote = CurrentNote;
	}
	

	// Pressure Handling
	 CurrentPressureRaw = abs(analogRead(PRESSURE) - CalibationValue);
	 CurrentPressureRaw = constrain(CurrentPressureRaw,0,1024);

	 //CurrentPressureMidiValue = map(CurrentPressureRaw, Presets[CurrentPreset].Breath_Pressure_Threshold , Presets[CurrentPreset].Breath_Max_Pressure, Presets[CurrentPreset].Breath_CC_Min, Presets[CurrentPreset].Breath_CC_Max);


	 if (CurrentPressureRaw > Presets[CurrentPreset].Breath_Pressure_Threshold)
	 {



		 // Es gibt Druck, also Spielen

		 // Erst mal schauen, ob sie die Note geändert hat
		 if (CurrentNote != LastNote)
		 {
			 // Ja hat sie.

			 // Sind wir noch am Spielen/Halten?	 
			 if (NoteIsPlaying)  // 
			 {
				 // Dann ein NoteOff für die Alte Note Senden
				// Serial.println("Note Off (Old) " + MidiToNote(LastNote));

				 usbMIDI.sendNoteOff(LastNote, 0, 1);


			 }
			 NoteIsPlaying = true;
			 // Und neue Note Raussenden
			// Serial.println("Note 1 On " + MidiToNote(CurrentNote));
			 usbMIDI.sendNoteOn(CurrentNote, Presets[CurrentPreset].Key_Expression, Presets[CurrentPreset].Key_MidiChannel);
			 
			 LastNote = CurrentNote;
		 }
		 else
		 {
			 // Die Note ist immernoch gleich
			 // Sind wir schon am Spielen?
			 if (NoteIsPlaying)
			 {
				 // Dann halten wir den Ton
				 //Serial.println("Note Hold " + MidiToNote(CurrentNote));

				 if (Presets[CurrentPreset].Key_AdaptiveExpression)
				 {

					 
					 int DynExValue = map(CurrentPressureRaw, Presets[CurrentPreset].Breath_Pressure_Threshold, Presets[CurrentPreset].Breath_Max_Pressure, Presets[CurrentPreset].Key_AdaptiveExpressionMin, Presets[CurrentPreset].Key_AdaptiveExpressionMax);
					 DynExValue = constrain(DynExValue, Presets[CurrentPreset].Key_AdaptiveExpressionMin, Presets[CurrentPreset].Key_AdaptiveExpressionMax);
					 //usbMIDI.sendAfterTouch(DynExValue, Presets[CurrentPreset].Key_MidiChannel);
					 usbMIDI.sendControlChange(7, DynExValue, Presets[CurrentPreset].Key_MidiChannel);
					// Serial.println("Dynamic Expression " +  (String)DynExValue);
					 // Hier dann die dynamic Expression reinbauen
				 }
				 
				 
			 }
			 else
			 {
				 // Wir Starten eine neue Note
				// Serial.println("Note 1 On " + MidiToNote(CurrentNote));
				 usbMIDI.sendNoteOn(CurrentNote, Presets[CurrentPreset].Key_Expression, Presets[CurrentPreset].Key_MidiChannel);
				 NoteIsPlaying = true;
			 }

		 }

		 int SendCCValue = map(CurrentPressureRaw, Presets[CurrentPreset].Breath_Pressure_Threshold, Presets[CurrentPreset].Breath_Max_Pressure, Presets[CurrentPreset].Breath_CC_Min, Presets[CurrentPreset].Breath_CC_Max);
		 SendCCValue = constrain(SendCCValue, Presets[CurrentPreset].Breath_CC_Min, Presets[CurrentPreset].Breath_CC_Max);

		 //// Jetzt noch den CC - Wert senden
		 //Serial.print("CC CH:" + (String)Presets[CurrentPreset].Breath_CC_MidiChannel + "  ");
		 //Serial.print("CC Con:" + (String)Presets[CurrentPreset].Breath_CC_Controller + "  ");
		 //Serial.print("CC Raw:" + (String) +CurrentPressureRaw);
		 //Serial.println("CC Val:" + (String)+SendCCValue);
		 
	 }
	 else
	 {
		 // Es ist kein Luftdruck mehr da
		 // Gibt es noch eine Note die Spielt ?
		 if (NoteIsPlaying)
		 {
			// Serial.println("Note Off (Old) " + MidiToNote(LastNote));
			 usbMIDI.sendNoteOff(LastNote, 0, Presets[CurrentPreset].Key_MidiChannel);
			 NoteIsPlaying = false;
		 }
	 }
	 //Serial.println(micros() - lastMillsX);

	 static unsigned long LastSliderStep[6];
	 //static int SlideCounter[6];
	 static int SliderCount =0;
	 static int SliderCount_Old;
	 // Hier das Sliding

	 
	 boolean IsSliding = false;
	 for (byte t = 0; t < 6; t++)
	 {
		 if (DoSlide[t])
		 {
			 IsSliding = true;
			 if (Presets[CurrentPreset].Buttons[t].SlideTo > SliderCount)
			 {
				 if (abs(millis() - LastSliderStep[t]) > 127 - Presets[CurrentPreset].Buttons[t].SlideSpeed)
				 {
					 SliderCount += 5;
					 if (SliderCount > 127)
					 {
						 SliderCount = 127;
					 }
					 /*Serial.println(SlideCounter[t]);*/
					 LastSliderStep[t] = millis();
				 }
			 }
			 if (Presets[CurrentPreset].Buttons[t].SlideTo < SliderCount)
			 {
				 if (abs(millis() - LastSliderStep[t]) > 127 - Presets[CurrentPreset].Buttons[t].SlideSpeed)
				 {

					 SliderCount  -= 5;
					 if (SliderCount < -127)
					 {
						 SliderCount = -127;
					 }

					 LastSliderStep[t] = millis();
				 }


			 }
		 }
	 
	 }
	 
	 if (IsSliding == false)
	 {
		 if (SliderCount > 0)
		 {
			 SliderCount-=5;
			 {
				 if (SliderCount < 0)
				 {
					 SliderCount = 0;
				 }
			 }

		 }
		 else if (SliderCount < 0)
		 {
			 SliderCount += 5;
			 {
				 if (SliderCount > 0)
				 {
					 SliderCount = 0;
				 }
			 }
		 }

	 }

	 if (SliderCount_Old != SliderCount)
	 {
		 //Serial.println(8192 +  SliderCount * 64);
		 SliderCount_Old = SliderCount;
		 usbMIDI.sendPitchBend(8192 + SliderCount * 64, Presets[CurrentPreset].Key_MidiChannel);

	 }
	
	 
		 


}
