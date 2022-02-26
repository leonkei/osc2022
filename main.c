#include"mini_uart.h"
int main(){
    init_uart();
    writes_uart("Initialize done.\n");
    while(1){
        writec_uart(read_uart()); 
    }
    return 0;
}