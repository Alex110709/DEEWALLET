# DEE WALLET - User Guide

## 📖 Complete User Manual

### Table of Contents
1. [Getting Started](#getting-started)
2. [Creating Your First Wallet](#creating-your-first-wallet)
3. [Checking Balances](#checking-balances)
4. [Sending Transactions](#sending-transactions)
5. [Receiving Payments](#receiving-payments)
6. [Managing Contacts](#managing-contacts)
7. [Token Support](#token-support)
8. [Security Best Practices](#security-best-practices)
9. [Troubleshooting](#troubleshooting)
10. [FAQ](#faq)

---

## Getting Started

### Installation

**macOS:**
1. Download `DEE-WALLET.dmg`
2. Open the DMG file
3. Drag DEE WALLET to Applications
4. Launch from Applications folder

**Windows:**
1. Download `DEE-WALLET-Setup.msi`
2. Run the installer
3. Follow installation wizard
4. Launch from Start Menu

### First Launch

When you first open DEE WALLET, you'll see the Welcome Screen with:
- List of existing keyfiles (if any)
- "Create Wallet" button for new users
- "Import Wallet from File" button to restore

---

## Creating Your First Wallet

### Step-by-Step Guide

1. **Click "Create Wallet"**
   - The Create Wallet Dialog opens

2. **Generate Mnemonic**
   - Click "Generate Mnemonic"
   - A 12-word recovery phrase appears
   - ⚠️ **CRITICAL**: Write these words down!
   - Store them in a safe place (not digitally)

3. **Enter Password**
   - Type a strong password (8+ characters)
   - Confirm the password
   - This encrypts your keyfile

4. **Create Wallet**
   - Click "Create Wallet"
   - Addresses are generated for all 4 chains:
     * Bitcoin (bc1q...)
     * Ethereum (0x...)
     * Tron (T...)
     * Solana (Base58)

5. **Save Your Information**
   - Keyfile saved automatically as `wallet_TIMESTAMP.keyfile`
   - Store recovery phrase offline
   - Remember your password!

### ⚠️ Important Warnings

**NEVER:**
- Share your 12-word recovery phrase
- Store recovery phrase digitally (no screenshots!)
- Forget your password (cannot be recovered)
- Trust anyone asking for your phrase

**ALWAYS:**
- Write recovery phrase on paper
- Store in multiple secure locations
- Use a unique, strong password
- Test wallet with small amounts first

---

## Checking Balances

### View Your Balance

1. **Wallet Detail Screen**
   - After creating/importing, you see the main wallet screen
   - Total balance shown at top (in USD)
   - Individual chain tabs below

2. **Chain Tabs**
   - Click Bitcoin/Ethereum/Tron/Solana tabs
   - Each tab shows:
     * Your address
     * Current balance
     * Copy button

3. **Refresh Balances**
   - Click "Refresh" button
   - Queries all blockchains
   - Updates in 5-10 seconds
   - Shows updated balances

### Understanding Balances

**Balance Display:**
```
Bitcoin:  0.12345678 BTC
Ethereum: 1.5 ETH
Tron:     100.000000 TRX
Solana:   5.123456789 SOL

Total: $12,345.67 USD
```

**Notes:**
- Balances query live blockchain data
- USD rates are approximate
- Refresh to get latest values
- Token balances shown separately

---

## Sending Transactions

### How to Send

1. **Select Chain**
   - Click the chain tab you want to send from
   - e.g., "Bitcoin" tab for BTC

2. **Click "Send"**
   - Send Transaction Dialog opens

3. **Enter Recipient**
   - Type or paste recipient address
   - Or click "📖 Address Book" to select saved contact
   - Address validates automatically (green = valid, red = invalid)

4. **Enter Amount**
   - Type amount to send
   - Or click "MAX" to send all
   - Balance shown at top

5. **Select Fee**
   - Choose fee level:
     * **Slow** - Low fee, slower confirmation
     * **Normal** - Recommended fee
     * **Fast** - High fee, faster confirmation
   - Fee displayed below dropdown
   - Total (amount + fee) calculated

6. **Review Transaction**
   - Check all details carefully
   - Recipient address
   - Amount
   - Fee
   - Total

7. **Send Transaction**
   - Click "Send Transaction"
   - Confirmation dialog appears
   - Click "Yes" to confirm
   - Transaction broadcasts to network

8. **Wait for Confirmation**
   - Transaction hash displayed
   - Confirmation times vary:
     * Bitcoin: ~10-60 minutes
     * Ethereum: ~1-5 minutes
     * Tron: ~3 seconds
     * Solana: ~1 second

### Fee Levels Explained

| Fee Level | Bitcoin (sat/vB) | Ethereum (Gwei) | Use When |
|-----------|------------------|-----------------|----------|
| Slow | 1 | 10 | Not urgent, save money |
| Normal | 5 | 20 | Standard transactions |
| Fast | 10 | 40 | Urgent, important |

---

## Receiving Payments

### How to Receive

1. **Select Chain**
   - Click the chain tab for the cryptocurrency you want to receive

2. **Click "Receive"**
   - QR Code Dialog opens

3. **Share Your Address**
   - **Option 1**: Show QR code
     * Sender scans with their wallet
   - **Option 2**: Copy address
     * Click "Copy Address"
     * Send via email/message
   - **Option 3**: Save QR code
     * Click "Save QR Code"
     * Share image file

4. **Optional: Specify Amount**
   - Enter amount in the "Amount" field
   - QR code includes payment request

5. **Wait for Payment**
   - After sender broadcasts transaction
   - Click "Refresh" on main screen
   - Balance updates when confirmed

### QR Code Features

```
┌─────────────────────┐
│  ██ ▄▄ ██ ██ ▄▄ ██ │  ← Your Address QR Code
│  ██ ▀▀ ▀▀ ▀▀ ██ ██ │
│  ▀▀ ██ ▄▄ ██ ▀▀ ▀▀ │
│  ██ ▀▀ ██ ▀▀ ██ ▄▄ │
└─────────────────────┘

Your Address:
bc1qxy2kgdygjrsqtzq2n0yrf2493p83kkfjhx0wlh

[Copy Address] [Save QR Code] [Close]
```

---

## Managing Contacts

### Address Book

The Address Book helps you save frequently-used addresses.

### Adding a Contact

1. **Open Address Book**
   - Click "📖 Address Book" button (in Send dialog)
   - Or from main menu

2. **Add New Contact**
   - Click "➕ Add"

3. **Enter Details**
   - **Label**: Friendly name (e.g., "Mom's ETH Wallet")
   - **Chain**: Select blockchain (Bitcoin/Ethereum/Tron/Solana)
   - **Address**: Paste or type address
   - **Note**: Optional memo (e.g., "Birthday gifts")

4. **Save**
   - Click OK
   - Contact saved to address book

### Using Contacts

**In Send Dialog:**
1. Click "📖 Address Book"
2. Find your contact
3. Click "Select"
4. Address auto-filled!

**Search and Filter:**
- Type in search box to find contacts
- Use chain filter dropdown
- Search by label or address

### Editing Contacts

1. Select contact in table
2. Click "✏️ Edit"
3. Update label or note
4. Save changes

### Deleting Contacts

1. Select contact
2. Click "🗑️ Delete"
3. Confirm deletion
4. Contact removed

### Address Book Storage

- Saved in `addressbook.json`
- Located in app directory
- Backup this file to keep contacts!

---

## Token Support

### Supported Token Standards

- **ERC-20** (Ethereum)
- **TRC-20** (Tron)
- **SPL** (Solana)

### Automatic Token Detection

DEE WALLET automatically detects popular tokens:

**Ethereum (ERC-20):**
- USDT (Tether)
- USDC (USD Coin)
- DAI (Dai Stablecoin)
- WETH (Wrapped Ether)
- UNI (Uniswap)

**Tron (TRC-20):**
- USDT (Tether)
- USDC (USD Coin)

**Solana (SPL):**
- USDT (Tether)
- USDC (USD Coin)
- Wrapped SOL

### Viewing Token Balances

1. Go to chain tab (Ethereum/Tron/Solana)
2. Click "Refresh"
3. Tokens automatically detected
4. Balances shown in wallet

### Sending Tokens

1. Select chain with token
2. Click "Send"
3. Choose token from dropdown
4. Enter recipient and amount
5. Send as normal transaction

---

## Security Best Practices

### Critical Security Rules

1. **Recovery Phrase**
   - ✅ Write on paper, store securely
   - ✅ Use multiple backup locations
   - ✅ Never share with anyone
   - ❌ Don't store digitally
   - ❌ Don't take screenshots
   - ❌ Don't email yourself

2. **Passwords**
   - ✅ Use 12+ characters
   - ✅ Mix uppercase, lowercase, numbers, symbols
   - ✅ Unique for this wallet
   - ✅ Use password manager
   - ❌ Don't use birthdays, names
   - ❌ Don't reuse from other sites

3. **Keyfiles**
   - ✅ Backup keyfile to USB/cloud
   - ✅ Encrypt backup storage
   - ✅ Keep multiple copies
   - ❌ Don't delete only copy
   - ❌ Don't share keyfile

4. **Transactions**
   - ✅ Double-check recipient address
   - ✅ Test with small amount first
   - ✅ Verify amount before confirming
   - ❌ Don't rush transactions
   - ❌ Transactions are irreversible!

5. **Computer Security**
   - ✅ Use antivirus software
   - ✅ Keep OS updated
   - ✅ Use firewall
   - ✅ Avoid public WiFi for large transactions
   - ❌ Don't download suspicious files

### Common Scams to Avoid

**❌ Phishing:**
- Fake websites asking for recovery phrase
- Emails claiming to be from DEE WALLET
- Messages asking for passwords

**❌ Ponzi Schemes:**
- "Send 1 BTC, get 2 BTC back"
- Guaranteed returns
- Too good to be true offers

**❌ Impersonation:**
- Fake support asking for keys
- Scammers pretending to be team
- "Official" accounts on social media

**✅ Remember:**
- No legitimate service asks for your recovery phrase
- DEE WALLET never asks for passwords via email
- Support will never message you first

---

## Troubleshooting

### Common Issues

**Q: Balances showing 0.0**
- Check internet connection
- Click "Refresh" button
- Wait 10-30 seconds for API response
- Try again in a few minutes (rate limits)

**Q: Transaction failed**
- Verify recipient address is correct
- Ensure sufficient balance (amount + fee)
- Check fee is adequate
- Network might be congested (try higher fee)

**Q: Can't open keyfile**
- Verify password is correct
- Check keyfile is not corrupted
- Try recovering from backup
- Import wallet from recovery phrase

**Q: Address validation failing**
- Check address format:
  * Bitcoin: starts with bc1, 1, or 3
  * Ethereum: starts with 0x, 42 chars
  * Tron: starts with T, 34 chars
  * Solana: Base58, 32-44 chars
- No spaces or typos
- Copy-paste instead of manual entry

**Q: QR code won't scan**
- Ensure good lighting
- Hold steady
- Try saving and sharing image instead
- Use "Copy Address" as fallback

### Error Messages

**"Failed to restore wallet from mnemonic"**
- Check all 12 words are correct
- Verify spelling
- Ensure correct word order
- Try generating new wallet

**"Network request failed"**
- Check internet connection
- Firewall might be blocking
- Try different network
- API might be down (try later)

**"Insufficient balance"**
- Amount + fee exceeds balance
- Reduce amount or fee
- Add more funds first

---

## FAQ

**Q: How much does DEE WALLET cost?**
A: DEE WALLET is completely free and open-source. You only pay blockchain transaction fees.

**Q: What fees do I pay?**
A: Only network fees (gas/miners/validators). DEE WALLET takes no fees.

**Q: Can I recover my wallet if I lose my computer?**
A: Yes! Use your 12-word recovery phrase on any device running DEE WALLET.

**Q: What if I forget my password?**
A: Use your recovery phrase to create a new wallet with a new password.

**Q: Can I use the same wallet on multiple devices?**
A: Yes! Import your keyfile or use recovery phrase on other devices.

**Q: How long do transactions take?**
A:
- Bitcoin: 10-60 minutes
- Ethereum: 1-5 minutes
- Tron: ~3 seconds
- Solana: ~1 second

**Q: Can I cancel a transaction after sending?**
A: No. Blockchain transactions are irreversible once confirmed.

**Q: What happens if I send to wrong address?**
A: Funds are lost permanently. Always double-check addresses!

**Q: Do you store my private keys?**
A: No! All keys are stored locally on your device, encrypted.

**Q: Can DEE WALLET access my funds?**
A: No. This is a non-custodial wallet. Only you control your keys.

**Q: How do I backup my wallet?**
A:
1. Write down 12-word recovery phrase
2. Backup keyfile to USB/cloud
3. Remember your password
4. Store all in secure locations

**Q: What's the minimum amount I can send?**
A: Technically any amount, but fees might exceed small sends. Recommended minimums:
- Bitcoin: 0.0001 BTC
- Ethereum: 0.01 ETH
- Tron: 1 TRX
- Solana: 0.01 SOL

---

## Support

Need help? Contact us:

- **GitHub Issues**: [Report bugs](https://github.com/yourusername/deewallet-cpp/issues)
- **Discussions**: [Ask questions](https://github.com/yourusername/deewallet-cpp/discussions)
- **Documentation**: [Full docs](https://github.com/yourusername/deewallet-cpp/wiki)

---

**Thank you for using DEE WALLET!** 🎉

*Your keys, your coins, your control*
