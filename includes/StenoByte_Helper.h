/**
    StenoByte: a stenotype inspired keyboard app for typing out bytes.

    StenoByte_Helper.h is the header file for defining resources for Keyboard Event Reading for a range of
    Operating Systems.

    TODO: Implement conditional declarations that will be dependent on the target OS.

	Copyright 2025 Asami De Almeida

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */

#ifndef STENOBYTE_HELPER_H
#define STENOBYTE_HELPER_H

#include <linux/input.h>
#include <libevdev/libevdev.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// Key Press States
#define EV_KEY_RELEASED 0
#define EV_KEY_PRESSED 1
#define EV_KEY_REPEATED 2

// Externally Declared Structs and Variables (expected to be declared and implemented in dependencies)
extern struct libevdev *keyboard_device; // Struct to store the evdev device
extern struct termios original_terminal_settings;    // Termios Struct to store original terminal settings
extern const int event_file_device;


// Externally Declared Methods & Functions
// (expected to be declared & defined in dependent libraries or in StenoByte_Core files)
extern void setup_subvalues_array();    // StenoByte_Core.h/c
extern void action();   // Run the core action in the loop
extern void compute_byte();
extern void print_bit_arr_summary();
extern void update_bit_arr(int key_code, bool new_state);


// Methods & Functions
int setup_stenobyte();
void update_bit_arr(int key_code, bool new_state);
void run_stenobyte();
void end_stenobyte();
void process_key_presses(const struct input_event* current_event);
bool is_valid_key(int key_code);
void print_event_summary(const struct input_event* current_event);
void disable_echo();
void restore_terminal();

#endif //STENOBYTE_HELPER_H
