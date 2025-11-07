# DEE WALLET - Complete Build Guide

## 📦 Prerequisites

### Required Software

1. **CMake** (3.20 or higher)
   ```bash
   # macOS (via Homebrew)
   brew install cmake

   # Windows (via Chocolatey)
   choco install cmake
   ```

2. **vcpkg** (Package Manager)
   ```bash
   # Clone vcpkg
   git clone https://github.com/microsoft/vcpkg.git
   cd vcpkg

   # macOS/Linux
   ./bootstrap-vcpkg.sh

   # Windows
   .\bootstrap-vcpkg.bat

   # Set environment variable
   export VCPKG_ROOT=$(pwd)  # macOS/Linux
   set VCPKG_ROOT=%CD%       # Windows CMD
   $env:VCPKG_ROOT="$PWD"    # Windows PowerShell
   ```

3. **C++ Compiler**
   - **macOS**: Xcode Command Line Tools
     ```bash
     xcode-select --install
     ```
   - **Windows**: Visual Studio 2019 or later
     - Download from: https://visualstudio.microsoft.com/downloads/
     - Install "Desktop development with C++" workload

### Installing Dependencies

```bash
# Navigate to vcpkg directory
cd $VCPKG_ROOT

# Install Qt6 and OpenSSL
vcpkg install qt6-base:x64-osx qt6-widgets:x64-osx qt6-network:x64-osx openssl:x64-osx  # macOS
vcpkg install qt6-base:x64-windows qt6-widgets:x64-windows qt6-network:x64-windows openssl:x64-windows  # Windows

# This may take 20-30 minutes on first install
```

## 🔨 Building DEE WALLET

### Step 1: Clone Repository

```bash
git clone https://github.com/yourusername/deewallet-cpp.git
cd deewallet-cpp
```

### Step 2: Configure with CMake

```bash
# Create build directory
mkdir build
cd build

# Configure (macOS/Linux)
cmake .. -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
         -DCMAKE_BUILD_TYPE=Release

# Configure (Windows)
cmake .. -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake ^
         -DCMAKE_BUILD_TYPE=Release
```

### Step 3: Build

```bash
# Build the project
cmake --build . --config Release

# On macOS/Linux, you can also use:
make -j$(nproc)
```

### Step 4: Run

```bash
# macOS/Linux
./deewallet

# Windows
.\Release\deewallet.exe

# macOS (if built as bundle)
open deewallet.app
```

## 🧪 Testing the Build

### Quick Test

1. Launch DEE WALLET
2. Click "Create Wallet"
3. Click "Generate Mnemonic"
4. Copy the generated mnemonic for testing
5. Enter password: `testpassword123`
6. Click "Create Wallet"
7. Verify all 4 addresses are generated

### Known Test Mnemonic

For testing with known addresses, use this BIP39 standard test mnemonic:

```
abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about
```

**Expected Addresses**:
- Bitcoin:  `bc1qcr8te4kr609gcawutmrza0j4xv80jy8z306fyu`
- Ethereum: `0x9858EfFD232B4033E47d90003D41EC34EcaEda94`
- Tron:     `TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC`
- Solana:   (varies based on Ed25519 implementation)

### Balance Query Test

1. Click "Refresh" button
2. Wait for network requests (5-10 seconds)
3. Verify balance displays (should show 0.0 for new addresses)
4. Check console for any network errors

## 🐛 Troubleshooting

### Build Errors

**Error: Qt6 not found**
```bash
# Solution: Reinstall Qt6 via vcpkg
vcpkg install qt6-base qt6-widgets qt6-network --recurse
```

**Error: OpenSSL not found**
```bash
# Solution: Reinstall OpenSSL
vcpkg install openssl
```

**Error: vcpkg toolchain not found**
```bash
# Solution: Verify VCPKG_ROOT is set correctly
echo $VCPKG_ROOT  # macOS/Linux
echo %VCPKG_ROOT%  # Windows

# Should point to vcpkg installation directory
```

### Runtime Errors

**Error: libQt6Core.dylib not found (macOS)**
```bash
# Solution: Copy Qt libraries to app bundle
install_name_tool -add_rpath @executable_path/../Frameworks deewallet
```

**Error: Qt platform plugin not found (Windows)**
```bash
# Solution: Copy Qt plugins to build directory
xcopy /E /I %VCPKG_ROOT%\installed\x64-windows\plugins build\Release\plugins
```

**Network Request Timeout**
- Check internet connection
- Some APIs may have rate limits
- Try again after 1-2 minutes

### Clean Build

If you encounter persistent issues:

```bash
# Remove build directory
cd ..
rm -rf build  # macOS/Linux
rmdir /s build  # Windows

# Reconfigure and rebuild
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

## 📦 Creating Distribution Packages

### macOS (.dmg)

```bash
# Install create-dmg
brew install create-dmg

# Create app bundle
cmake --build . --target install

# Create DMG
create-dmg \
  --volname "DEE WALLET" \
  --window-pos 200 120 \
  --window-size 600 300 \
  --icon-size 100 \
  --app-drop-link 450 150 \
  deewallet-1.0.0.dmg \
  deewallet.app
```

### Windows (.msi)

1. Install WiX Toolset: https://wixtoolset.org/
2. Create installer project using CMake CPack:

```cmake
# Add to CMakeLists.txt
set(CPACK_GENERATOR "WIX")
set(CPACK_PACKAGE_NAME "DEE WALLET")
set(CPACK_PACKAGE_VERSION "1.0.0")
include(CPack)
```

3. Build installer:

```bash
cpack -G WIX
```

## 🔒 Security Considerations

### Build Verification

```bash
# Verify binary signature (macOS)
codesign -v deewallet.app

# Check for hardening flags
otool -l deewallet | grep __PAGEZERO  # Should show large size

# Windows: Verify executable
sigcheck deewallet.exe
```

### Secure Build Environment

1. **Use official dependencies only**
   - All packages installed via vcpkg are from verified sources

2. **Verify repository integrity**
   ```bash
   git verify-commit HEAD
   ```

3. **Build in clean environment**
   - Use fresh VM or container for release builds
   - Verify no external libraries are dynamically linked

## 🚀 Optimization Builds

### Performance Build

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DCMAKE_CXX_FLAGS="-O3 -march=native" \
         -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```

### Debug Build

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug \
         -DCMAKE_CXX_FLAGS="-g -O0 -fsanitize=address" \
         -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```

### Size-Optimized Build

```bash
cmake .. -DCMAKE_BUILD_TYPE=MinSizeRel \
         -DCMAKE_CXX_FLAGS="-Os -flto" \
         -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```

## 📊 Build Metrics

### Expected Build Times (Release, parallel build)

| Platform | CPU | Time |
|----------|-----|------|
| macOS M1 | 8 cores | ~5 min |
| macOS Intel i7 | 4 cores | ~8 min |
| Windows Ryzen 9 | 16 cores | ~6 min |
| Windows i5 | 4 cores | ~12 min |

### Binary Sizes

| Platform | Debug | Release | MinSizeRel |
|----------|-------|---------|------------|
| macOS | ~45 MB | ~8 MB | ~5 MB |
| Windows | ~60 MB | ~12 MB | ~7 MB |

## 🔄 Continuous Integration

### GitHub Actions Example

```yaml
name: Build DEE WALLET

on: [push, pull_request]

jobs:
  build:
    strategy:
      matrix:
        os: [macos-latest, windows-latest]

    runs-on: ${{ matrix.os }}

    steps:
    - uses: actions/checkout@v3

    - name: Install vcpkg
      run: |
        git clone https://github.com/microsoft/vcpkg.git
        cd vcpkg
        ./bootstrap-vcpkg.sh  # macOS
        # .\bootstrap-vcpkg.bat  # Windows

    - name: Install dependencies
      run: |
        vcpkg install qt6-base qt6-widgets qt6-network openssl

    - name: Configure CMake
      run: |
        mkdir build
        cd build
        cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake

    - name: Build
      run: cmake --build build --config Release

    - name: Test
      run: |
        cd build
        ctest -C Release
```

## 📝 Development Workflow

### IDE Setup

**Visual Studio Code**:
1. Install "CMake Tools" extension
2. Install "C/C++" extension
3. Configure in `.vscode/settings.json`:

```json
{
  "cmake.configureArgs": [
    "-DCMAKE_TOOLCHAIN_FILE=${env:VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
  ]
}
```

**CLion**:
1. Open project
2. File → Settings → Build → CMake
3. Set CMake options:
   ```
   -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
   ```

## 🎯 Next Steps

After successful build:

1. ✅ **Test wallet creation and import**
2. ✅ **Verify address generation for all chains**
3. ✅ **Test balance refresh functionality**
4. ⏳ **Implement transaction signing (Phase 6)**
5. ⏳ **Add token support (ERC-20, TRC-20, SPL)**
6. ⏳ **Create distribution packages**

## 📞 Support

If you encounter build issues:

1. Check [Troubleshooting](#troubleshooting) section
2. Search [GitHub Issues](https://github.com/yourusername/deewallet-cpp/issues)
3. Create new issue with:
   - Operating system and version
   - Build output/error messages
   - CMake version (`cmake --version`)
   - vcpkg version (`vcpkg version`)

---

**Happy Building! 🚀**
