# DEEWALLET - Multi-Chain Cryptocurrency Wallet

<div align="center">

![DEEWALLET](https://via.placeholder.com/200x200?text=DEEWALLET)

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

## 🚀 Supported Blockchains

| Blockchain | Native Token | Token Standard | Status |
|-----------|-------------|---------------|--------|
| Bitcoin | BTC | - | ✅ Ready |
| Ethereum | ETH | ERC-20 | ✅ Ready |
| Tron | TRX | TRC-20 | ✅ Ready |
| Solana | SOL | SPL | ✅ Ready |

## 📦 Installation

### Prerequisites

- Node.js 18+
- npm or yarn

### Build from Source

```bash
# Clone repository
git clone https://github.com/yourusername/deewallet.git
cd deewallet

# Install dependencies
npm install

# Run in development mode
npm run dev

# Build for production
npm run build

# Package for distribution
npm run package
```

## 🔧 Usage

### Creating a New Wallet

```typescript
import { WalletCore } from './src/core/wallet.core';
import { NetworkType } from './src/types/wallet.types';

const config = {
  network: NetworkType.MAINNET,
  rpcEndpoints: {
    BTC: 'https://blockstream.info/api',
    ETH: 'https://eth-mainnet.g.alchemy.com/v2/YOUR_KEY',
    SOL: 'https://api.mainnet-beta.solana.com',
    TRX: 'https://api.trongrid.io'
  },
  derivationPaths: {}
};

const wallet = new WalletCore(config);

// Generate new wallet
const mnemonic = await wallet.generateWallet();
console.log('Save this mnemonic:', mnemonic);

// Export to encrypted keyfile
const keyfile = await wallet.exportToKeyfile('your-secure-password');
```

### Restoring from Keyfile

```typescript
// Import from keyfile
await wallet.importFromKeyfile(keyfileData, 'your-secure-password');

// Derive accounts
const btcAccount = wallet.deriveAccount(ChainType.BTC, 0);
const ethAccount = wallet.deriveAccount(ChainType.ETH, 0);
```

### Sending Transactions

```typescript
import { EthereumChainAdapter } from './src/chains/ethereum.chain';

const ethAdapter = new EthereumChainAdapter('https://eth-mainnet.g.alchemy.com/v2/YOUR_KEY');

const tx = await ethAdapter.sendTransaction({
  to: '0x742d35Cc6634C0532925a3b844Bc9e7595f0bEb',
  amount: '0.1',
  chainType: ChainType.ETH
}, privateKey);

console.log('Transaction hash:', tx.hash);
```

## 🔐 Security

### Keyfile Security

- **Encryption**: AES-256-CBC
- **Key Derivation**: PBKDF2 with 100,000 iterations
- **Random IV**: Unique initialization vector per keyfile
- **Salt**: Random salt for each encryption

### Best Practices

1. **Strong Password**: Use a strong, unique password for keyfile encryption
2. **Backup Mnemonic**: Write down and securely store your 12/24 word phrase
3. **Secure Storage**: Store keyfile in a secure location
4. **Regular Backups**: Keep multiple encrypted backups
5. **Verify Addresses**: Always verify recipient addresses before sending

## 📁 Project Structure

```
DEEWALLET/
├── src/
│   ├── core/              # Core wallet logic
│   │   └── wallet.core.ts
│   ├── chains/            # Blockchain adapters
│   │   ├── base.chain.ts
│   │   ├── bitcoin.chain.ts
│   │   ├── ethereum.chain.ts
│   │   ├── solana.chain.ts
│   │   └── tron.chain.ts
│   ├── types/             # TypeScript type definitions
│   │   └── wallet.types.ts
│   ├── ui/                # React UI components
│   ├── utils/             # Utility functions
│   └── main.ts            # Electron main process
├── tests/                 # Test suites
├── docs/                  # Documentation
├── config/                # Configuration files
└── package.json
```

## 🧪 Testing

```bash
# Run all tests
npm test

# Run tests in watch mode
npm run test:watch

# Run specific test suite
npm test -- wallet.core.test.ts
```

## 🤝 Contributing

Contributions are welcome! Please read our [Contributing Guidelines](CONTRIBUTING.md) first.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## ⚠️ Disclaimer

This wallet is provided "as is" without warranty of any kind. Use at your own risk. Always verify transactions before signing and sending.

## 🌐 Resources

- [Bitcoin Documentation](https://developer.bitcoin.org/)
- [Ethereum Documentation](https://ethereum.org/developers)
- [Solana Documentation](https://docs.solana.com/)
- [Tron Documentation](https://developers.tron.network/)
- [BIP39 Specification](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki)
- [BIP44 Specification](https://github.com/bitcoin/bips/blob/master/bip-0044.mediawiki)

## 📧 Support

For support, please open an issue on GitHub or contact support@deewallet.com

---

<div align="center">

**Built with ❤️ by the DEEWALLET Team**

</div>
