/*
============================ 
Authors:
Azka Siddiqui
Eirene Mehta

last update: May 30, 2024
============================
*/

#include <pic.h>
#include <htc.h>

__CONFIG( FOSC_INTRCIO & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_OFF & 
IESO_OFF & FCMEN_OFF );

//defined constants- the compiler does a substitution 

//Global Variables 

int leftSensor = 0;
int rightSensor = 0;
int counter = 0;
int LEFT_THRESHOLD = 140;
int RIGHT_THRESHOLD = 156;
int done = 0;
int buttonOn = 0;

// PREDEFINE PROCEDURES: procedure must be defined before you can use them

void init_hardware(void);
void left(void);
void right(void);
void stop(void);
void spinLeft(void);
void spinRight(void);
void runADC(void);
void sharpTurnLeft(void);
void sharpTurnRight(void);
void readSensors(void);
void buttonGo(void);
void backward(void);
void forward(void);
void flashLights(void):
void flashSignal(int);

// MAIN PROGRAM //

void main(void) {
	init_hardware();            //run procedure init_hardware()

    while(RA0 == 0);
    
    while(1){
        if (RA4 == 1) ADCON0bits.CHS = 0b1011; //set ADC to Channel AN11
        if (RA5 == 1) ADCON0bits.CHS = 0B1010; //set ADC to Channel AN11

        _delay(10);
        GO_DONE = 1; //Tell the ADC to take a reading
        while (GO_DONE == 1){
            //wait for ADC to be done
        }

        PORTC = ADRESH;//move answer into display LEDs
        _delay(1000000);
        while (1==1){
            buttonGo
        }
    }
	
	while (RA0 == 1){
		readSensors();	
	}
	
	LEFT_THRESHOLD=leftSensor + 10;
	RIGHT_THRESHOLD=rightSensor + 15;
	
	while(1)
	{
		RA5 = 1;
		
		readSensors();
		
		if((leftSensor >= LEFT_THRESHOLD) && (rightSensor >= RIGHT_THRESHOLD))  // touching line on both sides
		{	
			RB7 = 1;
			RC0 = 1;
			counter++;
			
				while ((leftSensor >= LEFT_THRESHOLD) && (rightSensor >= RIGHT_THRESHOLD))
				{
					forward();
					_delay(10000);
					right();
					_delay(9000);
					readSensors();
				}
				
				stop();
				RB7 = 0;
				RC0 = 0;
				
				sharpTurnRight();
				int code;
				
				if(counter>=4&&counter<=7){
					stop();
					_delay(500000);
					code++;
				}
			
		}
		if ((rightSensor >= RIGHT_THRESHOLD) && (leftSensor < LEFT_THRESHOLD)) // touching line on right
		{
			RC0 = 1;
			right();
		}
		else if ((leftSensor >= LEFT_THRESHOLD) && (rightSensor < RIGHT_THRESHOLD)) // touching line on left
		{
			RB7 = 1;
			left();
		}
		else if ((rightSensor < RIGHT_THRESHOLD) && (leftSensor < LEFT_THRESHOLD))
		{
			forward();
			RB7 = 0;
			RC0 = 0;
		}
		
	} 
} // end of main method
		
// INITIALIZE HARDWARE //

void init_hardware(void)
{
	// set analogue pins as analogue(1) or digital(0) 
	
    ADCON0 = 0b00101101;
	ANSEL  = 0b00000000;         // AN7 to AN0
	ANSELH = 0b00001010;	     //  n/c , n/c, n/c, n/c, AN11, AN10,AN9, AN8
	
  	// set pins as Input(1) or Output(0)
       
	TRISA = 0b00110001;	
	TRISB = 0b00110000;
	TRISC = 0b10000000;
	
  	//initalize the ports
  	 
	PORTA = 0b00000000;
	PORTB = 0b00000000;
	PORTC = 0b00000000;	
	
}

// FORWARD //

void forward(void)
{
	RB7 = 1;
	RB6 = 0;
	RA2 = 1;
	RA1 = 0;
}

// BACKWARD //

void backward(void)
{
	RB7 = 0;
	RB6 = 1;
	RA2 = 0;
	RA1 = 1;
}

// LEFT //

void left(void)
{
	RB7 = 1;
	RB6 = 0;
	RA2 = 1;
	RA1 = 1;
}

// RIGHT //

void right(void)
{
	RB7 = 1;
	RB6 = 1;
	RA2 = 1;
	RA1 = 0;
}

// LEFT SPIN //

void spinLeft(void)
{
	RB7 = 1;
	RB6 = 0;
	RA2 = 0;
	RA1 = 1;
}

// RIGHT SPIN //

void spinRight(void)
{
	RB7 = 0;
	RB6 = 1;
	RA2 = 1;
	RA1 = 0;
}
// STOP //

void stop(void)
{
	RB7 = 1;
	RB6 = 1;
	RA2 = 1;
	RA1 = 1;
}	

// RUN ADC //

void runADC(void)
{
	GO_DONE = 1;
	while (GO_DONE == 1) {}
}	

//Sharp right turn//

void sharpTurnRight(void)
{
	forward();
	_delay(300000);
	stop();
	ADCON0bits.CHS = 0b1001;
	spinRight();
	
	do
	{
		readSensors();
	
	} while(leftSensor <= LEFT_THRESHOLD);
	
	spinLeft();
	_delay(100000);
	stop();
}

// FLASH ROBOT LIGHT //

void flashSignal (int count)
{
	for (int i = 0; i < count; i++)
	{
		RA5 = 1;
		_delay(500000);
		RA5 = 0;
		_delay(500000);
	}
}

// READ LEFT AND RIGHT SENSORS //

 void readSensors(void)
 {
	ADCON0bits.CHS = 0b1001;
	runADC();
	leftSensor = ADRESH;
	
	ADCON0bits.CHS = 0b1011;
	runADC();
	rightSensor = ADRESH;
}

// ARM DOWN //

void sharpTurnLeft(void)
{
	forward();
	_delay(500000);
	stop();
	spinLeft();
	int num=0;
	do
	{
		readSensors();
		if(rightSensor<=RIGHT_THRESHOLD)
			num++;
	} while(rightSensor <=RIGHT_THRESHOLD/*&&num==2&&counter!=2*/);
	
	
	spinRight();
	_delay(100000);
	stop();
}