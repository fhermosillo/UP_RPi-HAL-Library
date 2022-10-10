#include <stdio.h>
#include <wiringPi.h>

const int pwmPin = 18; // PWM LED - GPIO18, P1 pin 12
const int ledPin = 16; // Regular LED - GPIO23, P1 pin 16
const int butPin = 17; // Active-low button - GPIO17, P1 pin 11

int main()
{
	// Initialize WiringPi library
	wiringPiSetup();
	wiringPiSetupGpio();
	
	// Setup GPIO
	pinMode(butPin, INPUT);
	pinMode(ledPin, OUTPUT);
	pinMode(pwmPin, PWM_OUTPUT);
	pullUpDnControl(butPin, PUD_UP);
	
	
	// Test GPIO
	int pwmValue = 500;
	while(1)
	{
		if (digitalRead(butPin)) // Button is released if this returns 1
		{
		    pwmWrite(pwmPin, pwmValue); // PWM LED at bright setting
		    digitalWrite(ledPin, LOW);     // Regular LED off
		}
		else // If digitalRead returns 0, button is pressed
		{
		    pwmWrite(pwmPin, 1024 - pwmValue); // PWM LED at dim setting		    
		}

		// Do some blinking on the ledPin:
		digitalWrite(ledPin, HIGH); // Turn LED ON
		delay(500); // Wait 75ms
		digitalWrite(ledPin, LOW); // Turn LED OFF
		delay(500); // Wait 75ms again
	}
}