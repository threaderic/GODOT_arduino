#ifndef ARDUINO_H
#define ARDUINO_H

////////////////////////////////////////////////////////////
/// arduino_f.h
/// \brief arduino class module for godot 3.1.2 stable
/// \name Threaderic
/// class from godot point of view
/// get (from arduino to godot)
/// set (from godot to arduino)
/// \param m_data characters from arduino to godot
/// \param m_data_to_arduino_string characters from godot to arduino
/// \return
///
/// \see
///
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// \def if TEST_arduino_f_module_in_main used, this module can be used in a main program (standalone, out of Godot for test)
/// #define TEST_arduino_f_module_in_main

////////////////////////////////////////////////////////////
// Headers
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <thread>
#include <atomic>
#include <string>
#include <string.h>
#include <unistd.h>
#include <array>
#include <mutex>

#include "ceSerial.h"

using namespace std;
using namespace ce;

#ifndef TEST_arduino_f_module_in_main

#include "core/reference.h"

class Arduino_f : public Reference
{
    GDCLASS(Arduino_f, Reference);

#else //#ifndef TEST_arduino_f_module_in_main

class Arduino_f
{
#endif //#ifndef TEST_arduino_f_module_in_main

    public:
        /// \brief defaut Constructor, connection to the serial port for communication with arduino, loop_speed = 10 ms
        Arduino_f();
        /// \brief Constructor, connection to the serial port for communication with arduino, set loop m_loop_speed with another speed
        Arduino_f(int loop_speed);
        /// \brief Destructor
        ~Arduino_f();

        /// \brief create a thread for function print() -> cout m_data (print m_data)
        /// \see print()
        void Thread_print();

        /// \brief create a thread for function generate_int(bool&) -> increase int++ till m_thread_generate_int_bool false
        /// \see generate_int()
        void Thread_generate_int();

        /// \brief create a thread for function generate_com -> communicate from and to the arduino every 10 ms
        /// \see generate_com()
        void Thread_generate_com();

        /// \brief stop Thread_print (join to main thread)
        void stop_thread_print();

        /// \brief stop Thread_generate_int (join to main thread)
        void stop_thread_generate_int();

        /// \brief stop Thread_generate_com (join to main thread)
        void stop_thread_generate_com();

        #ifndef TEST_arduino_f_module_in_main
        // String instead of char* for Godot
        
        /// \brief get (from arduino to godot) string
        String get_string_com();
        
        void set_string_com(String p_string);
        #else       // #ifndef TEST_arduino_f_module_in_main
        
        /// \brief get (from arduino to godot) char*
        char* get_string_com();
        
        void set_string_com(char* p_string);
        #endif      // #ifndef TEST_arduino_f_module_in_main

        #ifndef TEST_arduino_f_module_in_main
    protected:
        static void _bind_methods();
        #endif      // #ifndef TEST_arduino_f_module_in_main
    private:
        char m_data[100];
        char m_data_to_arduino_string[20];
        int m_loop_speed;
        std::thread m_thread_print;
        std::thread m_thread_generate_int;
        std::thread m_thread_generate_com;
        std::atomic<bool> m_thread_generate_com_bool;
        std::atomic<bool> m_thread_generate_int_bool;
        std::mutex m_mutex_get;
        std::mutex m_mutex_set;
        ceSerial com;

        void print() const;

        void generate_int(std::atomic<bool>& boolean_flag);

        void generate_com();
    };

    #endif // ARDUINO_H
