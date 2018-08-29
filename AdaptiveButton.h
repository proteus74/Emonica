#pragma once
#ifndef ADAPTIVEBUTTON_H
#define ADAPTIVEBUTTON_H
#include "Bounce.h"


class AdaptiveButton
{


private :
	long LastMillis = 0;
	int waitbetween = 0;
	Bounce mybutton;
	
	int  *var;
	int _min;
	int _max;
	int _step;
public:
	
	AdaptiveButton() {};
	AdaptiveButton(int Pin, int  *variable, int step, int min, int max)
	{
		var = variable;
		mybutton = Bounce(Pin, 25);
		
		//long LastMillis = 0;
		//int waitbetween = 0;
		  _step = step;
		  _min = min;
		  _max = max;
	};
	boolean Update()
	{
		mybutton.update();
		if (mybutton.fallingEdge())
		{
			waitbetween = 500;
			LastMillis = millis();
			int old = (*var);
			(*var) += _step;
			(*var) = constrain(*var, _min, _max);
			if ((*var) != old)
			{
				return true;
			}
			return false;
		}
		else if (mybutton.read() == false)  // Immer noch gedrückt
		{
			if (abs(millis() - LastMillis) > waitbetween)
			{

				LastMillis = millis();
				if (mybutton.duration() > 1000)
				{
					int dur = constrain(mybutton.duration(), 1000, 6000);
					waitbetween = constrain(500 - (dur / 10), 10, 500);
				}

				int old = (*var);
				
				if (waitbetween < 150)
				{
					(*var) += (_step  * 10 + random(0,5));
				}
				else
				{
					(*var) += _step;
				}

				(*var) = constrain(*var, _min, _max);
				if ((*var) != old)
				{
					return true;
				}
				return false;

			}
			return false;
		}
		return false;
	}
	

};
#endif