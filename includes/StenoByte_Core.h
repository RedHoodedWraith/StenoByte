/**
    StenoByte: a stenotype inspired keyboard app for typing out bytes.

    StenoByte_Core.h is the header file for defining resources related to processing the Bits into a Byte and
    for producing the result as an output.

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

#ifndef STENOBYTE_CORE_H
#define STENOBYTE_CORE_H

#include "StenoByte_Helper.h"

// Number of Bits in the Bit Array (should be 8)
# define BITS_ARR_SIZE 8

// Arrays & Variables
// Bit Array that contains the bits that forms a byte
extern bool bit_arr[BITS_ARR_SIZE]; // ordered from b0 to b7 during initialisation
extern char keys_arr[BITS_ARR_SIZE]; // ';' = b0, 'L' = b1, ... 'A' = b7
extern u_int8_t subvalues_arr[BITS_ARR_SIZE];
extern bool ready_to_compute_byte;  // the state for whether to convert the bit array into a byte and process it
extern u_int8_t current_byte;  // The byte last computed from the bit array
extern const char* output_file_path;  // The path to the file write to
extern FILE *output_file_ptr;  // The pointer of the file itself to write to


// Externally Declared Methods & Functions (expected to be declared & defined StenoByte_Helper files)
extern void update_bit_arr(int key_code, bool new_state);
extern int setup_stenobyte();
extern void run_stenobyte();
extern void end_stenobyte();

// Methods & Functions
int setup_stenobyte_demo();
int setup_stenobyte_writer(int argc, const char* argv[]);
void compute_byte();
void setup_subvalues_array();
void get_byte_summary(char* msg);
void print_byte_summary();
void print_bit_arr_summary();
void end_stenobyte_writer();

#endif //STENOBYTE_CORE_H
