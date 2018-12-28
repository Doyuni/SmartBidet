#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

int ultrasonic3(int range)
{
	int trig = 23;
	int echo = 24;
	int start_time, end_time;
	float distance;
	int sum =0, average =0;

	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);
	while(1) {


		digitalWrite(trig, LOW);
		delay(200);
		digitalWrite(trig, HIGH);
		delayMicroseconds(10);
		digitalWrite(trig, LOW);
		while(digitalRead(echo) == 0);
		start_time = micros();
		while(digitalRead(echo) == 1);
		end_time = micros();
		distance = (end_time - start_time) / 29. / 2. ;

		printf("distance ultrasonic3 : %lf\n", distance);

		if( (int)distance>range ) {
			printf("Person go out\n");
			return 0;
		}
	}	
	return 0;
}
