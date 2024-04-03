
#define R_enable 5 //D5
#define L_enable 6 //D6
#define IN1 0 //B0
#define IN2 1 //B1
#define IN3 2 //B2
#define IN4 3 //B3

uint8_t R_speed = 255;
uint8_t L_speed = 255;

//Functions for the rotation of motors.
void leftForward(const uint8_t &speed){
  analogWrite(L_enable, speed);
  PORTB = PORTB & ~(1 << IN1) | (1 << IN2);
}
void leftBack(const uint8_t &speed){
  analogWrite(L_enable, speed);
  PORTB = PORTB & ~(1 << IN2) | (1 << IN1);
}
void rightForward(const uint8_t &speed){
  analogWrite(R_enable, speed);
  PORTB = PORTB & ~(1 << IN4) | (1 << IN3);
}
void rightBack(const uint8_t &speed){
  analogWrite(R_enable, speed);
  PORTB = PORTB & ~(1 << IN3) | (1 << IN4);
}

//Functions for directional movement of the vehicle.
void release(){
  PORTB = PORTB & ~(1 << IN1) & ~(1 << IN2) & ~(1 << IN3) & ~(1 << IN4);
}
void forward(const uint8_t &_L_speed, const uint8_t &_R_speed){
  leftForward(_L_speed);
  rightForward(_R_speed);
}
void back(const uint8_t &_L_speed, const uint8_t &_R_speed){
  leftBack(_L_speed);
  rightBack(_R_speed);
}
void turnRight(){
  leftForward(L_speed);
  rightBack(R_speed);
}
void turnLeft(){
  leftBack(L_speed);
  rightForward(R_speed);
}

void setup() {
  DDRB |= (1 << IN1) | (1 << IN2) | (1 << IN3) | (1 << IN4); //set output
  PORTB = PORTB & ~(1 << IN1) & ~(1 << IN2) & ~(1 << IN3) & ~(1 << IN4);

  DDRD |= (1 << L_enable) | (1 << R_enable); //set output

  Serial.begin(9600);
  
}
void loop() {
  if(Serial.available()){ //if there is serial communicate.

    char data = Serial.read();   //get the data from bt.

    if(data == 'F'){
      forward(L_speed, R_speed); //movement in the forward direction.
    }
    else if(data == 'R'){
      turnRight();              //movement in the full-right direction without forward or back.
    }
    else if(data == 'L'){
      turnLeft();               //movement in the full-left direction without forward or back.
    }
    else if(data == 'B'){
      back(L_speed, R_speed);   //movement in the back direction.
    }
    else if(data == 'S'){   //for release
      release();                
    }
    else if(data == 'I'){
      forward(L_speed, R_speed/5);  //movement in the right direction while vehicle moveing in the forward direction.
    }
    else if(data == 'G'){
      forward(L_speed/5, R_speed);  //movement in the left direction while vehicle moveing in the forward direction.
    }
    else if(data == 'H'){
      back(L_speed/5, R_speed);     //movement in the left direction while vehicle moveing in the back direction.
    }
    else if(data == 'J'){
      back(L_speed, R_speed/5);     //movement in the right direction while vehicle moveing in the back direction.
    }
    else if(data == 'q'){           //set the full speed.
      R_speed = 255;
      L_speed = R_speed;
    }
    else if(data == '0'){           //set the speed to 0.
      R_speed = 0;
      L_speed = R_speed;
    }
    else if(data >=49 && data <=57){       //speed adjustment.
      R_speed = map(data, 49, 57, 100, 254);
      L_speed = R_speed;
    }
  }
}