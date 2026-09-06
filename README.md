# Copperhead Flight Software

All software that will run on the Copperhead vehicle.

## Requirements
- arm-none-eabi toolchain (see [Installing the Toolchain](#installing-the-toolchain))
- CMake
- Make or Ninja
  - Ninja is significantly faster than Make
- clang-format and clang-tidy for formatting and linting, respectively

## Building
Make sure submodules are pulled in:
```bash
git submodule init
git submodule update --depth=1
```
And then start the build:
```bash
cmake -B build -DTARGET=<TARGET>
cmake --build build
```

Supported Targets:
1. `STM32H730`
2. `Native`

## Installing the Toolchain

This project targets bare-metal ARM, so it needs the `arm-none-eabi` variant of the Arm GNU Toolchain. The latest release is 15.2.Rel1. The project's `toolchain/arm-none-eabi.cmake` works with either install method below; if the Debian paths it references (`/usr/lib/arm-none-eabi`) are absent, GCC falls back to its own bundled libraries.

### Linux

**Option A — apt (recommended, matches CI):**
```bash
sudo apt update
sudo apt install -y gcc-arm-none-eabi libnewlib-arm-none-eabi
```
On Ubuntu 24.04 this provides Arm GNU Toolchain 13.2.Rel1. The CI Docker image (`ci/Dockerfile`) installs the toolchain this way.

**Option B — official Arm tarball (latest version):**

For an x86_64 host:
```bash
wget https://developer.arm.com/-/media/Files/downloads/gnu/15.2.rel1/binrel/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi.tar.xz
tar xJf arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi.tar.xz -C $HOME
echo 'export PATH="$PATH:$HOME/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi/bin"' >> ~/.bashrc
source ~/.bashrc
```
For an aarch64 Linux host, substitute `arm-gnu-toolchain-15.2.rel1-aarch64-arm-none-eabi.tar.xz` in the commands above.

### Windows

The Windows build of the toolchain is 32-bit (i686) but runs fine on 64-bit Windows.

**Option A — MSI installer:**

Download `arm-gnu-toolchain-15.2.rel1-mingw-w64-i686-arm-none-eabi.msi` from the [Arm GNU Toolchain downloads page](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) and double-click it, or install silently from a command prompt:
```
msiexec /i arm-gnu-toolchain-15.2.rel1-mingw-w64-i686-arm-none-eabi.msi EULA=1 /quiet
```

**Option B — portable zip:**

Download `arm-gnu-toolchain-15.2.rel1-mingw-w64-i686-arm-none-eabi.zip` and extract it (e.g. to `C:\arm-gnu-toolchain`), then add `<extract-dir>\bin` to your PATH via Settings → System → About → Advanced system settings → Environment Variables… → select `Path` → Edit → New → browse to the `bin` folder.

(From a command prompt you can instead run `setx PATH "%PATH%;C:\arm-gnu-toolchain\bin"`, but `setx` truncates PATH once it exceeds 1024 characters, so the GUI route is safer.)

### Verify

Open a new terminal and check:
```bash
arm-none-eabi-gcc --version
```
You should see output like:
```
arm-none-eabi-gcc (Arm GNU Toolchain 15.2.Rel1 (Build arm-15.86)) 15.2.1 20251203
```
