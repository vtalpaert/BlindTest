/*
  BlindTest

  author Victor Talpaert, 2020
*/

#include "pitches.h"


class SevenSeg {
  private:
    uint8_t A_;
    uint8_t B_;
    uint8_t C_;
    uint8_t D_;
  public:
    void begin(uint8_t A, uint8_t B, uint8_t C, uint8_t D) {
      A_ = A;
      B_ = B;
      C_ = C;
      D_ = D;
      pinMode(A_, OUTPUT);
      pinMode(B_, OUTPUT);
      pinMode(C_, OUTPUT);
      pinMode(D_, OUTPUT);
    };
    void display(int var) {
      digitalWrite(D_, HIGH && (var & B00001000));
      digitalWrite(C_, HIGH && (var & B00000100));
      digitalWrite(B_, HIGH && (var & B00000010));
      digitalWrite(A_, HIGH && (var & B00000001));
    };
};


class RGLed { // only two colors RGB led
  private:
    uint8_t r_;
    uint8_t g_;
    uint8_t red_ = 0; // save quantity to avoid hardware call when already at the right color
    uint8_t green_ = 0; // this way repeated calls are not a problem if there is no change behind
    bool is_on_;
  public:
    void begin(uint8_t r, uint8_t g) {
      r_ = r;
      g_ = g;
      pinMode(r_, OUTPUT);
      pinMode(g_, OUTPUT);
    };
    void setColor(uint8_t red, uint8_t green) {
      if (red_ != red && green_ != green) {
        red_ = red;
        green_ = green;
        analogWrite(r_, red);
        analogWrite(g_, green); 
      }
    };
    void pulseColor(uint8_t red, uint8_t green, long ts, float frequency) {
      // ts [ms], frequency [Hz]
      float amplitude = 0.5 + 0.5 * sin(2 * PI * frequency * ts / 1000); // use a modulo if unstable
      setColor(amplitude * red, amplitude * green);
    };
    void off() {
      setColor(0, 0);
    };
    void red() {
      setColor(255, 20);
    };
    void green() {
      setColor(20, 255);
    };
    void yellow() {
      setColor(255, 255);
    };
};


class Relay {
  private:
    uint8_t pin_;
  public:
    void begin(uint8_t pin) {
      pin_ = pin;
      pinMode(pin_, OUTPUT);
      digitalWrite(pin_, HIGH);
    };
    void connectMelody() {
      // connects speakers with MelodyPlayer pins
      digitalWrite(pin_, LOW);
    };
    void connectMusic() {
      // connects speakers with amp
      digitalWrite(pin_, HIGH);
    };
};


class PullUpDigitalButton {
  private:
    uint8_t pin_;
  public:
    void begin(uint8_t pin) {
      pin_ = pin;
      pinMode(pin_, INPUT_PULLUP);
    };
    uint8_t read() {
      // Keep in mind the pull-up means the pushbutton's logic is inverted
      return digitalRead(pin_);
    };
    bool isPressed() {
      return (read() == LOW);
    }
};

class PullUpAnalogButton {
  // you must add the pullup resistor yourself
  // because A6 and A7 are analog read only buttons
  private:
    uint8_t pin_;
  public:
    void begin(uint8_t pin) {
      pin_ = pin;
      pinMode(pin_, INPUT);
    };
    uint8_t read() {
      // Keep in mind the pull-up means the pushbutton's logic is inverted
      return analogRead(pin_);
    };
    bool isPressed() {
      // value close to 0 means button is closed
      return (read() < 128);
    }
};


// Astronomia - Arduino version by Marko Creations
int astronomia[] = {
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5,
  NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5,
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5,
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5,
  NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_F4,
  NOTE_G4, 0, NOTE_G4, NOTE_D5,
  NOTE_C5, 0, NOTE_AS4, 0,
  NOTE_A4, 0, NOTE_A4, NOTE_A4,
  NOTE_C5, 0, NOTE_AS4, NOTE_A4,
  NOTE_G4,0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_G4,0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_G4, 0, NOTE_G4, NOTE_D5,
  NOTE_C5, 0, NOTE_AS4, 0,
  NOTE_A4, 0, NOTE_A4, NOTE_A4,
  NOTE_C5, 0, NOTE_AS4, NOTE_A4,
  NOTE_G4,0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_G4,0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5
};
int astronomiaDurations[] = {
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
  4,4,4,4,
};
int astronomiaLength = 112;

// Victory melody
int melody1[] = {
  NOTE_C4, NOTE_G4, NOTE_C4, NOTE_G4
};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations1[] = {
  6, 4, 6, 2
};
int melodyLength1 = 4;

// Failure melody
int melody2[] = {
  NOTE_C2, NOTE_B1, NOTE_AS1
};
int noteDurations2[] = {
  4, 4, 2
};
int melodyLength2 = 3;

