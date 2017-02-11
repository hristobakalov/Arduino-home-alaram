#include <SPI.h>
#include <Ethernet.h>

//unique mac address for the arduino
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x59, 0x67 };
IPAddress ip( 192, 168, 0, 106 ); //local ip, should be unique
IPAddress gateway( 192, 168, 0, 1 ); //get with ipconfig
IPAddress subnet( 255, 255, 255, 0 );

// gmail.com email server - ping aspmx.l.google.com , because it doesn't require login 
IPAddress server( 108, 177, 96, 27 );
int x = 5000;
int y = 20000;

EthernetClient client;

const int buttonPin = 2;
const int ledPin = 7;

int buttonState = 0;
int openedDoorCounter = 0;
byte openedDoor[] = {1,1}; //checks for 2 openings one after another

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Ethernet.begin(mac, ip, gateway, gateway, subnet);
  delay(x);
}

void loop()
{
  buttonState = digitalRead(buttonPin);
  openedDoor[openedDoorCounter] = buttonState;
  openedDoorCounter++;
  openedDoorCounter = openedDoorCounter % 2;
  Serial.println(buttonState);
  if (buttonState == HIGH) {
    //Serial.println("on");
    digitalWrite(ledPin, HIGH);
  }
  else {
    //openedDoorCounter++;
    //Serial.println("off");
    digitalWrite(ledPin, LOW);
    if(openedDoor[0] == 0 && openedDoor[1] == 0){
        while(!sendEmail()){
          Serial.println("Email sent");
          }
      }
    /*if (openedDoorCounter % 2 == 0) {
      openedDoorCounter = 0;
      if (sendEmail()) {
        Serial.println("Email sent");
        delay(10000);
      }
      else {
        Serial.println("Email failed");
        sendEmail();
      }
    }*/
   
  }
  delay(1000);
}

byte sendEmail()
{
  byte thisByte = 0;
  byte respCode;

  if (client.connect(server, 25)) { //25 is the port number
    Serial.println("connected");
  } else {
    Serial.println("connection failed");
    return 0;
  }

  if (!eRcv()) return 0;

  // change this ip to your public ip
  client.write("helo 130.225.244.180\r\n");

  if (!eRcv()) return 0;

  // from email
  client.write("MAIL From: <arduinohomealert@gmail.com>\r\n");

  if (!eRcv()) return 0;

  // to email
  client.write("RCPT To: <hristobak@gmail.com>\r\n");

  if (!eRcv()) return 0;

  client.write("DATA\r\n");

  if (!eRcv()) return 0;

  //change this
  client.write("To: You <hristobak@gmail.com>\r\n");

  // change this
  client.write("From: Me <arduinohomealert@gmail.com>\r\n");

  client.write("Subject: Arduino home alarm\r\n");

  client.write("The front door is opened bitch!\r\n");

  client.write(".\r\n");

  if (!eRcv()) return 0;

  client.write("QUIT\r\n");

  if (!eRcv()) return 0;

  client.stop();
  Serial.println("disconnected");
  return 1;
}

byte eRcv()
{
  byte respCode;
  byte thisByte;

  while (!client.available()) delay(x);

  respCode = client.peek();

  while (client.available())
  {
    thisByte = client.read();
    Serial.write(thisByte);
  }

  if (respCode >= '4')
  {
    efail();
    return 0;
  }

  return 1;
}

void efail()
{
  byte thisByte = 0;

  client.write("QUIT\r\n");

  while (!client.available()) delay(1);

  while (client.available())
  {
    thisByte = client.read();
    Serial.write(thisByte);
  }

  client.stop();
  Serial.println("disconnected");
}
