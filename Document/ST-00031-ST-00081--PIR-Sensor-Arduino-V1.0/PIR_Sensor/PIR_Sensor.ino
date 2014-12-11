/*
Files     	: PIR_Sensor.ino
Version 	: 1.0
Date     	: 2013/09/10
Description	: SimplyTronics ST-00031/81 PIR Sensor/ Wide angle PIR Sensor
History		:

    1. Date		: 2013/09/10
       Author		: John Zhong@SimplyTronics
       Modification	: Create

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#define PIR_OUT (2) /* PIR OUT pin*/

void setup(void)
{
  int i = 40;
  Serial.begin(9600);
  delay(100);
  Serial.println("SimplyTronics PIR Sensor(ST-00031/ST-000081)");
  Serial.println("===========================================");
  Serial.println("Please wait 40 seconds the PIR Sensor to warm up...");
  while(i-- > 0)
  {
    Serial.print(i);
    Serial.println("  seconds left");
    delay(1000);
  } 
 Serial.println("Start detect the state of PIR Sensor."); 
  pinMode(PIR_OUT, INPUT);
}

int pir_output = 0;
void loop(void)
{
   pir_output = digitalRead(PIR_OUT);
   Serial.print("IN0 = ");
   Serial.println(pir_output, BIN);
   delay(200);
}
