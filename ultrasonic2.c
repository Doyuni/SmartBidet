#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int ultrasonic2(int range, char* rec_but, char *rec_tou)
{
	int trig = 0;
	int echo = 2;
	int start_time, end_time;
	float distance;

	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);

	while(1) {
		if(strlen(rec_but)==7 || strlen(rec_tou) == 2){ // touch not sensend or button pressed
			printf("Ultrasonic sense button pressed\n");
			 break;
		}
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
		if((int)distance<range) {
			printf("Ultrasonic 2 detected\n");
			return (int)distance;
		}
	}
	return 0;
}
