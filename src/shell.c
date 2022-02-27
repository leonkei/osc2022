#include "shell.h"
#include "string.h"

int cmp_command(char *buffer){
    if(strcmp(buffer,"help")==0){
        return 1;
    }
    else if(strcmp(buffer,"hello")==0){
        return 2;
    }
    else{
        return 0;
    }
}

void read_command(){
    char buffer[256];
    int count=0;
    int action = 0;
    writes_uart("# ");
    while(1){
        char c = read_uart();
        writec_uart(c);
        if(c!='\n' && count<256){
            buffer[count++] = c;
        }else{
            buffer[count]='\0';
            if(buffer[0] != '\0'){
                action = cmp_command(buffer);
                handle_command(action, buffer);
            }
            break;
        }
        
    }
    return;
}
void handle_command(int action, char *buffer){
    if(action == 1){ // help
        writes_uart("help       : print this help menu\n");
        writes_uart("hello      : print Hello World!\n");
        writes_uart("reboot     : reboot the device\n");
    }else if(action==2){ // hello
        writes_uart("Hello World!\n");
    }else{
        writes_uart("command not found: ");
        writes_uart(buffer);
        writes_uart("\n");
    }
    return;
}