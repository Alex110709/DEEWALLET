# Agent Guidelines for DEEWALLET (C++ Version)

## Project Structure
- **Language**: C++17
- **Framework**: Qt 6.8 (Widgets, Network, Concurrent)
- **Build System**: CMake 3.20+
- **Encryption**: OpenSSL 3.x
- **Chains**: 15 blockchains (Bitcoin, Ethereum + 10 EVM, Tron, Solana)

## Build & Run Commands
```bash
# Build
cd deewallet-cpp/build
export OPENSSL_ROOT_DIR=/opt/homebrew/opt/openssl@3
cmake --build . --config Release

# Run
./deewallet.app/Contents/MacOS/deewallet

# Clean build
rm -rf build && cmake -S . -B build && cmake --build build
```

## Code Style
- **C++ Standard**: C++17, use modern features (auto, smart pointers, lambdas)
- **Naming**: 
  - camelCase for variables/functions
  - PascalCase for classes
  - ALL_CAPS for constants
- **Headers**: Include guards with `#ifndef CLASSNAME_H`
- **Memory**: Use RAII, smart pointers (unique_ptr, shared_ptr)
- **Qt Signals**: Use Qt::QueuedConnection for async operations
- **Error Handling**: Return empty QString/QByteArray on error, use qDebug() for logging
- **Async**: Use QtConcurrent::run() for heavy operations (crypto, network)
- **Security**: 
  - Use OPENSSL_cleanse() for sensitive data
  - Never log private keys or mnemonics
  - Use SecureMemory class for key material

## Project Organization
```
deewallet-cpp/
├── src/
│   ├── core/          # BIP39/32/44, KeyfileManager, SecureMemory
│   ├── chains/        # Chain adapters (Bitcoin, Ethereum, etc)
│   ├── utils/         # AddressUtils, Keccak256, TokenDetector
│   ├── rpc/           # RPC client and manager
│   └── ui/            # Qt Widgets screens and dialogs
├── CMakeLists.txt
└── vcpkg.json
```

## Supported Chains

### Non-EVM Chains
- Bitcoin (BTC) - m/44'/0'/0'/0/0
- Litecoin (LTC) - m/44'/2'/0'/0/0
- Dogecoin (DOGE) - m/44'/3'/0'/0/0
- Tron (TRX) - m/44'/195'/0'/0/0
- Solana (SOL) - m/44'/501'/0'/0/0

### EVM Chains (All use m/44'/60'/0'/0/0)
- Ethereum (ETH) - Chain ID: 1
- BNB Chain (BNB) - Chain ID: 56
- Polygon (POL) - Chain ID: 137
- Arbitrum (ARB) - Chain ID: 42161
- Optimism (OP) - Chain ID: 10
- Avalanche (AVAX) - Chain ID: 43114
- Base (BASE) - Chain ID: 8453
- Fantom (FTM) - Chain ID: 250
- Cronos (CRO) - Chain ID: 25
- Gnosis (xDAI) - Chain ID: 100

## Important Notes
- **TypeScript version has been removed** - Only C++ version remains
- **EVM Chains**: All use same BIP44 path and address derivation
- **Token Support**: ERC-20, TRC-20, SPL auto-detection
- **Async Decryption**: Uses QtConcurrent to prevent UI blocking
- **QR Code**: Proper QR code generation with Finder + Timing patterns
- **Address Scanning**: BIP44 GAP limit (20) compliant

## Common Tasks

### Adding a new EVM chain
1. Add to WalletDetailScreen.cpp chains list
2. Add RPC URL to EthereumAdapter::getRpcUrl()
3. Add Chain ID to EthereumAdapter::getChainId()
4. Add Explorer URL to EthereumAdapter::getExplorerUrl()
5. Update WalletCore.cpp chain type checks

### Debugging
- Use qDebug() for console output
- Check build/app.log for runtime logs
- Use Qt Creator debugger for breakpoints
