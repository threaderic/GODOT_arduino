/* arduino_f.cpp */

#include "arduino_f.h"
#include <sys/timeb.h>

int getMilliCount()
{
    timeb tb;
    ftime(&tb);
    int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
    return nCount;
}

Arduino_f::Arduino_f() :  m_thread_generate_com_bool(true),m_thread_generate_int_bool(true) {
    com.SetPort("/dev/ttyUSB0");
    com.SetBaudRate(115200);
    com.SetDataSize(8);
    com.SetParity('N');
    com.SetStopBits(1);
    memset(this->m_data,'\0',sizeof (this->m_data));
    strcpy(m_data_to_arduino_string, "oo\0");
    std::cout << com.GetPort() << com.GetBaudRate() << com.GetDataSize() << com.GetParity() << com.GetStopBits() << std::endl;
}

Arduino_f::~Arduino_f()
{// m_thread_print.join();
    //m_thread_generate_int.join();
}

void Arduino_f::Thread_print() { m_thread_print = std::thread(&Arduino_f::print, this); }

void Arduino_f::Thread_generate_int() {m_thread_generate_int =std::thread(&Arduino_f::generate_int,this,std::ref(m_thread_generate_int_bool));}

void Arduino_f::Thread_generate_com() {m_thread_generate_com=std::thread(&Arduino_f::generate_com,this);}

void Arduino_f::stop_thread_print() {m_thread_print.join();}

void Arduino_f::stop_thread_generate_int(){
    usleep(1000);
    // m_thread_generate_int_bool -> to go out of the while loop in function generate_int(bool&)
    this->m_thread_generate_int_bool=false;
    
    if (m_thread_generate_int.joinable()) m_thread_generate_int.join();
    
    this->m_thread_generate_int_bool=true;
}

void Arduino_f::stop_thread_generate_com() {
    m_thread_generate_com_bool=false;
    m_thread_generate_com.join();
}

///////////////////////////////
// private functions
///////////////////////////////

void Arduino_f::print() const { std::cout << m_data << '\n'; }

void Arduino_f::generate_int(std::atomic<bool>& boolean_flag) {
    int i=0;
    while(boolean_flag){
        std::cout << i << '\n';
        i++;
    }
    std::cout << "Hourra" << std::endl;
    
}

void Arduino_f::generate_com()  {
    printf("Opening port %s.\n",this->com.GetPort().c_str());
    if (com.Open() == 0) {
        printf("OK.\n");
    }
    else {
        printf("Error.\n");
        return;
    }
    
    int i=0;
    int j=0;                                //loop ok
    int k=0;                                //loop nok
    
    bool successFlag=true;
    
    const int numChars=100;                 //number of char send to arduino and table to store buffer
    char c[numChars];
    bool recvInProgress = false;
    char rc;                                // receive character from arduino
    int ndx = 0;                            // position of character
    
    //timer variables
    int startMillis;                        //some global variables available anywhere in the program
    int currentMillis;
    const int period = 10;                  //the value is a number of milliseconds
    
    printf("Reading.\n");
    printf("Waiting 1 seconds.\n");
    ceSerial::Delay(1000);
    
    // clear buffer
    while(i<1000000) {
        c[0]=com.ReadChar(successFlag);
        i++;
    }
    
    char m_data_to_arduino_temp[20];
    memset(m_data_to_arduino_temp,'\0',sizeof (m_data_to_arduino_temp));
    strcpy(m_data_to_arduino_temp, m_data_to_arduino_string);
    
    startMillis= getMilliCount();
    
    while(m_thread_generate_com_bool){
        currentMillis = getMilliCount();                // get the current "time" (actually the number of milliseconds since the program started)
        
        if (currentMillis - startMillis >= period) {    // test whether the period has elapsed
            startMillis = startMillis + period;         // update the startMillis for the next loop
            
            recvInProgress = false;
            ndx = 0;
            
            do {
                
                rc = com.ReadChar(successFlag);
                
                if (recvInProgress == true) {
                    if (rc != '\n') {
                        
                        ndx++;
                        c[ndx] = rc;
                        
                        if (ndx >= numChars) {
                            ndx = numChars - 1;
                        }
                    }
                    else {
                        
                        recvInProgress = false;
                        ndx = 0;
                        //
                        
                    }
                    
                }
                
                else if (rc == '<' ) {
                    recvInProgress = true;
                    c[ndx] = rc;
                }
            } while(recvInProgress==true);
            
            
            
            // if the message has been received copy to m_data
            if(strlen(c)>2) {
                m_mutex_get.lock();
                strcpy( m_data, c );
                m_mutex_get.unlock();
                j++;
                #ifdef TEST_arduino_f_module_in_main
                printf("%s\n",c);
                #endif
                ceSerial::Delay(5);
            } //end if
            else
            {
                k++;
                printf("fuckit\n");
                ceSerial::Delay(3);
            }
            
            //if the string m_data_to_arduino_string changed then send it to arduino
            if(strcmp (m_data_to_arduino_string,m_data_to_arduino_temp) != 0 )
            {
                // printf("m_data_to_arduino_string avant %s\n",m_data_to_arduino_string);
                m_mutex_set.lock();
                com.Write(m_data_to_arduino_string);
                m_mutex_set.unlock();
                // printf("m_data_to_arduino_string apres %s\n",m_data_to_arduino_string);
                strcpy(m_data_to_arduino_temp, m_data_to_arduino_string);
            }
            
            memset(c,'\0',sizeof (c));
        } // end if --> timer
    } //end while
    
    printf("Closing port %s.\n",com.GetPort().c_str());
    com.Close();
    m_thread_generate_com_bool=true;
    
    printf("average time between data (10s test) : %f ms\n",(float)247000/j);
    printf("number of loop with error (successFlag==false) : %f\n",(float)k);
    return;
}


