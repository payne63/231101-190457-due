

#include <Keypad.h>
#include <Key.h>
#include <SPI.h>
#include <SD.h>
#include <LinkedList.h>	//Library pour les liste chainés
#include <LiquidCrystalDue.h> //Library for liquidCrystal screen
#include "Order.h"
#include "Pendant.h"
#include "LibBipper.h"		//Library for Bipper Sound
#include "LibSpindle.h"		//Library for Spindle Managing
#include "LibKeyboard.h"	//Library for Keyboard Managing
#include "RotaryEncoder.h"
#include "define.h"
#include "main.h"

enum MENUSELECTED { MenuCoordinate, MenuSerial, MenuSD };
MENUSELECTED menuSelected;			// variable su menu en cours
String serialDraw[4] = { "", "", "", "" }; //tableau d'affichage du serial



void sendToGrbl(String data)	//fonction d'envoie des données à GRBL
{
	SerialGRBL.print(data);
	serialDrawAdd(data.substring(0,data.length()-1)); //le substring supprime un caractere fantome!!

#if DEBUG_MODE
	if (DEBUG_MODE)  SerialPC.print(data);
#endif
	
}


String resizeForScreen(String _text, unsigned int _size) // réduit la chaine pour passer sur l'écran
{
	if (_text.length() > _size)
	{
		return _text.substring(0, _size);
	}
	return _text;
}

void serialDrawAdd(String _data)	// affichage des commandes
{		
	_data = resizeForScreen(_data, 19);
	serialDraw[3] = serialDraw[2];
	serialDraw[2] = serialDraw[1];
	serialDraw[1] = serialDraw[0];
	serialDraw[0] = _data;
}

void initBasicSetupGRBL()	//initialise les commandes de base pour que GRBL fonctionne correctement avec le pendant
{
	sendToGrbl("G91\r"); //force le G91 au demarage (mode déplacement incrémentale)
	sendToGrbl("$10=2\r"); //force l'affichage du buffer complet de GRBL (nécessaire pour le retour d'info)
	//sendToGrbl("$110 = 5000");	//vitesse max axe
	//sendToGrbl("$111 = 5000");	//vitesse max axe
	//sendToGrbl("$112 = 5000");	//vitesse max axe
}

LiquidCrystalDue  lcd(	LCD_PINS_RS, LCD_PINS_ENABLE,
						LCD_PINS_D4, LCD_PINS_D5, 
						LCD_PINS_D6, LCD_PINS_D7);

boolean enterSdMenu = false;
boolean SDINIT = false;		// information si la carte SD est deja initialisée.
File root;	// chemin du fichier

String HEADER = "G01";			//Gcode de chaque ordre de mouvement

boolean stringComplete = false;  // whether the string is complete, when reading the serial port
String line = "";

uint32_t timerrefresh = millis(); // timer for the actions
uint32_t timerrefreshScreen = millis(); // timer for the drawing

Bipper bipper(BUZZER);		//instance of bipper
Spindle spindle(18000);		//instance of spindle

Keyboard keyboard;			//instance of keypad
//Encoder encoder(RotaryCLK, RotaryDT, RotaryPush);

// SD STUFF
typedef struct {
	bool exist;
	String text;
	bool isDirectory;
}structDataSD;

const int maxDataSD = 50;	//max éléments
int actualSizeDataSD = 0;	//nombre d'élements dans la tableau
structDataSD dataSD[maxDataSD]; //tableau des fichiers actuellement lu

File localRoot;
String nameRoot = "/";
int positionSelectSD = 0;

void upPositionSelectSD() {
	if (positionSelectSD > 0) positionSelectSD--;
}
void downPositionSelectSD() {
	if (positionSelectSD <actualSizeDataSD - 1) positionSelectSD++;
}
void pushPositionSelectSD(){
	if (SDINIT == true)	// si la carte SD est initialisé
	{
		String localName = dataSD[positionSelectSD].text;
		if (localName == BACKSTRING) //si la ligne selectionner est le retour en arrière
		{
			if (localName == "/") return;	// c'est le root de la carte SD
			localRoot.close();
			int lastPosStringcut = nameRoot.lastIndexOf('/', nameRoot.length()-2);
			nameRoot = nameRoot.substring(0, lastPosStringcut+1);
			SerialPC.println(lastPosStringcut);
			SerialPC.println(nameRoot);
			localRoot = SD.open(nameRoot);
			positionSelectSD = 0; //remet le curseur de selection en haut
			directoryData(localRoot);
			return;
		}
		localRoot.close();
		nameRoot = nameRoot + localName;
		localRoot = SD.open(nameRoot);

		if (localRoot.isDirectory()) //si le document selectionner est un répertoire
		{
			positionSelectSD = 0; //remet le curseur de selection en haut
			directoryData(localRoot);
			//localRoot = SD.open(localRoot.name()+ localName);
			SerialPC.println("localName:" + localName);
			SerialPC.println("nameRoot: "+ nameRoot);
		}
		else  //dans ce cas c'est un fichier à ouvrire
		{
			SerialPC.println("c'est un fichier");
			while (localRoot.available()) {
				SerialPC.write(localRoot.read());
			}
			// close the file:
			localRoot.close();
		}
	}
	
}


