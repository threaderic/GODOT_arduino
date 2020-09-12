/* register_types.cpp */

#include "register_types.h"

#include "core/class_db.h"
#include "arduino_f.h"

void register_arduino_f_types() {
    ClassDB::register_class<Arduino_f>();
}

void unregister_arduino_f_types() {
   // Nothing to do here in this example.
}