#ifndef TEST_arduino_f_module_in_main
// String instead of char* + function conversion p_string.ascii().get_data()

String Arduino_f::get_string_com() {
    std::lock_guard<std::mutex> lock(m_mutex_get);
    return m_data;
}

void Arduino_f::set_string_com(String p_string){
    std::lock_guard<std::mutex> lock(m_mutex_set);
    memset(m_data_to_arduino_string,'\0',sizeof (m_data_to_arduino_string));
    strncpy(m_data_to_arduino_string,p_string.ascii().get_data() , sizeof(m_data_to_arduino_string));
    return ;
}

#else // # ifndef TEST_arduino_f_module_in_main

char* Arduino_f::get_string_com() {
    std::lock_guard<std::mutex> lock(m_mutex_get);
    return m_data;
}

void Arduino_f::set_string_com(char* p_string){
    std::lock_guard<std::mutex> lock(m_mutex_set);
    memset(m_data_to_arduino_string,'\0',sizeof (m_data_to_arduino_string));
    
    strncpy(m_data_to_arduino_string,p_string , sizeof(m_data_to_arduino_string));
    return ;
}

#endif // #ifndef TEST_arduino_f_module_in_main


#ifndef TEST_arduino_f_module_in_main

void Arduino_f::_bind_methods(){
    ClassDB::bind_method(D_METHOD("Thread_print"), &Arduino_f::Thread_print);
    ClassDB::bind_method(D_METHOD("Thread_generate_int"), &Arduino_f::Thread_generate_int);
    ClassDB::bind_method(D_METHOD("Thread_generate_com"), &Arduino_f::Thread_generate_com);
    ClassDB::bind_method(D_METHOD("get_string_com"), &Arduino_f::get_string_com);
    ClassDB::bind_method(D_METHOD("set_string_com","value"), &Arduino_f::set_string_com);
    ClassDB::bind_method(D_METHOD("stop_thread_print"), &Arduino_f::stop_thread_print);
    ClassDB::bind_method(D_METHOD("stop_thread_generate_int"), &Arduino_f::stop_thread_generate_int);
    ClassDB::bind_method(D_METHOD("stop_thread_generate_com"), &Arduino_f::stop_thread_generate_com);
}

#endif // #ifndef TEST_arduino_f_module_in_main
