# ESE5180: Lab 0 Zephyr

| Team Member Name | Email Address                  |
| ---------------- | ------------------------------ |
| Fengyu Wu        | ericwu77@engineering.upenn.edu |

**GitHub Repository URL:** https://github.com/FengyuWu-77/lab0-zephyr-skeleton

## 1. Sample Header

## 2. Sample Second Header

## 3. Building and Flashing with West

The Nordic Zephyr application was built and flashed using West commands only,
without the graphical user interface.

### West build

The application was built for `nrf7002dk/nrf5340/cpuapp/ns` with Sysbuild.
The `BOARD_ROOT` option makes the application's board overlay available during
the build.

![West build command](section3-west-build-start.png)

![West build result](section3-west-build-finish.png)

### West flash

The generated image was flashed with the J-Link runner.

![West flash result](section3-west-flash.png)

### West command explanations

- `west init`: creates a West workspace and initializes its manifest.
- `west update`: downloads and synchronizes the repositories listed in the manifest.
- `west build`: configures and builds the application for the selected board.
- `west flash`: invokes the selected runner to program the built image.
- `--build-dir`: places generated build files in the specified out-of-tree directory.
- Application path: identifies the application source directory.
- `--pristine`: starts from a clean build directory.
- `--board`: selects the target board and processor variant.
- `--sysbuild`: builds the multi-image system, including TF-M and the application.
- `-DBOARD_ROOT`: specifies the directory containing application-specific board files.
- `--runner jlink`: selects the J-Link hardware programmer.

## Environment Baseline

| Item             | Value                               |
| ---------------- | ----------------------------------- |
| Host OS          | macOS                               |
| Python           | 3.12.14                             |
| West             | 1.5.0                               |
| CMake            | 4.3.1                               |
| Ninja            | 1.13.2                              |
| QEMU             | 11.1.1                              |
| Zephyr workspace | `/Users/ericcc/zephyrproject`     |
| Zephyr revision  | v4.4.0                              |
| Nordic NCS       | v3.4.0 (`/opt/nordic/ncs/v3.4.0`) |

Build directories are kept outside this repository. Vanilla Zephyr build/flash verification will start with the XIAO nRF52840 Sense and STM32 Nucleo F411RE. The nRF7002 DK target must use the course-required Nordic NCS 3.4.x environment.
