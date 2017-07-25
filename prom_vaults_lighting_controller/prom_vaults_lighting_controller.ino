/* Prom Vaults Ligting Controller
   Version 1.0
   Written by Andy Doswell 2015
   License: The MIT License (See full license at the bottom of this file)
   
   Schematic can be found at www.andydoz.blogspot.com/
   A sound to light controller using the MSGEQ7 filter IC to analyse audio. 
   Software extracts the beat, and uses this to trigger lighting effects. 
   Also has "light organ" and Strobe effects.   
   
   */
int analogPin = 0; // MSGEQ7 OUT 3
int strobePin = 2; // MSGEQ7 STROBE 4
int resetPin = 4; // MSGEQ7 RESET 7 
int channel_1 = 12; // Light output channel 1
int channel_2 = 11; // light output channel 2
int channel_3 = 10; // light output channel 3
int channel_4 = 9; // light output channel 4
int channel_red = 6; // Red output channel
int channel_green = 5; // Green output channel
int channel_blue = 3; // Blue output channel
int channel_strobe = 13; // stoboscope trigger on pin 13
// RGB outputs are on pins 3,5 and 6 respectively
int stroboPin = 7; // Not a stobe as in data , but stobe as in rapidly flashing lights. Active low.
int stroboOutPin = 13; // Output trigger for stroboscope light
int speedPotPin = A1; // Strobe speed pot connected here, and to +5V and GND
int strobeDelay; // delay for strobe effect
int strobeTime = 30000; // maximum time allowable for strobe operartion
unsigned long strobeClock = 0;
int strobeLocked = 0;
int spectrumValue[7]; // sets the array for the output from the MSGEQ7
int filterValue = 80; // cuts out the noise from the input, like a squelch control.
int beatAGC = 126; //set Beat detect AGC to halfway
int AGCInc = 150; //Sets the rate at which the AGC is incremented (attack)
int AGCDec = 1; //Sets the rate at which the AGC is decremented (decay)
int AGCLowLimit = 40; // Sets the lower limit of the beat detector AGC. Has a dramatic effect on beat detection.
int beatDebounce = 200; // bit of a debounce delay for the beat detector
int beatTimer = 100; // counter for the beat debounce
int beatFlag = 0; // set high when the level from the low frequency channel exceeds the AGC level
int pattern = 0; // set default pattern
int beat = 0; // global beat counter for the changing RGB mode
int RGBmode = 0; // default RGB mode
int beatcount = 0; // beat counter for change of RGB output if in RGB mode.
// Gains of each channel, used to alter the "grey scale" or colour balance of the strip
int redGain = 10;
int greenGain = 20;
int blueGain = 10;

void setup() {
  // Read from MSGEQ7 OUT
  pinMode(analogPin, INPUT);
  // Write to MSGEQ7 STROBE and RESET
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  //pinMode(beatPin, OUTPUT); // beat detector output pin
  pinMode(channel_1 , OUTPUT); // light channel 1
  pinMode(channel_2 , OUTPUT); // light channel 2
  pinMode(channel_3 , OUTPUT); // light channel 3
  pinMode(channel_4 , OUTPUT); // light channel 4
  pinMode(channel_strobe, OUTPUT); // Strobe trigger channel
  pinMode(stroboPin, INPUT_PULLUP); // Strobe switch connected between here and gnd.
  pinMode (speedPotPin, INPUT);
  // Set analogPin's reference voltage
  analogReference(DEFAULT); // 5V
 
  //check all channels
  digitalWrite(channel_1,HIGH);
  delay (500);
  digitalWrite(channel_1,LOW);
  delay (500);
  digitalWrite(channel_2,HIGH);
  delay (500);
  digitalWrite(channel_2,LOW);
  delay (500);
  digitalWrite(channel_3,HIGH);
  delay (500);
  digitalWrite(channel_3,LOW);
  delay (500);
  digitalWrite(channel_4,HIGH);
  delay (500);
  digitalWrite(channel_4,LOW);
  delay (500);
  analogWrite (channel_red,255);
  delay (500);
  analogWrite (channel_red,0);
  delay (500);
  analogWrite (channel_green,255);
  delay (500);
  analogWrite (channel_green,0);
  delay (500);
  analogWrite (channel_blue,255);
  delay (500);
  analogWrite (channel_blue,0);
  delay (500);
  
  // Set startup values for pins
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
}
// following are the patterns for the 4 main channels
void display_pattern1()
{
  digitalWrite (channel_1, HIGH);
  digitalWrite (channel_2, HIGH);
  digitalWrite (channel_3, HIGH);
  digitalWrite (channel_4, HIGH);
  delay (30);
  digitalWrite (channel_1, LOW);
  digitalWrite (channel_2, LOW);
  digitalWrite (channel_3, LOW);
  digitalWrite (channel_4, LOW);
}

void display_pattern2()
{
  digitalWrite (channel_1,HIGH); 
  delay (30);
  digitalWrite (channel_1, LOW);
}

void display_pattern3()
{
  digitalWrite(channel_2,HIGH); 
  delay (30);
  digitalWrite(channel_2, LOW);
}

