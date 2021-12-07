
#include <SPI.h>
#include <RH_NRF24.h>

#include <Wire.h>
#include <MPU6050.h>
MPU6050 mpu;
RH_NRF24 nrf24(8, 10);// For Leonardo, need explicit SS pin

int directn, currentDirectn;

void setup()
{
  Serial.begin(115200);

  Serial.println("Initialize MPU6050");

  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
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
  // Read normalized values
  Vector normAccel = mpu.readNormalizeAccel();

  // Calculate Pitch & Roll
  int pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis * normAccel.YAxis + normAccel.ZAxis * normAccel.ZAxis)) * 180.0) / M_PI;
  int roll = (atan2(normAccel.YAxis, normAccel.ZAxis) * 180.0) / M_PI;

  //Forward
  if ((pitch >= (-15) && pitch <= 5) && (roll >= (-110) && roll <= -50)) {
    directn = 1;
    uint8_t data[] = "1";
    nrf24.send(data, sizeof(data));
    Serial.print("Sent a message: ");         Serial.println((char*)data);
//    Serial.println("Forward");
  }
  
  //Leftward
  else if ((pitch >= (-10) && pitch <= 20) && (roll >= (-180) && roll <= -120)) {
    directn = 2;
    uint8_t data[] = "2";
    nrf24.send(data, sizeof(data));
    Serial.print("Sent a message: ");         Serial.println((char*)data);
//    Serial.println("Leftward");
  }
  
  //Backward
  else if ((pitch >= 20 && pitch <= 70) && (abs(roll) >= 130 && roll <= 180)) {
    directn = 3;
    uint8_t data[] = "3";
    nrf24.send(data, sizeof(data));
    Serial.print("Sent a message: ");         Serial.println((char*)data);
//    Serial.println("Backward");
  }
  
  //Rightward
  else if ((pitch >= (-30) && pitch <= 10) && (roll >= (-44) && roll <= 5)) {
    directn = 4;
    uint8_t data[] = "4";
    nrf24.send(data, sizeof(data));
    Serial.print("Sent a message: ");         Serial.println((char*)data);
//    Serial.println("Rightward");
  }
  
  //Halt
    else if ((pitch >= (-80) && pitch <= -40) && (roll >= (-180) && roll <= 120)) {
      directn = 5;
      uint8_t data[] = "5";
    nrf24.send(data, sizeof(data));
    Serial.print("Sent a message: ");         Serial.println((char*)data);
//      Serial.println("Halt");
    }
  else {
    directn = currentDirectn;
//    Serial.println("None");
  }

  currentDirectn = directn;
//-------------------------------------------------
  switch (directn) {
    case 5:    // your hand is on the sensor
      Serial.println("Halt");
      break;
    case 1:    // your hand is close to the sensor
      Serial.println("Forward");
      break;
    case 2:    // your hand is a few inches from the sensor
      Serial.println("Leftward");
      break;
    case 3:    // your hand is nowhere near the sensor
      Serial.println( "Backward");
      break;
      case 4:    // your hand is a few inches from the sensor
      Serial.println("Rightward");
      break;
  }
  delay(10);
}
