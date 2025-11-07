# DEEWALLET - Multi-Chain Cryptocurrency Wallet

<div align="center">

**Secure, Portable, Multi-Chain Wallet for macOS and Windows**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![TypeScript](https://img.shields.io/badge/TypeScript-5.5-blue)](https://www.typescriptlang.org/)
[![Electron](https://img.shields.io/badge/Electron-31-brightgreen)](https://www.electronjs.org/)

</div>

## 🌟 Features

- ✅ **Multi-Chain Support**: BTC, ETH, TRX, SOL
- ✅ **Token Support**: ERC-20, TRC-20, SPL tokens
- ✅ **Cross-Platform**: macOS and Windows
- ✅ **Portable**: Keyfile-based wallet (no cloud dependency)
- ✅ **Secure**: AES-256 encryption with PBKDF2
- ✅ **HD Wallet**: BIP39/BIP44 standard implementation
- ✅ **Offline Signing**: Sign transactions offline
- ✅ **No Custody**: Full control of your private keys
- ✅ **Multiple RPC Endpoints**: 20+ public RPCs with auto-failover
- ✅ **Smart RPC Management**: Automatic health monitoring and switching

## 🚀 Quick Start

```bash
# Install dependencies
npm install

# Run in development mode
npm run dev

# Build for production
npm run build

# Package for distribution
npm run package
```

## 📖 Documentation

- [Quick Start](docs/QUICK-START.md) - Get started in 5 minutes
- [Architecture](docs/ARCHITECTURE.md) - System architecture and design
- [Usage Guide](docs/USAGE.md) - How to use DEEWALLET
- [API Documentation](docs/API.md) - Developer API reference
- [RPC Configuration](docs/RPC-CONFIGURATION.md) - RPC setup and configuration
- [Installation](docs/INSTALLATION.md) - Installation guide

## 🔐 Security

DEEWALLET uses industry-standard security practices:
- **BIP39** mnemonic generation
- **BIP44** hierarchical deterministic wallets
- **AES-256-CBC** encryption
- **PBKDF2** key derivation (100,000 iterations)

## 🛠️ Tech Stack

- **Electron** - Cross-platform desktop framework
- **React** - UI library
- **TypeScript** - Type-safe development
- **ethers.js** - Ethereum operations
- **@solana/web3.js** - Solana operations
- **bitcoinjs-lib** - Bitcoin operations
- **TronWeb** - Tron operations

## 📦 Project Structure

```
DEEWALLET/
├── src/
│   ├── core/          # Wallet core logic
│   ├── chains/        # Blockchain adapters
│   ├── types/         # TypeScript definitions
│   ├── ui/            # React UI components
│   └── main.ts        # Electron main process
├── tests/             # Test suites
├── docs/              # Documentation
└── scripts/           # Build scripts
```

## 🧪 Testing

```bash
# Run all tests
npm test

# Run with coverage
npm test -- --coverage
```

## 📋 Requirements

- Node.js 18+
- npm or yarn

## ⚠️ Disclaimer

This wallet is provided "as is" without warranty. Use at your own risk. Always verify transactions and keep your mnemonic phrase secure.

## 📄 License

MIT License - see [LICENSE](LICENSE) file

## 🤝 Contributing

Contributions welcome! Please read our contributing guidelines first.

---

**Built with ❤️ by the DEEWALLET Team**
