#pragma once
#define SCALECOUNTS 5
struct ScaleDefinition
{

	byte IntervalLength;
	byte Interval[12];
	String Name;
	String Name2;

};
ScaleDefinition ScaleIntervals[SCALECOUNTS];


void CreateInterval(ScaleDefinition * Scale, byte *  myArray)
{
	for (byte t = 0; t < 12; t++)
	{
		Scale->Interval[t] = myArray[t];
	}
}


void FillScaleIntervals()
{
	ScaleIntervals[0].Name = "Chromatic";
	ScaleIntervals[0].IntervalLength = 12;
	CreateInterval(&ScaleIntervals[0], new byte[12] {0,1,2,3,4,5,6,7,8,9,10,11});

	ScaleIntervals[1].Name = "Major";
	ScaleIntervals[1].Name2 = "";
	ScaleIntervals[1].IntervalLength = 7;
	CreateInterval(&ScaleIntervals[1], new byte[12]{ 0,2,4,5,7,9,11 });

	ScaleIntervals[2].Name = "Minor";
	ScaleIntervals[2].Name2 = "";
	ScaleIntervals[2].IntervalLength = 7;
	CreateInterval(&ScaleIntervals[2], new byte[12]{ 0,2,3,5,7,8,10  });


	ScaleIntervals[3].Name = "Major";
	ScaleIntervals[3].Name2 = "Pentatonic";
	ScaleIntervals[3].IntervalLength = 5;
	CreateInterval(&ScaleIntervals[3], new byte[12]{ 0,2,5,7,9});

	ScaleIntervals[4].Name = "Minor";
	ScaleIntervals[4].Name2 = "Pentatonic";
	ScaleIntervals[4].IntervalLength = 5;
	CreateInterval(&ScaleIntervals[4], new byte[12]{ 0,3,5,7,10 });

}

int CalculateScaleSteps()
{


	int from = Presets[CurrentPreset].Key_LowNote;
	int to = Presets[CurrentPreset].Key_HighNote;
	byte ScaleNumber = Presets[CurrentPreset].Key_Scale;

	int intCounter = 0;
	boolean exit = false;
	int next = 0;
	int octavecounter = 0;
	int realSteps = 0;
	while (exit == false)
	{

		next = from + (ScaleIntervals[ScaleNumber].Interval[intCounter] + (octavecounter * 12));

		if (next > to)
		{
			exit = true;
		}
		else
		{
			Serial.println(next);
			realSteps++;
		}
		intCounter++;

		if (intCounter == ScaleIntervals[ScaleNumber].IntervalLength)
		{
			intCounter = 0;
			octavecounter++;
		}
	}
	Serial.println("Anzahl Schritte " + (String)(realSteps ));
	return realSteps ;
	

}
 

int GetNoteFromScale(int value)
{

	int from = Presets[CurrentPreset].Key_LowNote;
	byte ScaleNumber = Presets[CurrentPreset].Key_Scale;


	int oct = value / ScaleIntervals[ScaleNumber].IntervalLength;
	int rest = value % ScaleIntervals[ScaleNumber].IntervalLength;
	return from + (oct * 12) + ScaleIntervals[ScaleNumber].Interval[rest];
}