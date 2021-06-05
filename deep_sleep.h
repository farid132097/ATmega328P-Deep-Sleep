#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>



volatile uint32_t __wdt_ticks=0;
volatile uint8_t  __init_status=0;
volatile uint8_t  __prr,__ddrb,__ddrc,__ddrd;
volatile uint8_t  __portb,__portc,__portd;

void __WDT_sleep_init(void){
cli();
WDTCSR=(1<<WDCE)|(1<<WDE);
WDTCSR=(1<<WDP0)|(1<<WDP2)|(1<<WDIE);
sei();
set_sleep_mode(SLEEP_MODE_PWR_DOWN);
__wdt_ticks=0;
}

void turn_off_peripherals(void){
ADCSRA &=~(1<<ADEN);
ACSR   |= (1<<ACD);
__prr   = PRR;
__ddrb  = DDRB;
__ddrc  = DDRC;
__ddrd  = DDRD;
__portb = PORTB;
__portc = PORTC;
__portd = PORTD;
PRR     = 0xFF;
DDRB    = 0xFF;
PORTB   = 0x00;
DDRC    = 0xFF;
PORTC   = 0x00;
DDRD    = 0xFF;
PORTD   = 0x00;
}

void peripherals_prev_state(void){
PRR  =__prr;
DDRB =__ddrb;
DDRC =__ddrc;
DDRD =__ddrd;
PORTB=__portb;
PORTC=__portc;
PORTD=__portd;
}

void __sleep_wdt(void){
sleep_enable();
sleep_bod_disable();
sei();
sleep_cpu();
sleep_disable();
}

ISR(WDT_vect){
WDTCSR|=(1<<WDIE);
__wdt_ticks++;
}

void deep_sleep(uint32_t sleep_sec){
if(__init_status==0){__WDT_sleep_init();__init_status=1;}
turn_off_peripherals();
uint32_t current_ticks=__wdt_ticks;
while(__wdt_ticks<(sleep_sec+current_ticks)){__sleep_wdt();}
peripherals_prev_state();
}