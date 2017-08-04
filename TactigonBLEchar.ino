
#include <tactigon_led.h>
#include <tactigon_IMU.h>
#include <tactigon_BLE.h>
#include "IMU_main.h"

T_Led rLed, bLed, gLed;
T_ACC accMeter;
T_AccData accData;

T_QUAT qMeter;
T_QData qData;

T_BLE bleManager;
UUID uuid;
T_BLE_Characteristic bleChar;

int ticks;


void setup() {
  
  ticks = 0;
  rLed.init(T_Led::RED);
  gLed.init(T_Led::GREEN);
  bLed.init(T_Led::BLUE);

  rLed.off();
  gLed.off();
  bLed.off();

  //add acc characteristic
  uuid.set("7ac71000-503d-4920-b000-acc000000001");
  bleChar = bleManager.addNewChar(uuid, 14);

}

void loop() { 
  unsigned char buffData[14];


  //update BLE characteristic @ 50Hz
  if(GetCurrentMilli() >= (ticks +(1000 / 50)))
  {
    ticks = GetCurrentMilli();

    //read sensors
    IMU_loop();
              
    //get acc data
    accData = accMeter.getAxis();             

    //get quaternions data
    qData = qMeter.getQs();                   

    //prepare buffer
    memcpy(&buffData[0], &qData.q0, 2);
    memcpy(&buffData[2], &qData.q1, 2);
    memcpy(&buffData[4], &qData.q2, 2);
    memcpy(&buffData[6], &qData.q3, 2);
    
    memcpy(&buffData[8], &accData.x, 2);
    memcpy(&buffData[10], &accData.y, 2);
    memcpy(&buffData[12], &accData.z, 2);

    //update ble characteristic
    bleChar.update(buffData);
  }
}




