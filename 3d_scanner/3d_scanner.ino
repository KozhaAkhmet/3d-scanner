#include <Stepper.h>  // Include the header file
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define STEPS 32
#define ONE_TOUR 2048
#define BUTTON_PIN 2
const int big_teeth_number = 84;  // Teeth number of outer gear
const int small_teeth_number = 12; // Teeth number of inner gear
const int default_ang = 17;
const int shutter_ang = 0;
int analog_y = 0;
int analog_x = 0;
int joystick_button = 0;
int counter = 10; 

LiquidCrystal_I2C lcd(0x27, 16, 2);
Stepper stepper(STEPS, 9, 11, 10, 12);
Servo Servo1;

float big_to_small_ratio = big_teeth_number / small_teeth_number;  // kucugun buyuge orani

int val = 0;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  stepper.setSpeed(500);

  Servo1.attach(3);
  Servo1.write(default_ang);

  lcd.init();
  lcd.backlight();
  // lcd.setCursor(0, 0);
  // lcd.print("Scanner V2");
  // delay(500);
}

void loop() {

  if (Serial.available() > 0) {

    val = Serial.parseInt();
    //
    //    stepper.step(val);
    //
    //    Serial.println(val); //for debugging
    // start_scan(5);
    // move_stepper(ONE_TOUR);
  }

  lcd.setCursor(0, 1);
  lcd.print("Press to start.");
  joystick_control();
}

void start_scan(int shutter_times) {
  Serial.print("Divided into: ");
  Serial.print(shutter_times);
  Serial.print(" steps\n");

  lcd.clear();
  lcd.print("Divided into: ");
  lcd.print(shutter_times);
  lcd.print(" steps\n");

  float outer_gear_one_tour = ONE_TOUR * big_to_small_ratio;
  int partial_step = outer_gear_one_tour / shutter_times;

  Serial.println("Scanning Started");
  
  lcd.setCursor(0,1);
  lcd.print("Scanning Started");
  
  delay(2000);
  for (int i = 0; i < shutter_times; i++) {
    lcd.setCursor(0,1);
    lcd.print("Current step: ");
    lcd.print(i);
    move_stepper(partial_step);
    shutter();
  }
  Serial.println("Scan Complete");
  
  lcd.clear();
  lcd.print("Scan Complete");
}

void move_stepper(int amount) {
  Serial.print("Stepping a bit: ");
  Serial.print(amount);
  Serial.print("\n");
  stepper.step(amount);
}
void shutter() {
  Serial.println("Taking photo...");
  Servo1.write(shutter_ang);
  delay(2000);
  Servo1.write(default_ang);
  delay(1000);
}

void joystick_control(){
  analog_y = analogRead(A0);
  analog_x = analogRead(A1);
  joystick_button = digitalRead(BUTTON_PIN);

  if(analog_y > 800){
    counter++;
    delay(250);
    lcd.clear();
  } else if (analog_y < 300 && counter > 0){
    counter--;
    delay(250);
    lcd.clear();
  }
  if (joystick_button == LOW) {
    start_scan(counter);
  }
  
  lcd.setCursor(0, 0);
  lcd.print("Steps:   ");
  lcd.print(counter);

  // debug_joystick();
}

void debug_joystick() {
  int joystick_button = digitalRead(BUTTON_PIN);
  Serial.println(joystick_button);
  lcd.setCursor(0, 0);
  if (joystick_button == LOW) {
    // turn LED on:
    lcd.print("Pressed");
  } else {
    // turn LED off:
    lcd.print("Nothing");
  }
}
