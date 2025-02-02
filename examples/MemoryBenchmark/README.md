# Memory Benchmark

The `MemoryBenchmark.ino` program compiles example code snippets using the
AceRoutine library. The `FEATURE` macro flag controls which feature is compiled.
The `collect.sh` edits this `FEATURE` flag programmatically, then runs the
Arduino IDE compiler on the program, and extracts the flash and static memory
usage into a text file (e.g. `nano.txt`).

The numbers shown below should be considered to be rough estimates. It is often
difficult to separate out the code size of the library from the overhead imposed
by the runtime environment of the processor. For example, it often seems like
the ESP8266 allocates flash memory in blocks of a certain quantity, so the
calculated flash size can jump around in unexpected ways.

**NOTE**: This file was auto-generated using `make README.md`. DO NOT EDIT.

**Version**: AceRoutine v1.5

**Changes**:

* v1.1 to v1.2 saw a significant *decrease* of flash usage by about 200-400
  bytes for various scenarios. This was due to several factors including:
    * `Coroutine::setupCoroutine()` no longer sorts the coroutines in the linked
      list by name
    * `Coroutine::suspend()` and `resume()` no longer modify the linked list
    * `CoroutineScheduler::runCoroutine()` simplified due to immutable
      linked list
    * `CoroutineScheduler::setupScheduler()` simplified due to immutable linked
      list
* v1.3
    * Remove virtual destructor on `Coroutine` class. Reduces flash memory
      consumption by 500-600 bytes on AVR processors, 350 bytes on SAMD21, and
      50-150 bytes on other 32-bit processors. The static memory is also reduced
      by 14 bytes on AVR processors.
    * Replace clock ticking virtual methods (`coroutineMicros()`,
      `coroutineMillis()`, and `coroutineSeconds()`) with static functions that
      delegate to `ClockInterface` which is a template parameter. Saves only
      0-40 bytes of flash on on AVR processors, but 100-1500 bytes of flash on
      32-bit processors.
    * Add benchmark for 'One Delay Function' and 'Two Delay Functions` which use
      functions with a non-blocking if-statement to implement the functionality
      of a `COROUTINE()` that loops every 10 milliseconds.
    * Remove `COROUTINE_DELAY_SECONDS()` functionality. Saves about 200 bytes on
      AVR processors, mostly from the removal of `udiv1000()` which takes almost
      180 bytes. Replacing with native `/1000` does not help much because native
      long division consumes about 130 bytes and is 3X slower on AVR processors.
    * Remove `COROUTINE_DELAY_MICRO()` functionality. Saves about 15-20 bytes
      of flash memory per coroutine on AVR. Saves 80-100 bytes plus 20-30 bytes
      of flash per coroutine on 32-bit processors.
    * Add `Blink Function` and `Blink Coroutine`, 2 implementations of the same
      asymmetric blink functionality where the HIGH level lasts for a different
      duration than the LOW level.
    * Remove `Coroutine::getName()`, `Coroutine::mName()`, and
      `Coroutine::setupCoroutine()`. The human-readable name is no longer
      retained. Saves 10-30 bytes of flash and 3 bytes of static memory per
      coroutine instance for AVR; 10-40 bytes and 8 bytes of static memory per
      instance on 32-bit processors.
    * Use 2 different `Coroutine` subclasses, instead of 2 instances of the same
      `Coroutine` subclass, for the "Scheduler, Two Coroutines" benchmark . This
      makes it more functionally equal to the "Two Coroutines" benchmark which
      automatically creates 2 different subclasses through the `COROUTINE()`
      macro. Increases flash memory by 80-130 bytes across the board.
* v1.3.1
    * Bring back `COROUTINE_DELAY_MICROS()` and `COROUTINE_DELAY_SECONDS()`
      using an implementation that increases flash and static memory *only* if
      they are used.
        * `COROUTINE_DELAY_MICROS()` actually consumes about 30 bytes fewer
          flash memory (on AVR) than `COROUTINE_DELAY()`, probably because there
          is additional overhead in the Arduino `millis()` function compared to
          `micros()`.
        * `COROUTINE_DELAY_SECONDS()` consumes about 100 bytes more flash memory
          (on AVR) compared to `COROUTINE_DELAY()` because of the division by
          1000 which must be done in software on 8-bit processors.
        * The `CoroutineScheduler` is forced to become simpler  which reduces
          flash usage by 10-20 bytes.
