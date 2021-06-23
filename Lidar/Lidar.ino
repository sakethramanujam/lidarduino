#include<Servo.h>
#include<LIDARLite.h>
Servo X;
Servo Y;
LIDARLite lidar;


// Motor Setup
// Motor Min Max Angle Definitions
int xmin=30;
int xmax=90;
int ymin=26;
int ymax=86;
// Motor Step
int increment=1;
// Keeping Track of previous position
int prevx = 0;
int prevy = 0;
int xpos = (xmin + xmax)/2;
int ypos = (ymin + ymax)/2;

boolean scanx = false;
boolean scany = false;
// Lidar Loop Setups
int counter = 0;
int radius = 0;
int lastradius = 0;


// Math setup
float pi = 3.14159265;
float deg2rad = pi/180.0;



void setup() {
  lidar.begin(0, true); //initiate lidar
  lidar.configure(0); // configure lidar in mode 0
  X.attach(2);
  Y.attach(3);
  X.write(xpos);
  Y.write(ypos);
  Serial.begin(115200);
}

void loop() {
  //scanning
  if(scany){
    ypos +=increment;
  }else{
    ypos -= increment;
  }
  if(ypos>ymax || ypos<ymin){
    scany = !scany;
      if(scanx){
        xpos +=increment;
      }else{
        xpos -= increment;
      }
       if(xpos>xmax || xpos<xmin){
        scanx = !scanx;
       }
}
      
  xpos = min(max(xpos, xmin), xmax);
  ypos = min(max(ypos, ymin), ymax);
  bool moved = moveServos();
  counter+=1;
    if(counter%100 == 0){
      radius = lidar.distance();
    } else{
      radius = lidar.distance(false);
    }
    if(abs(radius-lastradius) > 2){
      lastradius = radius;
    }
    if(moved){
      float azimuth = xpos * deg2rad;
      float elevation = (180 - ymax + ypos) * deg2rad;
      double x = radius * sin(elevation) * cos(azimuth);
      double y = radius * sin(elevation) * sin(azimuth);
      double z = radius * cos(elevation);
      Serial.println(String(-x, 5) + " " + String(y, 5) + " " + String(-z, 5));
    }  
 }

bool moveServos()
{
  bool moved = false;
  static int lastPosX;
  static int lastPosY;
  int delta = 0;  
  if (xpos != prevx) {
    delta += abs(xpos - prevx);
    X.write(xpos);
    prevx = xpos;
    moved = true;
  }
  if (ypos!= prevy) {
    delta += abs(ypos- prevy);
    Y.write(ypos);
    prevy = ypos;
    moved = true;
  }
  delay(15);
  return moved;
}
