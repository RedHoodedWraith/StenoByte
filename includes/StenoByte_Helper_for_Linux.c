/**
    StenoByte: a stenotype inspired keyboard app for typing out bytes.

    StenoByte_Helper.c is the source file for implementing the Reading & Processing of Keyboard Events.

    This file is intended to be modular and to be used when building for Linux Operating Systems.

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
#include "StenoByte_Core.h"

// Struct to store the evdev device
struct libevdev *keyboard_device = nullptr;
struct termios original_terminal_settings;    // Termios Struct to store original terminal settings
const int event_file_device;


/*
 * Sets up the application and configures the devices to read from
 *
 * Returns 0 if there were no errors, 1 if there were errors
 */
int setup_stenobyte() {
    setup_subvalues_array();

    const int event_file_device = open("/dev/input/event3", O_RDONLY | O_NONBLOCK); // Change to the correct device

    // Opens the keyboard event file (usually event3) in Read-Only and Non-Blocking Modes
    // Reports an error if something went wrong

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
    return 0;
}

/*
 * Updates the current bits in the array and whether the array is ready to be computed into a byte (which is when
 * the space bar is pressed)
 */
void update_bit_arr(const int key_code, const bool new_state) {
    switch (key_code) {
        case KEY_A:
            bit_arr[7] = new_state;
            return;
        case KEY_S:
            bit_arr[6] = new_state;
            return;
        case KEY_D:
            bit_arr[5] = new_state;
            return;
        case KEY_F:
            bit_arr[4] = new_state;
            return;
        case KEY_J:
            bit_arr[3] = new_state;
            return;
        case KEY_K:
            bit_arr[2] = new_state;
            return;
        case KEY_L:
            bit_arr[1] = new_state;
            return;
        case KEY_SEMICOLON:
            bit_arr[0] = new_state;
            return;
        case KEY_SPACE:
            // sets ready_to_compute_byte to true if new_state is true, else leaves it as it is
            // ready_to_compute_byte should to be set to false after it computing the byte
            ready_to_compute_byte = new_state ? true : ready_to_compute_byte;
        default: ;
    }
}

/*
 * Runs the loop that constantly checks the keyboard events and performs the associated actions
 */
void run_stenobyte() {
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

        // If the ESC Key is pushed, then exit the app
        if (current_event.code == KEY_ESC) {
            printf("ESC pressed\nExiting...\n");
            break;
        }

        process_key_presses(&current_event);
        action();

        if (ready_to_compute_byte) {
            compute_byte();
        }

        usleep(1000); // Small delay
    }
}

void end_stenobyte() {
    // Frees up resources before application ends
    libevdev_free(keyboard_device);
    restore_terminal(); // Restores printing inputs to the terminal
    close(event_file_device);
}

/*
 * Checks whether a valid key is pressed
 */
bool is_valid_key(const int key_code) {
    switch (key_code) {
        case KEY_A:
        case KEY_S:
        case KEY_D:
        case KEY_F:
        case KEY_J:
        case KEY_K:
        case KEY_L:
        case KEY_SEMICOLON:
        case KEY_SPACE:
            return true;
        default:
            return false;
    }
}

/*
 * Prints the event summary of a key press. Used for debugging.
 */
void print_event_summary(const struct input_event* current_event) {
    if (current_event == NULL) {
        return;
    }

    if (current_event->type != EV_KEY) {
        return;
    }

    printf("Key %s (%d): code %d\n\n",
           current_event->value ? (current_event->value == EV_KEY_PRESSED ? "PRESSED" : "REPEATED") : "RELEASED",
           current_event->value,    // Prints what happened in the event
           current_event->code);    // Prints the ID for the key that is affected
}

/*
 * Processes the key events, such as key presses, key releases, and key repeats (when the key is held down)
 */
void process_key_presses(const struct input_event* current_event) {
    // Exits method if event is null
    if (current_event == NULL) {
        return;
    }

    // Exits method if the event_type variable is not related to a key event
    if (current_event->type != EV_KEY) {
        return;
    }

    // Exits method if event is for an irrelevant key
    if (!is_valid_key(current_event->code)) {
        return;
    }

    // Sets the bit value in the array to zero if the associated key is released
    // Then exits the method
    if (current_event->value == EV_KEY_RELEASED) {
        update_bit_arr(current_event->code, false);
        return;
    }

    // Exits method if key even is not a key press or key repeat
    if (current_event->value != EV_KEY_PRESSED && current_event->value != EV_KEY_REPEATED) {
        return;
    }

    // Updates Bit Value in Array
    update_bit_arr(current_event->code, true);
}

/*
 * Disables echoing/printing key presses in terminal
 */
void disable_echo() {
    struct termios temporary_terminal_settings;
    tcgetattr(STDIN_FILENO, &original_terminal_settings); // Get current terminal settings
    temporary_terminal_settings = original_terminal_settings;   // Copy original settings to temporary settings
    temporary_terminal_settings.c_lflag &= ~ECHO; // Disable ECHO flag
    tcsetattr(STDIN_FILENO, TCSANOW, &temporary_terminal_settings);   // Apply temporary settings
}

/*
 * Restores original terminal settings before this program running
 */
void restore_terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal_settings); // restore settings
}
