byte CWlimit = 5;
byte CCWlimit = 6;
byte dir = 6;
byte ServoState = 0;
int g = 0;
int tempInt = 0;
#include <Servo.h> 
#define ThermistorPIN 0   // Analog Pin 0
int temp;
int hundreds;
int tens;
Servo arm;  // create servo object to control a servo 
int pos = 0;    // variable to store the servo position 
 
void setup() {

  Serial.begin(115200); 
  
  pinMode(CWlimit, INPUT);
  pinMode(CCWlimit, INPUT);

  arm.attach(7);  // attaches the servo on pin 9 to the servo object 

  home();
  
} 
  
void loop() { 

	if (Serial.available() > 3) {
	
		if (1) {	//Sync byte "!" + 3 bytes of code
			
			g = Serial.read();				//Get "!" leading character				
			
			g = (Serial.read() - 48) * 100;
			g += (Serial.read() - 48) * 10;
			g += (Serial.read() - 48);
			
			//Serial.flush();
			//Serial.print("!");
			//Serial.print(g);
		
			ServoState = g;
			//ServoState = 0;
		
			if (ServoState == 90) {
				Stop();
			}
		
			if (ServoState < 90 and digitalRead(CCWlimit)) {	//Want to move CCW, and switch isn't hit?
			
				arm.write(ServoState);
			}
		
			if (ServoState > 90 and digitalRead(CWlimit)) {	//Want to move CW, and switch isn't hit?
			
				arm.write(ServoState);
			}	

                        SendTemperature();

		}

		//Serial.flush();	
		
	}      

  if (digitalRead(CCWlimit) == 0) { // and ServoState < 90) {
  
	arm.write(150);
        
        //Stop();

	}
 
  if (digitalRead(CWlimit) == 0) { // and ServoState > 90) {
  
	arm.write(40);
    
        //Stop();

	} 
  
  
}

void home() {
  
  if (digitalRead(CCWlimit) == 0) {  //Already at 0 position?
    return;
  }
  
  arm.write(0);
  
  while (digitalRead(CCWlimit)) {  //Keep going until it hits the switch
  }
  
  Stop();


}

void Stop() {

  ServoState = 90;  
  arm.write(ServoState);

  
}

void SendTemperature() {                                   //Send ! and 3 characters of temperature
 
   temp = Thermistor(analogRead(ThermistorPIN));           // read ADC and convert it to Celsius 
  
   Serial.write("!");
   Serial.print(temp);
   /*
   if (temp > 99) {
     Serial.write((temp / 100) + 48);                      //Write hundreds
     hundreds = (temp / 100) * 100;
     tens = (temp - hundreds) / 10;
     Serial.write(tens + 48);  //and tens, lobbing off the hundreds
   }
   else{
     Serial.write("0");                                   //Under 100
     Serial.write((temp / 10) + 48);                       //Tens
   } 
   
   Serial.write((temp - ((temp / 10) * 10)) + 48);        //Do ones
    */
   //Serial.println(" F");

}



double Thermistor(int RawADC) {
  
 long Resistance;  double Temp;  // Dual-Purpose variable to save space.
 Resistance=((1024000/RawADC) - 1000);  // Assuming a 10k Thermistor.  Calculation is actually: Resistance = (1024/ADC)
 Temp = log(Resistance); // Saving the Log(resistance) so not to calculate it 4 times later. // "Temp" means "Temporary" on this line.
 Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));   // Now it means both "Temporary" and "Temperature"
 Temp = Temp - 273.15;  // Convert Kelvin to Celsius                                         // Now it only means "Temperature"

 Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert to Fahrenheit
 return Temp;  // Return the Temperature

}

