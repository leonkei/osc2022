#include "mini_uart.h"
void init_uart(){
    *AUXENB |=1; // enable mini UART, then mini uart register can be accessed.
    *AUX_MU_CNTL_REG = 0; // Disable transmitter and receiver during configuration.
    *AUX_MU_IER_REG = 0;
    *AUX_MU_LCR_REG = 3;
    *AUX_MU_MCR_REG = 0;
    *AUX_MU_BAUD_REG  = 270;
    *AUX_MU_IIR_REG = 6;
    *AUX_MU_CNTL_REG = 3;


    /* configure gpio*/
    
    register unsigned int tmp_reg;
    tmp_reg = *GPFSEL1; // load the GPIO Function Select Registers SEL1 to tmp_reg.
    tmp_reg &= 0xfffc0fff; // use a mask to make the 12~17 bits(FSEL14 & FSEL15) of the register to 0.
    tmp_reg |= (0b010 << 12) | (0b010 << 15); // to make the 12~14bits and 15~17 bits of the register to b010, takes alternate function 5.
    
    /*  
    1. Write to GPPUD to set the required control signal (i.e. Pull-up or Pull-Down or neither
    to remove the current Pull-up/down)
    2. Wait 150 cycles – this provides the required set-up time for the control signal
    3. Write to GPPUDCLK0/1 to clock the control signal into the GPIO pads you wish to
    modify – NOTE only the pads which receive a clock will be modified, all others will
    retain their previous state.
    4. Wait 150 cycles – this provides the required hold time for the control signal
    5. Write to GPPUD to remove the control signal
    6. Write to GPPUDCLK0/1 to remove the clock
    */
    *GPPUD = 0; // disable pull-up/down tmp_regregister, allow gpio pins use alternate function, not only basic input-output.
    tmp_reg = 150;
    while(tmp_reg--){
        asm volatile("nop"); // wait 150 cycles.
    }
    *GPPUDCLK0 = (1<<14)|(1<<15); // assert clock on 14 and 15, because the gpio state would be modified.
    tmp_reg = 150;
    while(tmp_reg--){
        asm volatile("nop"); // wait 150 cycles.
    }

    *GPPUDCLK0 = 0; // remove the clock.
    *AUX_MU_CNTL_REG = 3; // enable tx and rx after configuration.

}
char read_uart(){
    while(!(*AUX_MU_LSR_REG & 0x01)){
        asm volatile("nop");
    }
    char r = (char)*AUX_MU_IO_REG;
    return r=='\r'?'\n':r;
}
void writec_uart(char s){
    while(!(*AUX_MU_LSR_REG & 0x20)) asm volatile("nop");
    *AUX_MU_IO_REG = s;
}
void writes_uart(char *s){
    while(*s){
        writec_uart(*s++);
    }
}