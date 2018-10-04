#pragma once

long SetupCounter = 0;
boolean NoteIsPlaying = false;
int LastNote = 0;

void CoreProcessing()
{

	CurrentNote = GetNoteFromSlider(); // Returns the note from slider-position

	if (JumpToSetup == false)
	{
		if (DoButtonProcessing())  // Returns true, if alle buttons are pressed at the same time
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
	

	// Read the pressure of the blow-sensor
	 CurrentPressureRaw = abs(analogRead(PRESSURE) - CalibationValue);
	 CurrentPressureRaw = constrain(CurrentPressureRaw,0,1024);

	 //CurrentPressureMidiValue = map(CurrentPressureRaw, Presets[CurrentPreset].Blow_Pressure_Threshold , Presets[CurrentPreset].Blow_Max_Pressure, Presets[CurrentPreset].Blow_CC_Min, Presets[CurrentPreset].Blow_CC_Max);


	 if (CurrentPressureRaw > Presets[CurrentPreset].Blow_Pressure_Threshold)
	 {
		 // Okay there is pressure on the sensor, so let's see what we can do.
		 

		 // Does the current note change against to the last note?
		 if (CurrentNote != LastNote) // Yes
		 {
			 
			 // Are we still blowing without stopping?
			 if (NoteIsPlaying)  // YES
			 {
				 //Okay, let's send a note off for the previous note
				 usbMIDI.sendNoteOff(LastNote, 0, 1);
			 }
			 



			 
   			 // Serial.println("Note 1 On " + MidiToNote(CurrentNote));
			 
			 
			 // Are we using adaptive expression?
			 if (Presets[CurrentPreset].Key_AdaptiveExpression)
			 {

				 // Send out note with dynamic expression
				 int DynExValue = map(CurrentPressureRaw, Presets[CurrentPreset].Blow_Pressure_Threshold, Presets[CurrentPreset].Blow_Max_Pressure, Presets[CurrentPreset].Key_AdaptiveExpressionMin, Presets[CurrentPreset].Key_AdaptiveExpressionMax);
				 DynExValue = constrain(DynExValue, Presets[CurrentPreset].Key_AdaptiveExpressionMin, Presets[CurrentPreset].Key_AdaptiveExpressionMax);
				 usbMIDI.sendNoteOn(CurrentNote, DynExValue, Presets[CurrentPreset].Key_MidiChannel);
			 }
			 else
			 {
				 // Send the new note out with static expression
				 usbMIDI.sendNoteOn(CurrentNote, Presets[CurrentPreset].Key_Expression, Presets[CurrentPreset].Key_MidiChannel);
			 }

			 NoteIsPlaying = true;
			 LastNote = CurrentNote;


		 }
		 else
		 {
			 // Okay, the current note hasn't changed and we are playing
			 if (NoteIsPlaying)
			 {
				 // Dann halten wir den Ton
				 //Serial.println("Note Hold " + MidiToNote(CurrentNote));
				 // If we are in AdaptiveExpression mode, then send only the current expression out
				 if (Presets[CurrentPreset].Key_AdaptiveExpression)
				 {
					 int DynExValue = map(CurrentPressureRaw, Presets[CurrentPreset].Blow_Pressure_Threshold, Presets[CurrentPreset].Blow_Max_Pressure, Presets[CurrentPreset].Key_AdaptiveExpressionMin, Presets[CurrentPreset].Key_AdaptiveExpressionMax);
					 DynExValue = constrain(DynExValue, Presets[CurrentPreset].Key_AdaptiveExpressionMin, Presets[CurrentPreset].Key_AdaptiveExpressionMax);
					 //usbMIDI.sendAfterTouch(DynExValue, Presets[CurrentPreset].Key_MidiChannel);
					 usbMIDI.sendControlChange(7, DynExValue, Presets[CurrentPreset].Key_MidiChannel);
				 }
				 
				 
			 }
			 else
			 {
				 
				 // We play the note again
				 // Are we using adaptive expression?
				 if (Presets[CurrentPreset].Key_AdaptiveExpression)
				 {

					 // Send out note with dynamic expression
					 int DynExValue = map(CurrentPressureRaw, Presets[CurrentPreset].Blow_Pressure_Threshold, Presets[CurrentPreset].Blow_Max_Pressure, Presets[CurrentPreset].Key_AdaptiveExpressionMin, Presets[CurrentPreset].Key_AdaptiveExpressionMax);
					 DynExValue = constrain(DynExValue, Presets[CurrentPreset].Key_AdaptiveExpressionMin, Presets[CurrentPreset].Key_AdaptiveExpressionMax);
					 usbMIDI.sendNoteOn(CurrentNote, DynExValue, Presets[CurrentPreset].Key_MidiChannel);
				 }
				 else
				 {
					 // Send the new note out with static expression
					 usbMIDI.sendNoteOn(CurrentNote, Presets[CurrentPreset].Key_Expression, Presets[CurrentPreset].Key_MidiChannel);
				 }
				 
					 
				 NoteIsPlaying = true;
			 }

		 }

		 //  int SendCCValue = map(CurrentPressureRaw, Presets[CurrentPreset].Blow_Pressure_Threshold, Presets[CurrentPreset].Blow_Max_Pressure, Presets[CurrentPreset].Blow_CC_Min, Presets[CurrentPreset].Blow_CC_Max);
		// SendCCValue = constrain(SendCCValue, Presets[CurrentPreset].Blow_CC_Min, Presets[CurrentPreset].Blow_CC_Max);

		 
		 //Serial.print("CC CH:" + (String)Presets[CurrentPreset].Blow_CC_MidiChannel + "  ");
		 //Serial.print("CC Con:" + (String)Presets[CurrentPreset].Blow_CC_Controller + "  ");
		 //Serial.print("CC Raw:" + (String) +CurrentPressureRaw);
		 //Serial.println("CC Val:" + (String)+SendCCValue);
		 
	 }
	 else
	 {
		 // Okay, no more pressure on the sensor (or less than threshold)
		 // Are we still playing?
		 if (NoteIsPlaying)
		 {
			// Let's send a note off.
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
		 Serial.println(SliderCount * 64);
		 SliderCount_Old = SliderCount;
		 usbMIDI.sendPitchBend(SliderCount * 64, Presets[CurrentPreset].Key_MidiChannel);

	 }
	
	 
		 


}
