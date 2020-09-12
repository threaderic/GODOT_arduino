#include <stdio.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <thread>
#include <atomic>
#include <string>
#include <unistd.h>
#include <array>
#include <mutex>

//#include "ceSerial.h"
#include "arduino_f.h"


using namespace std;
using namespace ce;

// test of the serial communication with arduino out of godot
int main()
{
    char testo[20];
    strcpy(testo, "<hello,050,1.7>");
    printf("%s",testo);
    
    Arduino_f c;
    sleep(1);
    
    c.Thread_generate_int();
    c.Thread_print();
    c.get_string_com();
    c.Thread_generate_com();
    c.stop_thread_print();
    c.stop_thread_generate_int();
    sleep(3);
    printf("hhhhhhhhhhhhhhhhh");
    printf(c.get_string_com());
    printf("hhhhhhhhhhhhhhhhh");
    c.get_string_com();
    c.get_string_com();
    c.get_string_com();
    c.get_string_com();
    c.get_string_com();
    c.get_string_com();
    c.get_string_com();
    c.get_string_com();
    c.get_string_com();
    c.get_string_com();
    c.get_string_com();
    c.get_string_com();
    c.get_string_com();
    c.get_string_com();
    c.get_string_com();
    c.get_string_com();
    c.get_string_com();
    c.set_string_com(testo);
    memset(testo,'\0',sizeof (testo));
    strcpy(testo, "<bye  ,250,8.7000>");
    printf("%s",testo);
    sleep(3);
    c.set_string_com(testo);
    c.set_string_com(testo);
    c.set_string_com(testo);
    sleep(240);
    
    c.stop_thread_generate_com();
}

