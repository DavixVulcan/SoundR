#include <Arduino.h>
#include <SoundDriver.h>



const char * Tetoris[64] =
{
  "E6",
  "x",
  "B5",
  "C6",
  "D6",
  "x",
  "C6",
  "B5",
  "A5",
  "x",
  "A5",
  "C6",
  "E6",
  "x",
  "D6",
  "C6",
  "B5",
  "x",
  "B5",
  "C6",
  "D6",
  "x",
  "E6",
  "x",
  "C6",
  "x",
  "A5",
  "x",
  "A5",
  "x",
  "x",
  "x",
  "x",
  "D6",
  "x",
  "F6",
  "A6",
  "x",
  "G6",
  "F6",
  "E6",
  "x",
  "x",
  "C6",
  "E6",
  "x",
  "D6",
  "C6",
  "B5",
  "x",
  "B5",
  "C6",
  "D6",
  "x",
  "E6",
  "x",
  "C6",
  "x",
  "A5",
  "x",
  "A5",
  "x",
  "x",
  "x"
};

PiezoDriver pdriver(16, 1);
char initialnote[] = "C4";
bool valid = false;
hw_timer_t *Timer0 = NULL;
int counter = 0;
PiezoDriver *ref = &pdriver;
void IRAM_ATTR timer0ISR()
{
  const char * note = Tetoris[counter];
  if(note[0] == 'x')
  {
    ref->toneOff();
  }
  else
  {
    ref->setNoteViaName(note);
  }
  counter++;
  counter %= 64;
}
void setup() {
  pdriver.setNote(notelist[4]);
  Serial.begin(115200);
  Timer0 = timerBegin(0, 8000, true);
  timerAttachInterrupt(Timer0, timer0ISR, true);
  timerAlarmWrite(Timer0, 2500, true);
  timerAlarmEnable(Timer0);
}

void loop() {
  valid = false;
  while(Serial.available() == 0){}
  
  String data = Serial.readString();

  if(data == "ton")
  {
    pdriver.toneOn();
    valid = true;
  }
  if(data == "toff")
  {
    pdriver.toneOff();
    valid = true;
  }
  if(data == "on")
  {
    pdriver.piezoOn();
    valid = true;
  }
  if(data == "off")
  {
    pdriver.piezoOff();
    valid = true;
  }
  
  if(valid == false && !pdriver.setNoteViaName(data.c_str()))
    Serial.println("Note not found!");
  Serial.println(data);
}
