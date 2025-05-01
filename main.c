#include <linux/input.h>
#include <libevdev/libevdev.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

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

    // Initialises the evdev device (stored in libevdev struct named "keyboard_device"))
    // Reports an error if evdev initialisation failed
    if (libevdev_new_from_fd(event_file_device, &keyboard_device) < 0) {
        perror("Failed to init libevdev");
        return 1;
    }

    // Prints evdev device name
    printf("Input device name: %s\n", libevdev_get_name(keyboard_device));
    printf("Press ESC to exit\n");


    struct input_event current_event;  // The current event struct
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

        // Prints the Key Event Summary
        printf("\nKey %s (%d): code %d\n\n",
               current_event.value ? (current_event.value == 1 ? "PRESSED" : "REPEATED") : "RELEASED",
               current_event.value,
               current_event.code);

        // If ESC Key is pushed, then exit app
        if (current_event.code == KEY_ESC) {
            printf("ESC pressed\nExiting...\n");
            break;
        }

        usleep(1000); // Small delay
    }

    // Frees up resources before application ends
    libevdev_free(keyboard_device);
    close(event_file_device);
    return 0;
}