class MelodyPlayer {
  private:
    uint8_t pin_;
    void playMelody_(int melody[], int noteDurations[], int melodyLength) {
      for (int thisNote = 0; thisNote < melodyLength; thisNote++) {
        // to calculate the note duration, take one second divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(pin_, melody[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(pin_);
      };
    };
  public:
    void begin(uint8_t pin) {
      pin_ = pin;
    };
    void playMelody1() {
      playMelody_(melody1, noteDurations1, melodyLength1);
    };
    void playMelody2() {
      playMelody_(melody2, noteDurations2, melodyLength2);
    };
};


volatile int state = 0; // determines all logic

void press1() {
  if (state == 0) {
    state = 1;
  }
}

void press2() {
  if (state == 0) {
    state = 2;
  }
}

class BlindTest {
  public:
    PullUpDigitalButton bigButton1;
    PullUpDigitalButton bigButton2;
    RGLed led1;
    RGLed led2;
    PullUpAnalogButton buttonYes;
    PullUpAnalogButton buttonNo;
    PullUpDigitalButton buttonNext;
    MelodyPlayer player1;
    MelodyPlayer player2;
    Relay relay;
    SevenSeg sevenSeg1;
    SevenSeg sevenSeg2;
    // Jack

    bool isPaused = false;  // permits repeated calls
    float pulseFrequency = 0.5;
    long delay_restart = 1000; // [ms] when the GM press Next instead of Yes or No
    int score1 = 0;
    int score2 = 0;
    void begin() {
      bigButton1.begin(2);
      attachInterrupt(digitalPinToInterrupt(2), press1, FALLING);
      bigButton2.begin(3);
      attachInterrupt(digitalPinToInterrupt(3), press2, FALLING);
      led1.begin(5, 6);
      led2.begin(9, 10);
      buttonYes.begin(A6);
      buttonNo.begin(A7);
      buttonNext.begin(4);
      player1.begin(11);
      player2.begin(12);
      relay.begin(13);
      sevenSeg1.begin(A0, A1, A2, A3);
      sevenSeg2.begin(7, 8, A4, A5);
      // jack on 17
    };
    void pauseMusic() {
      if (not isPaused) {
        isPaused = true;
        relay.connectMelody();
        // jack pause signal
      }
    };
    void playMusic() {
      if (isPaused) {
        isPaused = false;
        relay.connectMusic();
        // send jack replay if paused
      }
    }
    void displayScores() {
      sevenSeg1.display(score1);
      sevenSeg2.display(score2);
    };
    void start() {
      displayScores();
      playMusic();
      state = 0;
    };
    void testMelody() {
      pauseMusic();
      player1.playMelody1();
      player2.playMelody1();
      player1.playMelody2();
      player2.playMelody2();
    }
    void run() {
      long ts = millis();
      switch (state) {
        case 0: // wait for player to press or GM to press NEXT or NO
          led1.pulseColor(255, 30, ts, pulseFrequency);
          led2.pulseColor(240, 30, ts, pulseFrequency);
          if (isPaused and buttonYes.isPressed()) { // only reads button if isPaused necessary
            playMusic();
          }
          if (not isPaused and buttonNo.isPressed()) {
            pauseMusic();
          }
          if (buttonNext.isPressed()) {
            // next music with Jack
          }
          break;
        case 1: // side 1 has the hand
          pauseMusic();
          led2.off();
          led1.pulseColor(240, 150, ts, pulseFrequency); // yellow pulse, but could be yellow full
          if (buttonYes.isPressed()) {
            led1.green();
            player1.playMelody1(); // will delay execution
            score1 += 1;
            // test if max score
            if (score1 > 9) {
              score1 = 0;
              score2 = 0;
            }
            displayScores();
            playMusic();
            state = 0;
            break;
          }
          if (buttonNo.isPressed()) {
            led1.red();
            player1.playMelody2();
            playMusic();
            state = 0;
            break;
          }
          if (buttonNext.isPressed()) {
            led1.yellow();
            led2.yellow();
            delay(delay_restart);
            playMusic();
            state = 0;
            break;
          }
          break;
        case 2: // side 2 has the hand
          pauseMusic();
          led1.off();
          led2.pulseColor(240, 255, ts, pulseFrequency); // yellow pulse, but could be yellow full
          if (buttonYes.isPressed()) {
            led2.green();
            player2.playMelody1(); // will delay execution
            score2 += 1;
            // test if max score
            if (score2 > 9) {
              score2 = 0;
              score1 = 0;
            }
            displayScores();
            playMusic();
            state = 0;
            break;
          }
          if (buttonNo.isPressed()) {
            led2.red();
            player2.playMelody2();
            playMusic();
            state = 0;
            break;
          }
          if (buttonNext.isPressed()) {
            led1.yellow();
            led2.yellow();
            delay(delay_restart);
            playMusic();
            state = 0;
            break;
          }
          break;
      }
    };
};


BlindTest blindTest;

void setup() {
  blindTest.begin();
  blindTest.start();
  //blindTest.testMelody();
  Serial.begin(9600); // do not use serial while power through VIN
}

void loop() {
  Serial.print("Button Yes analog value: ");
  Serial.println(blindTest.buttonYes.read());
  Serial.print("State: ");
  Serial.println(state);
  blindTest.run();
}