* v1.4
    * Upgrade STM32duino Core to 2.0.0.
        * Flash usage increases by 2.3kB across the board, but static RAM goes
          down by 250 bytes.
        * Very little change to AceRoutine code itself.
        * "Blink Function" and "Blink Coroutine" both increase flash usage by
          about 50 bytes, probably due to additional resource consumption of
          `digitalWrite()` in 2.0.0.
    * Upgrade SparkFun SAMD Core to 1.8.3.
        * No change in memory usage.
    * Add a dummy `FooClass` with virtual methods for Teensy 3.2 so that
      `FEATURE_BASELINE` becomes a useful comparison to the other benchmarks.
      Teensyduino seems to pull in `malloc()` and `free()` functions as soon as
      any class with virtual methods are uses, unlike AVR where they are pulled
      in only if the destructor is virtual.
    * Add benchmarks for calling `CoroutineScheduler::setupCoroutine()`.
      Increases flash memory by 50-60 bytes *per coroutine* (AVR) and 30-40
      bytes per coroutine (32-bit processors).

* v1.4.1
    * Upgrade tool chain
        * Arduino IDE from 1.8.13 to 1.8.19
        * Arduino CLI from 0.14.0 to 0.19.2
        * Arduino AVR Core from 1.8.3 to 1.8.4
        * STM32duino from 2.0.0 to 2.2.0
        * ESP8266 Core from 2.7.4 to 3.0.2
        * ESP32 Core from 1.0.6 to 2.0.2
        * Teensyduino from 1.54 to 1.56

* v1.5
    * Add support for human-readable names to coroutine.
        * Increases flash usage by 6-10 bytes per coroutine, even if name isn't
          used.
        * Increases static ram by 3 bytes (AVR) and 4 bytes (32-bits) per
          coroutine.
    * Support `CoroutineProfiler` in the `CoroutineScheduler`.
        * Increases the flash size of the `CoroutineScheduler` by 90-140 bytes
          for both 8-bit and 32-bit processors, even if `CoroutineProfiler` is
          not used. This is a one-time hit.
    * Add `LogBinProfiler`
        * Adds about 90 bytes of flash and 70 bytes of RAM on AVR.
        * Adds about 140 bytes of flash and 70 bytes of RAM on ESP8266.
    * Add `LogBinTableRenderer`
        * Adds about 1400 bytes of flash and ~20 bytes of RAM on AVR.
        * Adds about 1900 bytes of flash and ~0 bytes of RAM on ESP8266.
    * Add `LogBinJsonRenderer`
        * Adds about 900 bytes of flash and ~20 bytes of RAM on AVR.
        * Adds about 1300 bytes of flash and ~0 bytes of RAM on ESP8266.


## How to Generate

