# Samevatting mrb 20-21

## week 1
https://canvas.hu.nl/courses/20247/pages/week%201

- PID + eindopdracht tldr

![alt-text](https://www.csimn.com/CSI_images/info/PIDforDummies_pid_controller1.png)

What’s going on this diagram?

The Setpoint (SP) is the value that we want the process to be.

For example, the temperature control system in our house may have a SP of 22°C. This means that

“we want the heating and cooling process in our house to achieve a steady temperature of as close to 22°C as possible”

The PID controller looks at the setpoint and compares it with the actual value of the Process Variable (PV). Back in our house, the box of electronics that is the PID controller in our Heating and Cooling system looks at the value of the temperature sensor in the room and sees how close it is to 22°C.

If the SP and the PV are the same – then the controller is a very happy little box. It doesn’t have to do anything, it will set its output to zero.

However, if there is a disparity between the SP and the PV we have an error and corrective action is needed. In our house this will either be cooling or heating depending on whether the PV is higher or lower than the SP respectively.

Let’s imagine the temperature PV in our house is higher than the SP. It is too hot. The air-con is switched on and the temperature drops.

The sensor picks up the lower temperature, feeds that back to the controller, the controller sees that the “temperature error” is not as great because the PV (temperature) has dropped and the air con is turned down a little.

This process is repeated until the house has cooled down to 22°C and there is no error.

Then a disturbance hits the system and the controller has to kick in again.

In our house the disturbance may be the sun beating down on the roof, raising the temperature of the air inside.

So that’s a really, really basic overview of a simple feedback control system. Sounds dead simple eh?

Understanding the controller
Unfortunately, in the real world we need a controller that is a bit more complicated than the one described above, if we want top performance from our loops. To understand why, we will be doing some “thought experiments” where we are the controller.

When we have gone through these thought experiments we will appreciate why a PID algorithm is needed and why/how it works to control the process.

We will be using the analogy of changing lanes on a freeway on a windy day. We are the driver, and therefore the controller of the process of changing the car’s position.

Here’s the Block Diagram we used before, with the labels changed to represent the car-on-windy-freeway control loop.




![alt-text](https://www.csimn.com/CSI_images/info/PIDforDummies_pid_controller2.png)




Notice how important closing the feedback loop is. If we removed the feedback loop we would be in “open loop control”, and would have to control the car’s position with our eyes closed!

Thankfully we are under “Closed loop control” -using our eyes for position feedback.

As we saw in the house-temperature example the controller takes the both the PV and SP signals, which it then puts through a black box to calculate a controller output. That controller output is sent to an actuator which moves to actually control the process.

We are interested here in what the black box actually does, which is that it applies 1, 2 or 3 calculations to the SP and Measured PV signals. These calculations, called the “Modes of Control” include:

Proportional (P)

Integral (I)

Derivative (D)






![alt-text](https://www.csimn.com/CSI_images/info/PIDforDummies_pid_simplified.png)



It is really very simple in operation. The PV is subtracted from the SP to create the Error. The error is simply multiplied by one, two or all of the calculated P, I and D actions (depending which ones are turned on). Then the resulting “error x control actions” are added together and sent to the controller output.

These 3 modes are used in different combinations:

P – Sometimes used

PI - Most often used

PID – Sometimes used

PD – rare but can be useful for controlling servomotors.


https://www.csimn.com/CSI_pages/PIDforDummies.html
 ## more: 

 
Proportional Integral Derivative (PID) control automatically adjusts a control output based on the difference between a set point (SP) and a measured process variable (PV). The value of the controller output is transferred as the system input.

The ubias term is a constant that is typically set to the value of u(t)
 when the controller is first switched from manual to automatic mode. This gives "bumpless" transfer if the error is zero when the controller is turned on. The three tuning values for a PID controller are the controller gain, 
Kc, the integral time constant 
τI
, and the derivative time constant 
τ
D
. The value of 
K
c
 is a multiplier on the proportional error and integral term and a higher value makes the controller more aggressive at responding to errors away from the set point. The integral time constant 
τ
I
 (also known as integral reset time) must be positive and has units of time. As 
τ
I
 gets smaller, the integral term is larger because 
τ
I
 is in the denominator. Derivative time constant 
τ
D
 also has units of time and must be positive. The set point (SP) is the target value and process variable (PV) is the measured value that may deviate from the desired value. The error from the set point is the difference between the SP and PV and is defined as e(t)=SP−PV.


https://apmonitor.com/pdc/index.php/Main/ProportionalIntegralDerivative#:~:text=Proportional%20Integral%20Derivative%20(PID)%20control,transferred%20as%20the%20system%20input.

![alt-text](https://upload.wikimedia.org/wikipedia/commons/thumb/4/43/PID_en.svg/600px-PID_en.svg.png)

## week 2
https://canvas.hu.nl/courses/20247/pages/week%202

## Fourier Tranformation

colleges: 
https://youtu.be/dEBeVEYmDQI
en 
https://www.youtube.com/watch?v=_pkHloohdlk

https://www.thefouriertransform.com/#introduction see this 

## *lijkt er op dat je aleen hoeft de kijken naar diagrammen, niet naar rekenen , kijk naar opdrachten dit krijge we denk ook tentamen*  kijk ff met die mrb dft python file

## Basis Electorica
 *Reader Basis Elektronica.pdf* 38 paginas lmao

 ![alt-text](https://ohmlaw.com/wp-content/uploads/2018/02/ohms-law-triangle-with-formulas.jpg)
 brugklas classic


* Power dissipated by a resistor is ```P = I^2 x R``` of ```P = V^2 / R```

![alt-text](https://www.electroschematics.com/wp-content/uploads/2013/02/resistors-parallel.jpg)
* **Parrallelweestand**. 1/Rv = 1/R1 +1/R2 , ofwel:  Rv = R1 R2 /(R1+R2)
Stel nu dat R2>>R1. ( >> betekent “is veel groter dan” )Dan is 1/R2 verwaarloosbaar klein vergeleken 1/R1, en mogen we dus verwaarlozen.Er blijft dan dus over: 1/Rv = 1/R1 Dus als je twee weerstanden parallel zet, en de ene weerstand is veel groter dan de andere, dan bepaalt de kleinste van de twee welke stroom er loopt, en is het net of de ander er niet is.








## week 3
https://canvas.hu.nl/courses/20247/pages/week%203



## week 4 / 5
https://canvas.hu.nl/courses/20247/pages/week%205
### CONDENSTATOREN = CAPACITORS
zie marius:  Condensator_Z_AC_DC.pdf 32 paginas lmao
tldr:

The capacitor is a component which has the ability or “capacity” to store energy in the form of an electrical charge producing a potential difference (Static Voltage) across its plates, much like a small rechargeable battery.


Wrap‐up:	Twee	brillen	om	naar	een	condensator	te	kijken	
Een samenvatting van het condensatorverhaal zou het volgende kunnen zijn. Er zijn twee soorten
situaties waarbij je het gedrag van de condensator op een andere manier analyseert:


* Als er spanningen of stromen worden geschakeld, dan gebruik je het “laadcurve‐model” van
de condensator om te beschrijven wat er gebeurt.  


* Als er voldoende traag wordt geschakeld, kun je met een “DC‐analyse” (alle
condensatoren door open verbindingen vervangen) snel zien naar welke eindniveaus
de knooppuntsspanningen gaan.


* Als er sprake is van (kleine) wisselspanningen(AC signalen) rond een gemiddelde DC‐
spanning, dan ben je normaal gesproken geinteresseerd in de “impedantie” van de
condensator, omdat je daarmee een goed beeld krijgt van hoe de AC‐signalen door het
circuit stromen.


* Bij die AC analyse mag je voor frequenties boven de kantelfrequentie de
condensators als kortsluiting beschouwen.
o Ook de voedingsspanningen mogen bij AC analyse als kortsluiting beschouwd
worden.

## week 6
https://canvas.hu.nl/courses/20247/pages/week%206


### Kirchoff stroom wet: 
![alt-text](https://upload.wikimedia.org/wikipedia/commons/thumb/4/46/KCL_-_Kirchhoff%27s_circuit_laws.svg/330px-KCL_-_Kirchhoff%27s_circuit_laws.svg.png)

*  *The current entering any junction is equal to the current leaving that junction. i2 + i3 = i1 + i4*

### Kirchoff spanning wet:
![alt-text](https://upload.wikimedia.org/wikipedia/commons/thumb/4/40/Kirchhoff_voltage_law.svg/300px-Kirchhoff_voltage_law.svg.png)

* *The sum of all the voltages around a loop is equal to zero.
v1 + v2 + v3 +v4 = 0*


### impedantie
https://simple.wikipedia.org/wiki/Electrical_impedance

*Impedantie wordt in de elektriciteitsleer gebruikt om te rekenen met spoelen en condensatoren die een tijds- en frequentieafhankelijke weerstand hebben. In het algemeen: het is een begrip dat wordt gebruikt in de beschrijving van een lineaire respons op een tijdsafhankelijk signaal.*

ZIE REKENEN MET VERVANGINGS IMPEDANTIES -. **Practicum AC_DC.docx**

### Darlington configuration 
![alt-text](https://upload.wikimedia.org/wikipedia/commons/thumb/7/78/Darlington_pair_diagram.svg/1200px-Darlington_pair_diagram.svg.png)
In electronics, a multi-transistor configuration called the Darlington configuration (commonly called a Darlington pair) is a circuit consisting of two bipolar transistors with the emitter of one transistor connected to the base of the other, in such a way that the current amplified by the first transistor is amplified further by the second one.[1] The collectors of both transistors are connected together. This configuration has a much higher current gain than each transistor taken separately. It acts like and is often packaged as a single transistor.


zie: **Practisch Kirchoff.pdf**




### Diodes

### Transistors
 
### OpAmps
**Opamps.pdf** maar 6 paginas kijk maar gwn dit


### Kirchhoff



**Trappen en Structuren.pdf** idk wat dit is ff naar kijken ziet leuk uit


## week 7
https://canvas.hu.nl/courses/20247/pages/week%207
bonus shit over opamps niet op tentamen

