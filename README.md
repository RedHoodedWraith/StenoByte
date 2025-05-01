# StenoByte Prototype
A stenotype inspired keyboard app for typing out bytes. Uses certain keys from your existing QWERTY keyboard layout.
This is currently designed to work in Linux. 

For the time-being, this app requires elevated privileges (e.g. `sudo`) to run.

## How to Compile

### Step 1: Install the Dependencies
Ensure the following dependencies are installed:
* gcc
* cmake
* libevdev

The steps assume for installing the dependencies assume the repositories that contain your dependencies are already
configured with your operating system's package manager.

#### Install Dependencies on Fedora, CentOS, Red Hat
```shell
sudo dnf -y update
sudo dnf -y install gcc cmake git libevdev libevdev-devel
```

#### Install Dependencies on Debian, Ubuntu, Mint
```shell
sudo apt update
sudo apt -y install build-essential cmake git libevdev-dev
```

#### Install Dependencies on Arch, Manjaro (to be confirmed)
```shell
sudo pacman -S gcc cmake git libevdev
````

### Step 2: Clone this repository
Run: 
```shell
git clone git@github.com:RedHoodedWraith/StenoByte.git
```

### Step 3: Build with cmake and make
Run:
```shell
cd ./StenoByte
mkdir output
cd ./output
cmake ..
```

If cmake runs successfully, then run:
```shell
make
```

### Step 4: Run the program/application
If `make` runs successfully, then run:
```shell
sudo ./StenoByte_Prototype
```

To exit the app, press the `Esc` key on your keyboard.

## Resources Referenced
* https://www.freedesktop.org/software/libevdev/doc/latest/index.html
* ChatGPT to generate example code to start from (prompt: "I would like to detect which keys are still pressed and
which are not")

## Information for Developers
Check out the [Development Info](DEVELOPMENT_INFO.md) page for some useful information if you are developing or
maintaining this repository.

## License
Copyright (C) 2025  Asami De Almeida

Released under the GPL 3 License. See the following pages for more information:
* [License Summary](LICENSE_SUMMARY.md)
* [Full GPL 3 License](LICENSE)