//------------------------------------------------- SETUP



void setup() {
	
	Order::init();
	RotaryEncoder::init();				//initialisation du bouton rotatif
	Pendant::init();					//initialisation de la télécomande
	//Encoder::init(RotaryCLK, RotaryDT, RotaryPush);

#if DEBUG_MODE
	SerialPC.begin(115200);	//initalisation du serial avec le PC
#endif

	SerialGRBL.begin(115200);	//initalisation du serial avec GRBL

	//caractere menu Coordonnées
	byte dotChar[8] = {		
		B11111,
		B10001,
		B10101,
		B10101,
		B10101,
		B10101,
		B10001,
		B11111
	};
	//caractere menu Serial
	byte crossChar[8] = {	
		B01000,
		B11100,
		B01000,
		B01000,
		B01000,
		B01010,
		B01111,
		B00010
	};	
	//caractere menu SD
	byte SDChar[8] = {		
		B00111,
		B01001,
		B10001,
		B10101,
		B10101,
		B10101,
		B10001,
		B11111
	};		
	//caractere de selection
	byte selectChar[8] = {
		B10000,
		B11000,
		B11100,
		B11110,
		B11110,
		B11100,
		B11000,
		B10000
	};	
	//caractere de retour arrière
	byte backChar[8] = {
		B00000,
		B00001,
		B00001,
		B00101,
		B01101,
		B11111,
		B01101,
		B00100
	};

	lcd.begin(20, 4);
	lcd.createChar(0, dotChar);
	lcd.createChar(1, crossChar);
	lcd.createChar(2, SDChar);
	lcd.createChar(3, selectChar);
	lcd.createChar(4, backChar);

	menuSelected = MENUSELECTED::MenuCoordinate;   //selection menu de départ
	
	clearDirectoryData(); //efface par securiter le tableau Directorydata

	delay(2000); // temporisation pour être sur que la carte Arduino avec GRBL à eu le temps de booter

	initBasicSetupGRBL();
}
//------------------------------------------------------MAIN


void loop() {

	bipper.update();				//update time of bipper sound
	Order::checkOrderToSend();		// regarde la liste des ordres à envoyé

	readSerialGRBL();


	if (millis() - timerrefresh > REFRESH_RATE)		// a chaque refresh
	{
		timerrefresh = millis();					// update variable time for refresh
		Pendant::commandReadStats();				// traite les infos de la commande
		managementKeyboard(keyboard.check());		// traite les infos du clavier
		managementEncoder();						// Traite les infos de l'encodeur rotatif
	}

	if (millis() - timerrefreshScreen > REFRESH_SCREEN)		// a chaque refresh pour l'affichage
	{
		requestPosition();
		timerrefreshScreen = millis();
		
		lcd.clear();
		switch (menuSelected)
		{
		case MENUSELECTED::MenuCoordinate:
			lcd.setCursor(0, 0); lcd.write(byte(0)); //lcd.print("___DIRECT_COMMAND___");
			lcd.setCursor(1, 0); lcd.print(getStatus(Order::status));
			lcd.setCursor(0, 1); lcd.print("X:");
			lcd.setCursor(8- Order::Xs.length(), 1); lcd.print(Order::Xs);
			lcd.setCursor(0, 2); lcd.print("Y:");
			lcd.setCursor(8 - Order::Ys.length(), 2); lcd.print(Order::Ys);
			lcd.setCursor(0, 3); lcd.print("Z:");
			lcd.setCursor(8 - Order::Zs.length(), 3); lcd.print(Order::Zs);
			
			lcd.setCursor(16, 0); lcd.print(":Fc");
			lcd.setCursor(16 - String(int(Order::F.val)).length(), 0); lcd.print(int(Order::F.val));
			lcd.setCursor(16, 1); lcd.print(":Fs");
			lcd.setCursor(16 - Order::Fs.length(), 1); lcd.print(Order::Fs);

			lcd.setCursor(16, 2); lcd.print(":Sc");
			lcd.setCursor(16 - String(spindle.getConsigneSpeed()).length(), 2); lcd.print(spindle.getConsigneSpeed());
			lcd.setCursor(16, 3); lcd.print(":Ss");
			lcd.setCursor(16 - Order::Ss.length(), 3); lcd.print(Order::Ss);

			break;
		case MENUSELECTED::MenuSerial:
			lcd.setCursor(0, 0); lcd.write(byte(1));// lcd.print("SERIAL_COMMUNICATION");

			for (int m = 0; m <4; m++)
			{
				lcd.setCursor(2, m); lcd.print(resizeForScreen(serialDraw[m], 18)); //affiche les dernière lignes du serial
			}
			break;

		case MENUSELECTED::MenuSD:
			lcd.setCursor(0, 0); lcd.write(byte(2));	//afiche l'icone SDcard Menu
			lcd.print(resizeForScreen(nameRoot,19));
			lcd.setCursor(1, 1); lcd.write(byte(3));	//affiche la fleche de selection
			
			if (enterSdMenu)
			{
				positionSelectSD = 0;
				enterSdMenu = false;
				initSDbasic();
				nameRoot ="/";
				localRoot = SD.open(nameRoot);
				directoryData(localRoot);
			}
			for (int m = 0; m <3; m++)
			{
				if (dataSD[m+ positionSelectSD].exist)
				{
					String localName = dataSD[m + positionSelectSD].text;
					if (localName == BACKSTRING)
					{
						lcd.setCursor(2, m + 1); lcd.write(byte(4)); lcd.print(localName);
					}
					else
					{
						lcd.setCursor(2, m + 1); lcd.print(localName);
					}
				}
					
			}
			
			break;

		default:
			break;
		}

	}

}
//-------------------------------------------------------------



