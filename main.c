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

#include "StenoByte_Helper.h"


int main() {
    printf("Starting StenoType...\n");
    // Struct to store the evdev device
    struct libevdev *keyboard_device = nullptr;

    setup_subvalues_array();

    // Opens the keyboard event file (usually event3) in Read Only and Non-Blocking Modes
    // Reports an error if something went wrong
    const int event_file_device = open("/dev/input/event3", O_RDONLY | O_NONBLOCK); // Change to correct device
    if (event_file_device < 0) {
        perror("Failed to open device");
        return 1;
    }

    // Disables printing inputs to the terminal
    disable_echo();

    // Initialises the evdev device (stored in libevdev struct named "keyboard_device")
    // Reports an error if evdev initialisation failed
    if (libevdev_new_from_fd(event_file_device, &keyboard_device) < 0) {
        perror("Failed to init libevdev");
        return 1;
    }

    // Prints evdev device name
    printf("Input device name: %s\n", libevdev_get_name(keyboard_device));
    printf("Press ESC to exit\n");


    struct input_event current_event;  // The current event struct

    print_bit_arr_summary();    // Initial Print Summary

    // Infinitely loops by default
    while (1) {
        // Gets the next event
        const int next_event_result_code = libevdev_next_event(keyboard_device,
            LIBEVDEV_READ_FLAG_NORMAL, &current_event);

        // Ignores Non-Success Read Events
        if (next_event_result_code != LIBEVDEV_READ_STATUS_SUCCESS) {
            continue;
        }

        // Ensures a Key Event Type Occurred, ignores otherwise
        if (current_event.type != EV_KEY) {
            continue;
        }

        // If ESC Key is pushed, then exit app
        if (current_event.code == KEY_ESC) {
            printf("ESC pressed\nExiting...\n");
            break;
        }

        process_key_presses(&current_event);
        print_bit_arr_summary();

        if (ready_to_compute_byte) {
            compute_byte();
        }

        usleep(1000); // Small delay
    }

    // Frees up resources before application ends
    libevdev_free(keyboard_device);
    restore_terminal(); // Restores printing inputs to the terminal
    close(event_file_device);
    return 0;
}
