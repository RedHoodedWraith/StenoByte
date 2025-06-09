/**
    StenoByte: a stenotype inspired keyboard app for typing out bytes.

    StenoByte_Core.c is the source file for implementing resources related to processing the Bits into a Byte and
    for producing the result as an output.

    This file is intended to be Operating System agnostic and to be compiled for any Operating System.

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

#include "StenoByte_Core.h"

// Arrays & Variables
// Bit Array that contains the bits that forms a byte
bool bit_arr[BITS_ARR_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0}; // ordered from b0 to b7 during initialisation
char keys_arr[BITS_ARR_SIZE] = {';', 'L', 'K', 'J', 'F', 'D', 'S', 'A'}; // ';' = b0, 'L' = b1, ... 'A' = b7
u_int8_t subvalues_arr[BITS_ARR_SIZE];
bool ready_to_compute_byte = false;  // the state for whether to convert the bit array into a byte and process it
const char* output_file_path;  // The path to the file write to
FILE *output_file_ptr;  // The pointer of the file itself to write to
enum stenobyte_mode mode = NOT_SET;

u_int8_t current_byte = 0x00;  // The byte last computed from the bit array


// Methods & Functions

int setup_stenobyte_demo() {
    printf("Starting StenoType...\n");
    mode = DEMO;
    return setup_stenobyte();
}

int setup_stenobyte_writer(int argc, const char* argv[]) {
    if (argc < 2) {
        // Default File Path if none is provided
        output_file_path = "./output.txt";
    } else {
        // File Path Provided by Command Line Arguments
        output_file_path = argv[1];
    }

    printf("Welcome to StenoByte Writer.\nWriting to file: %s\n", output_file_path);
    output_file_ptr = fopen(output_file_path, "w");
    mode = WRITER;
    return setup_stenobyte();
}

void action() {
    // TODO: this could probably be done once during setup by setting a pointer to the function to run as the action
    switch (mode) {
        case DEMO:
            print_bit_arr_summary();
            break;
        case WRITER:
            print_bit_arr_summary();
            write_byte_to_file();
            break;
        default:
            perror("No StenoByte Mode Set");
            exit(EXIT_FAILURE);
    }
}

/*
 * Generates the Byte based on the bits in the array
 */
void compute_byte() {
    current_byte = 0x00;    // Resets the Byte to zero
    for (int i = 0; i < BITS_ARR_SIZE; i++) {
        current_byte = current_byte ^ bit_arr[i] << i;
    }
    ready_to_compute_byte = false;
}

/*
 * Sets up the sub-values array for labelling
 */
void setup_subvalues_array() {
    for (int i = BITS_ARR_SIZE; i >= 0; i--) {
        const u_int8_t val = 0;
        subvalues_arr[i] = val ^ 1 << i;
    }
}

/*
 * Gets Byte Summary as a String (an array of chars)
 * Assumes msg has a minimum length of 35. Assumes value of current_byte will not exceed 255.
 */
void get_byte_summary(char* msg) {
    sprintf(msg + strlen(msg), "Last Computed Byte as decimal: %d\n", current_byte);  // Prints between 33 and 35 chars
    sprintf(msg + strlen(msg), R"(Last Computed Byte as Raw Value: %c)", (char) current_byte);
}

/*
 * Prints the Byte Summary
 */
void print_byte_summary() {
    char msg[35];
    get_byte_summary(msg);
    printf("%s", msg);
}

void print_current_mode(char* msg) {
    // Min Chars Printed: 11
    // Max Chars Printed: 19
    sprintf(msg+strlen(msg), "Mode: "); // Prints 6 chars

    switch (mode) {
        case DEMO:
            sprintf(msg+strlen(msg), "DEMO");   // Prints 4 chars
            break;
        case WRITER:
            sprintf(msg+strlen(msg), "WRITER"); // Prints 6 chars
            break;
        case NOT_SET:
            sprintf(msg+strlen(msg), "NOT_SET");    // Prints 7 chars
            break;
        default:
            sprintf(msg+strlen(msg), "UNKNOWN MODE");   // Prints 12 chars
            break;
    }

    sprintf(msg+strlen(msg), "\n"); // Prints 1 char
}

/*
 * Prints the current state of the Bit Array
 * TODO: The repeated for loops could probably be simplified into a dedicated method
 */
void print_bit_arr_summary() {
    char msg[673] = "";

    print_current_mode(msg);    // Prints between 11 and 19 chars
    sprintf(msg + strlen(msg), "\nBits in Array:\n");   // Prints 16 chars
    sprintf(msg + strlen(msg), "\tBit Value:\t| "); // Prints 14 chars
    for (int i = 7; i >= 0; i--) {  // Repeats 8 times
        sprintf(msg + strlen(msg), "\t%d\t|", bit_arr[i]);  // Prints between 4 and 6 chars
    }
    sprintf(msg + strlen(msg), "\n");   // Prints 1 char

    for (int i=0; i<24+16*BITS_ARR_SIZE; i++) { // Repeats 24+(16*8) times, which is 152
        sprintf(msg + strlen(msg), "-");    // Prints 1 char
    }

    sprintf(msg + strlen(msg), "\n\tSub-Value:\t|");    // Prints 14 chars
    for (int i = 7; i >= 0; i--) {  // Repeats 8 times
        sprintf(msg + strlen(msg), "\t[%d]\t|", subvalues_arr[i]);  // Prints between 6 and 8 chars
    }

    sprintf(msg + strlen(msg), "\n\tBit Index:\t|");    // Prints 14 chars
    for (int i = 7; i >= 0; i--) {  // Repeats 8 times
        sprintf(msg + strlen(msg), "\t[b%d]\t|", i);    // Prints 7 chars
    }
    sprintf(msg + strlen(msg), "\n\tKey:\t\t|");    // Prints 9 times

    for (int i = 7; i >= 0; i--) {  // Repeats 8 times
        sprintf(msg + strlen(msg), "\t[%c]\t|", keys_arr[i]);   // Prints 6 chars
    }
    sprintf(msg + strlen(msg), "\n");   // Prints 1 char
    get_byte_summary(msg);   // Prints between 33 and 35 chars
    sprintf(msg + strlen(msg), "\nPress & Hold the keys corresponding to the bits in the"
                               " byte you would like to set to 1.");    // Prints 88 chars
    sprintf(msg + strlen(msg), "\nBits will be 0 if keys are not pressed.");    // Prints 40 chars
    sprintf(msg + strlen(msg), "\nPress SPACE BAR to compute Byte\t\t|\t"
                               "Press ESC to exit\n");  // Prints 53 chars

    printf("%s", msg);
}

void write_byte_to_file() {
    printf("This is where the writing will happen...\n");
}

/*
 * Closes the File and frees up memory safely
 */
void end_stenobyte_writer() {
    fclose(output_file_ptr);
    end_stenobyte();
}