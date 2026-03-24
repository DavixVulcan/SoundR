#include <Arduino.h>
#include <SoundDriver.h>
#include <tetris.h>

PiezoDriver pdriver(18, 1);
PiezoDriver pdriver2(15, 2);

hw_timer_t *Timer0 = NULL;
hw_timer_t *Timer1 = NULL;
PiezoDriver *ref = &pdriver;
PiezoDriver *ref2 = &pdriver2;

volatile bool tick = false;

void IRAM_ATTR timer0ISR() {
  tick = true;
}

volatile bool tick2 = false;

void IRAM_ATTR timer1ISR() {
  tick2 = true;
}

void setup() {
  Serial.begin(115200);
  Serial.print("Starting!");

  Timer0 = timerBegin(0, 80, true);
  
  ref->setupTimer(Timer0, timer0ISR);
  ref->setSong(Tetoris, Tetoristimes, Tetorislen);
  ref->playCounterNote();

  Timer1 = timerBegin(1, 80, true);
  
  ref2->setupTimer(Timer1, timer1ISR);
  ref2->setSong(chords, chordtimes, chordlen);
  ref2->playCounterNote();
}

void loop() {
  if (tick) {
    tick = false;

    ref->toneOff();
    ref->playCounterNote();
  }
  if (tick2) {
    tick2 = false;

    ref2->toneOff();
    ref2->playCounterNote();
  }
}

