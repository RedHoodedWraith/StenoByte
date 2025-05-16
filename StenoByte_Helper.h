/**
    StenoByte: a stenotype inspired keyboard app for typing out bytes.

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

// Key Press States
#define EV_KEY_RELEASED 0
#define EV_KEY_PRESSED 1
#define EV_KEY_REPEATED 2

// Number of Bits in the Bit Array (should be 8)
# define BITS_ARR_SIZE 8

extern struct termios original_terminal_settings;    // Termios Struct to store original terminal settings

// Arrays & Variables
// Bit Array that contains the bits that forms a byte
extern bool bit_arr[BITS_ARR_SIZE]; // ordered from b0 to b7 during initialisation
extern char keys_arr[BITS_ARR_SIZE]; // ';' = b0, 'L' = b1, ... 'A' = b7
extern u_int8_t subvalues_arr[BITS_ARR_SIZE];
extern bool ready_to_compute_byte;  // the state for whether to convert the bit array into a byte and process it

extern u_int8_t current_byte;  // The byte last computed from the bit array


// Methods & Functions

bool is_valid_key(int key_code);

void update_bit_arr(int key_code, bool new_state);

void compute_byte();

void setup_subvalues_array();

void print_event_summary(const struct input_event* current_event);

void print_byte_summary();

void print_bit_arr_summary();

void process_key_presses(const struct input_event* current_event);

void disable_echo();

void restore_terminal();

#endif //STENOBYTE_HELPER_H
