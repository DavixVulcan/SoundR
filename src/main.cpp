#include <Arduino.h>
#include <SoundDriver.h>
#include <tetris.h>

PiezoDriver pdriver(18, 1);

volatile bool newnote = true;
volatile int counter = 0;

hw_timer_t *Timer0 = NULL;
PiezoDriver *ref = &pdriver;

void noteOneShot(const char * note) {
  ref->setNoteViaName(note);
}

void IRAM_ATTR timer0ISR() {
  counter = (counter + 1) % Tetorislen;
  newnote = true;
}

void setup() {
  pdriver.setNoteViaName(Tetoris[counter]);
  Serial.begin(115200);
  Serial.print("Starting!");
  Timer0 = timerBegin(0, 80, true);
  timerAttachInterrupt(Timer0, timer0ISR, true);
  timerAlarmWrite(Timer0, (uint32_t)(1000000.0f * Tetoristimes[counter]), false);
  timerAlarmEnable(Timer0);
}

void loop() {
  if (newnote) {
    newnote = false;


    ref->toneOff();
    if (Tetoris[counter][0] != 'x') {
      ref->setNoteViaName(Tetoris[counter]);
    }
    timerAlarmWrite(Timer0, (uint32_t)(1000000.0f * Tetoristimes[counter]), false);
    timerRestart(Timer0);
    timerAlarmEnable(Timer0);
    
    Serial.printf("idx=%d note=%s us=%u\n", counter, Tetoris[counter], (unsigned)(uint32_t)(1000000.0f * Tetoristimes[counter]));
  }
}

