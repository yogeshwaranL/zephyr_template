# Zephyr T3 Forest Template

A comprehensive template for developing Zephyr RTOS applications outside of the main Zephyr project tree using the T3 (Tethered to Tree) forest topology. This template allows you to maintain your own applications and modules while seamlessly integrating with the Zephyr ecosystem.

## What is T3 Forest Template?

The T3 (Tethered to Tree) forest template is a workspace structure that:

- **Separates your application code** from the Zephyr RTOS source tree
- **Manages dependencies** automatically using West (Zephyr's meta-tool)
- **Enables multi-project development** - easily add and manage multiple applications and custom modules
- **Simplifies version control** - track only your application code, not Zephyr itself
- **Provides clean organization** - clear separation between your code and external dependencies

## Features

✅ **Ready-to-use workspace structure**
✅ **Sample application** with logging and basic Zephyr features
✅ **West manifest** pre-configured with Zephyr and common modules
✅ **Comprehensive documentation** and user guides
✅ **Multi-project support** - easily add more applications
✅ **Custom module integration** - include your own Zephyr modules
✅ **Git-friendly** - proper `.gitignore` for Zephyr development

## Quick Start

### Prerequisites

- Python 3.8 or later
- CMake 3.20.0 or later
- Device Tree Compiler (dtc)
- A toolchain for your target architecture (e.g., Zephyr SDK, ARM GCC, etc.)

### Installation

1. **Clone this repository:**
   ```bash
   git clone <your-repo-url> my-zephyr-workspace
   cd my-zephyr-workspace
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

4. **Install Python dependencies:**
   ```bash
   pip3 install -r zephyr/scripts/requirements.txt
   ```

5. **Set up Zephyr environment (if not using Zephyr SDK):**
   ```bash
   export ZEPHYR_TOOLCHAIN_VARIANT=<your-toolchain>
   export ZEPHYR_SDK_INSTALL_DIR=<sdk-path>
   ```

### Build and Run

Build the sample application:

```bash
west build -b <your_board> app
```

Flash to your device:

```bash
west flash
```

## Directory Structure

```
my-zephyr-workspace/
├── .git/                    # Your repository git data
├── .gitignore              # Git ignore file
├── west.yml                # West manifest - defines workspace
├── VERSION                 # Template version
├── README.md               # This file
├── docs/                   # Documentation
│   ├── GETTING_STARTED.md  # Detailed setup guide
│   ├── BUILDING.md         # Build system guide
│   └── ADDING_PROJECTS.md  # How to add more projects
├── app/                    # Sample application
│   ├── CMakeLists.txt      # Application build configuration
│   ├── prj.conf            # Application Kconfig configuration
│   ├── Kconfig             # Custom Kconfig options
│   ├── src/                # Application source code
│   │   └── main.c
│   ├── boards/             # Board-specific configurations
│   └── README.md           # Application documentation
├── zephyr/                 # Zephyr RTOS (fetched by west)
├── modules/                # Zephyr modules and HALs (fetched by west)
└── tools/                  # Additional tools (fetched by west)
```

## Documentation

Detailed guides are available in the `docs/` directory:

- **[Getting Started](docs/GETTING_STARTED.md)** - Complete setup and installation guide
- **[Building Applications](docs/BUILDING.md)** - Build system, configuration, and debugging
- **[Adding Projects](docs/ADDING_PROJECTS.md)** - How to add more applications and modules
- **[Troubleshooting](docs/TROUBLESHOOTING.md)** - Common issues and solutions

## Adding More Applications

To add a new application to your workspace:

1. Create a new directory: `mkdir my-new-app`
2. Add the standard Zephyr app files (`CMakeLists.txt`, `prj.conf`, `src/`)
3. Build with: `west build -b <board> my-new-app`

See [docs/ADDING_PROJECTS.md](docs/ADDING_PROJECTS.md) for detailed instructions.

## Supported Boards

This template supports all boards supported by Zephyr. Common examples:

- **Nordic:** `nrf52840dk/nrf52840`, `nrf5340dk/nrf5340/cpuapp`
- **STM32:** `nucleo_f429zi`, `stm32f4_disco`
- **ESP32:** `esp32_devkitc_wrover`, `esp32s3_devkitm`
- **Native:** `native_sim`, `native_posix` (for simulation)
- **QEMU:** `qemu_cortex_m3`, `qemu_x86`

For a complete list: `west boards`

## West Commands Cheat Sheet

```bash
# Update all projects
west update

# List available boards
west boards

# Build application
west build -b <board> <app-path>

# Clean build
west build -t clean

# Flash to device
west flash

# Open serial console
west debug

# Run in QEMU (for supported boards)
west build -t run
```

## Customization

### Modifying Zephyr Version

Edit `west.yml` and change the `revision` field:

```yaml
projects:
  - name: zephyr
    revision: v3.7.0  # or any tag/branch/commit
```

Then run: `west update`

### Adding Custom Modules

Add to `west.yml` under `projects`:

```yaml
- name: my-module
  url: https://github.com/myorg/my-module
  revision: main
  path: modules/my-module
```

## Version Control

This template is designed to track only your application code. The `zephyr/` and `modules/` directories are excluded from git tracking (see `.gitignore`).

**What is tracked:**
- Your application code (`app/`, etc.)
- West manifest (`west.yml`)
- Documentation
- Configuration files

**What is not tracked:**
- Zephyr source (`zephyr/`)
- Modules and HALs (`modules/`)
- Build artifacts (`build/`)

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

## Resources

- **Zephyr Documentation:** https://docs.zephyrproject.org/
- **West Documentation:** https://docs.zephyrproject.org/latest/develop/west/index.html
- **Zephyr GitHub:** https://github.com/zephyrproject-rtos/zephyr
- **Zephyr Discord:** https://chat.zephyrproject.org/

## License

This template is provided under the Apache 2.0 license. See individual files for specific licensing information.

## Support

For questions and support:
- Check the [documentation](docs/)
- Review [troubleshooting guide](docs/TROUBLESHOOTING.md)
- Open an issue in this repository
- Ask on [Zephyr Discord](https://chat.zephyrproject.org/)

---

**Happy Zephyr Development! 🚀**
