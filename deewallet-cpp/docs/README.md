# DEE WALLET - Multi-Chain Cryptocurrency Wallet

**A secure, native C++ cryptocurrency wallet supporting Bitcoin, Ethereum, Tron, and Solana**

![Platform](https://img.shields.io/badge/platform-macOS%20%7C%20Windows-blue)
![Language](https://img.shields.io/badge/language-C%2B%2B17-green)
![License](https://img.shields.io/badge/license-MIT-orange)

## 🚀 Features

### Multi-Chain Support
- **Bitcoin (BTC)** - Native SegWit (P2WPKH) addresses with bc1q... format
- **Ethereum (ETH)** - Full Ethereum support with ERC-20 token capability
- **Tron (TRX)** - Tron mainnet with TRC-20 token support
- **Solana (SOL)** - Solana mainnet with SPL token support

### Security Features
- 🔐 **BIP39** - Industry-standard mnemonic generation (12-word recovery phrases)
- 🔑 **BIP32/BIP44** - Hierarchical deterministic key derivation
- 🛡️ **AES-256-GCM** - Military-grade encryption for keyfiles
- 🔒 **Secure Memory** - RAII pattern with `OPENSSL_cleanse` for sensitive data
- 💾 **Portable Keyfiles** - Encrypted wallet storage with password protection

### User Interface
- 🎨 Modern dark-themed Qt interface
- 📱 Native desktop performance (Qt 6.8)
- 💰 Real-time balance display
- 📊 Multi-currency total balance (USD)
- 📋 One-click address copying
- 🔄 Auto-scan for keyfiles on startup

### Blockchain Integration
- ⚡ Real-time balance queries via public APIs
  - Bitcoin: Blockstream API
  - Ethereum: LlamaRPC (public endpoint)
  - Tron: TronGrid API
  - Solana: Mainnet RPC
- 🌐 Cross-platform HTTP client (Qt Network)
- 🔄 Automatic failover and health checking

## 📋 Requirements

### Build Dependencies
- **CMake** 3.20 or higher
- **C++ Compiler** with C++17 support
  - macOS: Xcode Command Line Tools
  - Windows: Visual Studio 2019+
- **Qt 6.8+** (Core, Widgets, Network)
- **OpenSSL 3.x** (Crypto, SSL)
- **vcpkg** (Package manager)

### Runtime Requirements
- **Operating System**: macOS 10.15+ or Windows 10+
- **Internet Connection**: For balance queries and transaction broadcasting

## 🛠️ Build Instructions

### 1. Install vcpkg

```bash
# Clone vcpkg
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg

# Bootstrap (macOS/Linux)
./bootstrap-vcpkg.sh

# Bootstrap (Windows)
.\bootstrap-vcpkg.bat

# Set environment variable
export VCPKG_ROOT=/path/to/vcpkg  # macOS/Linux
set VCPKG_ROOT=C:\path\to\vcpkg   # Windows
```

### 2. Install Dependencies

```bash
# Install Qt6 and OpenSSL via vcpkg
vcpkg install qt6-base qt6-widgets qt6-network openssl
```

### 3. Build the Project

```bash
# Clone the repository
git clone https://github.com/yourusername/deewallet-cpp.git
cd deewallet-cpp

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake

# Build
cmake --build . --config Release

# Run
./deewallet  # macOS/Linux
.\Release\deewallet.exe  # Windows
```

## 📚 Usage Guide

### Creating a New Wallet

1. Launch **DEE WALLET**
2. Click **"Create Wallet"**
3. Click **"Generate Mnemonic"** to create a 12-word recovery phrase
4. **IMPORTANT**: Write down and securely store your recovery phrase
5. Enter a strong encryption password (8+ characters)
6. Click **"Create Wallet"**
7. Your wallet is created and addresses are displayed

### Importing an Existing Wallet

1. Click **"Import Wallet from File"**
2. Browse and select your `.keyfile`
3. Enter your keyfile password
4. Your wallet is restored with all addresses

### Viewing Balances

- Addresses are automatically generated for all supported chains
- Click **"Refresh"** to fetch current balances from blockchain
- Total balance is displayed in USD (approximate exchange rates)
- Each chain tab shows:
  - Address (click "Copy Address" to clipboard)
  - Current balance in native currency

### Security Best Practices

⚠️ **CRITICAL REMINDERS**:
1. **NEVER share your recovery phrase** - Anyone with it can access your funds
2. **Store recovery phrase offline** - Write it on paper, not digital files
3. **Use a strong password** - Minimum 8 characters, mix of letters/numbers/symbols
4. **Backup your keyfile** - Store encrypted keyfile in multiple secure locations
5. **Verify addresses** - Always double-check recipient addresses before sending

## 🏗️ Architecture

### Project Structure

```
deewallet-cpp/
├── src/
│   ├── core/           # Core wallet functionality
│   │   ├── WalletCore  # Main wallet engine
│   │   ├── BIP39       # Mnemonic generation
│   │   ├── BIP32       # HD key derivation
│   │   ├── KeyfileManager  # Encryption/decryption
│   │   └── SecureMemory    # Memory security
│   ├── chains/         # Blockchain adapters
│   │   ├── BitcoinAdapter  # P2WPKH addresses
│   │   ├── EthereumAdapter # Keccak256 addresses
│   │   ├── TronAdapter     # Base58Check addresses
│   │   └── SolanaAdapter   # Ed25519 addresses
│   ├── utils/          # Utility libraries
│   │   ├── AddressUtils    # Base58, Bech32 encoding
│   │   └── Keccak256       # Keccak hash (FIPS 202)
│   ├── rpc/            # Network communication
│   │   ├── RPCClient   # JSON-RPC client
│   │   └── RPCManager  # Endpoint management
│   └── ui/             # User interface
│       ├── MainWindow
│       ├── WelcomeScreen
│       ├── CreateWalletDialog
│       ├── ImportWalletDialog
│       └── WalletDetailScreen
├── docs/               # Documentation
├── build/              # Build output
└── CMakeLists.txt      # Build configuration
```

### Address Generation Algorithms

| Chain | Address Format | Algorithm | Example |
|-------|---------------|-----------|---------|
| Bitcoin | `bc1q...` (42-62 chars) | SHA256 → RIPEMD160 → Bech32 | `bc1qxy2kgdygjrsqtzq2n0yrf2493p83kkfjhx0wlh` |
| Ethereum | `0x...` (42 chars) | Keccak256 → last 20 bytes | `0x742d35Cc6634C0532925a3b844Bc9e7595f0bEb` |
| Tron | `T...` (34 chars) | SHA256 → last 20 bytes → Base58Check(0x41) | `TJCnKsPa7y5okkXvQAidZBzqx3QyQ6sxMW` |
| Solana | Base58 (32-44 chars) | Ed25519 public key → Base58 | `DYw8jCTfwHNRJhhmFcbXvVDTqWMEVFBX6ZKUmG5CNSKK` |

### BIP44 Derivation Paths

```
Bitcoin:  m/44'/0'/0'/0/0
Ethereum: m/44'/60'/0'/0/0
Tron:     m/44'/195'/0'/0/0
Solana:   m/44'/501'/0'/0/0
```

## 🧪 Testing

### Test Wallet Creation

```bash
# Run the application
./deewallet

# Test with known mnemonic (BIP39 standard)
# Mnemonic: "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"

# Expected addresses:
# Bitcoin:  bc1qcr8te4kr609gcawutmrza0j4xv80jy8z306fyu
# Ethereum: 0x9858EfFD232B4033E47d90003D41EC34EcaEda94
# Tron:     TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC
# Solana:   5mJdgNXF6qzkJfyTqHkTKWHgvXj7N3KJHMjgBV3TQYV7
```

### Manual Balance Testing

1. Send small amounts to generated addresses
2. Click "Refresh" to query balances
3. Verify displayed balances match blockchain explorers

## 🔧 Configuration

### RPC Endpoints

Default public endpoints are used. To customize, create `config/rpc.json`:

```json
{
  "mainnet": {
    "bitcoin": [
      {
        "name": "Blockstream",
        "url": "https://blockstream.info/api",
        "priority": 1,
        "rateLimit": 100
      }
    ],
    "ethereum": [
      {
        "name": "LlamaRPC",
        "url": "https://eth.llamarpc.com",
        "priority": 1,
        "rateLimit": 100
      }
    ],
    "tron": [
      {
        "name": "TronGrid",
        "url": "https://api.trongrid.io",
        "priority": 1,
        "rateLimit": 100
      }
    ],
    "solana": [
      {
        "name": "Mainnet",
        "url": "https://api.mainnet-beta.solana.com",
        "priority": 1,
        "rateLimit": 100
      }
    ]
  }
}
```

## 🚧 Roadmap

### Phase 1-3: ✅ Complete
- ✅ Project structure and build system
- ✅ BIP39/BIP32/BIP44 implementation
- ✅ Address generation for all chains
- ✅ Keyfile encryption and storage
- ✅ Qt GUI with wallet screens

### Phase 4-5: ✅ Complete
- ✅ RPC balance queries
- ✅ Real-time balance display
- ✅ WalletDetailScreen integration

### Phase 6: 🚧 In Progress
- ⏳ Transaction creation and signing
- ⏳ Transaction broadcasting
- ⏳ Fee estimation
- ⏳ Transaction history

### Phase 7: 📋 Planned
- 📋 ERC-20 token support
- 📋 TRC-20 token support
- 📋 SPL token support
- 📋 QR code generation
- 📋 Address book
- 📋 Multi-language support

### Phase 8: 📋 Future
- 📋 Hardware wallet integration
- 📋 Multi-signature support
- 📋 DeFi protocol integration
- 📋 NFT viewing
- 📋 Mobile versions (Qt for Mobile)

## 🤝 Contributing

Contributions are welcome! Please follow these guidelines:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Code Style
- Use C++17 modern features
- Follow Qt naming conventions
- Document all public APIs
- Include unit tests for new features
- Run `clang-format` before committing

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## ⚠️ Disclaimer

**DEE WALLET is provided "as is" without warranty of any kind.**

- This is experimental software - use at your own risk
- Always test with small amounts first
- The developers are not responsible for any loss of funds
- Cryptocurrency transactions are irreversible
- Keep your recovery phrase and keyfile password secure

## 🙏 Acknowledgments

- [Bitcoin BIPs](https://github.com/bitcoin/bips) - BIP39, BIP32, BIP44 specifications
- [Ethereum Yellow Paper](https://ethereum.github.io/yellowpaper/paper.pdf) - Keccak256 and address derivation
- [Qt Framework](https://www.qt.io/) - Cross-platform UI framework
- [OpenSSL](https://www.openssl.org/) - Cryptographic primitives
- [vcpkg](https://vcpkg.io/) - C++ package management

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/yourusername/deewallet-cpp/issues)
- **Discussions**: [GitHub Discussions](https://github.com/yourusername/deewallet-cpp/discussions)
- **Documentation**: [Wiki](https://github.com/yourusername/deewallet-cpp/wiki)

---

**Made with ❤️ for the cryptocurrency community**

*DEE WALLET - Your keys, your coins, your control*
