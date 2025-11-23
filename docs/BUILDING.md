# Building Applications

This guide covers everything you need to know about building, configuring, and debugging Zephyr applications in the T3 forest template.

## Table of Contents

1. [Build System Overview](#build-system-overview)
2. [Basic Building](#basic-building)
3. [Configuration Management](#configuration-management)
4. [Build Options and Flags](#build-options-and-flags)
5. [Board-Specific Configurations](#board-specific-configurations)
6. [Debugging](#debugging)
7. [Optimization](#optimization)
8. [Advanced Topics](#advanced-topics)

## Build System Overview

Zephyr uses a CMake-based build system with several layers:

1. **CMakeLists.txt** - Defines source files and build configuration
2. **Kconfig** - Configuration system for features and options
3. **Device Tree** - Hardware description
4. **West** - Meta-tool that wraps CMake and other tools

### Build Flow

```
west build -b <board> <app>
    ↓
CMake configuration
    ↓
Kconfig processing (generates .config)
    ↓
Device Tree compilation
    ↓
Source compilation
    ↓
Linking
    ↓
Binary generation (ELF, HEX, BIN)
```

## Basic Building

### Standard Build

```bash
# Build from workspace root
west build -b <board> <app-path>

# Example
west build -b nrf52840dk/nrf52840 app
```

### Build in Custom Directory

```bash
# Specify build directory
west build -b <board> -d my_build app
```

### Incremental Builds

After the first build, rebuild with:

```bash
# Rebuild (from project root)
west build

# Or specify build directory
west build -d build
```

### Clean Builds

```bash
# Clean build artifacts (keeps configuration)
west build -t clean

# Pristine build (removes everything, including configuration)
west build -t pristine

# Or delete and rebuild
rm -rf build
west build -b <board> app
```

## Configuration Management

Zephyr uses Kconfig for configuration management, similar to the Linux kernel.

### Configuration Files

1. **prj.conf** - Main project configuration
2. **boards/<board>.conf** - Board-specific configuration
3. **<build>/zephyr/.config** - Generated final configuration

### Using prj.conf

Create `app/prj.conf`:

```conf
# Enable features
CONFIG_GPIO=y
CONFIG_I2C=y
CONFIG_SPI=y

# Configure logging
CONFIG_LOG=y
CONFIG_LOG_DEFAULT_LEVEL=3
CONFIG_CONSOLE=y

# Memory settings
CONFIG_MAIN_STACK_SIZE=2048
CONFIG_HEAP_MEM_POOL_SIZE=4096

# Networking (example)
CONFIG_NETWORKING=y
CONFIG_NET_IPV4=y
CONFIG_NET_TCP=y
```

### Interactive Configuration (menuconfig)

```bash
# Build first, then open menuconfig
west build -b <board> app
west build -t menuconfig
```

This opens an interactive TUI for browsing and modifying configuration.

**Navigation:**
- Arrow keys: Navigate
- Enter: Select/enter submenu
- Space: Toggle option
- `/`: Search
- `?`: Help
- `Q`: Quit and save

### Viewing Current Configuration

```bash
# View final configuration
cat build/zephyr/.config

# View specific config values
grep CONFIG_GPIO build/zephyr/.config
```

### Configuration Overlays

For temporary configuration changes:

```bash
# Using overlay file
west build -b <board> app -- -DOVERLAY_CONFIG=debug.conf

# Multiple overlays
west build -b <board> app -- -DOVERLAY_CONFIG="debug.conf;extra.conf"
```

## Build Options and Flags

### CMake Options

Pass CMake options after `--`:

```bash
# Set CMake variable
west build -b <board> app -- -DCONFIG_OPTION=value

# Multiple options
west build -b <board> app -- -DOPT1=val1 -DOPT2=val2
```

### Common CMake Variables

```bash
# Overlay files
-DOVERLAY_CONFIG=my_overlay.conf
-DDTC_OVERLAY_FILE=my_overlay.overlay

# Extra compiler flags
-DEXTRA_CFLAGS="-Werror"
-DEXTRA_CXXFLAGS="-std=c++17"

# Zephyr modules
-DZEPHYR_MODULES=/path/to/module

# Board root
-DBOARD_ROOT=/path/to/custom/boards
```

### Verbose Build

```bash
# Show detailed build output
west build -v

# Or with ninja
west build -b <board> app -- -v
```

### Parallel Builds

```bash
# Use multiple CPU cores (usually automatic)
west build -b <board> app -- -j8

# Let ninja decide
west build -b <board> app -- -j
```

## Board-Specific Configurations

### Using Board Overlays

Create `app/boards/<board>.conf`:

```
app/
├── boards/
│   ├── nrf52840dk_nrf52840.conf
│   ├── nucleo_f429zi.conf
│   └── esp32_devkitc_wrover.conf
```

These are automatically applied when building for the specific board.

### Device Tree Overlays

Create `app/boards/<board>.overlay`:

```dts
/* app/boards/nrf52840dk_nrf52840.overlay */

&i2c0 {
    status = "okay";

    sensor@48 {
        compatible = "sensirion,sht3xd";
        reg = <0x48>;
    };
};

&spi1 {
    status = "okay";
};
```

### Multiple Board Support

```bash
# Build for different boards
west build -b nrf52840dk/nrf52840 app
west build -b nucleo_f429zi app -d build_nucleo
west build -b esp32_devkitc_wrover app -d build_esp32
```

## Debugging

### Debug Build

Enable debug symbols:

```conf
# prj.conf or debug.conf
CONFIG_DEBUG=y
CONFIG_DEBUG_OPTIMIZATIONS=y
CONFIG_DEBUG_INFO=y
```

```bash
west build -b <board> app -- -DOVERLAY_CONFIG=debug.conf
```

### Using GDB

```bash
# Start debug server and GDB
west debug

# Or manually
west debugserver  # In one terminal
arm-none-eabi-gdb build/zephyr/zephyr.elf  # In another
```

In GDB:
```gdb
# Connect to debug server
target remote :3333

# Load binary
load

# Set breakpoints
break main
break my_function

# Run
continue

# Step through code
step
next

# Print variables
print variable_name

# View backtrace
backtrace
```

### QEMU Debugging

```bash
# Build for QEMU
west build -b qemu_cortex_m3 app

# Run with GDB server
west build -t debugserver

# In another terminal
gdb-multiarch build/zephyr/zephyr.elf
(gdb) target remote :1234
(gdb) continue
```

### Assert and Logging

```c
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(my_module, LOG_LEVEL_DBG);

void my_function(void) {
    LOG_DBG("Debug message");
    LOG_INF("Info message");
    LOG_WRN("Warning message");
    LOG_ERR("Error message");

    __ASSERT(condition, "Assertion message");
}
```

Enable in configuration:

```conf
CONFIG_LOG=y
CONFIG_LOG_DEFAULT_LEVEL=4  # 0=OFF, 1=ERR, 2=WRN, 3=INF, 4=DBG
CONFIG_ASSERT=y
```

## Optimization

### Size Optimization

```conf
# prj.conf
CONFIG_SIZE_OPTIMIZATIONS=y
CONFIG_COMPILER_OPT="-Os"
CONFIG_LTO=y  # Link Time Optimization
```

### Speed Optimization

```conf
CONFIG_SPEED_OPTIMIZATIONS=y
CONFIG_COMPILER_OPT="-O2"
```

### Minimal Build

```conf
CONFIG_MINIMAL_LIBC=y
CONFIG_UART_CONSOLE=n
CONFIG_PRINTK=n
CONFIG_LOG=n
```

### Memory Footprint Analysis

```bash
# Build with puncover support
west build -b <board> app

# View memory usage
west build -t rom_report
west build -t ram_report

# Detailed size info
west build -t puncover
```

### Viewing Binary Size

```bash
# After build
arm-none-eabi-size build/zephyr/zephyr.elf

# Example output:
#    text    data     bss     dec     hex filename
#   12345    1234    5678   19257    4b39 build/zephyr/zephyr.elf
```

## Advanced Topics

### Build System Ninja Files

View build commands:

```bash
# Generate compile_commands.json
west build -t compile_commands

# View ninja build file
cat build/build.ninja
```

### Custom CMake Code

In your `app/CMakeLists.txt`:

```cmake
# Add preprocessor definitions
target_compile_definitions(app PRIVATE
    MY_CUSTOM_DEFINE=1
    FEATURE_ENABLED
)

# Add compiler flags
target_compile_options(app PRIVATE
    -Wall
    -Wextra
    -Werror
)

# Add include directories
target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Conditional compilation
if(CONFIG_MY_FEATURE)
    target_sources(app PRIVATE src/my_feature.c)
endif()
```

### Using External Libraries

```cmake
# Link external library
target_link_libraries(app PRIVATE
    m  # Math library
    /path/to/libcustom.a
)
```

### Multi-Image Builds

For boards with multiple cores or MCUboot:

```bash
# Build with MCUboot
west build -b <board> app -- -DCONFIG_BOOTLOADER_MCUBOOT=y

# Build for multi-core (e.g., nRF5340)
west build -b nrf5340dk/nrf5340/cpuapp app
```

### West Build Configuration

Create `.west/config` in workspace:

```ini
[build]
# Default board
board = nrf52840dk/nrf52840

# Default build directory
dir = build

# Cmake args
cmake-args = -DEXTRA_CFLAGS=-Werror
```

### Dependency Visualization

```bash
# Generate dependency graph
west build -t graph

# View with graphviz
dot -Tpng build/dependency-graph.dot -o deps.png
```

### Build Time Optimization

```bash
# Use ccache for faster rebuilds
sudo apt install ccache
export USE_CCACHE=1

# Configure ccache
ccache --max-size=5G
ccache --set-config=compression=true
```

### Cross-Compilation Variables

```bash
# Set toolchain
export ZEPHYR_TOOLCHAIN_VARIANT=gnuarmemb
export GNUARMEMB_TOOLCHAIN_PATH=/opt/gcc-arm-none-eabi

# Or use Zephyr SDK
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=~/zephyr-sdk-0.16.5
```

## Build Troubleshooting

### CMake Configuration Fails

```bash
# Clear CMake cache
rm -rf build/CMakeCache.txt build/CMakeFiles
west build -b <board> app
```

### Kconfig Errors

```bash
# Pristine build to reset configuration
west build -t pristine
west build -b <board> app
```

### Linker Errors

Check memory regions in `build/zephyr/zephyr.dts`:

```bash
# View memory layout
cat build/zephyr/zephyr.dts | grep memory
```

Adjust in `prj.conf` if needed:

```conf
CONFIG_MAIN_STACK_SIZE=2048
CONFIG_HEAP_MEM_POOL_SIZE=4096
CONFIG_SYSTEM_WORKQUEUE_STACK_SIZE=1024
```

## Build Best Practices

1. **Use version control for configurations** - Track `prj.conf` and board overlays
2. **Keep builds clean** - Regular pristine builds catch issues
3. **Document configuration** - Comment your Kconfig choices
4. **Use overlays for variants** - Don't modify `prj.conf` for temporary changes
5. **Optimize last** - Get it working first, then optimize
6. **Test on target** - QEMU is useful but not identical to hardware

## Useful Build Commands Summary

```bash
# Basic build
west build -b <board> app

# Clean builds
west build -t clean
west build -t pristine

# Configuration
west build -t menuconfig
west build -t guiconfig

# Analysis
west build -t rom_report
west build -t ram_report

# Debug
west build -v
west debug
west debugserver

# Flash
west flash
west flash --recover  # Recover locked device
```

---

For more information, see:
- [Zephyr Build System](https://docs.zephyrproject.org/latest/build/index.html)
- [Kconfig](https://docs.zephyrproject.org/latest/build/kconfig/index.html)
- [Device Tree](https://docs.zephyrproject.org/latest/build/dts/index.html)
