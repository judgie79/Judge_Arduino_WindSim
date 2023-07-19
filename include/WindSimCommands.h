// CockpitControlCommand.h

#ifndef _WINDSIMCOMMANDS_h
#define _WINDSIMCOMMANDS_h
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

enum class WindSimCommandKey : char {
	DATA = 'd',
	SETSETTINGS = 's',
	GETSETTINGS = 'o',
	RESETSETTINGS = 'n'
};
/*o#24

enum class CommandKey: char {
  HELLO = 'h',              // Hello order to initiate communication with the Arduino
  ALREADY_CONNECTED = 'c',  // Already connected to the Arduino
  ERROR = 'e',
  RECEIVED = 'r',  // Aknowlegment message
  DEBUG = 'l', // debug message
};

*/


#endif

