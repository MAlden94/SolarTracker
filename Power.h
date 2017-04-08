#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

void wakeUp(void);

ISR(WDT_vect){
  wdt_disable();
}
void Rest(void){
    MCUSR = 0;
    // allow changes, disable reset and set interrupt mode and an interval of 8 seconds
    // and pat the dog as well
    WDTCSR = _BV(WDCE) | _BV(WDE);
    WDTCSR = _BV(WDIE) | _BV(WDP3) | _BV(WDP0);
    wdt_reset();

    attachInterrupt(0, wakeUp, CHANGE);
    power_all_disable();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();

    // turn off brown-out enable in software and cancel sleep as a precaution
    MCUCR = _BV(BODS) | _BV(BODSE);
    MCUCR = _BV(BODS);
    sleep_cpu();
    // ...time passes ... 
    sleep_disable();
    power_all_enable();
    detachInterrupt(0);
} 


