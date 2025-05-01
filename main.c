#include <linux/input.h>
#include <libevdev/libevdev.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    struct libevdev *dev = NULL;
    int fd = open("/dev/input/event3", O_RDONLY | O_NONBLOCK); // Change to correct device
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    if (libevdev_new_from_fd(fd, &dev) < 0) {
        perror("Failed to init libevdev");
        return 1;
    }

    printf("Input device name: %s\n", libevdev_get_name(dev));

    struct input_event ev;
    while (1) {
        int rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
        if (rc == 0) {
            if (ev.type == EV_KEY) {
                printf("Key %s: code %d\n",
                       ev.value ? (ev.value == 1 ? "PRESSED" : "REPEATED") : "RELEASED",
                       ev.code);
            }
        }
        usleep(1000); // Small delay
    }

    libevdev_free(dev);
    close(fd);
    return 0;
}
