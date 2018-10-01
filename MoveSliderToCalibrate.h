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
 
	// Show the arrow animation
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
