# Lab 5: Sensing The World Around You
Now that your processor has taken its first glimpse at the world around, it is now time to teach it how interact with other things. Up to this point all you have taught your processor to do is count and turn something on or off. But in this world, only a small amount of things work like a switch. Most things require some form of resolution between them instead of gross movement. It is your task in your lab to teach your processor fine motor skills and precisely control something in the real world. 

##ADC10
Thinking I fried my G2553, so...the code is rather simple, just a basic UART echoing ADC values every cycle

##ADC12
Same story as before, FR5994 to convert a voltage to digital, UART to read it. 

##The Tests
Voltage:
A LM35DT was used to check the temperature of the room. by connecting 5V across it, and connecting the output to my FR5994. Using USB UART the value was returned to be visualized in RealTerm.

Current:
A Phototransistor was used to measure the light level of the room. With 5V at Vc, the base floating, and a 2k resistor from the emitter to ground, the voltage across the resistor varied according to the inverse of the light level. The brighter it was, the lower the voltage.

Resistance:
A Photoresistor was R1 in a voltage divider, along with a 47 ohm resistor and 5V. This creates a situation where as the light level increases, the photoresistor resistance decreases, and the output voltage increases. 
