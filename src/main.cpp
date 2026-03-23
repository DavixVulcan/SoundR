#include <Arduino.h>
#include <SoundDriver.h>
#include <tetris.h>

PiezoDriver pdriver(18, 1);

hw_timer_t *Timer0 = NULL;
PiezoDriver *ref = &pdriver;

volatile bool tick = false;

void IRAM_ATTR timer0ISR() {
  tick = true;
}

void setup() {
  Serial.begin(115200);
  Serial.print("Starting!");

  Timer0 = timerBegin(0, 80, true);
  timerAttachInterrupt(Timer0, timer0ISR, true);
  

  ref->attachTimer(Timer0);
  ref->setSong(Tetoris, Tetoristimes, Tetorislen);
  ref->playCounterNote();
}

void loop() {
  // Serial.println("looping!Q");
  if (tick) {
    tick = false;

    ref->toneOff();
    ref->playCounterNote();
    
    // Serial.printf("idx=%d note=%s us=%u\n", ref->note_counter, Tetoris[ref->note_counter], (unsigned)(uint32_t)(1000000.0f * Tetoristimes[ref->note_counter]));
  }
}

