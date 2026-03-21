#include <Arduino.h>
#include <SoundDriver.h>
#include <tetris.h>

PiezoDriver pdriver(18, 1);

hw_timer_t *Timer0 = NULL;
PiezoDriver *ref = &pdriver;

void noteOneShot(const char * note) {
  ref->setNoteViaName(note);
}

void IRAM_ATTR timer0ISR() {
  pdriver.note_counter = (pdriver.note_counter + 1) % pdriver.song_length;
  pdriver.new_note = true;
}

void setup() {
  Serial.begin(115200);
  Serial.print("Starting!");
  Timer0 = timerBegin(0, 80, true);
  pdriver.setSong(Tetoris, Tetoristimes, Tetorislen);
  pdriver.attachTimer(Timer0);
  pdriver.attachISR(timer0ISR);
  pdriver.playCounterNote();
}

void loop() {
  if (pdriver.new_note) {
    pdriver.new_note = false;
    if (pdriver.note_counter >= Tetorislen) {
      Serial.println("OUT OF BOUNDS!");
      while(1);
    }

    pdriver.toneOff();
    pdriver.playCounterNote();
    
    Serial.printf("idx=%d note=%s us=%u\n", pdriver.note_counter, Tetoris[pdriver.note_counter], (unsigned)(uint32_t)(1000000.0f * Tetoristimes[pdriver.note_counter]));
  }
}