void display_pattern4()
{
  digitalWrite(channel_3,HIGH); 
  delay (30);
  digitalWrite(channel_3, LOW);
}

void display_pattern5()
{
  digitalWrite(channel_4,HIGH); 
  delay (30);
  digitalWrite(channel_4, LOW);
}

void display_pattern6()
{
  digitalWrite (channel_1,HIGH);
  digitalWrite(channel_2,HIGH); 
  delay (30);
  digitalWrite (channel_1,LOW);
  digitalWrite(channel_2,LOW);
}

void display_pattern7()
{
  digitalWrite(channel_3,HIGH);
  digitalWrite(channel_4,HIGH); 
  delay (30);
  digitalWrite(channel_3,LOW);
  digitalWrite(channel_4,LOW);
}

void display_pattern8()
{
  digitalWrite(channel_3,HIGH);
  digitalWrite(channel_2,HIGH); 
  delay (30);
  digitalWrite(channel_3,LOW);
  digitalWrite(channel_2,LOW);
}

void display_pattern9()
{
  digitalWrite(channel_4,HIGH);
  digitalWrite(channel_2,HIGH); 
  delay (30);
  digitalWrite(channel_4,LOW);
  digitalWrite(channel_2,LOW);
}

void display_pattern10()
{
  digitalWrite(channel_4,HIGH);
  digitalWrite(channel_1,HIGH); 
  delay (30);
  digitalWrite(channel_4,LOW);
  digitalWrite(channel_1,LOW);
}
void display_pattern11()
{
  digitalWrite(channel_1,HIGH); 
  delay (30);
}

void display_pattern12()
{
  digitalWrite(channel_2,HIGH); 
  delay (30);
}

void display_pattern13()
{
  digitalWrite(channel_3,HIGH); 
  delay (30);
}

void display_pattern14()
{
  digitalWrite(channel_4,HIGH); 
  delay (30);
}

void display_pattern15()
{
  digitalWrite(channel_1,HIGH);
  digitalWrite(channel_2,HIGH); 
  delay (30);
}

void display_pattern16()
{
  digitalWrite(channel_3,HIGH);
  digitalWrite(channel_4,HIGH); 
  delay (30);
}

void display_pattern17()
{
  digitalWrite(channel_3,HIGH);
  digitalWrite(channel_2,HIGH); 
  delay (30);
}

void display_pattern18()
{
  digitalWrite(channel_4,HIGH);
  digitalWrite(channel_2,HIGH); 
  delay (30);
}

void display_pattern19()
{
  digitalWrite(channel_4,HIGH);
  digitalWrite(channel_1,HIGH); 
  delay (30);
}

void display_pattern20()
{
  digitalWrite(channel_4,HIGH);
  digitalWrite(channel_3,HIGH);
  digitalWrite(channel_2,HIGH);
  digitalWrite(channel_1,HIGH);
  
}

void display_pattern21()
{
  digitalWrite(channel_4,LOW);
  digitalWrite(channel_3,LOW);
  digitalWrite(channel_2,LOW);
  digitalWrite(channel_1,LOW);

  
}

 void strobe() {
      
    if (millis() > (strobeClock+30000)) { // if the strobe has been on for 30 seconds then inhibit
      strobeLocked = 1;
    }
    
    if (strobeLocked == 0) { // if the strobe isn't inhibited 
      
    // everything on 
    digitalWrite(channel_4,HIGH);
    digitalWrite(channel_3,HIGH);
    digitalWrite(channel_2,HIGH);
    digitalWrite(channel_1,HIGH);
    digitalWrite (channel_strobe,HIGH);
    analogWrite (channel_red,255);
    analogWrite (channel_green,255);
    analogWrite (channel_blue,255);
    strobeDelay = analogRead(speedPotPin);
    strobeDelay = map(strobeDelay, 0, 1023, 15, 255);
    delay (strobeDelay); // read the pot and delay for the returned value.
    
    // everything off
    digitalWrite(channel_4,LOW);
    digitalWrite(channel_3,LOW);
    digitalWrite(channel_2,LOW);
    digitalWrite(channel_1,LOW);
    digitalWrite (channel_strobe,LOW);
    analogWrite (channel_red,0);
    analogWrite (channel_green,0);
    analogWrite (channel_blue,0);
    delay (strobeDelay); 
    if (digitalRead (stroboPin) == LOW ) {
    strobe ();
    }

  }  

}

void getSpectrum () { // enables the MSQEQ7 and gets the data
  
   // Set reset pin low to enable strobe
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);

 
  // Get all 7 spectrum values from the MSGEQ7
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
    
    delayMicroseconds(30); // Allow output to settle
 
    spectrumValue[i] = analogRead(analogPin);
 
    // Constrain any value above 1023 or below filterValue
    spectrumValue[i] = constrain(spectrumValue[i], filterValue, 1023);
 
    // Remap the value to a number between 0 and 255
    spectrumValue[i] = map(spectrumValue[i], filterValue, 1023, 0, 255);
    
    digitalWrite(strobePin, HIGH);
    
   }
}

