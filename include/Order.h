#ifndef _ORDER_H
#define _ORDER_H

#include "LibData.h"
#include "Pendant.h"
#include <LinkedList.h>

#define MAX_ARRAY 1000;
enum STATUSORDER
{
    WAITING,
    SEND,
    OK,
    ERROR
};
enum DIRECTIONORDER
{
    SENDING,
    RECEIVING
};
enum GRBLSTATUS
{
    Idle,
    Run,
    Hold,
    Door,
    Home,
    Alarm,
    Check,
    Unkno
};

namespace Order
{

    // LinkedList<String> orderList = LinkedList<String>(); //liste d'ordre � envoy� a GRBL
    extern LinkedList<String> drawList; // Liste qui s'affiche

    extern int actualOrderCreate ; 
    extern int actualOrderSend;
    extern int actualOrderOk ;

    extern GRBLSTATUS status;
    extern Data F;
    extern Data S;

    extern String Xs;
    extern String Ys;
    extern String Zs;
    extern String Fs;
    extern String Ss;

    void init();
    void createOrder(AXE, float, String = "");
    void createOrder(String);
    void checkOrderToSend();
    bool addInListOrder(String);

    class Message
    {
    public:
        DIRECTIONORDER directionOrder;
        String text;
        STATUSORDER status;
        int num;
        static int totalnum;

        Message(DIRECTIONORDER, String);
        ~Message();
        static void init();

    private:
    };

};

#endif