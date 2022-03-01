#include "shell.h"
#include "string.h"
#include "mailbox.h"
#include "reboot.h"
int cmp_command(char *buffer){
    if(strcmp(buffer,"help")==0){
        return 1;
    }
    else if(strcmp(buffer,"hello")==0){
        return 2;
    }else if(strcmp(buffer,"revision")==0){
        return 3;
    }else if(strcmp(buffer,"memory")==0){
        return 4;
    }
    else if(strcmp(buffer,"reboot")==0){
        return 5;
    }
    else if(strcmp(buffer,"ccreboot")==0){
        return 6;
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
        
        if(c!='\n' && count<256){
            if(c>=0 && c<=127){ // avoid substitute character �
               writec_uart(c);
                buffer[count++] = c; 
            }
            
        }else{
            writec_uart('\r');
            writec_uart(c);
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
        writes_uart("help       : print this help menu\r\n");
        writes_uart("hello      : print Hello World!\r\n");
        writes_uart("revision   : print board revision\r\n");
        writes_uart("memory     : print ARM memory address and size\r\n");
        writes_uart("reboot     : reboot the device after 1000 ticks\r\n");
        writes_uart("ccreboot   : cancel reboot the device\r\n");
    }else if(action==2){ // hello
        writes_uart("Hello World!\r\n");
    }
    else if(action==3){
        get_board_revision();
    }
    else if(action==4){
        get_ARM_memory();
    }
    else if(action==5){
        writes_uart("reset after 1000 ticks\r\n");
        reset(1000);
    }
    else if(action==6){
        cancel_reset();
    }
    else{
        writes_uart("command not found: ");
        writes_uart(buffer);
        writes_uart("\r\n");
    }
    return;
}