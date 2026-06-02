#include <SoftwareSerial.h>
#include <AFMotor.h>

SoftwareSerial bluetooth(9, 10);
AF_DCMotor main_motor(1);
AF_DCMotor rw_motor(4);

String data_recv= "";
bool press[] = {0,0};
bool play = 0;

void setup() {
  pinMode(2, OUTPUT);
  main_motor.setSpeed(255);
  rw_motor.setSpeed(255);

  Serial.begin(9600);
  bluetooth.begin(9600);
}

void updateSerial() {
  if (bluetooth.available()) {
    data_recv = "";
    for (int c = bluetooth.read(); c!='\n' && c!='.'; c = bluetooth.read())
      if (c != -1) data_recv += char(c);
    Serial.println(data_recv);
  }
  // if (Serial.available()) {
  //   data_recv = "";
  //   for (int c = Serial.read(); c!='\n' && c!='.'; c = Serial.read())
  //     if (c != -1) data_recv += char(c);
  //   Serial.println(data_recv);
  // }
}
int getSpeed(String s) {
  int value = 0;
  for (int i = 1; i<s.length(); i++)
    value = value*10+s[i]-48;
  return value*255.0/180;
}
void forward() {
  main_motor.run(FORWARD);
}
void backward() {
  main_motor.run(BACKWARD);
}
void left() {
  rw_motor.run(FORWARD);
}
void right() {
  rw_motor.run(BACKWARD);
}
void stop() {
  main_motor.run(RELEASE);
  rw_motor.run(RELEASE);
}
// int melody[] = {1, 0, 1};
void music() {
  // int n = sizeof(melody)/sizeof(int);
  // tone(2, melody[millis()/500%n]);
  digitalWrite(2, millis()/500%2);
}
void input_manager() {
  if (data_recv == "") return;
  if (data_recv.length() == 1) {
    if (data_recv == "S") stop();
    else if (data_recv == "F") forward();
    else if (data_recv == "G") backward();
    else if (data_recv == "R") right();
    else if (data_recv == "L") left();
    else if (data_recv == "M") play = 1;
    else if (data_recv == "m") digitalWrite(2,0), play = 0;
    else if (data_recv == "X") {
      press[0] = !press[0];
      if (press[0]) digitalWrite(2,1);
      else digitalWrite(2,0);
    }
    // else if (data_recv == "Y") {
    //   press[1] = !press[1];
    //   if (press[1]) tone(2,80);
    //   else noTone(2);
    // }
  }
  else if (data_recv[0] == 'J' || data_recv[0] == 'K') {
    if ((data_recv[0] == 'J')) main_motor.setSpeed(getSpeed(data_recv));
    else if (data_recv[0] == 'K') rw_motor.setSpeed(getSpeed(data_recv));
  }
  data_recv = "";
}

void loop() {
  updateSerial();
  input_manager();
  if (play) music();
}