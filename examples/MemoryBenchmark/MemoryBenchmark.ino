/*
 * A program which attempts to calculate how much flash and static memory are
 * used by the AceRoutine library. Set the FEATURE macro to various integer to
 * compile different parts of the AceButton library.
 */

#include <Arduino.h>

// DO NOT MODIFY THIS LINE. This will be overwritten by collect.sh on each
// iteration, incrementing from 0 to N. The Arduino IDE will compile the
// program, then the script will extract the flash and static memory usage
// numbers printed out by the Arduino compiler. The numbers will be printed on
// the STDOUT, which then can be saved to a file specific for a particular
// hardware platform, e.g. "nano.txt" or "esp8266.txt".
#define FEATURE 0

// List of features to gather memory statistics.
#define FEATURE_BASELINE 0
#define FEATURE_ONE_DELAY_FUNCTION 1
#define FEATURE_TWO_DELAY_FUNCTIONS 2
#define FEATURE_ONE_COROUTINE 3
#define FEATURE_TWO_COROUTINES 4
#define FEATURE_ONE_COROUTINE_MICROS 5
#define FEATURE_TWO_COROUTINES_MICROS 6
#define FEATURE_ONE_COROUTINE_SECONDS 7
#define FEATURE_TWO_COROUTINES_SECONDS 8
#define FEATURE_SCHEDULER_ONE_COROUTINE 9
#define FEATURE_SCHEDULER_TWO_COROUTINES 10
#define FEATURE_SCHEDULER_ONE_COROUTINE_MICROS 11
#define FEATURE_SCHEDULER_TWO_COROUTINES_MICROS 12
#define FEATURE_SCHEDULER_ONE_COROUTINE_SECONDS 13
#define FEATURE_SCHEDULER_TWO_COROUTINES_SECONDS 14
#define FEATURE_BLINK_FUNCTION 15
#define FEATURE_BLINK_COROUTINE 16

#if FEATURE != FEATURE_BASELINE
  #include <AceRoutine.h>
  using namespace ace_routine;
#endif

// A volatile integer to prevent the compiler from optimizing away the entire
// program.
volatile int disableCompilerOptimization = 0;

#if FEATURE == FEATURE_ONE_DELAY_FUNCTION

  // Hand-rolled alternative to using a COROUTINE() that executes every 10
  // milliseconds.
  void delayFunction() {
    static uint16_t prevMillis;

    uint16_t nowMillis = millis();
    if ((uint16_t) nowMillis - prevMillis >= 10) {
      prevMillis = nowMillis;

      disableCompilerOptimization = 1;
    }
  }

#elif FEATURE == FEATURE_TWO_DELAY_FUNCTIONS

  // Hand-rolled alternative to using a COROUTINE() that executes every 10
  // milliseconds.
  void delayFunctionA() {
    static uint16_t prevMillis;

    uint16_t nowMillis = millis();
    if ((uint16_t) nowMillis - prevMillis >= 10) {
      prevMillis = nowMillis;

      disableCompilerOptimization = 1;
    }
  }

  void delayFunctionB() {
    static uint16_t prevMillis;

    uint16_t nowMillis = millis();
    if ((uint16_t) nowMillis - prevMillis >= 10) {
      prevMillis = nowMillis;

      disableCompilerOptimization = 1;
    }
  }

#elif FEATURE == FEATURE_ONE_COROUTINE

  COROUTINE(a) {
    COROUTINE_LOOP() {
      disableCompilerOptimization = 1;
      COROUTINE_DELAY(10);
    }
  }

#elif FEATURE == FEATURE_TWO_COROUTINES

  COROUTINE(a) {
    COROUTINE_LOOP() {
      disableCompilerOptimization = 1;
      COROUTINE_DELAY(10);
    }
  }

  COROUTINE(b) {
    COROUTINE_LOOP() {
      disableCompilerOptimization = 1;
      COROUTINE_DELAY(10);
    }
  }

#elif FEATURE == FEATURE_ONE_COROUTINE_MICROS

  COROUTINE(a) {
    COROUTINE_LOOP() {
      disableCompilerOptimization = 1;
      COROUTINE_DELAY_MICROS(10);
    }
  }

