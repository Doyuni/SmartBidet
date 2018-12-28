#include <stdio.h>
#include "ultrasonic.h"
#include "ultrasonic2.h"
#include "passive_buzzer.h"
#include "servo.h"
#include <unistd.h> 
#include <pthread.h> 
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "ultrasonic3.h"
#define TOUCH_PATH "/dev/touch_dev"
#define BUTTON_PATH "/dev/button_dev"
#define BUFFER_LENGTH 256
static char receive_button[BUFFER_LENGTH]; // for checking whether button is preessed
static char receive_touch[BUFFER_LENGTH]; // for checking whether touch is sensed

static int touch_sensed = 0;
static int button_sensed = 0;

void* touch(void *data) { // touch_app
	int ret, fd;

	fd = open(TOUCH_PATH, O_RDWR);
	if (fd < 0) {
		perror("Failed to open the device....");
		return errno;
	}

	while (1) {
		ret = read(fd, receive_touch, strlen(receive_touch) + 1);

		if (ret < 0) {
			perror("Failed to read the message from the device.");
			printf("error\n");
			return errno;
		}
	}
}
void* button(void *data) { // button_app
	int ret, fd;

	fd = open(BUTTON_PATH, O_RDWR);
	if (fd < 0) {
		perror("Failed to open the device....");
		return errno;
	}

	while (1) {
		ret = read(fd, receive_button, strlen(receive_button) + 1);

		if (ret < 0) {
			perror("Failed to read the message from the device.");
			printf("error\n");
			return errno;
		}
	}
}

int main(void) {
	int sec, targetdist;
	int i;
	int count; // poop counter
	int thr_id;
	pthread_t p_thread[2];
	int a = 1;
	int b = 2;

	thr_id = pthread_create(&p_thread[0], NULL, (void*)button, (void *)&a); // button thread start
	thr_id = pthread_create(&p_thread[1], NULL, (void*)touch, (void *)&b); // touch thread start

	if (wiringPiSetup() == -1) // wiringPi setup for all sensors with wiringPi
		return 1;
	while (1) {
		count = 0; 
		servo(0, 1); // initialize lid motor (0 degree)
		servo(0, 22); // initialize spoon motor (0 degree)

		do {
			i = ultrasonic(50); // ultrasonic1 is sensing until person comes to the bidet.
		} while (i == 0);
		printf("Person comes to the bidet\n");

		servo(90, 1); // when person comes to the bidet, lid motor opens.
		printf("!!!!! open !!!!\n");

		buzzer(0); // when lid opens, buzzer sounds
		printf("!!!!! sound !!!!\n");
		int delay_touch = 0; // 5 chances for sensing touch
		while (delay_touch != 5) {
			while (strlen(receive_touch) == 3) { // state: touch sensed. ( person sit )
				if (strlen(receive_button) == 7) { // when click the button, motors operate
					//Washing
					servo(90, 22); // spoon motor moves 90 degree
					servo(0, 22); //spoon motor moves 0 degree (Initialization)
					//Drying
					servo(360, 7); // dry motor rotates 360 degree 
					printf("button preessed\n");
					buzzer(3); 
					count = 0;
					continue; 
				}
				// ultrasonic2 operates
				i = ultrasonic2(15, &receive_button, &receive_touch); // long distance 30 => 0 
				printf("ULTRASONIC2 : %d\n", i);
				if (i > 0) { //ultrasonic2 senses poop
					printf("ultrasonic2 value %d\n", i);
					printf("!!!!!! drop %d !!!!!!!\n", count);
					printf(" receive button val = %d\n", strlen(receive_button));
					count++;
				}

				if (count == 3) {
					//Washing
					servo(90, 22);
					servo(0, 22);
					//Drying
					servo(360, 7);
					buzzer(3);
					printf("!!!!! count three !!!!!\n");
					count = 0; // Initialize count zero
				}
			}
			if (strlen(receive_touch) != 3) delay_touch++;
			printf("%dth touch\n", delay_touch);
			delay(2000); // touch delay calculates
		}
		printf("count END\n");

		if (strlen(receive_touch) != 3) { // touch is not sensed
			if (ultrasonic3(50) == 0) { // person go out
				printf("person go out\n"); 
				delay(2000);
				servo(0, 1);
				buzzer(2);
			}
		}
	}
	return 0;
}

