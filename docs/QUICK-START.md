# DEEWALLET - Quick Start Guide

## 🚀 Get Started in 5 Minutes

### Step 1: Installation

```bash
# Clone the repository
git clone https://github.com/yourusername/deewallet.git
cd deewallet

# Install dependencies
npm install
```

### Step 2: Run the Application

```bash
# Development mode
npm run dev
```

The application will launch automatically!

## 📋 First Time Setup

### Create Your First Wallet

1. **Click "Create New Wallet"**
   - App will generate a 24-word mnemonic phrase

2. **Backup Your Mnemonic**
   ```
   ⚠️ CRITICAL: Write these words on paper

   Example:
   1. apple    7. grape    13. monkey   19. tiger
   2. banana   8. house    14. nest     20. umbrella
   3. cherry   9. igloo    15. orange   21. violin
   4. dog     10. jungle   16. panda    22. whale
   5. elephant 11. kite    17. queen    23. xray
   6. fish    12. lion    18. rabbit   24. zebra

   Store this in a SAFE place - it's your only backup!
   ```

3. **Set a Strong Password**
   ```
   Requirements:
   ✓ At least 12 characters
   ✓ Mix of uppercase and lowercase
   ✓ Include numbers
   ✓ Include special characters

   Example: MyW@llet2024Secure!
   ```

4. **Save Your Keyfile**
   - Choose a secure location
   - Recommended: External encrypted drive
   - Backup this file too!

### View Your Accounts

The dashboard shows accounts for all supported chains:

```
┌─────────────────────────────┐
│ Bitcoin (BTC)               │
│ bc1q...                     │
│ Balance: 0.00000000 BTC     │
└─────────────────────────────┘

┌─────────────────────────────┐
│ Ethereum (ETH)              │
│ 0x742d...                   │
│ Balance: 0.0 ETH            │
└─────────────────────────────┘
```

## 💰 Receiving Cryptocurrency

### Get Your Address

1. Select blockchain (BTC/ETH/SOL/TRX)
2. Click on account to view address
3. Click 📋 to copy address
4. Share this address to receive funds

**Example Addresses:**
- **Bitcoin**: `bc1qxy2kgdygjrsqtzq2n0yrf2493p83kkfjhx0wlh`
- **Ethereum**: `0x742d35Cc6634C0532925a3b844Bc9e7595f0bEb`
- **Solana**: `7xKXtg2CW87d97TXJSDpbD5jBkheTqA83TZRuJosgAsU`
- **Tron**: `TXYZNmJHK5q2bMzVLLdPxwYBg5NqYxpGXK`

### Important Notes

⚠️ **Each blockchain has different address format**
- Don't send BTC to ETH address
- Don't send ETH to BTC address
- Always verify the blockchain before sending

✅ **Test with Small Amount First**
- Send $1-5 first
- Verify it arrives
- Then send larger amounts

## 📤 Sending Cryptocurrency

### Basic Send

1. **Select Account**
   - Choose which chain and account

2. **Click "Send"**
   - Enter recipient address
   - Enter amount
   - Review transaction fee

3. **Confirm**
   - Double-check address
   - Verify amount
   - Click "Send Transaction"

4. **Enter Password**
   - Unlock wallet with password
   - Transaction will be signed and broadcast

### Example Transaction

```typescript
Sending ETH:
━━━━━━━━━━━━━━━━━━━━━━━━━━━
To: 0x742d35Cc6634C0532925a3b844Bc9e7595f0bEb
Amount: 0.5 ETH
Fee: ~0.001 ETH (20 Gwei)
Total: 0.501 ETH
━━━━━━━━━━━━━━━━━━━━━━━━━━━

⚠️ This transaction is IRREVERSIBLE
Double-check everything before confirming!
```

## 🪙 Managing Tokens

### View Tokens

Tokens appear automatically under their blockchain:

```
Ethereum
├─ ETH: 1.5
├─ USDT: 1000
├─ USDC: 500
└─ DAI: 250
```

### Add Custom Token

1. Click "Add Token"
2. Select blockchain
3. Enter contract address:
   - **ETH (USDT)**: `0xdAC17F958D2ee523a2206206994597C13D831ec7`
   - **ETH (USDC)**: `0xA0b86991c6218b36c1d19D4a2e9Eb0cE3606eB48`
4. Click "Add"

## 🔒 Security Checklist

Before using with real funds:

- [ ] Mnemonic phrase written down on paper
- [ ] Mnemonic phrase stored in secure location
- [ ] Keyfile backed up to external drive
- [ ] Strong password set (12+ characters)
- [ ] Password stored in password manager
- [ ] Tested wallet restore on testnet
- [ ] Verified receiving address format
- [ ] Tested small transaction first

## ⚙️ Configuration

### Using Your Own RPC

For better performance and reliability:

1. **Get API Keys** (Free):
   - Ethereum: [Alchemy](https://www.alchemy.com/) or [Infura](https://infura.io/)
   - Solana: [Helius](https://www.helius.dev/)
   - Bitcoin: [GetBlock](https://getblock.io/)

2. **Edit Config**:
   ```bash
   # Edit config/rpc-endpoints.json
   {
     "mainnet": {
       "ETH": [{
         "name": "Alchemy",
         "url": "https://eth-mainnet.g.alchemy.com/v2/YOUR_KEY",
         "priority": 1,
         "rateLimit": 1000
       }]
     }
   }
   ```

See [RPC Configuration Guide](RPC-CONFIGURATION.md) for details.

## 🐛 Common Issues

### Issue: "No balance showing"

**Solution:**
1. Wait 1-2 minutes for sync
2. Check you're on correct network (mainnet/testnet)
3. Verify RPC endpoint is working

### Issue: "Transaction failed"

**Solution:**
1. Check sufficient balance for fee
2. Verify recipient address is correct
3. Try increasing gas price
4. Check RPC endpoint status

### Issue: "Can't open keyfile"

**Solution:**
1. Verify password is correct
2. Check keyfile not corrupted
3. Try restoring from mnemonic

## 📱 Next Steps

1. **Read Full Documentation**
   - [Usage Guide](USAGE.md) - Detailed features
   - [Security Best Practices](USAGE.md#security-best-practices)

2. **Test on Testnet**
   - Switch to testnet in settings
   - Practice transactions with test coins
   - Get familiar with interface

3. **Join Community**
   - GitHub Discussions
   - Discord/Telegram
   - Follow updates

## 🆘 Getting Help

**Documentation:**
- [Usage Guide](USAGE.md)
- [API Documentation](API.md)
- [RPC Configuration](RPC-CONFIGURATION.md)

**Support:**
- GitHub Issues: Report bugs
- Discussions: Ask questions
- Email: support@deewallet.com

## ⚠️ Important Reminders

1. **Never share your mnemonic phrase**
   - Not even with support
   - Keep it offline
   - Multiple backups

2. **Always verify addresses**
   - Check multiple times
   - Test with small amounts
   - Save frequently used addresses

3. **Keep software updated**
   - Check for updates regularly
   - Read changelogs
   - Backup before updating

4. **You are your own bank**
   - No password recovery
   - No transaction reversal
   - Complete responsibility

---

**Ready to start? Launch DEEWALLET and create your wallet!** 🚀
