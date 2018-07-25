/*
 * A demo sketch for the src/ace_routine/cli/ classes. It reads the serial port
 * for a command, parses the command line, then calls the appropriate command
 * handler.
 *
 * Run the sketch, then type 'help' on the serial port. The following
 * commands are supported:
 *  - `help [command]`
 *  - `list`
 *  - `free`
 *  - `echo [args ...]`
 *  - `delay (on | off) millis`
 */

#include <Arduino.h>
#include <AceRoutine.h>
#include "ace_routine/cli/StreamReader.h"
#include "ace_routine/cli/CommandDispatcher.h"
using namespace ace_routine;
using namespace ace_routine::cli;

//---------------------------------------------------------------------------

#ifdef LED_BUILTIN
  const int LED = LED_BUILTIN;
#else
  // Some ESP32 boards do not LED_BUILTIN, sometimes they have more than 1.
  const int LED = 5;
#endif

const int LED_ON = HIGH;
const int LED_OFF = LOW;

// Use asymmetric delays for blinking to demonstrate how easy it is using
// COROUTINES.
int ledOnDelay = 100;
int ledOffDelay = 500;

COROUTINE(blinkLed) {
  COROUTINE_LOOP() {
    digitalWrite(LED, LED_ON);
    COROUTINE_DELAY(ledOnDelay);
    digitalWrite(LED, LED_OFF);
    COROUTINE_DELAY(ledOffDelay);
  }
}

//---------------------------------------------------------------------------

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#elif defined(ESP8266)
extern "C" {
#include "user_interface.h"
}
#else
extern char *__brkval;
#endif

/**
 * Return the amount of free memory. For AVR and Teensy, see:
 *
 * - https://learn.adafruit.com/memories-of-an-arduino/measuring-free-memory
 * - https://arduino.stackexchange.com/questions/30497
 * - https://github.com/mpflaga/Arduino-MemoryFree/blob/master/MemoryFree.cpp
 *
 * For a Nano, the original code returns 2252, which seems too high since it
 * has only 2048 of static RAM. Changed to always test for non-zero value of
 * __brkval, which gives 1553 which seems more accurate because the Arduino
 * IDE says that the sketch leaves 1605 bytes for RAM.
 *
 * For ESP8266, see:
 * - https://github.com/esp8266/Arduino/issues/81
 *
 * For ESP32, see:
 * - https://techtutorialsx.com/2017/12/17/esp32-arduino-getting-the-free-heap/
 */
unsigned long freeMemory() {
#ifdef __arm__
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(ESP8266)
  return system_get_free_heap_size();
#elif defined(ESP32)
  return ESP.getFreeHeap();
#else
  char top;
  return &top - (__brkval ? __brkval : __malloc_heap_start);
#endif
}

//---------------------------------------------------------------------------

/** List the coroutines known by the CoroutineScheduler. */
void listCommand(Print& printer, int /* argc */, const char** /* argv */) {
  CoroutineScheduler::list(printer);
}

/** Echo the command line arguments. */
void echoCommand(Print& printer, int argc, const char** argv) {
 for (int i = 1; i < argc; i++) {
    printer.print(argv[i]);
    printer.print(' ');
  }
  printer.println();
}

/** Print amount of free memory between stack and heap. */
void freeCommand(Print& printer, int /* argc */, const char** /* argv */) {
  printer.print(F("Free memory: "));
  printer.println(freeMemory());
}

/** Change the blinking LED on and off delay parameters. */
void delayCommand(Print& printer, int argc, const char** argv) {
  if (argc != 3) {
    printer.println(F("Invalid number of arguments"));
    return;
  }
  const char* param = argv[1];
  const char* value = argv[2];
  if (strcmp(param, "on") == 0) {
    ledOnDelay = atoi(value);
  } else if (strcmp(param, "off") == 0) {
    ledOffDelay = atoi(value);
  } else {
    printer.print(F("Unknown argument: "));
    printer.println(param);
  }
}

/*
 * Select C_STRING to use C-strings (const char*)
 *
 * Select F_STRING to use FlashStrings (const __FlashString*) which
 * stores the strings in flash memory, saving 48 bytes out of
 * 457 bytes of statis RAM in this example.
 */
#define C_STRING 1
#define F_STRING 2
#define STRING_MODE F_STRING

#if STRING_MODE == C_STRING

const DispatchRecordC dispatchTable[] = {
  {delayCommand, "delay", "(on | off) millis"},
  {listCommand, "list", nullptr},
  {freeCommand, "free", nullptr},
  {echoCommand, "echo", "args ..."},
};

#else

const char DELAY_COMMAND[] PROGMEM = "delay";
const char DELAY_HELP_STRING[] PROGMEM = "(on | off) millis";
const char LIST_COMMAND[] PROGMEM = "list";
const char FREE_COMMAND[] PROGMEM = "free";
const char ECHO_COMMAND[] PROGMEM = "echo";
const char ECHO_HELP_STRING[] PROGMEM = "args ...";

const DispatchRecordF dispatchTable[] = {
  {delayCommand,
      ACE_ROUTINE_FPSTR(DELAY_COMMAND),
      ACE_ROUTINE_FPSTR(DELAY_HELP_STRING)},
  {listCommand, ACE_ROUTINE_FPSTR(LIST_COMMAND), nullptr},
  {freeCommand, ACE_ROUTINE_FPSTR(FREE_COMMAND), nullptr},
  {echoCommand,
      ACE_ROUTINE_FPSTR(ECHO_COMMAND),
      ACE_ROUTINE_FPSTR(ECHO_HELP_STRING)},
};

#endif

const uint8_t NUM_COMMANDS = sizeof(dispatchTable) / sizeof(DispatchRecordC);

const int BUF_SIZE = 64;
char lineBuffer[BUF_SIZE];
StreamReader streamReader(Serial, lineBuffer, BUF_SIZE);

const int8_t ARGV_SIZE = 10;
const char* argv[ARGV_SIZE];

#if STRING_MODE == C_STRING

CommandDispatcherC dispatcher(streamReader, Serial,
    dispatchTable, NUM_COMMANDS, argv, ARGV_SIZE);

#else

CommandDispatcherF dispatcher(streamReader, Serial,
    dispatchTable, NUM_COMMANDS, argv, ARGV_SIZE);

#endif
//---------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  while (!Serial); // micro/leonardo

  dispatcher.resume(); // insert into the scheduler
  CoroutineScheduler::setup();
}

void loop() {
  CoroutineScheduler::loop();
}
