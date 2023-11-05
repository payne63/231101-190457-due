#include <Arduino.h>
#include "RotaryEncoder.h"
#include "define.h"

namespace RotaryEncoder
{
    boolean turnDetected;
	boolean upDirectionDetected;
	boolean isPush;
	volatile unsigned int encoderPos = 0;  // un compteur
	unsigned int lastReportedPos = 1;   // gestion du changement
	//static boolean rotating = false;      // gestion de l'anti-rebonds
	boolean A_set = false;  // variable pour les routines d'interruption
	boolean B_set = false;
	boolean A_change = false;
	boolean B_change = false;

    void init()
    {
        pinMode(RotaryCLK, INPUT_PULLUP); // rotary encoder CLK
        pinMode(RotaryDT, INPUT_PULLUP);  // rotary encoder DT
        pinMode(RotaryPush, INPUT);
        digitalWrite(RotaryPush, HIGH); // PUSH BUTTON ENCODER

        attachInterrupt(RotaryCLK, interruptRotary, CHANGE); // interruption pour lire la rotation de l'encoder rotatif
        // attachInterrupt(RotaryDT, interruptRotaryB, CHANGE);

        isPush = true; // variable pour supprimer les rebonds
    }

    void interruptRotary()
    {
        if (digitalRead(RotaryCLK))
            upDirectionDetected = !digitalRead(RotaryDT);
        else
            upDirectionDetected = digitalRead(RotaryDT);
        turnDetected = true;
    }

    void interruptRotaryA()
    {
        // if (rotating) delay(1);  // attendre un petit peut
        // rotating = true; //activation de l'anti-rebond
        //  Confirmation du changement
        if (digitalRead(RotaryCLK) != A_set)
        {
            A_set = !A_set;

            if (B_change)
            {
                encoderPos += 1;
                B_change = false;
            }
            else
                A_change = true;

            // rotating = false;  //lib�ration de l'anti-rebond
        }
    }

    void interruptRotaryB()
    {
        // if (rotating) delay(1);
        // rotating = true;
        if (digitalRead(RotaryDT) != B_set)
        {
            B_set = !B_set;

            if (A_change)
            {
                encoderPos -= 1;
                A_change = false;
            }
            else
                B_change = true;

            // rotating = false;
        }
    }

    e_turnVal check()
    {

        if (turnDetected) // do this only if rotation was detected
        {
            turnDetected = false; // do NOT repeat IF loop until new rotation detected
            if (upDirectionDetected)
                return Up; // sens horaire
            else
                return Down; // sens antihoraire
        }
        return NoTurn;
    }

    e_turnVal checkNew()
    {
        if (lastReportedPos != encoderPos)
        {
            if (encoderPos > lastReportedPos)
            {
                lastReportedPos = encoderPos;
                return Up;
            }
            if (encoderPos < lastReportedPos)
            {
                lastReportedPos = encoderPos;
                return Down;
            }
        }
        return NoTurn;
    }

    e_pushVal checkPush()
    {
        int statsPushRotary = digitalRead(RotaryPush);

        if ((statsPushRotary == HIGH) && (isPush == true))
        {
            isPush = false;
        }

        if ((statsPushRotary == LOW) && (isPush == false))
        {
            isPush = true;
            return Push;
        }
        else
            return NoPush;
    }
}