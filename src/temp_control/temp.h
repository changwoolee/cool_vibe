#ifndef __TEMP_H__
#define __TEMP_H__

#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <queue>

using namespace std;
class Temp{
public:
	struct PeltierMessage{
		int mode;
		int pwmVal;
	};
	struct TempMessage{
		double currentTemp;
		double standardTemp;
		double setPoint;
		double dT;
		int isPeltierOn;
		int isPeltierActive;
		int mode;
	};


	Temp(int handType);
	~Temp();
	const static int LEFT_HAND_DIR = 1; // Direction, Physical Pin 12
	const static int LEFT_HAND = 4; // PWM, Physical Pin 16
	const static int RIGHT_HAND_DIR = 5; //Direction, Physical Pin 18
	const static int RIGHT_HAND = 6; // PWM, Physical Pin 22
	
	const static int LEFT_HAND_COOLER=24;
	const static int RIGHT_HAND_COOLER=25;
	

	const static int COOL = LOW;
	const static int HOT = HIGH;
	const static int OFF = 2;
	const static int RESET = 3; 
	

	void receiveMessage(char* message, int length);

	void setTemp(int raw_temp);
	void setPwmLimit(int val);
	TempMessage* getTempInfo();
	
	
private:
	double getCelciusTemperature(int val);
	
	void controlPeltier(double temp);

	Temp::TempMessage* tempMessage;

	queue<PeltierMessage> messageQueue;
	int hand;
	int dirPin;
	int coolerPin;
	int isHandInit;
	int isOn;
	int isActive;
	int pwmLimit;
	int currentMode;
	int currentPwmVal;
};
#endif /* __TEMP_H__ */
