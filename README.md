# Arduino-home-alaram
Home made arduino system that sends an email when the door is opened.

System requirements:
Arduino uno
Ethernet shield
2 metal plates
1 LED diode
1 resistor

Software description:
The plates should be positioned in a way that one of them is on the door and the other one on a stationary place,
which would allow both of them to be in contact when the door is closed.
The arduino sends 5V through the plates into a chosen port of the arduino.
If the plates are in contact the electricity would flow and the port would receive 1,
if they are not touching the port would receive 0.

If 0 is received 2 times in a row (trying to avoid sending mail by mistake) the arduino connects to the mail server
and sends an email to a given adress.
If 1 is received the LED diode is turned on just for testing purposes.

It could be improved by instead of using Arduino to send mail (takes around 30 secs) the arduino could connect to a server
and call a function from there, it would be faster and more realible.