#elif FEATURE == FEATURE_TWO_COROUTINES_MICROS

  COROUTINE(a) {
    COROUTINE_LOOP() {
      disableCompilerOptimization = 1;
      COROUTINE_DELAY_MICROS(10);
    }
  }

  COROUTINE(b) {
    COROUTINE_LOOP() {
      disableCompilerOptimization = 1;
      COROUTINE_DELAY_MICROS(10);
    }
  }

#elif FEATURE == FEATURE_ONE_COROUTINE_SECONDS

  COROUTINE(a) {
    COROUTINE_LOOP() {
      disableCompilerOptimization = 1;
      COROUTINE_DELAY_SECONDS(10);
    }
  }

#elif FEATURE == FEATURE_TWO_COROUTINES_SECONDS

  COROUTINE(a) {
    COROUTINE_LOOP() {
      disableCompilerOptimization = 1;
      COROUTINE_DELAY_SECONDS(10);
    }
  }

  COROUTINE(b) {
    COROUTINE_LOOP() {
      disableCompilerOptimization = 1;
      COROUTINE_DELAY_SECONDS(10);
    }
  }

#elif FEATURE == FEATURE_SCHEDULER_ONE_COROUTINE

  class MyCoroutine : public Coroutine {
    public:
      int runCoroutine() override {
        COROUTINE_LOOP() {
          disableCompilerOptimization = 1;
          COROUTINE_DELAY(10);
        }
      }
  };

  MyCoroutine a;

#elif FEATURE == FEATURE_SCHEDULER_TWO_COROUTINES

  class MyCoroutineA : public Coroutine {
    public:
      int runCoroutine() override {
        COROUTINE_LOOP() {
          disableCompilerOptimization = 1;
          COROUTINE_DELAY(10);
        }
      }
  };

  class MyCoroutineB : public Coroutine {
    public:
      int runCoroutine() override {
        COROUTINE_LOOP() {
          disableCompilerOptimization = 1;
          COROUTINE_DELAY(10);
        }
      }
  };

  MyCoroutineA a;
  MyCoroutineB b;

#elif FEATURE == FEATURE_SCHEDULER_ONE_COROUTINE_MICROS

  class MyCoroutine : public Coroutine {
    public:
      int runCoroutine() override {
        COROUTINE_LOOP() {
          disableCompilerOptimization = 1;
          COROUTINE_DELAY_MICROS(10);
        }
      }
  };

  MyCoroutine a;

#elif FEATURE == FEATURE_SCHEDULER_TWO_COROUTINES_MICROS

  class MyCoroutineA : public Coroutine {
    public:
      int runCoroutine() override {
        COROUTINE_LOOP() {
          disableCompilerOptimization = 1;
          COROUTINE_DELAY_MICROS(10);
        }
      }
  };

  class MyCoroutineB : public Coroutine {
    public:
      int runCoroutine() override {
        COROUTINE_LOOP() {
          disableCompilerOptimization = 1;
          COROUTINE_DELAY_MICROS(10);
        }
      }
  };

  MyCoroutineA a;
  MyCoroutineB b;

#elif FEATURE == FEATURE_SCHEDULER_ONE_COROUTINE_SECONDS

  class MyCoroutine : public Coroutine {
    public:
      int runCoroutine() override {
        COROUTINE_LOOP() {
          disableCompilerOptimization = 1;
          COROUTINE_DELAY_SECONDS(10);
        }
      }
  };

  MyCoroutine a;

#elif FEATURE == FEATURE_SCHEDULER_TWO_COROUTINES_SECONDS

  class MyCoroutineA : public Coroutine {
    public:
      int runCoroutine() override {
        COROUTINE_LOOP() {
          disableCompilerOptimization = 1;
          COROUTINE_DELAY_SECONDS(10);
        }
      }
  };

  class MyCoroutineB : public Coroutine {
    public:
      int runCoroutine() override {
        COROUTINE_LOOP() {
          disableCompilerOptimization = 1;
          COROUTINE_DELAY_SECONDS(10);
        }
      }
  };

  MyCoroutineA a;
  MyCoroutineB b;

