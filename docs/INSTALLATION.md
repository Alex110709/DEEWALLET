# DEEWALLET - Installation Guide

## System Requirements

### macOS
- macOS 10.13 (High Sierra) or later
- 4GB RAM minimum (8GB recommended)
- 500MB free disk space

### Windows
- Windows 10 or later (64-bit)
- 4GB RAM minimum (8GB recommended)
- 500MB free disk space

## Development Setup

### Prerequisites

1. **Node.js 18+**
   ```bash
   # macOS (using Homebrew)
   brew install node@18

   # Windows (download from nodejs.org)
   https://nodejs.org/
   ```

2. **Git**
   ```bash
   # macOS
   brew install git

   # Windows
   https://git-scm.com/download/win
   ```

### Clone Repository

```bash
git clone https://github.com/yourusername/deewallet.git
cd deewallet
```

### Install Dependencies

```bash
npm install
```

This will install:
- Core dependencies (ethers, @solana/web3.js, bitcoinjs-lib, TronWeb)
- Cryptography libraries (bip39, bip32, crypto-js)
- Electron framework
- React and UI dependencies
- Development tools (TypeScript, Jest, ESLint)

### Build the Application

```bash
# Build all components
npm run build

# Or build individually
npm run build:main      # Main process
npm run build:preload   # Preload script
npm run build:renderer  # React UI
```

### Run in Development Mode

```bash
npm run dev
```

This will:
1. Start the Vite dev server (port 5173)
2. Build the Electron main process
3. Launch the Electron application

## Production Build

### Build for macOS

```bash
# Package as DMG
npm run package

# Output: release/DEEWALLET-1.0.0.dmg
```

**Distribution:**
- DMG file for easy installation
- ZIP file for portable use
- Notarized and signed (requires Apple Developer Account)

### Build for Windows

```bash
# Package as installer
npm run package

# Output: release/DEEWALLET Setup 1.0.0.exe
```

**Distribution:**
- NSIS installer
- Portable executable
- Code signed (requires Windows code signing certificate)

### Build for Both Platforms

```bash
# On macOS (can build both)
npm run package -- -mw

# On Windows (can only build Windows)
npm run package -- -w
```

## Configuration

### RPC Endpoints

Edit default RPC endpoints in `src/main.ts`:

```typescript
const defaultConfig: WalletConfig = {
  network: NetworkType.MAINNET,
  rpcEndpoints: {
    BTC: 'https://blockstream.info/api',
    ETH: 'https://eth-mainnet.g.alchemy.com/v2/YOUR_API_KEY',
    SOL: 'https://api.mainnet-beta.solana.com',
    TRX: 'https://api.trongrid.io'
  },
  derivationPaths: {}
};
```

**Recommended RPC Providers:**

- **Ethereum**: Alchemy, Infura, QuickNode
- **Solana**: Helius, QuickNode, GenesysGo
- **Bitcoin**: Blockstream, Blockchain.com
- **Tron**: TronGrid (official)

### Build Configuration

Edit `package.json` for build options:

```json
{
  "build": {
    "appId": "com.deewallet.app",
    "productName": "DEEWALLET",
    "mac": {
      "target": ["dmg", "zip"],
      "category": "public.app-category.finance",
      "hardenedRuntime": true,
      "gatekeeperAssess": false,
      "entitlements": "build/entitlements.mac.plist"
    },
    "win": {
      "target": ["nsis", "portable"],
      "icon": "assets/icon.ico",
      "certificateFile": "path/to/cert.pfx",
      "certificatePassword": "password"
    }
  }
}
```

## Troubleshooting

### Common Issues

#### 1. Node-gyp Build Errors

Some native modules may require build tools:

**macOS:**
```bash
xcode-select --install
```

**Windows:**
```bash
npm install --global windows-build-tools
```

#### 2. Electron Binary Not Found

Clear cache and reinstall:
```bash
rm -rf node_modules package-lock.json
npm cache clean --force
npm install
```

#### 3. TypeScript Errors

Ensure TypeScript version matches:
```bash
npm install -D typescript@5.5.0
```

#### 4. Vite Build Fails

Clear Vite cache:
```bash
rm -rf node_modules/.vite
npm run build:renderer
```

#### 5. Permission Denied on macOS

Make scripts executable:
```bash
chmod +x scripts/build.sh
```

### Platform-Specific Issues

#### macOS Code Signing

For distribution outside App Store:
```bash
# Sign the app
codesign --deep --force --verify --verbose --sign "Developer ID Application: Your Name" DEEWALLET.app

# Notarize (requires Apple Developer account)
xcrun notarytool submit DEEWALLET.dmg --apple-id your@email.com --password app-specific-password --team-id TEAM_ID
```

#### Windows Code Signing

Requires a valid code signing certificate:
```bash
signtool sign /f cert.pfx /p password /tr http://timestamp.digicert.com /td sha256 /fd sha256 DEEWALLET.exe
```

## Testing

### Run Tests

```bash
# All tests
npm test

# Watch mode
npm test:watch

# With coverage
npm test -- --coverage
```

### Manual Testing Checklist

- [ ] Create new wallet
- [ ] Backup mnemonic phrase
- [ ] Export keyfile
- [ ] Import keyfile
- [ ] Derive accounts for all chains
- [ ] Check balances
- [ ] Send test transactions (on testnet)
- [ ] View transaction history
- [ ] Add custom tokens

## Security Considerations

### Development

1. **Never commit sensitive data**
   - Add `.env` files to `.gitignore`
   - Don't commit real private keys
   - Use testnet for development

2. **Secure build environment**
   - Build on clean, trusted machines
   - Verify all dependencies
   - Use npm audit regularly

3. **Code signing**
   - Always sign production builds
   - Store certificates securely
   - Use hardware security modules when possible

### Distribution

1. **Provide checksums**
   ```bash
   shasum -a 256 DEEWALLET-1.0.0.dmg > checksums.txt
   ```

2. **Use HTTPS only**
   - Host downloads on HTTPS
   - Provide GPG signatures

3. **Update mechanism**
   - Implement secure auto-updates
   - Verify update signatures
   - Use delta updates when possible

## Continuous Integration

### GitHub Actions Example

```yaml
name: Build

on:
  push:
    branches: [ main ]
  pull_request:
    branches: [ main ]

jobs:
  build:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [macos-latest, windows-latest]

    steps:
    - uses: actions/checkout@v3
    - uses: actions/setup-node@v3
      with:
        node-version: 18

    - run: npm install
    - run: npm test
    - run: npm run build
    - run: npm run package

    - uses: actions/upload-artifact@v3
      with:
        name: release-${{ matrix.os }}
        path: release/
```

## Next Steps

After installation:
1. Read the [Usage Guide](USAGE.md)
2. Review [Security Best Practices](USAGE.md#security-best-practices)
3. Check [API Documentation](API.md) for development
4. Join our community for support

## Support

- **Issues**: https://github.com/yourusername/deewallet/issues
- **Discussions**: https://github.com/yourusername/deewallet/discussions
- **Email**: support@deewallet.com

---

**Remember**: This is cryptocurrency wallet software. Always test thoroughly on testnets before handling real funds!
