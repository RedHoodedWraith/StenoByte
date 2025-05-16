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
