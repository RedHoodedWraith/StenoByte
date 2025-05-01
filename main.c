/**
    StenoByte: a stenotype inspired keyboard app for typing out bytes.
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

#include <linux/input.h>
#include <libevdev/libevdev.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

// Key Press States
#define EV_KEY_RELEASED 0
#define EV_KEY_PRESSED 1
#define EV_KEY_REPEATED 2

// Bit Array that contains the bits that forms a byte
bool bit_arr[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // ordered from b0 to b7 during initialisation
bool ready_to_compute_byte = false;  // the state for whether to convert the bit array into a byte and process it

u_int8_t current_byte = 0x00;  // The byte last computed from the bit array


/*
 * Generates the Byte based on the bits in the array
 */
void compute_byte() {
    current_byte = 0x00;
    for (int i = 0; i < 8; i++) {
        current_byte = current_byte ^ bit_arr[i] << i;
    }
    ready_to_compute_byte = false;
}

/*
 * Prints the event summary of a key press *
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
 * Prints the current state of the Bit Array
 */
void print_bit_arr_summary() {
    printf("\nBits in Array: ");
    for (int i = 7; i >= 0; i--) {
        printf("b[%d]: %d", i, bit_arr[i]);
        if (i > 0) {
            printf(", ");
        }
    }
    printf("\n");
}

void print_byte_summary() {
    printf("Last Computed Byte as decimal: %d\n", current_byte);
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
 * Processes the key events, such as key presses, key releases, and key repeats (when the key is held down)
 */
void process_key_presses(const struct input_event* current_event) {
    // Exits method if event is null
    if (current_event == NULL) {
        return;
    }

    // Exits method if event type is not related to a key event
    if (current_event->type != EV_KEY) {
        return;
    }

    // Exits method if event is for an irrelevant key
    if (!is_valid_key(current_event->code)) {
        return;
    }

    // Sets the bit value in array to zero if the associated key is released
    // Then exits method
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


int main() {
    // Struct to store the evdev device
    struct libevdev *keyboard_device = nullptr;

    // Opens the keyboard event file (usually event3) in Read Only and Non-Blocking Modes
    // Reports an error if something went wrong
    const int event_file_device = open("/dev/input/event3", O_RDONLY | O_NONBLOCK); // Change to correct device
    if (event_file_device < 0) {
        perror("Failed to open device");
        return 1;
    }

    // Grab the device to prevent event propagation
    if (ioctl(event_file_device, EVIOCGRAB, (void*)1) == -1) {
        perror("Failed to grab device");
        close(event_file_device);
        return 1;
    }

    // Initialises the evdev device (stored in libevdev struct named "keyboard_device")
    // Reports an error if evdev initialisation failed
    if (libevdev_new_from_fd(event_file_device, &keyboard_device) < 0) {
        perror("Failed to init libevdev");
        return 1;
    }

    // Prints evdev device name
    printf("Input device name: %s\n", libevdev_get_name(keyboard_device));
    printf("Keyboard will locked to this program while it is running.");
    printf("Press ESC to exit\n");


    struct input_event current_event;  // The current event struct

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

        // print_event_summary(&current_event);

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
        print_byte_summary();

        usleep(1000); // Small delay
    }

    // Frees up resources before application ends
    libevdev_free(keyboard_device);
    ioctl(event_file_device, EVIOCGRAB, (void*)0); // Release grab when done
    close(event_file_device);
    return 0;
}
