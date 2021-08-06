// libraries
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>
// all globals

// initialize the lcd library with the numbers of the interface pins
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

// keypad intialization
const byte rows = 4;
const byte cols = 3;
char key[rows][cols] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'},
};
byte rowPins[rows] = {9, 8, 7, 6};
byte colPins[cols] = {5, 4, 3};
Keypad keypad = Keypad(makeKeymap(key), rowPins, colPins, rows, cols);

long int password_hash = 1111;

int led = 11;
int counter = 0;
int attempts = 0;
int max_attempts = 3;

// buzzer
int buzzer = 2;

// lock
int lock = 10;
Servo lock_servo;

void default_print() {
  lcd.clear();
  // print enter pin
  lcd.setCursor(0,0);
  lcd.print("Enter PIN:");
}

void open_door() {
  lock_servo.write(90);
  lcd.clear();
  Serial.println("opened the door\n");
  lcd.setCursor(0,0);
  lcd.print("Door will open");
  lcd.setCursor(0, 1);
  lcd.print("Enter");
}

void close_door() {
  lcd.clear();
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("Door will close");
  Serial.println("door closed\n");
  lock_servo.write(0);
  delay(1000);
}
void buzz() {
  tone(2, 440, 50);
}

long int get_pin(void) {
  lcd.setCursor(0, 1);
  char tmp[4] ;
  long int pin_entered;
  for (int i = 0; i < 4; i++) {
    tmp[i] = keypad.waitForKey(); 
    lcd.print("*");
  }
  return atoi(tmp);
}

// start of main
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2); // 16x2 lcd cells
  Serial.begin(9600);
  // init servo
  lock_servo.attach(lock);
  lock_servo.write(0);
  // init buzzer
  pinMode(buzzer, OUTPUT);
}

void wrong_password_entered(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wrong password");
  Serial.println("wrong password\n");
  delay(100);
  buzz();
}
void loop() {
  default_print();
  long int input = get_pin();
  Serial.print(input);
  if (input == password_hash) {
    open_door();
    delay(1000);
    close_door();
  } else {
    wrong_password_entered();
  }
  delay(100);
}
