# Zephyr T2 Freestanding Application Template

A production-ready template for developing Zephyr RTOS applications using the T2 (Application-centric) topology. This is the **most popular** workspace structure for commercial Zephyr projects, providing a self-contained, version-controlled application repository.

## What is T2 Freestanding Template?

The T2 (Tethered to Tree) freestanding template is an application-centric workspace where:

- **Your application IS the manifest repository** - Complete control over dependencies
- **Zephyr is brought in as a dependency** - Clean separation of concerns
- **Version pinning** - Lock Zephyr and modules to specific versions for reproducible builds
- **Self-contained** - Clone, build, and deploy years later with identical results
- **Production-ready** - Used by commercial Zephyr projects worldwide
- **Simple version control** - Track only your application code

## Why Choose T2?

✅ **Industry Standard** - Most popular topology for commercial products
✅ **Reproducible Builds** - Pin exact Zephyr version and all dependencies
✅ **Self-Contained** - One repository contains everything needed
✅ **Clean Separation** - Application code separate from Zephyr source
✅ **Version Control Friendly** - Track only your code, not Zephyr
✅ **Long-term Stability** - Build identical binaries years later
✅ **Easy Collaboration** - Share complete working environment

## When to Use T2

- ✅ Building a **specific product** or application
- ✅ Need **reproducible builds** across teams/time
- ✅ Want to **pin Zephyr version** for stability
- ✅ Commercial or production projects
- ✅ Single application focus

**For multiple related applications**, consider the T3 Forest template instead.

## Quick Start

### Prerequisites

- Python 3.8 or later
- CMake 3.20.0 or later
- Device Tree Compiler (dtc)
- A toolchain for your target architecture (e.g., Zephyr SDK, ARM GCC, etc.)

### Installation

1. **Clone this repository:**
   ```bash
   git clone <your-repo-url> my-zephyr-app
   cd my-zephyr-app
   ```

2. **Install West:**
   ```bash
   pip3 install west
   ```

3. **Initialize the workspace:**
   ```bash
   west init -l .
   west update
   ```

   This fetches Zephyr (pinned to v3.7.0) and all required modules.

4. **Install Python dependencies:**
   ```bash
   pip3 install -r zephyr/scripts/requirements.txt
   ```

5. **Set up toolchain:**
   ```bash
   export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
   export ZEPHYR_SDK_INSTALL_DIR=~/zephyr-sdk-0.16.5
   ```

### Build and Run

Build the application:

```bash
west build -b <your_board>
```

Examples:
```bash
# For nRF52840 DK
west build -b nrf52840dk/nrf52840

# For QEMU (no hardware needed)
west build -b qemu_cortex_m3

# For STM32 Nucleo
west build -b nucleo_f429zi
```

Flash to your device:

```bash
west flash
```

Run in QEMU:

```bash
west build -t run
```

## Directory Structure

```
my-zephyr-app/           # This repository (manifest + application)
├── .git/                # Your git repository
├── .gitignore           # Git ignore file
├── west.yml             # West manifest - brings in Zephyr
├── VERSION              # Application version
├── README.md            # This file
├── CMakeLists.txt       # Application build configuration
├── prj.conf             # Application Kconfig configuration
├── Kconfig              # Custom Kconfig options
├── src/                 # Application source code
│   └── main.c           # Main application file
├── boards/              # Board-specific configurations
│   └── <board>.conf     # Board-specific config overlays
│   └── <board>.overlay  # Board-specific DT overlays
├── docs/                # Documentation
│   ├── GETTING_STARTED.md  # Detailed setup guide
│   ├── BUILDING.md         # Build system guide
│   └── TROUBLESHOOTING.md  # Common issues
├── zephyr/              # Zephyr RTOS (fetched by west, NOT in git)
├── modules/             # HALs and modules (fetched by west, NOT in git)
└── tools/               # Build tools (fetched by west, NOT in git)
```

## Documentation

Comprehensive guides available in the `docs/` directory:

- **[Getting Started](docs/GETTING_STARTED.md)** - Complete setup and installation guide
- **[Building Applications](docs/BUILDING.md)** - Build system, configuration, and debugging
- **[Troubleshooting](docs/TROUBLESHOOTING.md)** - Common issues and solutions

## Application Structure

### Main Files

- **`west.yml`** - Defines Zephyr version and module dependencies
- **`CMakeLists.txt`** - Build configuration
- **`prj.conf`** - Kconfig configuration (features, drivers, etc.)
- **`Kconfig`** - Custom application Kconfig options
- **`src/main.c`** - Your application entry point

### Configuration

**prj.conf** - Enable Zephyr features:
```conf
CONFIG_LOG=y              # Enable logging
CONFIG_GPIO=y             # Enable GPIO driver
CONFIG_I2C=y              # Enable I2C
CONFIG_NETWORKING=y       # Enable networking stack
```

**boards/<board>.conf** - Board-specific configuration:
```conf
# Custom settings for specific board
CONFIG_UART_CONSOLE_BAUDRATE=921600
```

**boards/<board>.overlay** - Board-specific device tree:
```dts
&i2c0 {
    status = "okay";
    sensor@48 {
        compatible = "vendor,sensor";
        reg = <0x48>;
    };
};
```

## Supported Boards

All Zephyr-supported boards work with this template:

- **Nordic:** `nrf52840dk/nrf52840`, `nrf5340dk/nrf5340/cpuapp`
- **STM32:** `nucleo_f429zi`, `stm32f4_disco`, `nucleo_l476rg`
- **ESP32:** `esp32_devkitc_wrover`, `esp32s3_devkitm`
- **Native:** `native_sim`, `native_posix` (simulation)
- **QEMU:** `qemu_cortex_m3`, `qemu_x86`