void requestPosition() {		//Request position to GRBL
	//sendToGrbl("?\r");
	SerialGRBL.print("?\r");
}

String cuteGRBLstring(String _grblString, char _separator,int _numSeparator, bool _before)
{
	if (_grblString.length() == 0) return "no data string";
	if (_separator == (char) 0) return "no char separator";
	if (_numSeparator < 1) return "_numseparator must be >1";
	int found = 0;
	for (unsigned int i = 0; i < _grblString.length(); i++)
	{
		if (_grblString.charAt(i) == _separator)
		{
			found++;
			if (found == _numSeparator)
			{
				if (_before == true)
				{
					return _grblString.substring(0, i);
				}
				else
				{
					//if (i + 1 > _grblString.length()) return "endingstring";
					return _grblString.substring(i+1, _grblString.length());
				}
			}
		}
	}
	return "separator not found";
}

void parse_and_store() {	//decode les valeurs de retour de GRBL
							/*  what the line from GRBL will look like -> grbl 1.0
							<Idle|WPos:1.000,287.408,-2.152|FS:0,0|WCO:-142.396,-287.408,2.152>
							*/
	line.replace('<', '|');
	line.replace('>', '|');
	int champ = -1;
	for ( unsigned int j = 0; j < line.length(); j++)
	{
		if (line.charAt(j) == '|') champ++;
	}

	String valStatus;
	String valWpos;
	String valBf;
	String valFS;
	String valWCO;
	String valOv;
	for (int k = 1; k <= champ; k++)
	{
		String localString = cuteGRBLstring(cuteGRBLstring(line, '|', k+1, true), '|', k, false);
		//SerialPC.println(localString);
		if (k == 1) valStatus = localString;	// if it's the first, it's the status of GRBL
		else 
		{
			if (localString.startsWith("WPos"))	valWpos = localString;
			else if (localString.startsWith("Bf")) valBf = localString;
			else if (localString.startsWith("FS")) valFS = localString;
			else if (localString.startsWith("WCO")) valWCO = localString;
			else if (localString.startsWith("Ov")) valOv = localString;				
		}
	
	}
	

	String _valX = cuteGRBLstring(cuteGRBLstring(valWpos, ',', 1, true), ':',1,false);
	String _valY = cuteGRBLstring(cuteGRBLstring(valWpos, ',', 2, true), ',', 1, false);
	String _valZ = cuteGRBLstring(valWpos, ',', 2, false);
	String _valF = cuteGRBLstring(cuteGRBLstring(valFS, ',', 1, true),':',1,false);
	String _valS = cuteGRBLstring(valFS, ',', 1, false);

	//SerialPC.println(valStatus);
	//SerialPC.println(_valX);
	//SerialPC.println(_valY);
	//SerialPC.println(_valZ);

	Order::status = findStatus(valStatus);
	Order::Xs = _valX.substring(0,_valX.length()-1);
	Order::Ys = _valY.substring(0, _valY.length() - 1);
	Order::Zs = _valZ.substring(0, _valZ.length() - 1);
	Order::Fs = _valF;
	Order::Ss = _valS;
	
}

