#include <Arduino.h>
#include "rpLidar.h"
#include "rpLidarTypes.h"

//rpLidar lidar(&Serial2,1000000);
rpLidar lidar(&Serial2,460800);


void setup() {
  Serial.begin(115200);
  
  lidar.resetDevice(); //reset the device to be sure that the status is good
  stDeviceStatus_t sdst = lidar.getDeviceHealth();
  printf("sdst.errorCode_high=%d  sdst.errorCode_low=%d sdst.status=%d\r\n", sdst.errorCode_high, sdst.errorCode_low, sdst.status);
  
  lidar.setAngleOfInterest(5,175); //Set the field of view that is saved to Data
  // bool ret = lidar.start(express); //start the express scan of the lidar
  
  bool ret = lidar.start(standard); //start the express scan of the lidar
  if(ret)
    printf("rplidar C1 started correctly!\r\n");
  else
    printf("error starting rplidar C1\r\n");
  
  //sdst = lidar.getDeviceHealth();
  //printf("sdst.errorCode_high=%d  sdst.errorCode_low=%d sdst.status=%d\r\n", sdst.errorCode_high, sdst.errorCode_low, sdst.status);
  
  // stDeviceInfo_t sdi = lidar.getDeviceInfo();
  // printf("sdi.firmware_major=%d sdi.firmware_minor=%d sdi.hardware=%d sdi.model=%d sdi.serialnumber=%d\r\n", 
  //   sdi.firmware_major, sdi.firmware_minor, sdi.hardware, sdi.model, sdi.serialnumber);

}

void loop()
{
  int count = lidar.readMeasurePoints();// reads a full scan and save it to Data
  printf("\r\ncount=%d\r\n", count);
  if(count == 0){
    //restart the standard scan of the lidar
    lidar.resetDevice();
    bool ret = lidar.start(standard); 
    if(ret)
      printf("rplidar C1 restarted correctly!\r\n");
    else
      printf("error restarting rplidar C1\r\n");
    delay(100); 
  }else {
    for(int i=0;i<count;i++){
      float angle = (lidar.DataBuffer[i].angle_high * 128 + lidar.DataBuffer[i].angle_low / 2) / 64.0;
      int distance = lidar.DataBuffer[i].distance_high * 256 + lidar.DataBuffer[i].distance_low;
      int quality = lidar.DataBuffer[i].quality / 4;

      // if (quality == 0 || distance == 0) {
      //     // Skip invalid data
      //     continue;
      // }

      Serial.print("angle\t");
      Serial.printf("%.1f", angle);
      Serial.print("\tdistance\t");
      Serial.print(distance);
      Serial.print("\tquality\t");
      Serial.println(quality);
    }
      
  }
}