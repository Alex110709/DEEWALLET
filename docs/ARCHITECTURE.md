# DEEWALLET - Architecture Documentation

## Overview

DEEWALLET is a cross-platform (macOS, Windows) multi-chain cryptocurrency wallet supporting BTC, ETH, TRX, SOL and their respective tokens (ERC-20, TRC-20, SPL).

## Technology Stack

### Frontend
- **Electron**: Cross-platform desktop application framework
- **React**: UI library
- **TypeScript**: Type-safe development
- **Zustand**: State management
- **Vite**: Fast build tool

### Backend/Core
- **Node.js**: Runtime environment
- **bitcoinjs-lib**: Bitcoin operations
- **ethers.js**: Ethereum operations
- **@solana/web3.js**: Solana operations
- **TronWeb**: Tron operations
- **bip39/bip32**: HD wallet implementation

## Architecture Layers

```
┌─────────────────────────────────────┐
│         UI Layer (React)            │
│  - Wallet Management                │
│  - Transaction Interface            │
│  - Token Display                    │
└─────────────────────────────────────┘
              ↓
┌─────────────────────────────────────┐
│    Application Layer (Electron)     │
│  - IPC Communication                │
│  - Window Management                │
│  - File System Access               │
└─────────────────────────────────────┘
              ↓
┌─────────────────────────────────────┐
│       Core Layer (WalletCore)       │
│  - HD Wallet Generation             │
│  - Mnemonic Management              │
│  - Key Derivation (BIP44)           │
│  - Keyfile Encryption/Decryption    │
└─────────────────────────────────────┘
              ↓
┌─────────────────────────────────────┐
│      Chain Adapters Layer           │
│  - BitcoinChainAdapter              │
│  - EthereumChainAdapter             │
│  - SolanaChainAdapter               │
│  - TronChainAdapter                 │
└─────────────────────────────────────┘
              ↓
┌─────────────────────────────────────┐
│    Blockchain Networks (RPC)        │
│  - Bitcoin Network                  │
│  - Ethereum Network                 │
│  - Solana Network                   │
│  - Tron Network                     │
└─────────────────────────────────────┘
```

## Core Components

### 1. WalletCore (`src/core/wallet.core.ts`)

The central wallet management system handling:
- **Mnemonic Generation**: BIP39-compliant 12/24 word phrases
- **Seed Derivation**: Converting mnemonic to master seed
- **Key Derivation**: BIP44 hierarchical deterministic paths
- **Keyfile Management**: AES-256 encrypted keyfile format

**Security Features:**
- PBKDF2 key derivation with 100,000 iterations
- AES-256-CBC encryption
- Secure random IV and salt generation
- No private keys stored in memory longer than necessary

### 2. Chain Adapters

Each blockchain has a dedicated adapter implementing `IChainAdapter`:

#### BitcoinChainAdapter
- Native SegWit (P2WPKH) addresses
- UTXO management
- Transaction building with PSBT
- Fee estimation using network data

#### EthereumChainAdapter
- EIP-55 checksum addresses
- ERC-20 token support
- Gas estimation
- Smart contract interactions

#### SolanaChainAdapter
- Ed25519 keypairs
- SPL token support
- Transaction serialization
- Account management

#### TronChainAdapter
- Base58 encoded addresses
- TRC-20 token support
- Energy/bandwidth management
- Smart contract interactions

### 3. Keyfile Format

```json
{
  "version": 1,
  "id": "unique-wallet-id",
  "crypto": {
    "cipher": "aes-256-cbc",
    "ciphertext": "encrypted-mnemonic",
    "cipherparams": {
      "iv": "initialization-vector"
    },
    "kdf": "pbkdf2",
    "kdfparams": {
      "salt": "random-salt",
      "iterations": 100000
    }
  },
  "accounts": {
    "BTC": [...],
    "ETH": [...],
    "SOL": [...],
    "TRX": [...]
  },
  "createdAt": 1234567890,
  "updatedAt": 1234567890
}
```

## Derivation Paths (BIP44)

- **Bitcoin**: `m/44'/0'/0'/0/n`
- **Ethereum**: `m/44'/60'/0'/0/n`
- **Tron**: `m/44'/195'/0'/0/n`
- **Solana**: `m/44'/501'/0'/0/n`

Where `n` is the account index.

## Security Considerations

1. **Private Key Handling**
   - Never logged or persisted unencrypted
   - Cleared from memory after use
   - Only decrypted when needed for signing

2. **Keyfile Storage**
   - User-controlled location
   - Encrypted with user password
   - Strong KDF parameters (100,000 iterations)

3. **Network Communication**
   - All RPC calls over HTTPS
   - No sensitive data in query params
   - Proper error handling without leaking info

4. **Input Validation**
   - Address validation before transactions
   - Amount validation (positive, within limits)
   - Transaction parameter sanitization

## Token Support

### ERC-20 (Ethereum)
- Standard ERC-20 interface
- Automatic token discovery
- Custom token addition
- Token balance tracking

### TRC-20 (Tron)
- Tron's token standard
- Energy/bandwidth consideration
- Token approval mechanisms

### SPL (Solana)
- Associated token accounts
- Automatic account creation
- Metaplex metadata support

## Development Workflow

1. **Build**: `npm run build`
2. **Development**: `npm run dev`
3. **Testing**: `npm test`
4. **Package**: `npm run package`

## Future Enhancements

- Hardware wallet support (Ledger, Trezor)
- Multi-signature wallets
- DApp browser integration
- NFT support
- Staking interfaces
- DEX integration
- Price tracking
- Transaction history export
