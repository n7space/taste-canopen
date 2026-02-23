# CANopen TASTE component

This component provides a singular CANopen device node, with it's configuration based on a DCF file.

That node provides interfaces for:

- Initialization,
- Configuring its own state via NMT,
- Configuring the state of other nodes in the network via NMT,
- Manipulating its Object Dictionary,
- Indicating NMT, SYNC and heartbeat events.

In order for this node to work, it requires an external time provider and CAN bus interfaces.

## Building the component

In order to build this component, `dcf2dev` from `lely-core` library must be installed and available in your PATH. Installation script called `install-dcf-tools.sh` should be provided with your TASTE installation, in `tool-src/add-ons` directory. Alternatively, it can be copied there from the repository containing this demo project.

## Configuration

This component can be configured by in two ways

- By modifying the `canopen/c/src/master_dev.dcf` file (CANopen node configuration)
- By defining C macros (component configuration)

### CANopen node configuration

In order to configure the CANopen device, you must edit DCF file representing it: `canopen/c/src/master_dev.dcf`.
The component will automatically (re-)generate `canopen/c/src/master_dev.h` file containing the representation of the node in C language at build time, using `dcf2dev` tool from `lely-core`.

### Component configuration

This component can be configured by defining following C macros:

- `CANOPEN_MEMORY_POOL_SIZE` - specifies the size of statically-allocated memory pool for CANopen node. Default value is 30720 bytes, increase if necessary (e.g. initialization of the node fails),
- `CANOPEN_DEBUG` - when defined, it will enable `printf`-based debug logs from the component,
- `CANOPEN_LINUX` - when defined, it will allow to use SocketCAN, should be defined when running on Linux,
- `CANOPEN_STDIO` - when defined, it will allow `lely-core` to use `stdio.h`.

## lely-core integration

This component integrates `lely-core` library directly, as the source code, so no external libraries are required.

The code was copied directly from `ecss` branch of [N7 Space lely-core repository](https://gitlab.com/n7space/canopen/lely-core).
Since the library is typically built using GNU Autotools, it also contains a `config.h` header, generated for ECSS-compatible build.

Some modifications were necessary in order to make this integration work, namely:

- `HAVE_CONFIG_H` macro checks have been commented out, there's no need to define it externally as this header is included with the component,
- Source file names have been prefixed with their respective module names, in order to prevent name collisions.
