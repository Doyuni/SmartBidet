#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>

int servo(int range, int servoPin) {
	char sel;

	softPwmCreate(servoPin, 0, 200);

	int i = 40;
	if(range==360) i=50;

	while(i){
		if(range == 90) {
			if(servoPin==1){
				softPwmWrite(servoPin, 5);
			}
			else softPwmWrite(servoPin, 16);
			
		}

		else if (range == 0) {
			if(servoPin == 22) softPwmWrite(servoPin, 24); // spoon goes converted
			else if(servoPin == 1){
				softPwmWrite(servoPin, 24);
			}
			else softPwmWrite(servoPin, 5);
		}
		else if(range == 360){
			softPwmWrite(servoPin, 24);
			if(i==1) {
				softPwmWrite(servoPin, 0);
			}
		}
		delay(19);
		i--;
	}
	return 0;
}
