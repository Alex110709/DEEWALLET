# DEEWALLET - Usage Guide

## Table of Contents

1. [Getting Started](#getting-started)
2. [Creating a Wallet](#creating-a-wallet)
3. [Importing a Wallet](#importing-a-wallet)
4. [Managing Accounts](#managing-accounts)
5. [Sending Transactions](#sending-transactions)
6. [Token Management](#token-management)
7. [Security Best Practices](#security-best-practices)

## Getting Started

### Prerequisites

Before using DEEWALLET, ensure you have:
- Downloaded and installed the latest version
- A secure location to store your keyfile
- A strong password (12+ characters with mix of letters, numbers, symbols)

### First Launch

On first launch, you'll see two options:
1. **Create New Wallet** - Generate a brand new wallet
2. **Import Wallet** - Import from keyfile or mnemonic

## Creating a Wallet

### Step 1: Generate New Wallet

1. Click "Create New Wallet"
2. The system will generate a 24-word mnemonic phrase
3. **IMPORTANT**: Write down this phrase and store it securely

```
Example mnemonic:
apple banana cherry dog elephant fish grape house
igloo jungle kite lion monkey nest orange panda
queen rabbit snake tiger umbrella violin whale xray
```

### Step 2: Verify Mnemonic

You'll be asked to verify by selecting words in order. This ensures you've recorded it correctly.

### Step 3: Set Password

Create a strong password for your keyfile:
- Minimum 12 characters
- Mix of uppercase and lowercase letters
- Include numbers and symbols
- Don't use common words or patterns

### Step 4: Save Keyfile

Choose a secure location to save your encrypted keyfile:
- External encrypted drive (recommended)
- Password-protected USB drive
- Secure cloud storage with 2FA

## Importing a Wallet

### From Keyfile

1. Click "Import Wallet"
2. Select "From Keyfile"
3. Browse and select your `.keyfile`
4. Enter your password
5. Wait for decryption and wallet load

### From Mnemonic

1. Click "Import Wallet"
2. Select "From Mnemonic"
3. Enter your 12 or 24-word phrase
4. Set a new password
5. Save the new keyfile

## Managing Accounts

### Viewing Accounts

The main dashboard shows all your accounts:

```
┌─────────────────────────────────────┐
│ Bitcoin (BTC)                       │
│ bc1q... (Segwit)                   │
│ Balance: 0.12345678 BTC            │
└─────────────────────────────────────┘

┌─────────────────────────────────────┐
│ Ethereum (ETH)                      │
│ 0x742d... (EIP-55)                 │
│ Balance: 1.5 ETH                   │
│ Tokens: 3                          │
└─────────────────────────────────────┘
```

### Adding New Accounts

Each blockchain supports multiple accounts (HD wallet):

1. Select the blockchain
2. Click "Add Account"
3. New account will be derived with next index
4. Each account has its own address

### Account Structure

```
Wallet (Mnemonic)
├── Bitcoin
│   ├── Account 0 (m/44'/0'/0'/0/0)
│   ├── Account 1 (m/44'/0'/0'/0/1)
│   └── Account 2 (m/44'/0'/0'/0/2)
├── Ethereum
│   ├── Account 0 (m/44'/60'/0'/0/0)
│   └── Account 1 (m/44'/60'/0'/0/1)
├── Solana
│   └── Account 0 (m/44'/501'/0'/0/0)
└── Tron
    └── Account 0 (m/44'/195'/0'/0/0)
```

## Sending Transactions

### Basic Transaction Flow

1. **Select Account**: Choose which account to send from
2. **Enter Details**:
   - Recipient address
   - Amount to send
   - (Optional) Memo/note
3. **Review Fee**: Check and adjust gas/fee if needed
4. **Confirm**: Review all details carefully
5. **Sign**: Enter password to unlock wallet
6. **Broadcast**: Transaction sent to network
7. **Track**: Monitor confirmation status

### Bitcoin (BTC) Transaction

```typescript
To: bc1qxy2kgdygjrsqtzq2n0yrf2493p83kkfjhx0wlh
Amount: 0.01 BTC
Fee: 0.00001 BTC (10 sat/vB)
Total: 0.01001 BTC

Estimated confirmation: ~10 minutes
```

### Ethereum (ETH) Transaction

```typescript
To: 0x742d35Cc6634C0532925a3b844Bc9e7595f0bEb
Amount: 0.5 ETH
Gas Limit: 21000
Gas Price: 20 Gwei
Total Fee: 0.00042 ETH
Total: 0.50042 ETH

Estimated confirmation: ~15 seconds
```

### Token Transactions

For ERC-20/TRC-20/SPL tokens:

```typescript
Token: USDT (ERC-20)
To: 0x742d35Cc6634C0532925a3b844Bc9e7595f0bEb
Amount: 100 USDT
Gas Limit: 65000
Gas Price: 20 Gwei
Total Fee: 0.0013 ETH

Note: ETH needed for gas fee
```

## Token Management

### Adding Custom Tokens

#### ERC-20 Tokens (Ethereum)

1. Click "Add Token"
2. Enter contract address: `0xdAC17F958D2ee523a2206206994597C13D831ec7`
3. Symbol and decimals auto-populate
4. Click "Add"

#### SPL Tokens (Solana)

1. Click "Add Token"
2. Enter token mint address
3. Symbol auto-populates from metadata
4. Click "Add"

### Viewing Token Balances

Tokens appear under their respective blockchain:

```
Ethereum Account
├── ETH: 1.5
├── USDT: 1000
├── USDC: 500
└── DAI: 250

Solana Account
├── SOL: 10.5
├── USDC: 100
└── RAY: 50
```

### Removing Tokens

1. Right-click on token
2. Select "Hide Token"
3. Confirm

(Note: This only hides from view, doesn't affect blockchain)

## Security Best Practices

### Mnemonic Safety

- **Write it down** - Use pen and paper, not digital
- **Store securely** - Fireproof safe, bank vault
- **Never share** - Not even with support staff
- **Test recovery** - Practice restoring on testnet
- **Multiple copies** - Store in different locations

### Keyfile Security

- **Strong password** - 12+ characters, complex
- **Backup copies** - Keep 2-3 encrypted backups
- **Secure storage** - Encrypted external drives
- **Regular updates** - Re-encrypt periodically
- **Access control** - Limit who can access

### Transaction Verification

Always verify before confirming:

✅ **Check recipient address** - Verify multiple times
✅ **Verify amount** - Ensure correct decimal places
✅ **Review fee** - Reasonable for network conditions
✅ **Check network** - Mainnet vs testnet
✅ **Test small amount** - For new recipients

### Common Scams to Avoid

❌ **Address poisoning** - Always verify full address
❌ **Phishing websites** - Only use official app
❌ **Support scams** - No one will ask for your mnemonic
❌ **Too-good offers** - No free money schemes
❌ **Clipboard hijacking** - Verify pasted addresses

### Best Practices Summary

1. **Never share mnemonic or private keys**
2. **Always verify addresses before sending**
3. **Keep software updated**
4. **Use strong, unique passwords**
5. **Enable all available security features**
6. **Keep backups in multiple secure locations**
7. **Test recovery process**
8. **Stay informed about security threats**

## Troubleshooting

### Wallet Won't Open

- Check password is correct
- Verify keyfile not corrupted
- Try re-importing from mnemonic

### Transaction Stuck

- Check network status
- Verify sufficient balance for fees
- Try increasing gas price (ETH/TRX)

### Wrong Balance

- Wait for full sync
- Check correct network (mainnet/testnet)
- Verify account index

### Lost Access

With mnemonic:
1. Create new wallet
2. Import using mnemonic
3. Derive same accounts

Without mnemonic:
- Unfortunately, funds are unrecoverable
- This is why backup is crucial

## Support

- **Documentation**: Check docs/ folder
- **GitHub Issues**: Report bugs and features
- **Community**: Join our Discord/Telegram
- **Email**: support@deewallet.com

---

**Remember**: You are your own bank. Security is your responsibility!
