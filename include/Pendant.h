#ifndef _PENDANT_H
#define _PENDANT_H

//enum MENU{ Menu_1, Menu_2, Menu_3, Menu_4 }; //�num�ration des 4 menus diff�rents
enum AXE{ axeX = 'X', axeY = 'Y', axeZ ='Z', axeF = 'F', axeS = 'S', axeNOTHING };	//enum�ration du type d'axe de la commande
enum SCALE { scale1000micro, scale100micro, scale10micro, scaleNOTHING };
//enum DIRECTION{ dirUp, dirDown, dirNOTHING };

namespace Pendant
{

	
	
	extern AXE axeSelect;						//valeur actuelle de la selection axe
	extern SCALE scaleSelect;
	//DIRECTION directionSelect;

	extern int encoder0PinALast ;
	extern int encoder0PinANow ;

	extern int rotation; //variable qui accumule les rotations de la commande

	void init();
	void fastRead();
	void commandReadStats();
	//void read();
	
	AXE readAxisEnum();
	SCALE readScaleEnum();
	//DIRECTION readEncoderEnum();
	void interruptEstop(); //interruption bouton d'urgence


};

#endif
