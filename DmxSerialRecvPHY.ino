// - - - - -
// DmxSerial - A hardware supported interface to DMX.
// DmxSerialRecvPHY.ino:  DMX application for retrieving 6 DMX values


// base on DmxSerialRecv by Matthias Hertel
// Copyright (c) 2011-2015 by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// Documentation and samples are available at http://www.mathertel.de/Arduino


// Modified for 6 monochromes channels with convertion 0V 10V with a sample 2N2222
// Copyright (c) 03/2025 by net234   net234@frdev.com
// Merci a Jules pour ce petit projet qui m'a permis de me remettre a coder a une periode difficile pour moi :)

//Arduino UNO/NANO specs say following things:

//The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
//The base frequency for pins 5 and 6 is 62500 Hz.
//The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64, 256, and 1024.
//The divisors available on pins 3 and 11 are: 1, 8, 32, 64, 128, 256, and 1024.

//PWM frequencies are tied together in pairs of pins. If one in a pair is changed, the other is also changed to match.

//Pins 5 and 6 are paired on timer0.
//Pins 9 and 10 are paired on timer1.
//Pins 3 and 11 are paired on timer2.

// address 1 (channel1) -> PWM Port 5
const byte channel1 = 5;
// address 2 (channel2) -> PWM Port 6
const byte channel2 = 6;
// address 3 (channel3) -> PWM Port 9
const byte channel3 = 9;
// address 4 (channel4) -> PWM Port 10
const byte channel4 = 10;
// address 5 (channel5) -> PWM Port 3
const byte channel5 = 3;
// address 6 (channel6) -> PWM Port 11
const byte channel6 = 11;



#include <DMXSerial.h>



// This Example receives the 3 values starting with this channel:
const int startChannel = 0 * 6 + 1;  // univers1  channel 1-6

#define defaultLevel 10  // force les sortie a 10% en absence de trames

void setup() {

  DMXSerial.init(DMXReceiver);

  // set some default values
  DMXSerial.write(1, defaultLevel);
  DMXSerial.write(2, defaultLevel);
  DMXSerial.write(3, defaultLevel);
  DMXSerial.write(4, defaultLevel);
  DMXSerial.write(5, defaultLevel);
  DMXSerial.write(6, defaultLevel);

  // enable pwm outputs
  pinMode(channel1, OUTPUT);  // sets the digital pin as output
  pinMode(channel2, OUTPUT);
  pinMode(channel3, OUTPUT);
  pinMode(channel4, OUTPUT);
  pinMode(channel5, OUTPUT);
  pinMode(channel6, OUTPUT);
}



void loop() {
  // Calculate how long no data bucket was received
  unsigned long lastPacket = DMXSerial.noDataSince();

  if (lastPacket < 5000) {
    // read recent DMX values and set pwm levels


    analogWrite(channel1, adjustPWM(DMXSerial.read(startChannel + 0)));
    analogWrite(channel2, adjustPWM(DMXSerial.read(startChannel + 1)));
    analogWrite(channel3, adjustPWM(DMXSerial.read(startChannel + 2)));
    analogWrite(channel4, adjustPWM(DMXSerial.read(startChannel + 3)));
    analogWrite(channel5, adjustPWM(DMXSerial.read(startChannel + 4)));
    analogWrite(channel6, adjustPWM(DMXSerial.read(startChannel + 5)));

  } else {
    // Show defaultLevel, when no data was received since 5 seconds or more.
    analogWrite(channel1, defaultLevel);
    analogWrite(channel2, defaultLevel);
    analogWrite(channel3, defaultLevel);
    analogWrite(channel4, defaultLevel);
    analogWrite(channel5, defaultLevel);
    analogWrite(channel6, defaultLevel);


  }  // if
}

//correction de la sortie PWM avec un montage simple filtre + 2N222
// filtre 3.9K + 10micro Farad
// 2N222  39K en entree 1K en pull up
// TODO!! corriger cela en version2 avec un AMPLI OP OPA4197 ou TLV2374
// TODO!! faire une correction avec des tables 256 valeurs (1 par voie) plutot qu'une fonction
int  adjustPWM(int aValue) {
  //float ratio = 1000 / 254;
  int result = 0;
  result = 255 - aValue;
  result *= 220 / 255;
  result += 35;
  return result;
}
// End.
