#ifndef SoundDriver_h
#define SoundDriver_h

#include "Arduino.h"

/**
 * @brief Note structure 
 * @param name a character array holding the current note name
 * @param freq a float representing the note frequency in Hz
 */
typedef struct Note
{
    const char * name;
    float freq;
} Note;

extern Note notelist[48];

/**
 * @class PiezoDriver
 * @brief Drives a Piezo buzzer tied to between a pin and ground
 * 
 */
class PiezoDriver
{
    public:
    /**
    * @param pin The pin meant to drive the Piezo
    * @param channel The channel that the pin is tied to. (Note: pins tied to the same channel will run the same frequency)
    */
    PiezoDriver(int pin, int channel);
    /**
    * @brief Play the note through the currently attached pin
    * @param note 
    * A Note type representing the note to be played.
    * Caches the note for further use
    */
    void setNote(Note note);
    /**
    * @brief Play the note through the currently attached pin
    * @param note 
    * A character array representing the note to be played. Ex: "A5".
    * Caches the note for further use
    * @retval true Returns if the note was successfully found and set
    * @retval false Returns if the note was not successfully found and set
    */
    bool setNoteViaName(const char * note);
    /**
    * @brief Turns off the note currently playing
    */
    void toneOff();
    /**
    * @brief Turns on the currently cached note
    */
    void toneOn();
    /**
    * @brief Completely turns off the Piezo function from the pin
    */
    void piezoOff();
    /**
    * @brief Turns on the Piezo function from the pin, reenabling the currently cached note, if any
    */
    void piezoOn();

    private:
    int _pin;
    int _channel;
    Note _note;
    uint32_t _freq;
    bool _tone_out;
};

#endif