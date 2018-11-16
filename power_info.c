#include<stdio.h>

int main(int argc,char *argv[]){
    
    system("upower -i /org/freedesktop/UPower/devices/battery_BAT0| grep -E 'state|to\\ full|percentage|time'");
    
    return 0;
}