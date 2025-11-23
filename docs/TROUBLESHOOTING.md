# Troubleshooting Guide

Common issues and solutions for Zephyr T2 Freestanding Application Template development.

## Table of Contents

1. [West Issues](#west-issues)
2. [Build Problems](#build-problems)
3. [Configuration Issues](#configuration-issues)
4. [Flashing and Debugging](#flashing-and-debugging)
5. [Hardware Issues](#hardware-issues)
6. [Environment Problems](#environment-problems)
7. [Git and Version Control](#git-and-version-control)

## West Issues

### "west: command not found"

**Problem:** West is not installed or not in PATH

**Solution:**
```bash
# Install west
pip3 install --user west

# Add to PATH (Linux/macOS)
export PATH="$HOME/.local/bin:$PATH"

# Add to ~/.bashrc or ~/.zshrc for persistence
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
```

### "ZEPHYR_BASE not set"

**Problem:** West workspace not initialized

**Solution:**
```bash
# Initialize workspace from manifest repository
west init -l .

# Update all projects
west update
```

### "west update" Fails with Network Errors

**Problem:** Network connectivity or Git issues

**Solution:**
```bash
# Retry with verbose output
west update -v

# Use different fetch strategy
west update --fetch=smart

# If behind proxy, configure Git
git config --global http.proxy http://proxy.example.com:8080

# For specific authentication issues
git config --global credential.helper store
```

### "Project is in a bad state"

**Problem:** Local changes conflict with west update

**Solution:**
```bash
# Stash local changes
cd zephyr
git stash

# Return to workspace root and update
cd ..
west update

# Or force reset (WARNING: loses local changes)
west forall -c "git reset --hard HEAD"
west update
```

### West Can't Find Board

**Problem:** Custom board or board not in Zephyr tree

**Solution:**
```bash
# List available boards
west boards

# Specify custom board root
west build -b <board> app -- -DBOARD_ROOT=/path/to/boards

# Or set in CMakeLists.txt
set(BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/boards)
```

## Build Problems

### "CMake Error: Could not find Zephyr-SDK"

**Problem:** Toolchain not properly configured

**Solution:**
```bash
# Set environment variables
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=~/zephyr-sdk-0.16.5

# Or use different toolchain
export ZEPHYR_TOOLCHAIN_VARIANT=gnuarmemb
export GNUARMEMB_TOOLCHAIN_PATH=/opt/gcc-arm-none-eabi

# Verify toolchain
echo $ZEPHYR_TOOLCHAIN_VARIANT
echo $ZEPHYR_SDK_INSTALL_DIR
```

### "No such file or directory: zephyr.h"

**Problem:** Missing include paths or Zephyr not found

**Solution:**
```bash
# Ensure Zephyr is fetched
west update

# Pristine build
west build -t pristine
west build -b <board> app

# Check ZEPHYR_BASE
echo $ZEPHYR_BASE  # Should point to zephyr directory
```

### "region 'FLASH' overflowed"

**Problem:** Application too large for flash memory

**Solution:**

1. **Enable size optimizations** (`prj.conf`):
```conf
CONFIG_SIZE_OPTIMIZATIONS=y
CONFIG_LTO=y
CONFIG_COMPILER_OPT="-Os"
```

2. **Disable unused features**:
```conf
CONFIG_LOG=n
CONFIG_PRINTK=n
CONFIG_ASSERT=n
CONFIG_MINIMAL_LIBC=y
```

3. **View memory usage**:
```bash
west build -t rom_report
```

4. **Increase flash size** (if board supports):
```dts
/* In board overlay */
&flash0 {
    reg = <0x00000000 DT_SIZE_K(512)>;
};
```

### "region 'RAM' overflowed"

**Problem:** Application uses too much RAM

**Solution:**

1. **Reduce stack sizes** (`prj.conf`):
```conf
CONFIG_MAIN_STACK_SIZE=1024
CONFIG_IDLE_STACK_SIZE=256
CONFIG_SYSTEM_WORKQUEUE_STACK_SIZE=1024
```

2. **Reduce heap**:
```conf
CONFIG_HEAP_MEM_POOL_SIZE=0
```

3. **View RAM usage**:
```bash
west build -t ram_report
```

### "undefined reference to `function_name`"

**Problem:** Missing implementation or library

**Solution:**

1. **Check source files** in CMakeLists.txt:
```cmake
target_sources(app PRIVATE
    src/main.c
    src/missing_file.c  # Add this
)
```

2. **Enable required Kconfig option**:
```conf
CONFIG_REQUIRED_MODULE=y
```

3. **Link required library**:
```cmake
target_link_libraries(app PRIVATE m)  # Math library
```

### Build Extremely Slow

**Problem:** Not using parallel builds or ccache

**Solution:**
```bash
# Use multiple cores
west build -b <board> app -- -j8

# Install and use ccache
sudo apt install ccache
export USE_CCACHE=1
ccache --max-size=5G

# Configure ccache
ccache --set-config=compression=true
```

## Configuration Issues

### "Configuration option not available in menuconfig"

**Problem:** Kconfig dependency not met

**Solution:**
```bash
# Check dependencies in menuconfig
west build -t menuconfig
# Press '/' to search for option
# Check 'Depends on:' field

# Enable dependencies first
# Then the desired option will appear
```

### "Selected X but Y is not enabled"

**Problem:** Kconfig dependency warning

**Solution:**
```conf
# Enable both in prj.conf
CONFIG_Y=y  # Dependency
CONFIG_X=y  # Desired option
```

### Changes in prj.conf Not Applied

**Problem:** Build system cached old configuration

**Solution:**
```bash
# Pristine build
west build -t pristine
west build -b <board> app

# Or delete .config
rm build/zephyr/.config
west build
```

### Device Tree Errors

**Problem:** DT overlay syntax or reference errors

**Solution:**
```bash
# Check generated device tree
cat build/zephyr/zephyr.dts

# Validate syntax in overlay
# Common issues:
# - Missing semicolons
# - Incorrect node paths
# - Invalid property values

# Example correct syntax:
&i2c0 {
    status = "okay";

    sensor@48 {
        compatible = "vendor,device";
        reg = <0x48>;
    };
};
```

## Flashing and Debugging

### "No connected boards"

**Problem:** Board not detected or permissions

**Solution:**

1. **Check connection**:
```bash
# List USB devices
lsusb

# Check dmesg for device
dmesg | tail
```

2. **Fix permissions** (Linux):
```bash
# Add user to dialout group
sudo usermod -a -G dialout $USER

# Install udev rules (for specific debugger)
# Nordic nRF
wget https://raw.githubusercontent.com/NordicSemiconductor/nrf-udev/main/nrf-udev_1.0.1-all.deb
sudo dpkg -i nrf-udev_1.0.1-all.deb

# J-Link
sudo cp 99-jlink.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
```

3. **Log out and back in** (for group changes)

### "Flash Failed: Error 4"

**Problem:** Board locked or flash protection enabled

**Solution:**
```bash
# Recover/erase board (Nordic)
west flash --recover

# Or use nrfjprog
nrfjprog --eraseall
nrfjprog --reset

# For STM32
west flash --runner openocd --cmd-pre-init "reset halt"
```

### Can't Open Serial Console

**Problem:** Device busy or wrong port

**Solution:**
```bash
# List serial ports
ls /dev/tty*

# Kill processes using port
sudo lsof | grep ttyACM0
sudo kill <PID>

# Check permissions
sudo chmod 666 /dev/ttyACM0

# Try different tool
# Instead of screen:
minicom -D /dev/ttyACM0 -b 115200
# Or
picocom /dev/ttyACM0 -b 115200
```

### "GDB can't connect to target"

**Problem:** Debug server not running or wrong port

**Solution:**
```bash
# Start debug server first
west debugserver

# In another terminal
west attach

# Or manually with correct GDB
arm-none-eabi-gdb build/zephyr/zephyr.elf
(gdb) target remote :3333

# Check port isn't blocked
netstat -tuln | grep 3333
```

### Serial Output is Garbled

**Problem:** Wrong baud rate

**Solution:**
```bash
# Check board's default baud rate (usually 115200)
# Common rates: 9600, 115200

# Set in serial terminal
screen /dev/ttyACM0 115200
minicom -D /dev/ttyACM0 -b 115200

# Or configure in prj.conf
CONFIG_UART_CONSOLE_ON_DEV_NAME="UART_0"
CONFIG_UART_CONSOLE_BAUDRATE=115200
```

## Hardware Issues

### Board Boots but No Output

**Problem:** UART pins or console configuration

**Solution:**

1. **Check console config** (`prj.conf`):
```conf
CONFIG_CONSOLE=y
CONFIG_UART_CONSOLE=y
CONFIG_LOG=y
CONFIG_PRINTK=y
```

2. **Verify UART pins** in device tree overlay:
```dts
&uart0 {
    status = "okay";
    current-speed = <115200>;
    tx-pin = <6>;
    rx-pin = <8>;
};

chosen {
    zephyr,console = &uart0;
};
```

3. **Test with LED blink** to confirm code is running

### Sensors Not Detected

**Problem:** I2C/SPI configuration or wiring

**Solution:**

1. **Enable bus** (`prj.conf`):
```conf
CONFIG_I2C=y
CONFIG_SPI=y
CONFIG_GPIO=y
```

2. **Configure in device tree**:
```dts
&i2c0 {
    status = "okay";
    clock-frequency = <I2C_BITRATE_STANDARD>;

    sensor@48 {
        compatible = "vendor,sensor";
        reg = <0x48>;
    };
};
```

3. **Scan I2C bus**:
```c
#include <zephyr/drivers/i2c.h>

// Scan for devices
for (uint8_t addr = 0x03; addr < 0x78; addr++) {
    if (i2c_write(i2c_dev, NULL, 0, addr) == 0) {
        printk("Found device at 0x%02X\n", addr);
    }
}
```

4. **Check wiring and pullups**

### Random Crashes or Hangs

**Problem:** Stack overflow, heap corruption, or timing

**Solution:**

1. **Enable stack protection** (`prj.conf`):
```conf
CONFIG_STACK_SENTINEL=y
CONFIG_DEBUG_INFO=y
CONFIG_ASSERT=y
```

2. **Increase stack sizes**:
```conf
CONFIG_MAIN_STACK_SIZE=2048
CONFIG_SYSTEM_WORKQUEUE_STACK_SIZE=2048
```

3. **Enable thread monitoring**:
```conf
CONFIG_THREAD_MONITOR=y
CONFIG_THREAD_NAME=y
```

4. **Check with debugger**:
```bash
west debug
(gdb) backtrace
```

## Environment Problems

### Python Version Issues

**Problem:** Python 2 vs Python 3

**Solution:**
```bash
# Ensure Python 3
python --version  # Should be 3.8+

# Use python3 explicitly
alias python=python3
alias pip=pip3

# Or use virtual environment
python3 -m venv zephyr-env
source zephyr-env/bin/activate
```

### CMake Too Old

**Problem:** CMake version < 3.20.0

**Solution:**
```bash
# Ubuntu - use Kitware's repo
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | sudo apt-key add -
sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ focal main'
sudo apt update
sudo apt install cmake

# Or install from pip
pip3 install cmake

# Verify version
cmake --version
```

### "dtc: command not found"

**Problem:** Device tree compiler not installed

**Solution:**
```bash
# Ubuntu/Debian
sudo apt install device-tree-compiler

# Fedora
sudo dnf install dtc

# macOS
brew install dtc

# Verify
dtc --version
```

## Git and Version Control

### Merge Conflicts in west.yml

**Problem:** Conflicting changes to manifest

**Solution:**
```bash
# Resolve conflicts manually
nano west.yml

# Or use your version
git checkout --ours west.yml
git add west.yml

# Or their version
git checkout --theirs west.yml
git add west.yml

# Complete merge
git commit
```

### Accidentally Committed Build Artifacts

**Problem:** build/ directory in git

**Solution:**
```bash
# Remove from tracking
git rm -r --cached build/

# Ensure .gitignore is correct
echo "build/" >> .gitignore
echo "zephyr/" >> .gitignore

# Commit changes
git add .gitignore
git commit -m "Remove build artifacts from tracking"
```

### Large Repository Size

**Problem:** Unnecessary files tracked

**Solution:**
```bash
# Check large files
git rev-list --objects --all | \
  git cat-file --batch-check='%(objecttype) %(objectname) %(objectsize) %(rest)' | \
  awk '/^blob/ {print substr($0,6)}' | \
  sort --numeric-sort --key=2 | \
  tail -20

# Remove large files from history
git filter-branch --tree-filter 'rm -rf build/' HEAD

# Or use BFG Repo-Cleaner
# https://rtyley.github.io/bfg-repo-cleaner/
```

## Getting Help

If you're still stuck:

1. **Check Zephyr docs**: https://docs.zephyrproject.org/
2. **Search GitHub issues**: https://github.com/zephyrproject-rtos/zephyr/issues
3. **Ask on Discord**: https://chat.zephyrproject.org/
4. **Post on mailing list**: https://lists.zephyrproject.org/g/users
5. **Stack Overflow**: Tag with `zephyr-rtos`

### Reporting Bugs

When reporting issues, include:

```bash
# Zephyr version
git -C zephyr describe --tags

# West version
west --version

# Build log
west build -b <board> app -v 2>&1 | tee build.log

# Environment
echo "OS: $(uname -a)"
echo "CMake: $(cmake --version | head -1)"
echo "Python: $(python --version)"
echo "Toolchain: $ZEPHYR_TOOLCHAIN_VARIANT"
```

## Diagnostic Commands

```bash
# Check workspace state
west status

# List all projects and versions
west list -f "{name:18} {path:28} {revision}"

# Verify toolchain
which arm-none-eabi-gcc
arm-none-eabi-gcc --version

# Check board support
west boards | grep <board>

# View build configuration
cat build/zephyr/.config | grep CONFIG_BOARD

# Check device tree
cat build/zephyr/zephyr.dts

# View memory usage
west build -t rom_report
west build -t ram_report
```

---

**Still having issues?** Open an issue in this repository with details and we'll help you debug it!