GRBLSTATUS findStatus(String _val)
{
	if (_val == "Idle") return Idle;
	else if (_val == "Run") return Run;
	else if (_val == "Hold") return Hold;
	else if (_val == "Door") return Door;
	else if (_val == "Home") return Home;
	else if (_val == "Alarm") return Alarm;
	else if (_val == "Check") return Check;
	else return Unkno;
}

String getStatus(GRBLSTATUS _val)
{
	if (_val == Idle )return "Idle";
	else if (_val == Run) return "Run";
	else if (_val == Hold) return "Hold";
	else if (_val == Door) return "Door";
	else if (_val == Home) return "Home";
	else if (_val == Alarm) return "Alarm";
	else if (_val == Check) return "Check";
	else return "Unkno";
}


void clearDirectoryData()
{
	for (int i = 0; i < maxDataSD; i++)
	{
		dataSD[i].exist = false;
		dataSD[i].text = "";
		//dataSD[i].isDirectory = false;
	}
}

bool addDirectoryData(String _text)//,bool _isDirectory)
{
	for (int i = 0; i < maxDataSD; i++)
	{
		if (!dataSD[i].exist)
		{
			dataSD[i].exist = true;
			dataSD[i].text = _text;
			//dataSD[i].isDirectory = _isDirectory;
			return true;
		}
		
	}
	return false;
}

bool directoryData(File dir)
{
	clearDirectoryData();

	addDirectoryData(BACKSTRING); //premiere ligne retour en arrière
	actualSizeDataSD = 1;

	while (true)
	{
		String name;
		File nextFile = dir.openNextFile();
		if (!nextFile) break;

		if (nextFile.isDirectory())
		{
			name = nextFile.name();
			name += '/';
		}
		else
		{
			name = String(nextFile.name()) + " "  + String (nextFile.size());
		}
		
		SerialPC.println(name);
		if (addDirectoryData(name))
		{
			actualSizeDataSD+=1;
		}
		else
		{
			SerialPC.println("tableau plein");
		}
		nextFile.close();

	}

	if (dataSD[0].exist) return true; else return false; // si il y a au moins 1 données on returne True
	
}

void printDirectory(File dir, int numTabs) {
	while (true) {
		
		File entry = dir.openNextFile();
		if (!entry) {
			// no more files
			break;
		}
		for (uint8_t i = 0; i < numTabs; i++) {
			SerialPC.print('\t');
		}
		SerialPC.print(entry.name());
		if (entry.isDirectory()) {
			SerialPC.println("/");
			printDirectory(entry, numTabs + 1);
		}
		else {
			// files have sizes, directories do not
			SerialPC.print("\t\t");
			SerialPC.println(entry.size(), DEC);
		}
		entry.close();
	}
}

boolean returnfirstfile(File dir, File *FName) {
	while (true) {

		File entry = dir.openNextFile();
		if (!entry) {
			entry.close();
			// pas de fichier
			return false;
			//break;
		}
		else
		{
			if (!entry.isDirectory())
			{
				//entry.close();
				*FName = entry;
				return true;
			}
		}
	}
}

bool initSDbasic()
{
	if (SDINIT == false)
	{
		if (SD.begin(CS))//initialise le lecteur de carte SD
		{
			SerialPC.println("SD BEGIN OK");
			SDINIT = true;
			return true;
		}
		else
		{
			SerialPC.println("SD BEGIN BAD");
			return false;
		}
	}
	else return true;

}

void initSD()
{

	if (SDINIT == false)
	{
		if (SD.begin(4))//initialise le lecteur de carte SD
		{
			SerialPC.println("SD BEGIN OK");
			SDINIT = true;
		}
	}
	if (SDINIT == true)	// si la carte est initialisé
	{

		//root = SD.open("/GCODE/");
		root = SD.open("/");

		File thefile;

		returnfirstfile(root, &thefile);
		SerialPC.print("the name is: ");

		SerialPC.println(thefile.name());
		SerialPC.print("the size is: ");
		SerialPC.print(thefile.size() / 1024, DEC);
		SerialPC.println(" Kb");
		SerialPC.println();
		//SD.remove(thefile.name());
		//SD.open(thefile.name(), FILE_READ);

		/*

		String outBuf;
		int theline = 0;
		while (thefile.available()) {
			Serial.write(thefile.read());
			outBuf = thefile.readStringUntil('\r');
			//outBuf.replace('\r', '\0');
			//outBuf.replace('\n', '\0');
			//outBuf.remove(outBuf.length());
			Serial.print("line ");
			Serial.print(theline);
			Serial.print(": ");
			Serial.print(outBuf);
			Serial.print(" length:");
			Serial.print(outBuf.length());
			//delay(10);
			theline++;

		}
		*/
		thefile.close();

		//printDirectory(root, 0);

	}
	else
	{
		SerialPC.println("SD BEGIN BAD");
	}
}

