#include <Arduino.h>
#include "Order.h"
#include "Pendant.h"
#include "LibData.h"
#include "define.h"
#include "main.h"

namespace Order
{

    LinkedList<String> drawList = LinkedList<String>(); // Liste qui s'affiche

    int actualOrderCreate = -1; // demarre � -1 pour que le 1er ordre soit � 0
    int actualOrderSend = 0;
    int actualOrderOk = -1;

    GRBLSTATUS status;
    Data F;
    Data S;
    String Xs;
    String Ys;
    String Zs;
    String Fs;
    String Ss;

    void init()
    {
        // orderList.clear();		//clear the list for security
        drawList.clear(); // clear the list for security

        status = Idle;
        // X = Data("X", -1000, 1000, 0);					//initialise les bornes de cette variable
        // Y = Data("Y", -1000, 1000, 0);					//initialise les bornes de cette variable
        // Z = Data("Z", -1000, 1000, 0);					//initialise les bornes de cette variable
        F = Data("F", 0, MAX_MOVEMENT_SPEED, 3000); // initialise les bornes de cette variable
        // S = Data("S", 0, MAX_SPINDLE_SPEED, 9000);		//initialise les bornes de cette variable

        Xs = "000.00";
        Ys = "000.00";
        Zs = "000.00";
        Ss = "0000";
        Fs = "0000";
    }

    void createOrder(AXE _axe, float _val, String _header)
    {
        
        if (_axe == axeNOTHING)
            return; // si pas d'axe on sort

        String newOrder = _header + char(_axe) + _val;
        if (Order::addInListOrder(newOrder) == true) // si l'ordre est bien rajouter on met � jour les valeurs.
        {
            switch (_axe)
            {
            case axeX:
                // Order::X.add(_val);
                break;
            case axeY:
                // Order::Y.add(_val);
                break;
            case axeZ:
                // Order::Z.add(_val);
                break;
            case axeF:
                // Order::F.add(_val);
                break;
            case axeS:
                Order::S.add(_val);
                break;
            case axeNOTHING:
                break;
            default:
                break;
            }
        }
    }

    void checkOrderToSend()
    {
        if (drawList.size() != 0)
        {
            String h = drawList.pop();
            sendToGrbl(h + "\r");
        }
    }

    // ajoute un ordre � la liste
    bool addInListOrder(String orderString)
    {

        // bipper.start(10);
        return drawList.add(0, orderString); // add to the list to send
        // serialDrawAdd(orderString);	// add the string for draw it
    }

    Message::Message(DIRECTIONORDER _directionOrder, String _text)
    {
        directionOrder = _directionOrder;
        text = _text;
        status = WAITING;
        num = 0;
    }

    Message::~Message()
    {
    }

    void Message::init()
    {
        totalnum = 0;
    }

}