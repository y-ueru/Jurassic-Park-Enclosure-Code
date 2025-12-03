/*
Jurassic Park Enclosure

Course: AET 313 Code and Fabrication
Team: Usagi Fish
Members: Claire Huang, Siddharth Ramalingam, Han Xu

This program controls 5 interactive scenes in the enclosure, each triggered by its own switch:
1. The switch triggers haptic motor vibration.
2. The goat head rotates to reveal its death.
3. The dinosaur flips up as a jump scare.
4. The car flips upside down.
5. The car lands in a tree and plays the ending melody.
*/

/*
Revising:

1. Change from delay() to millis()
2. 
*/

#include <Servo.h>
#include "pitches.h";

class Scene{
  private:
    int switchPin;
    int currentState;
    int previousState;
  
  public:
    Scene() {
      switchPin = -1;
      currentState = 0;
      previousState = 0;
    }
  
  void attachPin(int pin){
    switchPin = pin;
    pinMode(switchPin, INPUT);
  }

  void update(int sceneNum, void (*onPress)(), void (*onRelease)() = nullptr){
    currentState = digitalRead(switchPin);
    if (currentState != previousState) {
      if (currentState == HIGH) {
        Serial.print("switch ");
        Serial.print(sceneNum);
        Serial.println(" activated");
        onPress();
      } else {
        Serial.print("switch ");
        Serial.print(sceneNum);
        Serial.println(" released");
        onRelease();
      }
    }
    previousState = currentState;
  }
};

// Create Scene objects
Scene scene1;
Scene scene2;
Scene scene3;
Scene scene4;
Scene scene5;

Servo goatServo;
Servo dinoServo;
Servo carServo;

const int motorPin = 5;
const int piezoPin = 8;

int melody[] = {
  NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_B4, NOTE_G4, NOTE_A4, NOTE_F4, NOTE_G4, 0, NOTE_C5
};
int noteDurations[] = {
  4, 4, 4, 2, 4, 4, 4, 4, 2, 4, 1
};


void setup() {
  Serial.begin(9600);
  Serial.println("Start Testing");

  scene1.attachPin(9);
  scene2.attachPin(10);
  scene3.attachPin(11);
  scene4.attachPin(3);
  scene5.attachPin(6);

  goatServo.attach(A5);
  dinoServo.attach(A4);
  carServo.attach(A3);

  pinMode(motorPin, OUTPUT);
}

void loop() {
  scene1.update(1, vibrateMotor);
  scene2.update(2, rotateGoat, resetGoat);
  scene3.update(3, flipDino);
  scene4.update(4, flipCar, resetCar);
  scene5.update(5, playEndingMelody);
}

// Scene 1: Haptic Motor Vibration
void vibrateMotor() {
  digitalWrite(motorPin, HIGH);
  delay(300);
  digitalWrite(motorPin, LOW);
  delay(1200);
  digitalWrite(motorPin, HIGH);
  delay(300);
  digitalWrite(motorPin, LOW);
  delay(1200);
  digitalWrite(motorPin, HIGH);
  delay(300);
  digitalWrite(motorPin, LOW);
  delay(1800);
}

// Scene 2: Rotate Goat Head
void rotateGoat(){
  goatServo.write(180);
  Serial.println("Goat rotated");
}

// Scene 2: Reset
void resetGoat(){
  delay(5000);
  goatServo.write(0);
  Serial.println("Goat reset");
}

// Scene 3: Dinosaur Movement
// A 360 degree Servo is used here
void flipDino(){
  dinoServo.write(180);
  delay(2750);
  dinoServo.write(90);
  delay(5000);
  Serial.print("Dinosaur Up");
  dinoServo.write(0);s
  delay(2750);
  dinoServo.write(90);
  Serial.println("Dinosaur Down");
}

// Scene 4: Car Flip
void flipCar(){
  carServo.write(-180);
  Serial.println("Car flipped");
}

// Scene 4: Reset 
void resetCar(){
  delay(5000);
  carServo.write(180);
  Serial.println("Car reset");
}

// Scene 5: Play Ending Melody
void playEndingMelody() {
  Serial.println("Melody ON");
  for (int thisNote = 0; thisNote < 11; thisNote++) {
    Serial.println(melody[thisNote]);
    // Play each note for its duration
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(piezoPin, melody[thisNote], noteDuration);

    // Pause between notes
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(piezoPin);
  }
}