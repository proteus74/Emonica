#pragma once


int maxSlider = 0;
int minSlider = 0;
int GetNoteFromSlider()
{
	int SliderRaw = Slider.read();
//	if (SliderRaw <= minSlider) 	minSlider = SliderRaw;
//	if (SliderRaw >= maxSlider) 	maxSlider = SliderRaw;
	int SliderValue = map(SliderRaw, minSlider, maxSlider, 0, 127);

	int StepValue = map(SliderValue, 0, 127, 0, CalculatedScaleSteps);

	StepValue = constrain(StepValue, 0, CalculatedScaleSteps -1);
	if (StepValue < 0) { StepValue = 0; }

	int Octave = StepValue / ScaleIntervals[Presets[CurrentPreset].Key_Scale].IntervalLength;
	int Rest = StepValue % ScaleIntervals[Presets[CurrentPreset].Key_Scale].IntervalLength;
	int Note = Presets[CurrentPreset].Key_LowNote + (Octave * 12) +  ScaleIntervals[Presets[CurrentPreset].Key_Scale].Interval[Rest];

 
	return Note;
}