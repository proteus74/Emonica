#pragma once



void WelcomeScreen()
{

	//while (1)
	{
		for (byte t = 0; t<128; t++)
		{
			t++;
			t++;
			t++;
			display.clearDisplay();
			display.drawBitmap(5, 17, Logo_Ohne_Punkt, 123, 29, 1);
			display.fillRect(4 + t, 17, 130, 30, 0);
			display.display();
			//delay(10);
		}
		delay(1000);
		for (byte t = 0; t<10; t++)
		{

			display.clearDisplay();
			display.drawBitmap(5, 17, Logo, 123, 29, 1);
			display.fillRect(4, 0, 130, 25 - t, 0);
			display.display();
			delay(10);
		}
		delay(200);
		display.setFont(NULL);
		printCentered("Version 1.0", 52);
		display.display();

		delay(1000);
		for (byte t = 0; t<128; t++)
		{
			t += 7;
			display.clearDisplay();
			display.drawBitmap(5 + t, 17, Logo, 123, 29, 1);
			display.display();
		}
		delay(1000);

	}


}