void managementKeyboard(e_Button _pressedButton)
{
	if (_pressedButton != ButtonNone) bipper.start(10); //if a button is pressed, do a 'bip'
	switch (_pressedButton)
	{
	case ButtonNone:
		return;
		break;
	case ButtonMenu:
		if (DEBUG_MODE) SerialPC.print("changement de menu\n");
		if (menuSelected == MenuCoordinate)
		{
			menuSelected = MenuSerial;
			break;
		}
		if (menuSelected == MenuSerial)
		{
			menuSelected = MenuSD;
			enterSdMenu = true;
			break;
		}
		if (menuSelected == MenuSD)
		{
			menuSelected = MenuCoordinate;
			break;
		}
		break;
	case ButtonSpindle:
		if (spindle.isOn())
		{
			//SerialPC.println("Spindle stop");
			spindle.stop();
		}
		else
		{
			//SerialPC.println("Spindle start");
			spindle.start();
		}
		break;
	case ButtonHomingC:
		//sendToGrbl("$H\r");	// Run the homing cycle
		sendToGrbl("$$\r");
		break;
	case ButtonOriginX:
		sendToGrbl("G92 X0\r");	// set X  to 0
		break;
	case ButtonOriginY:
		sendToGrbl("G92 Y0\r");	// set Y  to 0
		break;
	case ButtonOriginZ:
		sendToGrbl("G92 Z0\r");	// set Z  to 0

		break;
	case ButtonStart:
		sendToGrbl("~\r");	//Start /Resume
		break;
	case ButtonPause:
		sendToGrbl("$$\r");
		break;
	case ButtonStop:
		sendToGrbl("!\r"); // pause mode
		break;
	case ButtonReset:
		sendToGrbl("$X\r"); // Kill alarm lock
		break;
	case ButtonReinit:
		initBasicSetupGRBL();
		break;

	default:
		break;
	}

}

void managementEncoder()
{
	e_turnVal actualTurn = RotaryEncoder::check();		// traite les infos de l'encodeur rotatif
	e_pushVal actualPush = RotaryEncoder::checkPush(); // traite le bouton poussoir de l'encodeur rotatif
	if (actualPush == Push)
	{
		actualPush = NoPush; //reset variable
							 //Buzzer::start(50);
		bipper.start(100);
		if (menuSelected == MenuCoordinate) spindle.speedSet(5000);
		if (menuSelected == MenuSD) pushPositionSelectSD();
	}
	switch (actualTurn)
	{
	case NoTurn:
		break;
	case Up:
		bipper.start(10);
		if (menuSelected == MenuCoordinate)  spindle.speedAdd(500);
		if (menuSelected == MenuSD) upPositionSelectSD();
		break;
	case Down:
		bipper.start(10);
		if (menuSelected == MenuCoordinate)spindle.speedAdd(-500);
		if (menuSelected == MenuSD) downPositionSelectSD();
		break;
	default:
		break;
		
	}
}

void readSerialGRBL()
{
	while (SerialGRBL.available())					//capture un message d'entrée et le met dans line
	{		
		char inChar = (char)SerialGRBL.read();		// get the new byte:
		line += inChar;								// add it to the inputString:

		if (inChar == '\n') {			//if caracterer endline
			stringComplete = true;		//the line is complete
		}
	}
														
	if ((stringComplete) && (line.length() != 0)) {		//if the message entering is complete

		line.remove(line.length() - 2);			//supprime le dernier caractere : le caractere retour chariot

		if (line.startsWith("<")) {						//si il commence par le caractere "<"

			while (!line.endsWith(">"))					// on repete jusqu'a voir le caractere ">"
			{
				line.remove(line.length() - 1);			//supprime le dernier caractere
			}

			parse_and_store();							// decode et recupere les valeurs de l'état de GRBL

		}
		else
		{
			SerialPC.print("-"+line+"-");	//DEBUG si c'est pas une ligne de position
			serialDrawAdd("~"+line);
		}
		line = "";					// clear the string:
		stringComplete = false;		// réintialise la varaible
	}
}
