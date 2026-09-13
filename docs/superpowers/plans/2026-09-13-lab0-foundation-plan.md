# Lab 0 Foundation Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Verify the local Zephyr/NCS toolchains and complete the first Vanilla Zephyr Blinky build/flash evidence for the three Lab 0 boards.

**Architecture:** Keep the course repository at `/Users/ericcc/Desktop/ESE 5180/lab0-zephyr-skeleton`. Use the existing upstream Zephyr workspace for the XIAO and Nucleo targets, and use Nordic nRF Connect SDK v3.4.x for the course-specific nRF7002 DK `ns` target. Keep build directories outside the submission repository.

**Tech Stack:** Zephyr RTOS, Nordic nRF Connect SDK 3.4.x, West, CMake, Ninja, Python 3.12, QEMU, Git, VS Code, XIAO nRF52840 Sense, STM32 Nucleo F411RE, nRF7002 DK.

---

## Task 1: Capture the clean repository and toolchain baseline

**Files:**
- Modify: `README.md` after the baseline commands are verified.
- Do not commit generated build directories.

- [ ] **Step 1: Verify repository state**

Run:

```bash
cd "/Users/ericcc/Desktop/ESE 5180/lab0-zephyr-skeleton"
git status --short --branch
git log -2 --oneline --decorate
```

Expected: branch `main`, design commit `46af088`, and no untracked implementation files.

- [ ] **Step 2: Verify upstream Zephyr tools**

Run:

```bash
cd /Users/ericcc/zephyrproject
source .venv/bin/activate
python --version
west --version
cmake --version | head -n 1
ninja --version
qemu-system-arm --version | head -n 1
west topdir
git -C zephyr describe --tags --always --dirty
```

Expected: Python 3.12.x, working West/CMake/Ninja/QEMU, topdir `/Users/ericcc/zephyrproject`, and a Zephyr revision.

- [ ] **Step 3: Check NCS without changing it**

Run:

```bash
command -v nrfutil || true
find /Users/ericcc -maxdepth 4 -type d \( -name 'nrf-connect-sdk-*' -o -name 'v3.4.*' \) -print 2>/dev/null
find /opt /Applications -maxdepth 4 -iname '*nrf*' -print 2>/dev/null | sed -n '1,120p'
```

Record whether NCS 3.4.x exists. If it does not, record the dependency and do not substitute the upstream `nrf7002dk/nrf5340/cpuapp` target for the required `nrf7002dk/nrf5340/cpuapp/ns` target.

- [ ] **Step 4: Record the observed values in `README.md`**

Add an `## Environment Baseline` table containing the actual Python, West, Zephyr revision, NCS, and QEMU values printed above. State that build directories remain outside the repository.

- [ ] **Step 5: Verify and commit the baseline notes**

Run:

```bash
cd "/Users/ericcc/Desktop/ESE 5180/lab0-zephyr-skeleton"
git diff --check
git diff -- README.md
git status --short
git add README.md
git commit -m "docs: record lab 0 environment baseline"
```

Expected: no whitespace errors and only the intended README change in the commit.

## Task 2: Build and flash XIAO nRF52840 Sense

**Build output:** `/tmp/ese5180-lab0-build-xiao`.

- [ ] **Step 1: Confirm the target**

Use `xiao_ble/nrf52840/sense`; verify it with `west boards` and the board YAML before building.

- [ ] **Step 2: Build the stock sample**

Run:

```bash
cd /Users/ericcc/zephyrproject
source .venv/bin/activate
west build -p always -d /tmp/ese5180-lab0-build-xiao \
  -b xiao_ble/nrf52840/sense \
  zephyr/samples/basic/blinky
```

Expected: exit code 0 and a Zephyr executable under `/tmp/ese5180-lab0-build-xiao/zephyr/`.

- [ ] **Step 3: Connect, identify, and flash the XIAO**

Connect the data-capable USB-C cable, confirm a new USB/serial device with `system_profiler SPUSBDataType` and `/dev/cu.*`, then run:

```bash
cd /Users/ericcc/zephyrproject
source .venv/bin/activate
west flash -d /tmp/ese5180-lab0-build-xiao
```

Expected: a supported runner reports a successful flash. Observe the onboard LED for at least 10 seconds and record the actual behavior in README.

## Task 3: Build and flash STM32 Nucleo F411RE

**Build output:** `/tmp/ese5180-lab0-build-nucleo-f411re`.

- [ ] **Step 1: Build the stock sample**

Run:

```bash
cd /Users/ericcc/zephyrproject
source .venv/bin/activate
west build -p always -d /tmp/ese5180-lab0-build-nucleo-f411re \
  -b nucleo_f411re \
  zephyr/samples/basic/blinky
```

Expected: exit code 0 and a Zephyr executable under the build directory.

- [ ] **Step 2: Connect, identify, and flash the Nucleo**

Connect the Mini-USB data cable to the ST-LINK USB connector. Confirm the ST-LINK device, then run:

```bash
cd /Users/ericcc/zephyrproject
source .venv/bin/activate
west flash -d /tmp/ese5180-lab0-build-nucleo-f411re
```

Expected: the flash runner succeeds. Observe the user LED for at least 10 seconds and record the actual behavior in README.

## Task 4: Prepare and verify the nRF7002 DK path

**Target:** `nrf7002dk/nrf5340/cpuapp/ns`.

- [ ] **Step 1: Connect the DK**

Connect the Micro-USB data cable to the DK debug USB port. Confirm the onboard J-Link/Nordic device in `system_profiler SPUSBDataType` and `/dev/cu.*`.

- [ ] **Step 2: Use the NCS 3.4.x environment**

Select the exact course target `nrf7002dk/nrf5340/cpuapp/ns` in the NCS/VS Code environment. The build must resolve Zephyr and all NCS modules from the same NCS installation.

- [ ] **Step 3: Build, flash, and verify Nordic Blinky**

Run the exact build and flash commands exposed by the installed NCS environment, record them in README, and observe the DK user LED. Do not claim hardware success until the LED is visibly blinking.

## Task 5: First checkpoint and video evidence

- [ ] **Step 1: Update README evidence**

Record the three board targets, exact build/flash commands, toolchain used, and observed LED behavior. Keep build directories out of Git.

- [ ] **Step 2: Capture the required video**

The `f26_lab0_1.1_pennkey` video must show terminal history for compiling/flashing all three boards and each board's LED blinking.

- [ ] **Step 3: Verify and commit**

Run:

```bash
cd "/Users/ericcc/Desktop/ESE 5180/lab0-zephyr-skeleton"
git diff --check
git status --short
git add README.md
git commit -m "feat: verify vanilla zephyr blinky targets"
```

Expected: only intended README/evidence changes are committed. The next implementation plan starts after the NCS result and all available build/flash evidence are recorded.
