#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/boot.h>
#include <util/delay.h>

/* The entry point of the Arduino Uno chip upon power-up */
int main(void) {
  uint8_t ch;
  
  // 1. Check what caused the reset (Power-on vs. Watchdog)
  ch = MCUSR;
  MCUSR = 0;

  // If it was a normal power-on (not a computer programming command),
  // jump immediately to your sketch (like the Blink code) without waiting.
  if (!(ch & _BV(EXTRF))) {
    app_start(); 
  }

  // 2. Initialize the Hardware Serial UART (Universal Asynchronous Receiver-Transmitter)
  // This allows the Uno to talk to your computer over the USB cable at 115200 baud.
  UART_STATUS = 0;
  UART_DATA = 0;
  
  // Flash the onboard Pin 13 LED three times quickly to say "I am ready for a sketch"
  flash_led(3);

  // 3. Main Communication Loop (STK500 Protocol)
  // It waits to see if the computer is sending a new program.
  while (1) {
    ch = getch(); // Listen to the serial port

    if (ch == 'A') { // STK_GET_PARAMETER
      // Computer is asking about the chip details
      verifySpace();
      putch(OPTIBOOT_MAJVER);
      putch(OPTIBOOT_MINVER);
    } 
    else if (ch == 'd') { // STK_PROG_PAGE
      // Computer is sending a block of your new program code!
      // This block erases the chip's memory and flashes the new code line-by-line.
      write_memory_page(); 
    } 
    else if (ch == 'Q') { // STK_LEAVE_PROGMODE
      // Computer says: "Finished programming!"
      verifySpace();
      watchdogConfig(WATCHDOG_16MS); // Trigger a tiny reset to start the new app
      while (1);
    }
    else {
      // If it doesn't get a valid computer command within 1 second,
      // it times out and starts your existing program.
      if (timeout) app_start();
    }
  }
}

