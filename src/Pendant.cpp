#include <Arduino.h>
#include "Pendant.h"
#include "define.h"
#include "LibBipper.h"
#include "Order.h"

extern Bipper bipper;
extern String HEADER;

namespace Pendant
{
    AXE axeSelect; // valeur actuelle de la selection axe
    SCALE scaleSelect;
    // DIRECTION directionSelect;

    int encoder0PinALast = LOW;
    int encoder0PinANow = LOW;
    int rotation = 0;

    void init()
    {
        pinMode(x_buttonPin, INPUT);
        digitalWrite(x_buttonPin, HIGH); // X
        pinMode(y_buttonPin, INPUT);
        digitalWrite(y_buttonPin, HIGH); // Y
        pinMode(z_buttonPin, INPUT);
        digitalWrite(z_buttonPin, HIGH); // Z
        pinMode(a_buttonPin, INPUT);
        digitalWrite(a_buttonPin, HIGH); // A
        pinMode(X1_buttonPin, INPUT);
        digitalWrite(X1_buttonPin, HIGH); // 1
        pinMode(X10_buttonPin, INPUT);
        digitalWrite(X10_buttonPin, HIGH); // 0.1
        pinMode(X100_buttonPin, INPUT);
        digitalWrite(X100_buttonPin, HIGH); // 0.01
        pinMode(encoder0PinA, INPUT);       // encoder +A
        pinMode(encoder0PinB, INPUT);       // encoder +B
        pinMode(EstopIn, OUTPUT);
        digitalWrite(EstopIn, LOW); // met l'alim arret d'urgence en niveau bas
        pinMode(EstopNC, INPUT);
        digitalWrite(EstopNC, HIGH); // et le nomaly close en �tat haut
        pinMode(LEDpendant, OUTPUT);
        digitalWrite(LEDpendant, LOW); // allume et eteint la led de la commande externe

        attachInterrupt(encoder0PinA, fastRead, CHANGE); // interruption pour lire une rotation de la telecommande
        // attachInterrupt(EstopNC, interruptEstop, RISING);	// Interruption pour lire l'arret d'urgence de la commande externe

        encoder0PinANow = digitalRead(encoder0PinA);  // initialise les valeurs de la pin A au d�marrage
        encoder0PinALast = digitalRead(encoder0PinA); // initialise les valeurs de la pin A au d�marrage
    }

    void fastRead()
    {
        encoder0PinANow = digitalRead(encoder0PinA); // lit la pin A de l'encoder
        if ((encoder0PinALast == LOW) && (encoder0PinANow == HIGH))
        { // si elle � changer d'etat c'est que l'encoder tourne
            if (digitalRead(encoder0PinB) == LOW)
            {
                rotation++;
            } // sens horaire
            else
            {
                rotation--;
            } // sens anti horaire
        }
        encoder0PinALast = encoder0PinANow; // on met � jour la valeur pr�cedente
    }

    void commandReadStats()
    {

        if (rotation == 0)
            return;                  // si pas de rotation on sort de la fonction
        axeSelect = readAxisEnum();  // lecture de l'axe
        if (axeSelect == axeNOTHING) // si pas d'axe choisi on efface le buffer de mouvement
        {
            rotation = 0;
            return;
        }
        scaleSelect = readScaleEnum(); // lecture de l'echelle

        // a partir d'ici il y a eu une action sur la t�l�commande
        bipper.start(10);

        float factorScale;   // valeur temporaire pour l'echelle de d�placement
        switch (scaleSelect) // selectionne l'echelle de d�placement
        {
        case scale1000micro:
            factorScale = 1;
            break;
        case scale100micro:
            factorScale = 0.1;
            break;
        case scale10micro:
            factorScale = 0.01;
            break;
        default:
            break;
        }

        switch (axeSelect)
        {
        case axeX:
            Order::createOrder(axeSelect, factorScale * rotation, HEADER);
            break;

        case axeY:
            Order::createOrder(axeSelect, factorScale * rotation, HEADER);
            break;

        case axeZ:
            Order::createOrder(axeSelect, factorScale * rotation, HEADER);
            break;

        case axeF: // si axe A on modifie la vitesse de deplacement
            Order::F.add(factorScale * rotation * 1000);
            Order::createOrder(axeSelect, Order::F.val);
            break;
        case axeNOTHING:
            break;
        }

        rotation = 0; // remise � zero
    }

    AXE readAxisEnum()
    {
        if (digitalRead(x_buttonPin) == LOW)
        {
            return axeX;
        }
        if (digitalRead(y_buttonPin) == LOW)
        {
            return axeY;
        }
        if (digitalRead(z_buttonPin) == LOW)
        {
            return axeZ;
        }
        if (digitalRead(a_buttonPin) == LOW)
        {
            return axeF;
        }
        return axeNOTHING; // si pas de selection , c'est la position off
    }

    SCALE readScaleEnum()
    {
        if (digitalRead(X1_buttonPin) == LOW)
        {
            return scale1000micro;
        }
        if (digitalRead(X10_buttonPin) == LOW)
        {
            return scale100micro;
        }
        if (digitalRead(X100_buttonPin) == LOW)
        {
            return scale10micro;
        }
        return scaleNOTHING; // en cours de rotation il se peut que le selecteur soit sur rien... on renvoie donc rien
    }

    void interruptEstop() // Interrupt fonction for Estop
    {
        Order::addInListOrder("^X (Arret d'urgence commande)");
    }
}