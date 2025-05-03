/**
    StenoByte: a stenotype inspired keyboard app for typing out bytes.
    Copyright (C) 2025  Asami De Almeida

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

#ifndef STENOBYTE_HELPER_H
#define STENOBYTE_HELPER_H

#include <linux/input.h>
#include <libevdev/libevdev.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

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
