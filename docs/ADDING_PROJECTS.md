# Adding Projects and Modules

This guide explains how to add new applications, custom modules, and external dependencies to your T3 forest workspace.

## Table of Contents

1. [Adding New Applications](#adding-new-applications)
2. [Adding Custom Zephyr Modules](#adding-custom-zephyr-modules)
3. [Adding External Dependencies](#adding-external-dependencies)
4. [Organizing Multi-Project Workspaces](#organizing-multi-project-workspaces)
5. [Sharing Code Between Projects](#sharing-code-between-projects)
6. [Best Practices](#best-practices)

## Adding New Applications

### Method 1: Simple Application

Create a new application directory alongside the sample `app`:

```bash
# From workspace root
mkdir my-new-app
cd my-new-app
```

Create the basic structure:

```bash
my-new-app/
├── CMakeLists.txt
├── prj.conf
├── Kconfig (optional)
├── README.md
└── src/
    └── main.c
```

**CMakeLists.txt:**
```cmake
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(my_new_app)

target_sources(app PRIVATE src/main.c)
```

**prj.conf:**
```conf
CONFIG_CONSOLE=y
CONFIG_UART_CONSOLE=y
CONFIG_LOG=y
```

**src/main.c:**
```c
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    LOG_INF("My new application started!");

    while (1) {
        k_sleep(K_SECONDS(1));
    }

    return 0;
}
```

**Build it:**
```bash
west build -b <board> my-new-app
```

### Method 2: Copy and Modify Sample App

```bash
# Copy the sample app
cp -r app my-sensor-app

# Modify as needed
cd my-sensor-app
# Edit src/main.c, prj.conf, etc.

# Build
west build -b <board> my-sensor-app
```

### Method 3: From Zephyr Sample

```bash
# Copy a Zephyr sample
cp -r zephyr/samples/basic/blinky my-blinky-app

# Customize
cd my-blinky-app
# Modify files as needed

# Build
west build -b <board> my-blinky-app
```

### Application with Subdirectories

For larger applications:

```bash
my-complex-app/
├── CMakeLists.txt
├── prj.conf
├── Kconfig
├── README.md
├── include/
│   ├── app_config.h
│   └── sensors.h
├── src/
│   ├── main.c
│   ├── sensors.c
│   ├── network.c
│   └── storage.c
└── boards/
    ├── nrf52840dk_nrf52840.conf
    └── nrf52840dk_nrf52840.overlay
```

**CMakeLists.txt:**
```cmake
cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(my_complex_app)

# Add all source files
target_sources(app PRIVATE
    src/main.c
    src/sensors.c
    src/network.c
    src/storage.c
)

# Add include directory
target_include_directories(app PRIVATE include)
```

## Adding Custom Zephyr Modules

Zephyr modules extend Zephyr with custom drivers, libraries, or boards.

### Module Structure

```bash
# Create module directory
mkdir -p modules/my-module

modules/my-module/
├── zephyr/
│   ├── module.yml          # Module metadata
│   ├── CMakeLists.txt      # Build integration
│   └── Kconfig             # Configuration options
├── include/
│   └── my_module/
│       └── api.h
├── src/
│   └── my_module.c
└── README.md
```

### Module Metadata (zephyr/module.yml)

```yaml
name: my-module
build:
  cmake: .
  kconfig: zephyr/Kconfig
```

### Module CMakeLists.txt

```cmake
# modules/my-module/zephyr/CMakeLists.txt

if(CONFIG_MY_MODULE)
    zephyr_include_directories(../include)
    zephyr_library()
    zephyr_library_sources(../src/my_module.c)
endif()
```

### Module Kconfig

```
# modules/my-module/zephyr/Kconfig

config MY_MODULE
    bool "Enable My Custom Module"
    default n
    help
      Enable custom module functionality.

if MY_MODULE

config MY_MODULE_FEATURE
    bool "Enable feature"
    default y
    help
      Enable specific feature in module.

endif # MY_MODULE
```

### Register Module in West

**Option 1: Local module (already in workspace)**

Create `.west/config` or edit `west.yml`:

```yaml
# In west.yml, add to manifest.projects if you want west to manage it
# Or just reference it directly from applications

# When building, specify module path:
```

```bash
west build -b <board> app -- -DZEPHYR_MODULES=/path/to/modules/my-module
```

**Option 2: External Git repository**

Add to `west.yml`:

```yaml
manifest:
  projects:
    # ... existing projects ...

    - name: my-module
      url: https://github.com/myorg/my-module
      revision: main
      path: modules/my-module
```

Update workspace:

```bash
west update
```

### Using the Module in Application

In `prj.conf`:

```conf
CONFIG_MY_MODULE=y
CONFIG_MY_MODULE_FEATURE=y
```

In application code:

```c
#include <my_module/api.h>

void my_function(void) {
    my_module_init();
    my_module_do_something();
}
```

## Adding External Dependencies

### Adding External Git Repositories

Edit `west.yml`:

```yaml
manifest:
  version: "0.13"

  remotes:
    - name: zephyrproject-rtos
      url-base: https://github.com/zephyrproject-rtos
    - name: myorg
      url-base: https://github.com/myorg

  projects:
    - name: zephyr
      remote: zephyrproject-rtos
      revision: main
      import: true

    # Add your external dependency
    - name: my-library
      remote: myorg
      revision: v1.0.0
      path: external/my-library

    # Another example
    - name: sensor-drivers
      url: https://github.com/someorg/sensor-drivers
      revision: main
      path: modules/sensor-drivers
```

Update and fetch:

```bash
west update
```

### Adding as Zephyr Module

If the external repo is a Zephyr module, it will be automatically detected. If not, specify it manually:

```bash
west build -b <board> app -- \
    -DZEPHYR_MODULES="modules/my-module;external/my-library"
```

Or set in CMakeLists.txt:

```cmake
list(APPEND ZEPHYR_EXTRA_MODULES
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/my-module
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/my-library
)
```

### Using Zephyr's Module System

Create `zephyr/module.yml` in external library:

```yaml
name: my-library
build:
  cmake: .
  kconfig: Kconfig
  settings:
    board_root: .
    dts_root: .
```

## Organizing Multi-Project Workspaces

### Recommended Structure

```bash
workspace/
├── .git/
├── .gitignore
├── west.yml
├── README.md
├── docs/
├── apps/                    # Multiple applications
│   ├── sensor-app/
│   ├── gateway-app/
│   └── test-app/
├── modules/                 # Custom modules
│   ├── common-lib/
│   └── hardware-drivers/
├── boards/                  # Custom board definitions
│   └── my-custom-board/
├── scripts/                 # Build scripts, utilities
│   ├── build-all.sh
│   └── flash-all.sh
├── zephyr/                  # Managed by west
├── modules/                 # HALs, etc. (managed by west)
└── tools/                   # Tools (managed by west)
```

### Building Multiple Applications

Create a build script (`scripts/build-all.sh`):

```bash
#!/bin/bash

BOARD=${1:-nrf52840dk/nrf52840}

echo "Building all applications for $BOARD"

west build -b $BOARD apps/sensor-app -d build/sensor-app
west build -b $BOARD apps/gateway-app -d build/gateway-app
west build -b $BOARD apps/test-app -d build/test-app

echo "All builds complete!"
```

Run it:

```bash
chmod +x scripts/build-all.sh
./scripts/build-all.sh nrf52840dk/nrf52840
```

## Sharing Code Between Projects

### Method 1: Shared Library as Module

Create a shared library module:

```bash
modules/common-lib/
├── zephyr/
│   ├── module.yml
│   ├── CMakeLists.txt
│   └── Kconfig
├── include/
│   └── common/
│       ├── utils.h
│       └── config.h
└── src/
    ├── utils.c
    └── helpers.c
```

Use in applications:

```c
// In any application
#include <common/utils.h>

void my_function(void) {
    common_util_init();
}
```

### Method 2: CMake Interface Library

In workspace root `CMakeLists.txt`:

```cmake
add_library(shared_utils INTERFACE)

target_include_directories(shared_utils INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/include
)

target_sources(shared_utils INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/src/utils.c
)
```

In application `CMakeLists.txt`:

```cmake
include(${CMAKE_CURRENT_SOURCE_DIR}/../shared.cmake)

target_link_libraries(app PRIVATE shared_utils)
```

### Method 3: Git Submodules

For truly shared code across multiple repositories:

```bash
# Add as submodule
git submodule add https://github.com/myorg/shared-lib modules/shared-lib

# Clone workspace with submodules
git clone --recursive <workspace-url>

# Or initialize after cloning
git submodule update --init --recursive
```

## Best Practices

### Application Organization

1. **One application per directory** - Each app should be self-contained
2. **Use descriptive names** - `sensor-app` not `app1`
3. **Include README** - Document purpose, dependencies, board support
4. **Board-specific configs** - Use `boards/` subdirectory for overlays

### Module Organization

1. **Follow Zephyr conventions** - Use standard structure (src/, include/, zephyr/)
2. **Version your modules** - Use Git tags for releases
3. **Document APIs** - Header comments and README
4. **Kconfig for features** - Make features configurable

### Workspace Management

1. **Pin dependencies** - Use specific commits/tags in `west.yml` for reproducibility
2. **Update regularly** - Keep Zephyr and modules up to date
3. **Test before committing** - Build all applications before pushing
4. **Use CI/CD** - Automate builds and tests

### Configuration Management

1. **Minimize prj.conf** - Only enable what you need
2. **Use overlays for variants** - Debug, release, test configurations
3. **Document choices** - Comment why you enabled specific options
4. **Board-specific only in boards/** - Keep main prj.conf generic

## Example: Complete Multi-App Workspace

Let's create a complete example:

```bash
# Create applications
mkdir -p apps/{sensor,gateway,bootloader}

# Create shared module
mkdir -p modules/protocol/zephyr/src
mkdir -p modules/protocol/include/protocol

# Add to west.yml (if needed)
# Build all
west build -b nrf52840dk/nrf52840 apps/sensor -d build/sensor
west build -b nrf52840dk/nrf52840 apps/gateway -d build/gateway
```

Each application can now:
- Use the shared protocol module
- Have its own configuration
- Be built independently
- Share common dependencies via west

## Troubleshooting

### Module Not Found

```bash
# List detected modules
west list

# Manually specify modules
west build -b <board> app -- -DZEPHYR_MODULES=/path/to/module
```

### Kconfig Option Not Available

1. Check module's `Kconfig` is included
2. Verify module.yml is correct
3. Run `west build -t menuconfig` to verify option exists

### CMake Can't Find Files

1. Check paths in CMakeLists.txt
2. Verify `zephyr_include_directories()` is used
3. Check module.yml cmake path

### West Update Fails

```bash
# Force update
west update --rebase

# Or reset to manifest state
west forall -c "git reset --hard HEAD"
west update
```

## Resources

- [Zephyr Modules](https://docs.zephyrproject.org/latest/develop/modules.html)
- [West Manifest](https://docs.zephyrproject.org/latest/develop/west/manifest.html)
- [CMake in Zephyr](https://docs.zephyrproject.org/latest/build/cmake/index.html)

---

With this knowledge, you can now create complex multi-project workspaces that scale with your needs!
