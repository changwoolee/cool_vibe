#include "temp.h"



using namespace std;


Temp::Temp(int handType){
	if(handType!=4 || handType!=5){
		cout<<"Invalid Hand Type. Hand Type must be eithre Temp.LEFT_HAND or Temp.RIGHT_HAND"<<endl;
		exit(1);
	}
	
	hand = handType;

	if(wiringPiSetup()==1){
		perror("Cannot Start wiringPi\n");
		exit(1);
	}
	pinMode(hand,OUTPUT);
	
	
	if(hand==LEFT_HAND){
		pinMode(LEFT_HAND_DIR,OUTPUT);
		dirPin = LEFT_HAND_DIR;
	}
	else{
		pinMode(RIGHT_HAND_DIR,OUTPUT);
		dirPin = RIGHT_HAND_DIR;
	}


	softPwmCreate(hand,0,255);

	digitalWrite(dirPin,LOW);
	softPwmWrite(hand,0);

	isHandInit=1;
	isOn=0;
	isActive=0;
	pwmLimit = 200;
	
	tempMessage = new TempMessage;
	tempMessage->currentTemp=0;
	tempMessage->standardTemp=0;
	tempMessage->setPoint=0;
	tempMessage->dT=0;
	tempMessage->isPeltierOn=0;
	tempMessage->isPeltierActive=0;
	
}


Temp::~Temp(){
	softPwmWrite(LEFT_HAND,0);
	softPwmWrite(RIGHT_HAND,0);
}


void Temp::setPwmLimit(int val){
	pwmLimit = val;
}

Temp::TempMessage* Temp::getTempInfo(){
	return tempMessage;
}

void Temp::setTemp(int raw_temp){
	double temp = getCelciusTemperature(raw_temp);
	controlPeltier(temp);
}


double Temp::getCelciusTemperature(int val){
	int tval = val;
	int t1 = 10*tval;
	int t2 = 1024-tval;
	double R = (double)t1/t2;
	
      	if(R>12.14){return -1.0;}
	else if(R>10.0){
	   	return 20.0 + (12.14-R)*2.27;
	}
	else if(R>8.282){
		return 25 + (10-R)*2.79;
	}
	else if(R>7.15){
		return 30 + (8.282-R)*3.38;
	}
	else if(R>5.771){
		return 34+(7.15-R)*4.2;
	}
	else{
		return -1.0;
	}

}

void Temp::controlPeltier(double temp){
	if(isHandInit){
		tempMessage->currentTemp=temp;
		tempMessage->standardTemp=temp;
		tempMessage->setPoint=temp;	
		isHandInit=0;		
	}
	else{
		tempMessage->currentTemp=temp;
	}
	
	tempMessage->dT = temp-tempMessage->standardTemp;
	tempMessage->isPeltierOn = isOn;
	tempMessage->isPeltierActive = isActive;
	double dT = tempMessage->dT;
	if((dT>5||dT<-5||tempMessage->currentTemp==-1)&&isOn){
		
		softPwmWrite(hand,0);
		isOn=0;
		isActive=0;
		while(!messageQueue.empty()){
			messageQueue.pop();
		}
		return;

	}

	if(!messageQueue.empty()){
		PeltierMessage msg = messageQueue.front();
		messageQueue.pop();
		currentMode = msg.mode;
		currentPwmVal = msg.pwmVal;
		if(currentMode==HOT){
			tempMessage->setPoint = tempMessage->standardTemp+3;
			isActive=1;
			isOn=1;
		}


		else if(currentMode==COOL){
			tempMessage->setPoint = tempMessage->standardTemp-3;
			isActive=1;
			isOn=1;
		}
		else if(currentMode==RESET){
			tempMessage->standardTemp = tempMessage->currentTemp;
			isActive=0;
			isOn=0;
			return;
		}
		else{
			softPwmWrite(hand,0);
			isActive=0;
			isOn=0;
			return;
		}
	}
	if(isActive){
		if(currentMode==COOL){
			if(tempMessage->currentTemp < tempMessage->setPoint){
				softPwmWrite(hand,0);
			}
			else{
				digitalWrite(dirPin,COOL);
				softPwmWrite(hand,currentPwmVal);
			}
		}
		else{
			if(tempMessage->currentTemp > tempMessage->setPoint){
				softPwmWrite(hand,0);
			}
			else{
				digitalWrite(dirPin,HOT);
				softPwmWrite(hand,currentPwmVal);
			}
		}
	}
}	
		


void Temp::receiveMessage(char *message, int length){
	if(length<2){
		cout<<"Invalid Peltier Command. Too short\n";
		return;
	}

	int mode = message[0]=='c' ? (LOW) : (HIGH);
	int pwmVal = (int)message[1];

	if(mode!='c' || mode!='h'){
		cout<<"Invalid Peltier Command. Peltier Command must be either 'c' or 'h'"<<endl;
		return;
	}

	if(pwmVal<0 || pwmVal>pwmLimit){
		cout<<"Invalid PWM value. PWM value must be 0 to "<<pwmLimit<<".\n";
		return;
	}
	PeltierMessage msg = {mode,pwmVal};
	messageQueue.push(msg);

}
