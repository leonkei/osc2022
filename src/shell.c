#include "shell.h"
#include "string.h"

int read_command(){
    char buffer[256];
    int count=0;
    writes_uart("# ");
    while(1){
        char c = read_uart();
        writec_uart(c);
        if(c!='\n' && count<256){
            buffer[count++] = c;
        }else{
            buffer[count]='\0';
            if(strcmp(buffer,"help")==0){
                return 1;
            }else if(strcmp(buffer,"hello")==0){
                return 2;
            }
            else if(count>0){
                writes_uart("command not found: ");
                writes_uart(buffer);
                writes_uart("\n");
                return 0;
            }
            break;
        }
        
    }
    
    return 0;
}
void handle_command(int c){
    if(c == 1){ // help
        writes_uart("help       : print this help menu\n");
        writes_uart("hello      : print Hello World!\n");
        writes_uart("reboot     : reboot the device\n");
    }else if(c==2){ // hello
        writes_uart("Hello World!\n");
    }
}