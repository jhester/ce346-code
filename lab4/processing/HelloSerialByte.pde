
import processing.serial.*;

Serial myPort;  // The serial port
float a;

void setup() {
  size(640, 360);
  stroke(255);
  textSize(55);
  a = height/2;
  myPort = new Serial(this, "/dev/tty.usbmodem14603", 9600);
}

void draw() {
  while (myPort.available() > 0) {
    int inByte = myPort.read();
    println(inByte);
    background(51);
    text(inByte, 310, 170); 
  }
}
