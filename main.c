
#include "deep_sleep.h"
#include <util/delay.h>


int main(void){

/*LED PIN PC5*/
DDRC|=(1<<5);

while(1){
              
			  deep_sleep(10);  /*Sleep 10s*/
			  
			  PORTC|=(1<<5);   /*Turn on LED for 1s*/
			  _delay_ms(1000);
			  
			  }
}


