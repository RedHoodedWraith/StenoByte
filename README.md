# StenoByte Prototype
A stenotype inspired keyboard app for typing out bytes. Uses certain keys from your existing QWERTY keyboard layout.
This is currently designed to work in Linux.

# Codes
When developing this application, the following codes from the keyboard correspond to the following keys and events.

## Keys
* a - 30
* s - 31
* d - 32
* f - 33

* j - 36
* k - 37
* l - 38
* ; - 39

* SPACE - 57

## Events
* 0 - Released
* 1 - Pressed
* 2 - Repeated



# Known Dependencies
* libevdev

# Resources Referenced
* https://www.freedesktop.org/software/libevdev/doc/latest/index.html
* ChatGPT to generate example code to start from (prompt: "I would like to detect which keys are still pressed and
which are not")