void loop() {
   if ((digitalRead(stroboPin) == LOW) && strobeLocked ==0) { // if the strobo is set, and isn't inhibited
     strobeClock = millis ();  
     strobe();
  }
   if ((strobeLocked == 1 && (millis() >= (strobeClock+90000)))) { // if the strobo is inhibited and 90 seconds have passed since it was last activated then unlock 
     strobeLocked = 0;
   }  
   
  getSpectrum (); //gets the current values from the MSGEQ7
  
  beatAGC = constrain (beatAGC,AGCLowLimit,255); // ensures the AGC is between the filter setting and the maximum
     
  if (spectrumValue[0] > beatAGC) {
       beatFlag = 1;
       beatAGC = beatAGC+AGCInc; //increase the AGC if it's been set. Changing the values of the AGCInc & AGCDec (attack and decay) has a quite marked effect on the performance of the beat detector
     }
  else { //decrease the ACG if it hasn't
       beatAGC = beatAGC-AGCDec;
       beatFlag = 0; 
     }
    
   if (beatFlag == 1 && beatTimer <= 0) {
     beatFlag=0;// reset the beat flag
     beatTimer = beatDebounce; // dont allow a trigger again for a number of program cycles
     
       pattern = random(24); // select a random pattern and call it
     
       if (pattern == 0) {
        display_pattern1();
       }   
       if (pattern == 1 ) {
         display_pattern2();
       } 
       if (pattern == 2 ) {
         display_pattern3();
       }   
       if (pattern == 3 ) {
         display_pattern4();
       }
       if (pattern ==4 ) {
         display_pattern5();
       }    
       if (pattern ==5 ) {
         display_pattern6();
       } 
       if (pattern ==6) {
         display_pattern7();
       }
       if (pattern ==7) {
         display_pattern8();
       } 
       if (pattern ==8 ) {
         display_pattern9();
       } 
       if (pattern ==9 ) {
         display_pattern10();
       } 
       if (pattern == 10) {
        display_pattern11();
       }   
       if (pattern == 11 ) {
         display_pattern12();
       } 
       if (pattern == 12 ) {
         display_pattern13();
       }   
       if (pattern == 13 ) {
         display_pattern14();
       }
       if (pattern ==14 ) {
         display_pattern15();
       }    
       if (pattern ==15 ) {
         display_pattern16();
       } 
       if (pattern ==16) {
         display_pattern17();
       }
       if (pattern ==17) {
         display_pattern18();
       } 
       if (pattern ==18 ) {
         display_pattern19();
       } 
       if (pattern ==19 ) {
         display_pattern20();
       } 
       if (pattern ==20 ) {
         display_pattern1();
       } 
       if (pattern ==21 ) {
         display_pattern21();
       } 
       if (pattern ==22 ) {
         display_pattern20();
       } 
       if (pattern ==23 ) {
         display_pattern20();
       } 
       beat ++; // increament the beat counter
       if (beat % 16 ==0) { // every 16 beats change the RGB mode
         beatcount = 5;
         RGBmode = random (3);
       }
       // cycles through primary / tertiary colours every beat
       if (RGBmode == 1 && beatcount == 0) {
           analogWrite (channel_red,255);
           analogWrite (channel_green,0);
           analogWrite (channel_blue,0);
           beatcount = 5;
         }
         if (RGBmode == 1 &&beatcount == 1) {
           analogWrite (channel_green,255);
           analogWrite (channel_red,0);
           analogWrite (channel_blue,0);
           beatcount --;
         }
         if (RGBmode == 1 &&beatcount == 2) {
           analogWrite (channel_blue,255);
           analogWrite (channel_green,0);
           analogWrite (channel_red,0);
           beatcount --;
         }
         if (RGBmode == 1 &&beatcount == 3) {
           analogWrite (channel_red,255);
           analogWrite (channel_green,255);
           analogWrite (channel_blue,0);
           beatcount --;
         }
         if (RGBmode == 1 &&beatcount == 4) {
           analogWrite (channel_red,255);
           analogWrite (channel_green,0);
           analogWrite (channel_blue,255);
           beatcount --;
         }
         if (RGBmode == 1 &&beatcount == 5) {
           analogWrite (channel_red,0);
           analogWrite (channel_green,255);
           analogWrite (channel_blue,255);
           beatcount --;
           }
        // cycles through random colours every beat
       if (RGBmode == 2) {
           analogWrite (channel_red,(random (256)));
           analogWrite (channel_green,(random (256)));
           analogWrite (channel_blue,(random (256)));
       }
         
   }   
     beatTimer --; // provides some debounce to the beat detector
     beatTimer = constrain(beatTimer,-1,beatDebounce);
     if (beatTimer == 0){
      display_pattern21();
     } 
     
     if (RGBmode == 0) { // spectral RGB output (multiplication factor changes the scaling of each colour)
     analogWrite (channel_red,((spectrumValue[1])/10)*redGain);
     analogWrite (channel_green,((spectrumValue[3])/10)*greenGain);
     analogWrite (channel_blue,((spectrumValue[6])/10)*blueGain);
     }
   }
/*
 * Copyright (c) 2015 Andrew Doswell
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHOR(S) OR COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
