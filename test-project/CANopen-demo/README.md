# Demo project for CANopen TASTE module

This project provides an example of CANopen module usage. It utilizes Virtual CAN interface for communication (via SocketCAN).

## Prerequisites

### Building

In order to build this project, `dcf2dev` from `lely-core` library must be installed and available in your PATH. Installation script called `install-dcf-tools.sh` should be provided with your TASTE installation, in `tool-src/add-ons` directory. Alternatively, it can be copied there from the repository containing this demo project.

### Running

In order to run this project, you must make sure to create a Virtual CAN interface called `vcan0` and enable it. You can run `create-vcan-interface.sh` script to do that, but you must make sure to have `vcan` Linux kernel module available.
Note that the `vcan0` interface created with `create-vcan-interface.sh` script will not persist past system reboot.

On Debian/Ubuntu, in order to install `vcan` module, run following command:

```sh
sudo apt update && sudo apt install linux-modules-extra-$(uname -r)
```

Additionally, it's highly recommended to install `can-utils` package in order to monitor the network and send custom CANopen packets for testing.
Installing this package is **required** for running the `test-canopen` target.

```sh
sudo apt update && sudo apt install can-utils
```

*Note for WSL2 users: Installing `vcan` module requires re-compiling the Linux kernel with `CONFIG_CAN=y` and `CONFIG_CAN_VCAN=y` in the configuration file. Kernel re-compilation process description is out of the scope of this README. It's recommended to use a standard virtual machine, or native Linux installation.*

## Running the demo

The demo can be run either by building the binary via `make` and manually running `./work/binaries/demo`, or by running `make test-canopen` target.

Manually running the demo will start up a CANopen node with NodeID == 1 that will boot-up after 1 second, keep incrementing a counter in it's object dictionary every 2 seconds, and will stop the node after 5 seconds.
The demo node is also configured to send SYNC and heartbeat messages every 500 milliseconds. It's configuration can be freely modified by editing the content of `master_dev.dcf` file in `work/canopen/C/src` directory.
The `master_dev.h` file representing the CANopen node will be automaticaly re-generated after re-building the project.

All the events will be logged to standard output. CAN traffic can be monitored by running `candump vcan0` in another terminal.

After stopping the node, the demo will keep running and incrementing the counter in object dictionary, but node will stay in stopped state, waiting for external NMT command to boot-up.

Alternatively, if `can-utils` module is installed, `test-canopen` target can be executed via `make test-canopen` and it will trigger node re-start after 6 seconds by sending a NMT boot-up packet.
