#include <linux/input.h>
#include <libevdev/libevdev.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

// The starting code was generated from ChatGPT to get an example code working

int main() {
    // Struct to store the evdev device
    struct libevdev *dev = NULL;

    // Opens the keyboard event file (usually event3) in Read Only and Non-Blocking Modes
    // Reports an error if something went wrong
    int fd = open("/dev/input/event3", O_RDONLY | O_NONBLOCK); // Change to correct device
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


    struct input_event ev;  // The current event
    while (1) {
        const int rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);  // Gets the next event
        if (rc == 0) {
            if (ev.type == EV_KEY) {
                printf("Key %s (%d): code %d\n",
                       ev.value ? (ev.value == 1 ? "PRESSED" : "REPEATED") : "RELEASED",
                       ev.value,
                       ev.code);
            }
        }
        usleep(1000); // Small delay
    }

    libevdev_free(dev);
    close(fd);
    return 0;
}