#elif FEATURE == FEATURE_BLINK_FUNCTION

  #ifndef LED_BUILTIN
    #define LED_BUILTIN 2
  #endif

  // Asymmetric blink, same functionality as COROUTINE(blink) below, but using
  // a delay function and a finite state machine. The logic is a lot harder to
  // follow than a coroutine.
  void blink() {
    static uint16_t prevMillis;
    static uint8_t blinkState;
    const uint8_t kBlinkStateLow = 0;
    const uint8_t kBlinkStateHigh = 1;

    if (blinkState == kBlinkStateHigh) {
      uint16_t nowMillis = millis();
      if ((uint16_t) (nowMillis - prevMillis) >= 100) {
        prevMillis = nowMillis;
        digitalWrite(LED_BUILTIN, LOW);
        blinkState = kBlinkStateLow;
      }
    } else {
      uint16_t nowMillis = millis();
      if ((uint16_t) (nowMillis - prevMillis) >= 500) {
        prevMillis = nowMillis;
        digitalWrite(LED_BUILTIN, HIGH);
        blinkState = kBlinkStateHigh;
      }
    }
  }

#elif FEATURE == FEATURE_BLINK_COROUTINE

  #ifndef LED_BUILTIN
    #define LED_BUILTIN 2
  #endif

  // Asymmetric blink, same functionality as blink() function above. HIGH is
  // different than LOW.
  COROUTINE(blink) {
    COROUTINE_LOOP() {
      digitalWrite(LED_BUILTIN, HIGH);
      COROUTINE_DELAY(100);
      digitalWrite(LED_BUILTIN, LOW);
      COROUTINE_DELAY(500);
    }
  }

#endif

// TeensyDuino seems to pull in malloc() and free() when a class with virtual
// functions is used polymorphically. This causes the memory consumption of
// FEATURE_BASELINE (which normally has no classes defined, so does not include
// malloc() and free()) to be artificially small which throws off the memory
// consumption calculations for all subsequent features. Let's define a
// throw-away class and call its method for all FEATURES, including BASELINE.
#if defined(TEENSYDUINO)
  class FooClass {
    public:
      virtual void doit() {
        disableCompilerOptimization = 0;
      }
  };

  FooClass* foo;
#endif

void setup() {
  delay(1000);

#if defined(TEENSYDUINO)
  // Force Teensy to bring in malloc(), free() and other things for virtual
  // dispatch.
  foo = new FooClass();
#endif

#if FEATURE >= FEATURE_SCHEDULER_ONE_COROUTINE \
    && FEATURE <= FEATURE_SCHEDULER_TWO_COROUTINES_SECONDS
  CoroutineScheduler::setup();
#endif
}

void loop() {
#if defined(TEENSYDUINO)
  foo->doit();
#endif

#if FEATURE == FEATURE_BASELINE
  disableCompilerOptimization = 1;
#elif FEATURE == FEATURE_ONE_DELAY_FUNCTION
  delayFunction();
#elif FEATURE == FEATURE_TWO_DELAY_FUNCTIONS
  delayFunctionA();
  delayFunctionB();
#elif FEATURE == FEATURE_ONE_COROUTINE
  a.runCoroutine();
#elif FEATURE == FEATURE_TWO_COROUTINES
  a.runCoroutine();
  b.runCoroutine();
#elif FEATURE == FEATURE_ONE_COROUTINE_MICROS
  a.runCoroutine();
#elif FEATURE == FEATURE_TWO_COROUTINES_MICROS
  a.runCoroutine();
  b.runCoroutine();
#elif FEATURE == FEATURE_ONE_COROUTINE_SECONDS
  a.runCoroutine();
#elif FEATURE == FEATURE_TWO_COROUTINES_SECONDS
  a.runCoroutine();
  b.runCoroutine();
#elif FEATURE == FEATURE_SCHEDULER_ONE_COROUTINE
  CoroutineScheduler::loop();
#elif FEATURE == FEATURE_SCHEDULER_TWO_COROUTINES
  CoroutineScheduler::loop();
#elif FEATURE == FEATURE_SCHEDULER_ONE_COROUTINE_MICROS
  CoroutineScheduler::loop();
#elif FEATURE == FEATURE_SCHEDULER_TWO_COROUTINES_MICROS
  CoroutineScheduler::loop();
#elif FEATURE == FEATURE_SCHEDULER_ONE_COROUTINE_SECONDS
  CoroutineScheduler::loop();
#elif FEATURE == FEATURE_SCHEDULER_TWO_COROUTINES_SECONDS
  CoroutineScheduler::loop();
#elif FEATURE == FEATURE_BLINK_COROUTINE
  blink.runCoroutine();
#elif FEATURE == FEATURE_BLINK_FUNCTION
  blink();
#endif
}
