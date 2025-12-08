/*
Jurassic Park

Team Name: Usagi Fish
Team Members: Claire Huang, Siddharth Ramalingam, Han Xu

Descritpion:
This program controls 5 interactive scenes in the enclosure, each triggered by its own switch:
1. Haptic motor vibrates to simulate the T-Rex footsteps.
2. Goat body rotates to reveal its death.
3. Dinosaur flips up as a jump scare.
4. Car flips upside down.
5. Characters escape and the ending victory melody plays.
*/

#include <Servo.h>
#include "pitches.h";

// Scene Class - Detect switch state changes to execute corresponding scene actions
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
  
  // Attach switch to specified pin
  void attachPin(int pin){
    switchPin = pin;
    pinMode(switchPin, INPUT);
  }

  // Monitor switch state and trigger action on activation
  void update(int sceneNum, void (*action)()){
    // sceneNum: Scene number (1-5) for which scene is being updated
    // action: Function to execute when switch is activated
    currentState = digitalRead(switchPin);
    
    if (currentState != previousState) {''
      if (currentState == HIGH) {
        // Switch activated - print message and perform the specific action
        Serial.print("switch ");
        Serial.print(sceneNum);
        Serial.println(" activated");
        action();
      } else {
        // Switch released - print message
        Serial.print("switch ");
        Serial.print(sceneNum);
        Serial.println(" released");
      }
    }
    
    previousState = currentState;
  }
};

// Initialize Scenes, Servos, and Pins
Scene scene1, scene2, scene3, scene4, scene5;

Servo goatServo;
Servo dinoServo;
Servo carServo;

const int motorPin = 5;
const int piezoPin = 8;

// Define notes and durations for the Ending Melody
int melody[] = {
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_E4, NOTE_G4
};
int noteDurations[] = {
  8, 8, 8, 4, 8, 2
};

// Initalize serial communication and attach pins 
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

// Update each scene with its corresponding action
void loop() {
  scene1.update(1, vibrateMotor);
  scene2.update(2, rotateGoat);
  scene3.update(3, flipDino);
  scene4.update(4, flipCar);
  scene5.update(5, playEndingMelody);
}

// Scene 1: Haptic Motor Vibration
void vibrateMotor() {
  // Simulate T-Rex footsteps with 3 vibration pulses
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

// Scene 2: Goat Rotation
void rotateGoat(){
  // Rotate goat
  goatServo.write(180);
  Serial.println("Goat rotated");

  // Hold for 5 seconds
  delay(5000);

  // Rest goat position
  goatServo.write(0);
  Serial.println("Goat reset");
}

// Scene 3: Dinosaur Jump Scare
void flipDino(){
  // Flip dinosaur up
  dinoServo.write(180);
  delay(2750);
  Serial.println("Dinosaur Up");

  // Hold for 5 seconds
  dinoServo.write(90);
  delay(5000);

  // Flip dinosaur down
  dinoServo.write(0);
  delay(2750);
  dinoServo.write(90);
  Serial.println("Dinosaur Down");
}

// Scene 4: Car Flip
void flipCar(){
  // Flip car upside down
  carServo.write(-180);
  Serial.println("Car flipped");

  // Hold for 5 seconds
  delay(5000);

  // Reset car position
  carServo.write(180);
  Serial.println("Car reset");
}

// Scene 5: Play Ending Melody
void playEndingMelody() {
  Serial.println("Melody ON");
  for (int thisNote = 0; thisNote < 6; thisNote++) {
    // Play each note for its duration
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(piezoPin, melody[thisNote], noteDuration);

    // Pause between notes
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(piezoPin);
  }
}