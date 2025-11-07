# DEE WALLET - Detailed Build Guide

## 🍎 macOS Build Instructions

### Prerequisites

```bash
# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install required tools
brew install cmake git

# Install Qt 6
brew install qt@6

# Add Qt to PATH
echo 'export PATH="/opt/homebrew/opt/qt@6/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc
```

### Install vcpkg

```bash
cd ~
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh

# Add to shell profile
echo 'export VCPKG_ROOT=~/vcpkg' >> ~/.zshrc
echo 'export PATH="$VCPKG_ROOT:$PATH"' >> ~/.zshrc
source ~/.zshrc
```

### Build DEE WALLET

```bash
cd /Users/yuchan/Desktop/DEEWALLET/deewallet-cpp

# Install dependencies
$VCPKG_ROOT/vcpkg install

# Create build directory
mkdir build && cd build

# Configure (automatically finds Qt via Homebrew)
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
  -DCMAKE_PREFIX_PATH=/opt/homebrew/opt/qt@6 \
  -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --config Release -j$(sysctl -n hw.ncpu)

# Run
./deewallet
```

### Create macOS .dmg Package

```bash
# Build package
cmake --build . --target package

# Output: DEE WALLET-1.0.0-Darwin.dmg
```

### Code Signing (Optional, requires Apple Developer account)

```bash
# Sign the app bundle
codesign --force --deep --sign "Developer ID Application: Your Name (TEAM_ID)" \
  ./deewallet.app

# Verify signature
codesign --verify --deep --strict --verbose=2 ./deewallet.app

# Notarize for distribution
xcrun notarytool submit DEE\ WALLET-1.0.0-Darwin.dmg \
  --apple-id your@email.com \
  --team-id TEAM_ID \
  --password app-specific-password
```

## 🪟 Windows Build Instructions

### Prerequisites

1. **Install Visual Studio 2022**:
   - Download from https://visualstudio.microsoft.com/
   - Select "Desktop development with C++" workload
   - Include CMake tools

2. **Install Qt 6**:
   - Download from https://www.qt.io/download-qt-installer
   - Select Qt 6.8+ for MSVC 2022 64-bit
   - Default install path: `C:\Qt\6.8.0\msvc2022_64`

3. **Install vcpkg**:
   ```powershell
   cd C:\
   git clone https://github.com/microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat

   # Add to environment variables
   [Environment]::SetEnvironmentVariable("VCPKG_ROOT", "C:\vcpkg", "User")
   [Environment]::SetEnvironmentVariable("Path", "$env:Path;C:\vcpkg", "User")
   ```

### Build DEE WALLET

```powershell
cd C:\Users\YourName\Desktop\DEEWALLET\deewallet-cpp

# Install dependencies
$env:VCPKG_ROOT\vcpkg.exe install

# Create build directory
mkdir build
cd build

# Configure
cmake .. `
  -DCMAKE_TOOLCHAIN_FILE=$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake `
  -DCMAKE_PREFIX_PATH=C:\Qt\6.8.0\msvc2022_64 `
  -G "Visual Studio 17 2022" `
  -A x64

# Build
cmake --build . --config Release

# Run
.\Release\deewallet.exe
```

### Create Windows Installer

```powershell
# Install NSIS
choco install nsis

# Build installer
cpack -G NSIS

# Output: DEE WALLET-1.0.0-win64.exe
```

## 🐧 Linux Build Instructions (Ubuntu/Debian)

### Prerequisites

```bash
# Update package list
sudo apt update

# Install build tools
sudo apt install -y build-essential cmake git

# Install Qt 6
sudo apt install -y qt6-base-dev qt6-tools-dev libqt6network6

# Install OpenSSL
sudo apt install -y libssl-dev
```

### Install vcpkg

```bash
cd ~
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh

echo 'export VCPKG_ROOT=~/vcpkg' >> ~/.bashrc
echo 'export PATH="$VCPKG_ROOT:$PATH"' >> ~/.bashrc
source ~/.bashrc
```

### Build DEE WALLET

```bash
cd ~/Desktop/DEEWALLET/deewallet-cpp

# Install dependencies
$VCPKG_ROOT/vcpkg install

# Create build directory
mkdir build && cd build

# Configure
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
  -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . -j$(nproc)

# Run
./deewallet
```