List all boards: `west boards`

## West Commands

```bash
# Update dependencies (after changing west.yml)
west update

# Build application
west build -b <board>

# Clean build
west build -t clean

# Pristine build (complete clean)
west build -t pristine

# Flash to device
west flash

# Debug with GDB
west debug

# Open menuconfig
west build -t menuconfig

# View memory usage
west build -t rom_report
west build -t ram_report
```

## Version Control Best Practices

### What IS Tracked (in git)

✅ Your application source code (`src/`)
✅ Build configuration (`CMakeLists.txt`, `prj.conf`)
✅ West manifest (`west.yml`)
✅ Documentation
✅ Board overlays (`boards/`)
✅ Custom Kconfig

### What is NOT Tracked (in .gitignore)

❌ `zephyr/` - Zephyr source (managed by west)
❌ `modules/` - HALs and modules (managed by west)
❌ `build/` - Build artifacts
❌ `tools/` - Build tools

### Why This Matters

When you or a teammate clones this repository:
1. `git clone` - Gets your application code and `west.yml`
2. `west update` - Fetches exact Zephyr version specified in `west.yml`
3. Build produces **identical binary** to original

## Customization

### Changing Zephyr Version

Edit `west.yml`:

```yaml
projects:
  - name: zephyr
    revision: v3.7.0  # Change to desired version/tag/branch
```

Then update:
```bash
west update
```

**Recommendations:**
- **Production:** Use specific tags (e.g., `v3.7.0`)
- **Development:** Use `main` or `v3.7-branch`
- **LTS Projects:** Use LTS releases

### Adding Custom Modules

Add dependencies to `west.yml`:

```yaml
projects:
  - name: zephyr
    # ... existing config ...

  - name: my-custom-driver
    url: https://github.com/myorg/my-driver
    revision: v1.2.0
    path: modules/my-driver
```

Then:
```bash
west update
```

Enable in `prj.conf`:
```conf
CONFIG_MY_DRIVER=y
```

### Adding Application Modules

For code shared within your application:

```
src/
├── main.c
├── drivers/
│   ├── sensor.c
│   └── sensor.h
├── utils/
│   ├── helpers.c
│   └── helpers.h
└── network/
    └── protocol.c
```

Update `CMakeLists.txt`:
```cmake
target_sources(app PRIVATE
    src/main.c
    src/drivers/sensor.c
    src/utils/helpers.c
    src/network/protocol.c
)
```

## Reproducible Builds

This template ensures **deterministic builds**:

1. **Locked Zephyr version** - `west.yml` specifies exact version
2. **Locked modules** - All dependencies versioned
3. **Configuration tracked** - `prj.conf` in version control
4. **Toolchain specification** - Document required toolchain

### Build Reproducibility Example

**Today:**
```bash
git clone https://github.com/myorg/my-app
cd my-app
west init -l . && west update
west build -b nrf52840dk/nrf52840
# Binary: zephyr.bin (SHA256: abc123...)
```

**5 Years Later:**
```bash
git clone https://github.com/myorg/my-app
cd my-app
west init -l . && west update
west build -b nrf52840dk/nrf52840
# Binary: zephyr.bin (SHA256: abc123...) <- IDENTICAL
```

## CI/CD Integration

Example GitHub Actions workflow:

```yaml
name: Build Application

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3

      - name: Install dependencies
        run: |
          pip3 install west
          west init -l .
          west update
          pip3 install -r zephyr/scripts/requirements.txt

      - name: Build for nRF52840
        run: west build -b nrf52840dk/nrf52840

      - name: Upload binary
        uses: actions/upload-artifact@v3
        with:
          name: firmware
          path: build/zephyr/zephyr.hex
```

## Migration to T2

If you have an existing Zephyr application in the Zephyr tree (T1):

1. Copy your application to new repo
2. Add `west.yml` (from this template)
3. Move app files to root level
4. Initialize: `west init -l . && west update`
5. Build: `west build -b <board>`

## Comparison with Other Topologies

| Feature | T1 (Zephyr-centric) | **T2 (This)** | T3 (Forest) |
|---------|---------------------|---------------|-------------|
| **Best for** | Zephyr dev | **Single product** | Multi-product |
| **Reproducibility** | ⚠️ | ✅ | ✅ |
| **Simplicity** | ✅ | ✅ | ⚠️ |
| **Production** | ❌ | ✅ | ✅ |
| **Multi-app** | ❌ | ❌ | ✅ |
| **Industry standard** | ❌ | **✅** | ⚠️ |

## Contributing

Contributions welcome! Please submit issues and pull requests.

## Resources

- **Zephyr Documentation:** https://docs.zephyrproject.org/
- **West Tool:** https://docs.zephyrproject.org/latest/develop/west/index.html
- **Workspace Topologies:** https://docs.zephyrproject.org/latest/develop/west/workspaces.html
- **Zephyr GitHub:** https://github.com/zephyrproject-rtos/zephyr
- **Zephyr Discord:** https://chat.zephyrproject.org/

## License

Apache 2.0 - See individual files for specific licensing information.

## Support

Need help?
- Check [documentation](docs/)
- Review [troubleshooting guide](docs/TROUBLESHOOTING.md)
- Open an issue
- Ask on [Zephyr Discord](https://chat.zephyrproject.org/)

---

**Ready for Production! 🚀**
