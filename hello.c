#include <stdio.h>
#include <stdlib.h>

#define CCM_CCGR1_BASE				(0X020C406C)	
#define REGISTER_LENGTH				4

int main(){
    
    char name[10];
    printf("Hello Git!\r\n");
    printf("%x\r\n",CCM_CCGR1_BASE + REGISTER_LENGTH - 1);
   // gets(name);
    return 0;
}

