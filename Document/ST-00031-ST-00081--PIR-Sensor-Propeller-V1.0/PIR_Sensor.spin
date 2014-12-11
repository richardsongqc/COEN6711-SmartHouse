{{

PIR_Sensor.spin   
Files           : PIR_Sensor.spin
Version         : 1.0
Date            : 2013/09/10
Description     : SimplyTronics ST-00031/81 PIR Sensor/ Wide angle PIR Sensor
                  Displays the current state of the PIR Sensor connected to P0
                  
History         :

    1. Date             : 2013/09/10
       Author           : John Zhong@SimplyTronics
       Modification     : Create
}}


CON
 
  _clkmode = xtal1 + pll16x     ' Setting Clock Mode to Crystal 1 with 16 multiplier 
  _xinfreq = 5_000_000          ' Propeller set to run at 80MHz
  
VAR

 byte state                     ' Declare variable state to store PIR output
  
OBJ
 
 pst : "Parallax Serial Terminal.spin"
  
PUB PIR   | i                   ' Public Method name PIR

 dira[2]~                       ' Set pin 2 to input 
 pst.start(115200)              ' Start Parallax Serial Terminal at 115200 baud
 
 pst.clear                      ' Clear the screen    
 pst.str(@SimplyTronicsInfo)
 pst.str(@WarmUpInfo)
 
 repeat i from 39 to 0
    pst.dec(i)
    pst.str(@CounterInfo)
    waitcnt(clkfreq + cnt)      ' PIR "warm-up" time
 
                                             
 pst.clear                      ' Clear the screen    
 pst.str(@SimplyTronicsInfo)
 
 repeat 
    state := ina[2]             ' Save state of PIR Sensor 
    pst.Home                    ' Move cursor to position(0,3) 
    pst.NewLine
    pst.NewLine
    pst.NewLine
    pst.str(string("IN0 =  ")) 
    pst.bin(state, 1)           ' Display state 
    waitcnt(clkfreq/200 + cnt)  ' Small delay

Dat

SimplyTronicsInfo byte 9,"SimplyTronics PIR Sensor",13
                  byte 9,"(ST-00031/ST-000081)", 13
                  byte "===========================================", 13,0       
WarmUpInfo        byte "Please wait 40 seconds the PIR Sensor to warm up...",13,0
CounterInfo       byte " seconds left  ",13,0