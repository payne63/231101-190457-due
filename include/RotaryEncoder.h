#ifndef _ROTARYENCODER_
#define _ROTARYENCODER_

#include <Arduino.h>


enum e_turnVal
{
    NoTurn,
    Up,
    Down
};
enum e_pushVal
{
    NoPush,
    Push
};

namespace RotaryEncoder
{

    extern boolean turnDetected;
    extern boolean upDirectionDetected;
    extern boolean isPush;
    extern volatile unsigned int encoderPos; // un compteur
    extern unsigned int lastReportedPos;     // gestion du changement
    //extern static boolean rotating;          // gestion de l'anti-rebonds
    extern boolean A_set;                    // variable pour les routines d'interruption
    extern boolean B_set;
    extern boolean A_change;
    extern boolean B_change;

    void init();
    // void fastRead();
    void interruptRotary();
    void interruptRotaryA();
    void interruptRotaryB();
    e_turnVal check();
    e_turnVal checkNew();
    e_pushVal checkPush();

};
#endif