This requires the [AUniter](https://github.com/bxparks/AUniter) script
to execute the Arduino IDE programmatically.

The `Makefile` has rules for several microcontrollers:

```
$ make benchmarks
```
produces the following files:

```
attiny.txt
nano.txt
micro.txt
stm32.txt
esp8266.txt
esp32.txt
teensy32.txt
```

The `generate_table.awk` program reads one of `*.txt` files and prints out an
ASCII table that can be directly embedded into this README.md file. For example
the following command produces the table in the Nano section below:

```
$ ./generate_table.awk < nano.txt
```

Fortunately, we no longer need to run `generate_table.awk` for each `*.txt`
file. The process has been automated using the `generate_readme.py` script which
will be invoked by the following command:
```
$ make README.md
```

## Results

* Baseline: A program that does (almost) nothing
* One Delay Function: A single non-blocking delay function that waits 10 millis.
* Two Delay Functions: Two non-blocking delay functions.
* One Coroutine: One instance of `Coroutine` that waits 10 millis, executed
  directly through `runCoroutine()`.
* Two Coroutines: Two instances of `Coroutine` that wait 10 millis, executed
  directly through `runCoroutine()`.
* Scheduler, One Coroutine: One instance of `Coroutine` executed through the
  `CoroutineScheduler`.
* Scheduler, Two Coroutines: Two instances of `Coroutine` executed through the
  `CoroutineScheduler`.
* Blink Function: A function that blinks the LED asymmetrically, with HIGH
  lasting a different duration than LOW.
* Blink Coroutine: A `Coroutine` that blinks asymmetrically, exactly the same as
  the `blink()` function.

### ATtiny85

* 8MHz ATtiny85
* Arduino IDE 1.8.19, Arduino CLI 0.19.2
* SpenceKonde/ATTinyCore 1.5.2

```
+--------------------------------------------------------------------+
| functionality                         |  flash/  ram |       delta |
|---------------------------------------+--------------+-------------|
| Baseline                              |   1026/   81 |     0/    0 |
|---------------------------------------+--------------+-------------|
| One Delay Function                    |   1080/   83 |    54/    2 |
| Two Delay Functions                   |   1134/   85 |   108/    4 |
|---------------------------------------+--------------+-------------|
| One Coroutine (millis)                |   1206/  107 |   180/   26 |
| Two Coroutines (millis)               |   1400/  131 |   374/   50 |
|---------------------------------------+--------------+-------------|
| One Coroutine (micros)                |   1174/  107 |   148/   26 |
| Two Coroutines (micros)               |   1336/  131 |   310/   50 |
|---------------------------------------+--------------+-------------|
| One Coroutine (seconds)               |   1302/  107 |   276/   26 |
| Two Coroutines (seconds)              |   1524/  131 |   498/   50 |
|---------------------------------------+--------------+-------------|
| One Coroutine, Profiler               |   1276/  107 |   250/   26 |
| Two Coroutines, Profiler              |   1528/  131 |   502/   50 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (millis)     |   1336/  109 |   310/   28 |
| Scheduler, Two Coroutines (millis)    |   1522/  133 |   496/   52 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (micros)     |   1304/  109 |   278/   28 |
| Scheduler, Two Coroutines (micros)    |   1458/  133 |   432/   52 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (seconds)    |   1432/  109 |   406/   28 |
| Scheduler, Two Coroutines (seconds)   |   1646/  133 |   620/   52 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (setup)      |   1388/  109 |   362/   28 |
| Scheduler, Two Coroutines (setup)     |   1670/  133 |   644/   52 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (man setup)  |   1364/  109 |   338/   28 |
| Scheduler, Two Coroutines (man setup) |   1654/  133 |   628/   52 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine, Profiler    |   1396/  109 |   370/   28 |
| Scheduler, Two Coroutines, Profiler   |   1582/  133 |   556/   52 |
|---------------------------------------+--------------+-------------|
| Scheduler, LogBinProfiler             |   1512/  181 |   486/  100 |
| Scheduler, LogBinTableRenderer        |   2876/  193 |  1850/  112 |
| Scheduler, LogBinJsonRenderer         |   2410/  197 |  1384/  116 |
|---------------------------------------+--------------+-------------|
| Blink Function                        |   1176/   84 |   150/    3 |
| Blink Coroutine                       |   1330/  107 |   304/   26 |
+--------------------------------------------------------------------+

```

### Arduino Nano

* 16MHz ATmega328P
* Arduino IDE 1.8.19, Arduino CLI 0.19.2
* Arduino AVR Boards 1.8.4

```
+--------------------------------------------------------------------+
| functionality                         |  flash/  ram |       delta |
|---------------------------------------+--------------+-------------|
| Baseline                              |   1616/  186 |     0/    0 |
|---------------------------------------+--------------+-------------|
| One Delay Function                    |   1664/  188 |    48/    2 |
| Two Delay Functions                   |   1726/  190 |   110/    4 |
|---------------------------------------+--------------+-------------|
| One Coroutine (millis)                |   1804/  212 |   188/   26 |
| Two Coroutines (millis)               |   1998/  236 |   382/   50 |
|---------------------------------------+--------------+-------------|
| One Coroutine (micros)                |   1776/  212 |   160/   26 |
| Two Coroutines (micros)               |   1942/  236 |   326/   50 |
|---------------------------------------+--------------+-------------|
| One Coroutine (seconds)               |   1904/  212 |   288/   26 |
| Two Coroutines (seconds)              |   2130/  236 |   514/   50 |
|---------------------------------------+--------------+-------------|
| One Coroutine, Profiler               |   1874/  212 |   258/   26 |
| Two Coroutines, Profiler              |   2132/  236 |   516/   50 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (millis)     |   1928/  214 |   312/   28 |
| Scheduler, Two Coroutines (millis)    |   2114/  238 |   498/   52 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (micros)     |   1900/  214 |   284/   28 |
| Scheduler, Two Coroutines (micros)    |   2058/  238 |   442/   52 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (seconds)    |   2028/  214 |   412/   28 |
| Scheduler, Two Coroutines (seconds)   |   2246/  238 |   630/   52 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (setup)      |   1978/  214 |   362/   28 |
| Scheduler, Two Coroutines (setup)     |   2264/  238 |   648/   52 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (man setup)  |   1956/  214 |   340/   28 |
| Scheduler, Two Coroutines (man setup) |   2250/  238 |   634/   52 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine, Profiler    |   1992/  214 |   376/   28 |
| Scheduler, Two Coroutines, Profiler   |   2178/  238 |   562/   52 |
|---------------------------------------+--------------+-------------|
| Scheduler, LogBinProfiler             |   2112/  286 |   496/  100 |
| Scheduler, LogBinTableRenderer        |   3514/  304 |  1898/  118 |
| Scheduler, LogBinJsonRenderer         |   3034/  308 |  1418/  122 |
|---------------------------------------+--------------+-------------|
| Blink Function                        |   1948/  189 |   332/    3 |
| Blink Coroutine                       |   2118/  212 |   502/   26 |
+--------------------------------------------------------------------+

```

### SparkFun Pro Micro

* 16 MHz ATmega32U4
* Arduino IDE 1.8.19, Arduino CLI 0.19.2
* SparkFun AVR Boards 1.1.13

```
+--------------------------------------------------------------------+
| functionality                         |  flash/  ram |       delta |
|---------------------------------------+--------------+-------------|
| Baseline                              |   3582/  151 |     0/    0 |
|---------------------------------------+--------------+-------------|
| One Delay Function                    |   3630/  153 |    48/    2 |
| Two Delay Functions                   |   3690/  155 |   108/    4 |
|---------------------------------------+--------------+-------------|
| One Coroutine (millis)                |   3770/  177 |   188/   26 |
| Two Coroutines (millis)               |   3964/  201 |   382/   50 |
|---------------------------------------+--------------+-------------|
| One Coroutine (micros)                |   3742/  177 |   160/   26 |
| Two Coroutines (micros)               |   3908/  201 |   326/   50 |
|---------------------------------------+--------------+-------------|
| One Coroutine (seconds)               |   3870/  177 |   288/   26 |
| Two Coroutines (seconds)              |   4096/  201 |   514/   50 |
|---------------------------------------+--------------+-------------|
| One Coroutine, Profiler               |   3840/  177 |   258/   26 |
| Two Coroutines, Profiler              |   4098/  201 |   516/   50 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (millis)     |   3894/  179 |   312/   28 |
| Scheduler, Two Coroutines (millis)    |   4080/  203 |   498/   52 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (micros)     |   3866/  179 |   284/   28 |
| Scheduler, Two Coroutines (micros)    |   4024/  203 |   442/   52 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (seconds)    |   3994/  179 |   412/   28 |
| Scheduler, Two Coroutines (seconds)   |   4212/  203 |   630/   52 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (setup)      |   3944/  179 |   362/   28 |
| Scheduler, Two Coroutines (setup)     |   4230/  203 |   648/   52 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (man setup)  |   3922/  179 |   340/   28 |
| Scheduler, Two Coroutines (man setup) |   4216/  203 |   634/   52 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine, Profiler    |   3958/  179 |   376/   28 |
| Scheduler, Two Coroutines, Profiler   |   4144/  203 |   562/   52 |
|---------------------------------------+--------------+-------------|
| Scheduler, LogBinProfiler             |   4078/  251 |   496/  100 |
| Scheduler, LogBinTableRenderer        |   5478/  269 |  1896/  118 |
| Scheduler, LogBinJsonRenderer         |   4998/  273 |  1416/  122 |
|---------------------------------------+--------------+-------------|
| Blink Function                        |   4022/  154 |   440/    3 |
| Blink Coroutine                       |   4192/  177 |   610/   26 |
+--------------------------------------------------------------------+

```

### STM32 Blue Pill

* STM32F103C8, 72 MHz ARM Cortex-M3
* Arduino IDE 1.8.19, Arduino CLI 0.19.2
* STM32duino 2.2.0

```
+--------------------------------------------------------------------+
| functionality                         |  flash/  ram |       delta |
|---------------------------------------+--------------+-------------|
| Baseline                              |  21932/ 3540 |     0/    0 |
|---------------------------------------+--------------+-------------|
| One Delay Function                    |  21960/ 3544 |    28/    4 |
| Two Delay Functions                   |  22008/ 3544 |    76/    4 |
|---------------------------------------+--------------+-------------|
| One Coroutine (millis)                |  22064/ 3572 |   132/   32 |
| Two Coroutines (millis)               |  22212/ 3600 |   280/   60 |
|---------------------------------------+--------------+-------------|
| One Coroutine (micros)                |  22128/ 3572 |   196/   32 |
| Two Coroutines (micros)               |  22276/ 3600 |   344/   60 |
|---------------------------------------+--------------+-------------|
| One Coroutine (seconds)               |  22080/ 3572 |   148/   32 |
| Two Coroutines (seconds)              |  22244/ 3600 |   312/   60 |
|---------------------------------------+--------------+-------------|
| One Coroutine, Profiler               |  22172/ 3572 |   240/   32 |
| Two Coroutines, Profiler              |  22328/ 3600 |   396/   60 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (millis)     |  22132/ 3576 |   200/   36 |
| Scheduler, Two Coroutines (millis)    |  22236/ 3604 |   304/   64 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (micros)     |  22196/ 3576 |   264/   36 |
| Scheduler, Two Coroutines (micros)    |  22300/ 3604 |   368/   64 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (seconds)    |  22148/ 3576 |   216/   36 |
| Scheduler, Two Coroutines (seconds)   |  22268/ 3604 |   336/   64 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (setup)      |  22152/ 3576 |   220/   36 |
| Scheduler, Two Coroutines (setup)     |  22284/ 3604 |   352/   64 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (man setup)  |  22144/ 3576 |   212/   36 |
| Scheduler, Two Coroutines (man setup) |  22276/ 3604 |   344/   64 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine, Profiler    |  22232/ 3576 |   300/   36 |
| Scheduler, Two Coroutines, Profiler   |  22336/ 3604 |   404/   64 |
|---------------------------------------+--------------+-------------|
| Scheduler, LogBinProfiler             |  22368/ 3644 |   436/  104 |
| Scheduler, LogBinTableRenderer        |  23752/ 3644 |  1820/  104 |
| Scheduler, LogBinJsonRenderer         |  23212/ 3644 |  1280/  104 |
|---------------------------------------+--------------+-------------|
| Blink Function                        |  22168/ 3544 |   236/    4 |
| Blink Coroutine                       |  22280/ 3572 |   348/   32 |
+--------------------------------------------------------------------+

```

### ESP8266

* NodeMCU 1.0 clone, 80MHz ESP8266
* Arduino IDE 1.8.19, Arduino CLI 0.19.2
* ESP8266 Boards 3.0.2

```
+--------------------------------------------------------------------+
| functionality                         |  flash/  ram |       delta |
|---------------------------------------+--------------+-------------|
| Baseline                              | 264981/27984 |     0/    0 |
|---------------------------------------+--------------+-------------|
| One Delay Function                    | 265045/27992 |    64/    8 |
| Two Delay Functions                   | 265109/27992 |   128/    8 |
|---------------------------------------+--------------+-------------|
| One Coroutine (millis)                | 265177/28028 |   196/   44 |
| Two Coroutines (millis)               | 265337/28060 |   356/   76 |
|---------------------------------------+--------------+-------------|
| One Coroutine (micros)                | 265209/28028 |   228/   44 |
| Two Coroutines (micros)               | 265369/28060 |   388/   76 |
|---------------------------------------+--------------+-------------|
| One Coroutine (seconds)               | 265209/28028 |   228/   44 |
| Two Coroutines (seconds)              | 265385/28060 |   404/   76 |
|---------------------------------------+--------------+-------------|
| One Coroutine, Profiler               | 265257/28028 |   276/   44 |
| Two Coroutines, Profiler              | 265433/28060 |   452/   76 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (millis)     | 265241/28036 |   260/   52 |
| Scheduler, Two Coroutines (millis)    | 265385/28060 |   404/   76 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (micros)     | 265257/28036 |   276/   52 |
| Scheduler, Two Coroutines (micros)    | 265401/28060 |   420/   76 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (seconds)    | 265257/28036 |   276/   52 |
| Scheduler, Two Coroutines (seconds)   | 265417/28060 |   436/   76 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (setup)      | 265273/28036 |   292/   52 |
| Scheduler, Two Coroutines (setup)     | 265433/28060 |   452/   76 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (man setup)  | 265257/28036 |   276/   52 |
| Scheduler, Two Coroutines (man setup) | 265433/28060 |   452/   76 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine, Profiler    | 265321/28036 |   340/   52 |
| Scheduler, Two Coroutines, Profiler   | 265449/28060 |   468/   76 |
|---------------------------------------+--------------+-------------|
| Scheduler, LogBinProfiler             | 265465/28100 |   484/  116 |
| Scheduler, LogBinTableRenderer        | 267381/28100 |  2400/  116 |
| Scheduler, LogBinJsonRenderer         | 266789/28104 |  1808/  120 |
|---------------------------------------+--------------+-------------|
| Blink Function                        | 265669/28064 |   688/   80 |
| Blink Coroutine                       | 265801/28100 |   820/  116 |
+--------------------------------------------------------------------+

```

### ESP32

* ESP32-01 Dev Board, 240 MHz Tensilica LX6
* Arduino IDE 1.8.19, Arduino CLI 0.19.2
* ESP32 Boards 2.0.2

```
+--------------------------------------------------------------------+
| functionality                         |  flash/  ram |       delta |
|---------------------------------------+--------------+-------------|
| Baseline                              | 230413/16220 |     0/    0 |
|---------------------------------------+--------------+-------------|
| One Delay Function                    | 230953/16260 |   540/   40 |
| Two Delay Functions                   | 231025/16260 |   612/   40 |
|---------------------------------------+--------------+-------------|
| One Coroutine (millis)                | 231065/16292 |   652/   72 |
| Two Coroutines (millis)               | 231237/16316 |   824/   96 |
|---------------------------------------+--------------+-------------|
| One Coroutine (micros)                | 231077/16292 |   664/   72 |
| Two Coroutines (micros)               | 231249/16316 |   836/   96 |
|---------------------------------------+--------------+-------------|
| One Coroutine (seconds)               | 231081/16292 |   668/   72 |
| Two Coroutines (seconds)              | 231269/16316 |   856/   96 |
|---------------------------------------+--------------+-------------|
| One Coroutine, Profiler               | 231117/16292 |   704/   72 |
| Two Coroutines, Profiler              | 231333/16316 |   920/   96 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (millis)     | 231129/16292 |   716/   72 |
| Scheduler, Two Coroutines (millis)    | 231269/16324 |   856/  104 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (micros)     | 231141/16292 |   728/   72 |
| Scheduler, Two Coroutines (micros)    | 231281/16324 |   868/  104 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (seconds)    | 231145/16292 |   732/   72 |
| Scheduler, Two Coroutines (seconds)   | 231301/16324 |   888/  104 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (setup)      | 231157/16292 |   744/   72 |
| Scheduler, Two Coroutines (setup)     | 231329/16324 |   916/  104 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (man setup)  | 231145/16292 |   732/   72 |
| Scheduler, Two Coroutines (man setup) | 231321/16324 |   908/  104 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine, Profiler    | 231181/16292 |   768/   72 |
| Scheduler, Two Coroutines, Profiler   | 231321/16324 |   908/  104 |
|---------------------------------------+--------------+-------------|
| Scheduler, LogBinProfiler             | 231321/16364 |   908/  144 |
| Scheduler, LogBinTableRenderer        | 232653/16364 |  2240/  144 |
| Scheduler, LogBinJsonRenderer         | 232101/16364 |  1688/  144 |
|---------------------------------------+--------------+-------------|
| Blink Function                        | 231301/16268 |   888/   48 |
| Blink Coroutine                       | 231413/16300 |  1000/   80 |
+--------------------------------------------------------------------+

```

### Teensy 3.2

* 96 MHz ARM Cortex-M4
* Arduino IDE 1.8.19, Arduino CLI 0.19.2
* Teensyduino 1.56
* Compiler options: "Faster"

```
+--------------------------------------------------------------------+
| functionality                         |  flash/  ram |       delta |
|---------------------------------------+--------------+-------------|
| Baseline                              |  10648/ 4156 |     0/    0 |
|---------------------------------------+--------------+-------------|
| One Delay Function                    |  10680/ 4160 |    32/    4 |
| Two Delay Functions                   |  10708/ 4160 |    60/    4 |
|---------------------------------------+--------------+-------------|
| One Coroutine (millis)                |  10804/ 4188 |   156/   32 |
| Two Coroutines (millis)               |  10924/ 4216 |   276/   60 |
|---------------------------------------+--------------+-------------|
| One Coroutine (micros)                |  10860/ 4188 |   212/   32 |
| Two Coroutines (micros)               |  10968/ 4216 |   320/   60 |
|---------------------------------------+--------------+-------------|
| One Coroutine (seconds)               |  10824/ 4188 |   176/   32 |
| Two Coroutines (seconds)              |  10964/ 4216 |   316/   60 |
|---------------------------------------+--------------+-------------|
| One Coroutine, Profiler               |  10904/ 4188 |   256/   32 |
| Two Coroutines, Profiler              |  11072/ 4216 |   424/   60 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (millis)     |  10868/ 4192 |   220/   36 |
| Scheduler, Two Coroutines (millis)    |  10984/ 4220 |   336/   64 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (micros)     |  10924/ 4192 |   276/   36 |
| Scheduler, Two Coroutines (micros)    |  11028/ 4220 |   380/   64 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (seconds)    |  10888/ 4192 |   240/   36 |
| Scheduler, Two Coroutines (seconds)   |  11024/ 4220 |   376/   64 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (setup)      |  10904/ 4192 |   256/   36 |
| Scheduler, Two Coroutines (setup)     |  11052/ 4220 |   404/   64 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine (man setup)  |  10880/ 4192 |   232/   36 |
| Scheduler, Two Coroutines (man setup) |  11032/ 4220 |   384/   64 |
|---------------------------------------+--------------+-------------|
| Scheduler, One Coroutine, Profiler    |  10976/ 4192 |   328/   36 |
| Scheduler, Two Coroutines, Profiler   |  11092/ 4220 |   444/   64 |
|---------------------------------------+--------------+-------------|
| Scheduler, LogBinProfiler             |  11396/ 4268 |   748/  112 |
| Scheduler, LogBinTableRenderer        |  14120/ 4284 |  3472/  128 |
| Scheduler, LogBinJsonRenderer         |  13476/ 4284 |  2828/  128 |
|---------------------------------------+--------------+-------------|
| Blink Function                        |  11104/ 4164 |   456/    8 |
| Blink Coroutine                       |  11236/ 4188 |   588/   32 |
+--------------------------------------------------------------------+

```

