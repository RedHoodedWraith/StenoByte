#include <linux/input.h>
#include <libevdev/libevdev.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    // Struct to store the evdev device
    struct libevdev *dev = nullptr;

    // Opens the keyboard event file (usually event3) in Read Only and Non-Blocking Modes
    // Reports an error if something went wrong
    const int fd = open("/dev/input/event3", O_RDONLY | O_NONBLOCK); // Change to correct device
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    // Initialises the evdev device (stored in libevdev struct named dev)
    // Reports an error if evdev initialisation failed
    if (libevdev_new_from_fd(fd, &dev) < 0) {
        perror("Failed to init libevdev");
        return 1;
    }

    // Prints evdev device name
    printf("Input device name: %s\n", libevdev_get_name(dev));
    printf("Press ESC to exit\n");


    struct input_event ev;  // The current event
    while (1) {
        const int rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);  // Gets the next event

        // Ignores Non-Success Read Events
        if (rc != LIBEVDEV_READ_STATUS_SUCCESS) {
            continue;
        }

        // Ensures a Key Event Type Occurred, ignores otherwise
        if (ev.type != EV_KEY) {
            continue;
        }

        // Prints the Key Event Summary
        printf("\nKey %s (%d): code %d\n\n",
               ev.value ? (ev.value == 1 ? "PRESSED" : "REPEATED") : "RELEASED",
               ev.value,
               ev.code);

        // If ESC Key is pushed, then exit app
        if (ev.code == KEY_ESC) {
            printf("ESC pressed\nExiting...\n");
            break;
        }

        usleep(1000); // Small delay
    }

    // Frees up resources before application ends
    libevdev_free(dev);
    close(fd);
    return 0;
}
