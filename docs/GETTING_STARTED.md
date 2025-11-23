# Getting Started with Zephyr T3 Forest Template

This guide will walk you through setting up your development environment and building your first Zephyr application using the T3 forest template.

## Table of Contents

1. [System Requirements](#system-requirements)
2. [Installing Dependencies](#installing-dependencies)
3. [Setting Up the Workspace](#setting-up-the-workspace)
4. [Installing a Toolchain](#installing-a-toolchain)
5. [Building Your First Application](#building-your-first-application)
6. [Flashing and Running](#flashing-and-running)
7. [Next Steps](#next-steps)

## System Requirements

### Supported Operating Systems

- **Linux:** Ubuntu 20.04+, Fedora 38+, Arch Linux
- **macOS:** macOS 10.15+ (Catalina or newer)
- **Windows:** Windows 10+ with WSL2 (Ubuntu 20.04+ recommended)

### Hardware Requirements

- **RAM:** Minimum 4 GB (8 GB recommended)
- **Disk Space:** Minimum 5 GB free space
- **Development Board:** Any Zephyr-supported board (optional, can use QEMU)

## Installing Dependencies

### Linux (Ubuntu/Debian)

```bash
# Update package list
sudo apt update

# Install dependencies
sudo apt install --no-install-recommends git cmake ninja-build gperf \
  ccache dfu-util device-tree-compiler wget \
  python3-dev python3-pip python3-setuptools python3-tk python3-wheel \
  xz-utils file make gcc gcc-multilib g++-multilib libsdl2-dev \
  libmagic1

# Install West
pip3 install --user west
```

### Linux (Fedora)

```bash
# Install dependencies
sudo dnf install git cmake ninja-build gperf ccache dfu-util dtc wget \
  python3-pip python3-tkinter xz file glibc-devel.i686 libstdc++-devel.i686 \
  SDL2-devel

# Install West
pip3 install --user west
```

### macOS

```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake ninja gperf ccache dfu-util dtc python3 qemu

# Install West
pip3 install west
```

### Windows (WSL2)

1. **Install WSL2:**
   - Open PowerShell as Administrator
   - Run: `wsl --install -d Ubuntu-22.04`
   - Restart your computer

2. **Inside WSL2, follow Linux (Ubuntu) instructions above**

## Setting Up the Workspace

### 1. Clone the Template Repository

```bash
# Clone your workspace repository
git clone <your-repository-url> my-zephyr-workspace
cd my-zephyr-workspace
```

### 2. Install West and Initialize Workspace

```bash
# Install West (if not already installed)
pip3 install --user west

# Add West to PATH (add to ~/.bashrc for persistence)
export PATH="$HOME/.local/bin:$PATH"

# Initialize the workspace (use the current directory as manifest)
west init -l .

# Fetch all projects defined in west.yml
west update
```

This will download:
- Zephyr RTOS source code
- Hardware Abstraction Layers (HALs)
- Required modules (mbedTLS, MCUboot, etc.)

The download is approximately 1-2 GB and may take several minutes.

### 3. Install Python Dependencies

```bash
# Install Zephyr's Python requirements
pip3 install --user -r zephyr/scripts/requirements.txt
```

## Installing a Toolchain

You need a toolchain to compile code for your target architecture.

### Option 1: Zephyr SDK (Recommended)

The Zephyr SDK includes toolchains for all supported architectures.

#### Linux

```bash
# Download Zephyr SDK (adjust version as needed)
cd ~
wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.5/zephyr-sdk-0.16.5_linux-x86_64.tar.xz

# Extract
tar xvf zephyr-sdk-0.16.5_linux-x86_64.tar.xz

# Run setup script
cd zephyr-sdk-0.16.5
./setup.sh -t all -h -c

# Set environment variables (add to ~/.bashrc)
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=~/zephyr-sdk-0.16.5
```

#### macOS

```bash
# Download Zephyr SDK
cd ~
wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.5/zephyr-sdk-0.16.5_macos-x86_64.tar.xz

# Extract
tar xvf zephyr-sdk-0.16.5_macos-x86_64.tar.xz

# Run setup script
cd zephyr-sdk-0.16.5
./setup.sh -t all -h -c

# Set environment variables (add to ~/.zshrc or ~/.bashrc)
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=~/zephyr-sdk-0.16.5
```

### Option 2: Other Toolchains

- **ARM GCC:** For ARM Cortex-M devices
- **LLVM:** For various architectures
- **GCC:** For x86 and others

See [Zephyr documentation](https://docs.zephyrproject.org/latest/develop/toolchains/index.html) for details.

## Building Your First Application

### 1. Navigate to Workspace

```bash
cd my-zephyr-workspace
```

### 2. Build the Sample Application

#### For a Real Board (e.g., nRF52840 DK):

```bash
west build -b nrf52840dk/nrf52840 app
```

#### For QEMU Emulation (no hardware needed):

```bash
west build -b qemu_cortex_m3 app
```

#### For Native Simulation:

```bash
west build -b native_sim app
```

### 3. Understanding Build Output

The build process will:
1. Configure the build using CMake
2. Generate device tree configurations
3. Compile source files
4. Link the final binary

Output files are in `build/zephyr/`:
- `zephyr.elf` - Executable binary with debug symbols
- `zephyr.hex` - Intel HEX format for flashing
- `zephyr.bin` - Raw binary image

## Flashing and Running

### On Real Hardware

Connect your board via USB and run:

```bash
west flash
```

This will:
1. Detect your connected board
2. Flash the binary using the appropriate tool (JLink, OpenOCD, pyOCD, etc.)

### In QEMU

```bash
west build -t run
```

This will start QEMU and you'll see the output in the terminal.

### Using Native Simulator

```bash
./build/zephyr/zephyr.exe  # or just 'zephyr.exe' on the path
```

### Viewing Serial Output

For real hardware, connect to the serial console:

```bash
# Linux/macOS (adjust device as needed)
screen /dev/ttyACM0 115200

# Or use minicom
minicom -D /dev/ttyACM0 -b 115200

# Or use West's built-in terminal
west attach
```

For QEMU, output appears directly in the terminal.

## Expected Output

You should see output similar to:

```
*** Booting Zephyr OS build v3.x.x ***
[00:00:00.000,000] <inf> main: Zephyr T3 Forest Template Application Started!
[00:00:00.000,000] <inf> main: Board: qemu_cortex_m3
[00:00:00.000,000] <inf> main: Zephyr version: 3.x.x
[00:00:00.000,000] <inf> main: Hello World! Count: 0
[00:00:02.000,000] <inf> main: Hello World! Count: 1
[00:00:04.000,000] <inf> main: Hello World! Count: 2
```

## Troubleshooting First Build

### West Command Not Found

```bash
# Add to PATH
export PATH="$HOME/.local/bin:$PATH"

# Or reinstall with pip
pip3 install --user --upgrade west
```

### CMake Version Too Old

```bash
# Ubuntu: Use Kitware APT repository
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | sudo apt-key add -
sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ focal main'
sudo apt update
sudo apt install cmake
```

### Board Not Detected During Flash

1. Check USB connection
2. Verify board is powered
3. Check user permissions (Linux):
   ```bash
   sudo usermod -a -G dialout $USER
   # Log out and back in
   ```

### Build Errors

1. Ensure all dependencies are installed
2. Try a clean build:
   ```bash
   west build -t clean
   west build -b <board> app
   ```
3. Verify toolchain is properly configured

## Next Steps

Now that you have a working setup:

1. **[Learn about building](BUILDING.md)** - Configuration, optimization, debugging
2. **[Add your own projects](ADDING_PROJECTS.md)** - Create new applications
3. **[Explore Zephyr samples](https://github.com/zephyrproject-rtos/zephyr/tree/main/samples)** - Learn from examples
4. **[Read Zephyr docs](https://docs.zephyrproject.org/)** - Comprehensive documentation

## Common Build Targets

```bash
# Clean build
west build -t clean

# Pristine build (complete clean)
west build -t pristine

# Build with verbose output
west build -v

# Build for different board
west build -b another_board app

# Build and flash
west build -b <board> app && west flash

# Open menuconfig for configuration
west build -t menuconfig

# Generate compile_commands.json for IDE
west build -t compile_commands
```

## Environment Setup Script

Create a script to set up your environment quickly:

```bash
# ~/zephyr-env.sh
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=~/zephyr-sdk-0.16.5
export PATH="$HOME/.local/bin:$PATH"
```

Source it before working:

```bash
source ~/zephyr-env.sh
```

Or add to your `~/.bashrc` or `~/.zshrc` for automatic setup.

---

**Congratulations!** You've successfully set up your Zephyr development environment and built your first application. 🎉
