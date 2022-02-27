#include "mini_uart.h"
#include "shell.h"
int main(){
    init_uart();
    writes_uart("Initialize done.\n");
    while(1){
        //writec_uart(read_uart());
        read_command();
    }
    return 0;
}