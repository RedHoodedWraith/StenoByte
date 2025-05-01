/**
StenoByte_Helper.h: Header File for StenoByte Tools
    Copyright (C) 2025  Asami De Almeida

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
