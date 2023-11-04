#ifndef _DEFINE_H
#define _DEFINE_H

#include <Key.h>
#include <SPI.h>
#include <SD.h>

#define VERSION 0.4
#define SerialPC Serial
#define SerialGRBL Serial3


#define GRBL_REQUEST   "?"	// requete GRBL pour connaitre l'etat de GRBL
#define REFRESH_RATE 100 // 40ms c'est 25Hz valeur de grblcontrol // était 200ms //According to GRBL the refresh rate should not be higher than 5 HZ
#define REFRESH_SCREEN 300 // speed for refresh screen
#define maxListItem 25		//max item pour la liste d'ordre
#define maxListDraw 22		// max ligne d'affichage
#define MAX_MOVEMENT_SPEED 5000		//vitesse max des moteurs
#define MAX_SPINDLE_SPEED 18000
#define DEBUG_MODE 1
#define BACKSTRING "BACK    "

//#define FONT_A 4			//Fonte grande taille
//#define FONT_B 1			//Fonte petite taille 6pixels
//#define FONT_C 2			//Fonte moyenne taille



#define x_buttonPin   A7   // the pin for X that the pushbutton is attached to		BROWN // change from 4 to 14 for CS PIN 
#define y_buttonPin   5    // the pin for Y that the pushbutton is attached to		ORANGE
#define z_buttonPin   6    // the pin for Z that the pushbutton is attached to		LIGHT BLUE
#define a_buttonPin   7		// AXE A												BLUE

#define X1_buttonPin   8   // the pin that the pushbutton is attached to			GREEN
#define X10_buttonPin   9    // the pin that the pushbutton is attached to			GRAY
#define X100_buttonPin   10    // the pin that the pushbutton is attached to		BLUE-BLACK

#define encoder0PinA  2		//the pin for rotary encoder		YELLOW
#define encoder0PinB  3		//the pin for rotary encoder		WHITE

#define EstopIn 11			// arret d'urgence alim
#define EstopNC 12			// arret d'urgence normaly close 

#define LEDpendant 13		//led sur boitier de commande

#define MOSI ICSP-4			// SD CARD PIN MOSI
#define MISO ICSP-1			// SD CARD PIN MISO
#define CLK	ICSP-3			// SD CARD PIN CLK
#define CS 4				// SD CARD PIN CS

#define RotaryCLK A9		// the pin for CLK of Rotatif encoder
#define RotaryDT A10		// the pin for DT of Rotatif encoder
#define RotaryPush A11		// the pin for Button push of Rotatif encoder

#define BUZZER A8			//BUZZER PIN FOR SOUND


#define LCD_PINS_RS 30		//screen RS 
#define LCD_PINS_ENABLE 31	//screen ENABLE
#define LCD_PINS_D4 32		//screen D4
#define LCD_PINS_D5 33		//screen D5
#define LCD_PINS_D6 34		//screen D6
#define LCD_PINS_D7 35		//screen D7

#endif