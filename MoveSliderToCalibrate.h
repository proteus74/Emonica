#pragma once

void MoveSliderToCalibrate()
{


	display.setFont(&FreeSans9pt7b);
	
	printCentered("Move slider",13);
	printCentered("to both ends", 32);
	boolean exit = false;
		
	boolean a;
	boolean b;
	int counter = 0;

	while (1==2)
	{

		Serial.print(digitalRead(10));
		Serial.print(digitalRead(11));
		Serial.print(digitalRead(14));
		Serial.println(digitalRead(15));
		delay(20);
	}
	while (1==2)
	{


		//delay(10);
		boolean x =! digitalRead(IR0);
		boolean y =! digitalRead(IR1);
		Serial.println(Slider.read());
	//		Serial.print("  ");

			if (y!=b)
			{

				counter++;
			}

			if (x != a || y != b)
			{
				a = x;
				b = y;
				Serial.print(Slider.read());
					Serial.print("  ");
					Serial.print(counter);
					Serial.print("  ");
				Serial.print(a);
				Serial.println(b);
			}
	}

	while (exit == false)
	{
		for (int t = 0; t < 102; t+=4)
		{
			
			
			int SliderRaw = Slider.read();
			if (SliderRaw <= minSlider) 	minSlider = SliderRaw;
			if (SliderRaw >= maxSlider) 	maxSlider = SliderRaw;
			if (maxSlider - minSlider > 55)
			{
				exit = true;
				break;
			}

			display.fillRect(0, 42, 128, 21, 0);
			display.drawBitmap(t, 42, Arrow_Right, 26, 20,1);
			display.display();
		}
		for (int t = 102; t > 0 ; t -= 4)
		{
			display.fillRect(0, 42, 128, 21, 0);
			display.drawBitmap(t, 42, Arrow_Left, 26, 20, 1);
			display.display();
			int SliderRaw = Slider.read();
			if (SliderRaw <= minSlider) 	minSlider = SliderRaw;
			if (SliderRaw >= maxSlider) 	maxSlider = SliderRaw;
			if (maxSlider - minSlider > 55)
			{
				exit = true;
				break;
			}
		}

	}
}
