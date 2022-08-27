#include <Servo.h>
#include <SoftwareSerial.h>

// pin Definition
int Tx = 2, Rx = 3;
int motor[2][2] = {{A1, A2}, {A3, A4}};
int pump = 4;

// else
int receive[8];
float deg; 
int dist;
int x, y;
int left, right;

SoftwareSerial bt(Tx, Rx);

void setup() {
  Serial.begin(9600);
  bt.begin(9600);
  pinMode(pump, OUTPUT);
}



// bluetooth통신으로 입력을 받아 receive[8]에 저장한 뒤
// deg에 각도, dist에 거리로 변환해서 저장함
void input(); 

// button을 입력받았을때 행동
void button();

// joystick을 입력받았을 때 행동
void joystick();


void loop() {
  if(bt.available()){
    input();
    button();
    joystick();
  }
}

void input(){
  for(int i = 0; i < 8;){
    receive[i] = bt.read();
    if(receive[i] > 0){receive[i] -= '0'; i++;}
  }
  deg = receive[0] * 100 + receive[1] * 10 + receive[2];
  dist = receive[3] * 100 + receive[4] * 10 + receive[5];
}

void button(){
  if(receive[6] == 0) {
    }
  else if(receive[6] == 1){
    digitalWrite(pump, LOW);
    } // X 버튼
  else if(receive[6] == 2){
    digitalWrite(pump, HIGH);
    } // O 버튼
  else if(receive[6] == 3){
    } // 세모 버튼
  else if(receive[6] == 4){
    } // 네모 버튼
}

int getVelocity(int v){
  if(v <= 0) return 0;
  else return map(v, 0, 142, 80, 255);
}

void getCoordinate(){
  x = dist * cos(radians(deg));
  y = dist * sin(radians(deg));
}

void joystick(){
  getCoordinate();
  left = y + x;
  right = y - x;
  // motor[0] : 왼쪽,  motor[1] : 오른쪽
  
  if(x == 0 && y == 0){ // break
    for(int i =0 ; i < 2; ++i)
      for(int j = 0; j < 2; ++j)
        digitalWrite(motor[i][j], LOW);
  }
  else{ // move
    analogWrite(motor[0][0], getVelocity(left));
    analogWrite(motor[0][1], getVelocity(-left));
    analogWrite(motor[1][0], getVelocity(right));
    analogWrite(motor[1][1], getVelocity(-right));
  }
}