## 🛠️ Development Build (with Debug Symbols)

```bash
# Configure with debug symbols
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
  -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build .

# Run with debugger (macOS/Linux)
lldb ./deewallet

# Run with debugger (Windows)
devenv /debugexe .\Debug\deewallet.exe
```

## 🧪 Testing

```bash
# Enable tests in CMakeLists.txt first
cmake .. -DBUILD_TESTING=ON

# Build tests
cmake --build . --target tests

# Run tests
ctest --output-on-failure
```

## 📦 Static Linking

To create a fully standalone executable with no external dependencies:

```bash
# macOS/Linux
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_SHARED_LIBS=OFF \
  -DQt6_DIR=/path/to/qt/lib/cmake/Qt6

# Windows
cmake .. `
  -DCMAKE_TOOLCHAIN_FILE=$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake `
  -DCMAKE_BUILD_TYPE=Release `
  -DBUILD_SHARED_LIBS=OFF
```

## 🔧 Troubleshooting

### Issue: Qt not found

**Solution (macOS)**:
```bash
export Qt6_DIR=/opt/homebrew/opt/qt@6/lib/cmake/Qt6
cmake .. -DQt6_DIR=$Qt6_DIR
```

**Solution (Windows)**:
```powershell
$env:Qt6_DIR = "C:\Qt\6.8.0\msvc2022_64\lib\cmake\Qt6"
cmake .. -DQt6_DIR=$env:Qt6_DIR
```

### Issue: vcpkg dependencies not found

**Solution**:
```bash
# Reinstall dependencies
$VCPKG_ROOT/vcpkg remove --outdated
$VCPKG_ROOT/vcpkg install

# Update vcpkg
cd $VCPKG_ROOT
git pull
./bootstrap-vcpkg.sh  # or .bat on Windows
```

### Issue: OpenSSL version mismatch

**Solution**:
```bash
# macOS
brew upgrade openssl

# Windows
vcpkg remove openssl
vcpkg install openssl:x64-windows
```

### Issue: Missing MOC/UIC files

**Solution**:
```bash
# Clean build
rm -rf build
mkdir build && cd build
cmake .. -DCMAKE_AUTOMOC=ON -DCMAKE_AUTOUIC=ON
```

## 📊 Build Performance Tips

1. **Use Ninja** (faster than Make):
   ```bash
   # Install Ninja
   brew install ninja  # macOS
   sudo apt install ninja-build  # Linux
   choco install ninja  # Windows

   # Configure with Ninja
   cmake .. -G Ninja
   ```

2. **Parallel builds**:
   ```bash
   # macOS/Linux
   cmake --build . -j$(nproc)

   # Windows
   cmake --build . --parallel
   ```

3. **Precompiled headers** (already enabled in CMakeLists.txt):
   ```cmake
   target_precompile_headers(deewallet PRIVATE <QString> <QByteArray>)
   ```

## 🔍 Verification

```bash
# Check binary size
ls -lh ./deewallet  # macOS/Linux
dir .\Release\deewallet.exe  # Windows

# Check dependencies (macOS)
otool -L ./deewallet

# Check dependencies (Linux)
ldd ./deewallet

# Check dependencies (Windows)
dumpbin /dependents .\Release\deewallet.exe
```

## 📝 CI/CD Setup (GitHub Actions)

Create `.github/workflows/build.yml`:

```yaml
name: Build DEE WALLET

on: [push, pull_request]

jobs:
  build-macos:
    runs-on: macos-latest
    steps:
      - uses: actions/checkout@v3
      - name: Install dependencies
        run: |
          brew install cmake qt@6
          git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
          ~/vcpkg/bootstrap-vcpkg.sh
      - name: Build
        run: |
          mkdir build && cd build
          cmake .. -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake
          cmake --build . -j$(sysctl -n hw.ncpu)

  build-windows:
    runs-on: windows-latest
    steps:
      - uses: actions/checkout@v3
      - name: Install dependencies
        run: |
          choco install qt6
          git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
          C:\vcpkg\bootstrap-vcpkg.bat
      - name: Build
        run: |
          mkdir build && cd build
          cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
          cmake --build . --config Release
```

---

**For support, open an issue on GitHub** 🚀
