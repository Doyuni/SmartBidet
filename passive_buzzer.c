#include <wiringPi.h>
#include <softTone.h>
#include <stdio.h>

#define BeepPin 28

#define a 440
#define b 466

int open_sound[8] ={1047, 1175, 1319, 1047, 1175, 1319, 1047, 1175};
int close_sound[8] = {1319, 1175, 1047, 1319, 1175, 1047, 1319, 1175};
int wind_sound[5] = {3136, 3520, 3136, 3520, 3952};

int buzzer(int mode)
{
	if(wiringPiSetup() == -1)
	{
		printf("failed!");
		return 0;
	}

	softToneCreate(BeepPin);


switch(mode)
{
case 1:
	{
	int i;
	for(i = 0; i < 8; ++i)
		{
		printf("open sound : %3d\n", i);
		softToneWrite(BeepPin, open_sound[i]);
		delay(150);
		digitalWrite(BeepPin, 0);
		}
	}

	case 2:
	{
		int j;
		for(j = 0; j < 8; ++j)
		{
		printf("close sound : %3d\n", j);
		softToneWrite(BeepPin, close_sound[j]);
		delay(150);
		digitalWrite(BeepPin, 0);
		}


	}

	case 3:
	{
		int k;
		for(k = 0; k < 5; ++k)
		{
		printf("wind sound : %3d\n", k);
		softToneWrite(BeepPin, wind_sound[k]);
		delay(150);
		digitalWrite(BeepPin, 0);
		}
	}
}
	return 0;
}
