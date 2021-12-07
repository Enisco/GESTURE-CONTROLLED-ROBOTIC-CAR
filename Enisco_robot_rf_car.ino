
#include <SPI.h>
#include <RH_NRF24.h>
RH_NRF24 nrf24(8, 10);// For Leonardo, need explicit SS pin
/************************************************/
//void forward(), backward(), leftward(), rightward(), turnAround(), haltMotion();

// motor 2 control pins
#define ENB 3
#define IN4 2
#define IN3 4
// motor 1 control pins
#define IN2 5
#define IN1 7
#define ENA 6

int directn;


void setup()
{
  Serial.begin(115200);
  //set output for motor 1 related pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  //set output for motor 2 related pins
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  while (!Serial)
    ; // wait for serial port to connect. Needed for Leonardo only
  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
}

void loop()
{
  int PWM_ValueA = 160; //PWM value for motor 1 and 2
  int PWM_ValueB = 120; //PWM value for motor 1 and 2;
  analogWrite(ENA, PWM_ValueA); //set PWM to Motor 1
  analogWrite(ENB, PWM_ValueB); //set PWM to Motor 2

  if (nrf24.available())
  {
    // Any incomiung signal data
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (nrf24.recv(buf, &len))
    {
      Serial.print("Got message: ");
      Serial.println((char*)buf);

      char* received = (char*)buf;
      directn = atoi(received);
      //      Serial.println(rec);
    }
    else{
      directn = 0;
    }
  }
  //-------------------------------------------------
  switch (directn) {
    case 0:    // your hand is close to the sensor
      Serial.println("Halt");
      haltMotion();
      break;
    case 1:    // your hand is close to the sensor
      Serial.println("Forward");
      forward();
      break;
    case 2:    // your hand is a few inches from the sensor
      Serial.println("Leftward");
      leftward();
      break;
    case 3:    // your hand is nowhere near the sensor
      Serial.println("Backward");
      backward();
      break;
    case 4:    // your hand is a few inches from the sensor
      Serial.println("Rightward");
      rightward();
      break;
      case 5:    // your hand is on the sensor
      Serial.println("Halt");
      haltMotion();
      break;
  }
}


void forward()
{
  // motor 1 run Clockwise
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  // motor 2 run Anticlockwise
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void backward()
{
  // motor 1 run Anticlock wise
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  // motor 2 run Clockwise
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void rightward()
{
  // motor 1 run
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  // motor 2 stop
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void leftward()
{
  // motor 1 stop
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  // motor 2 run
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void haltMotion()
{
  // All motor stops
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
