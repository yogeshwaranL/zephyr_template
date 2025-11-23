# Sample Zephyr Application

This is a sample Zephyr RTOS application that demonstrates the basic structure of an application in the T3 forest template.

## Features

- Basic Zephyr kernel usage
- Logging subsystem demonstration
- Periodic task execution
- Example configuration options

## Building

From the workspace root:

```bash
west build -b <your_board> app
```

For example, to build for the nRF52840 DK:

```bash
west build -b nrf52840dk/nrf52840 app
```

## Flashing

```bash
west flash
```

## Expected Output

The application will print a "Hello World" message every 2 seconds with an incrementing counter.

## Customization

- Modify `src/main.c` for your application logic
- Update `prj.conf` for Zephyr configuration
- Add custom Kconfig options in `Kconfig`
- Add board-specific configurations in `boards/`
