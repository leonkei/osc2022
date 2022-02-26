#include"mini_uart.h"
int main(){
    init_uart();
    while(1){
        char c = read_uart();
        write_uart(c);
    }
    return 0;
}