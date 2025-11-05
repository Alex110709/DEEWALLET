# DEEWALLET - API Documentation

## Core Wallet API

### WalletCore

The main wallet management class handling mnemonic generation, key derivation, and keyfile management.

#### Constructor

```typescript
constructor(config: WalletConfig)
```

**Parameters:**
- `config`: Wallet configuration object

**Example:**
```typescript
const config: WalletConfig = {
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
```

#### Methods

##### generateWallet()

Generate a new HD wallet with BIP39 mnemonic.

```typescript
async generateWallet(strength: 128 | 256 = 256): Promise<string>
```

**Parameters:**
- `strength`: Mnemonic strength (128 = 12 words, 256 = 24 words)

**Returns:** Mnemonic phrase string

**Example:**
```typescript
const mnemonic = await wallet.generateWallet(256);
console.log(mnemonic); // "word1 word2 ... word24"
```

##### restoreFromMnemonic()

Restore wallet from existing mnemonic phrase.

```typescript
async restoreFromMnemonic(mnemonic: string): Promise<boolean>
```

**Parameters:**
- `mnemonic`: 12 or 24-word mnemonic phrase

**Returns:** Boolean indicating success

**Throws:** Error if mnemonic is invalid

**Example:**
```typescript
await wallet.restoreFromMnemonic("apple banana cherry...");
```

##### deriveAccount()

Derive a blockchain account at specified index.

```typescript
deriveAccount(chainType: ChainType, accountIndex: number = 0): WalletAccount
```

**Parameters:**
- `chainType`: Blockchain type (BTC, ETH, SOL, TRX)
- `accountIndex`: Account index in HD wallet (default: 0)

**Returns:** WalletAccount object

**Example:**
```typescript
const btcAccount = wallet.deriveAccount(ChainType.BTC, 0);
const ethAccount = wallet.deriveAccount(ChainType.ETH, 0);
```

##### exportToKeyfile()

Export wallet to encrypted keyfile.

```typescript
async exportToKeyfile(password: string): Promise<KeyfileData>
```

**Parameters:**
- `password`: Encryption password

**Returns:** KeyfileData object

**Example:**
```typescript
const keyfile = await wallet.exportToKeyfile('secure-password-123');
fs.writeFileSync('wallet.keyfile', JSON.stringify(keyfile));
```

##### importFromKeyfile()

Import wallet from encrypted keyfile.

```typescript
async importFromKeyfile(keyfileData: KeyfileData, password: string): Promise<boolean>
```

**Parameters:**
- `keyfileData`: Keyfile data object
- `password`: Decryption password

**Returns:** Boolean indicating success

**Throws:** Error if password is incorrect or keyfile is corrupted

**Example:**
```typescript
const keyfile = JSON.parse(fs.readFileSync('wallet.keyfile', 'utf-8'));
await wallet.importFromKeyfile(keyfile, 'secure-password-123');
```

## Chain Adapters

### IChainAdapter Interface

All chain adapters implement this interface.

```typescript
interface IChainAdapter {
  chainType: ChainType;
  generateAddress(publicKey: string, privateKey?: string): string;
  getBalance(address: string): Promise<string>;
  getTokens(address: string): Promise<Token[]>;
  sendTransaction(params: SendTransactionParams, privateKey: string): Promise<Transaction>;
  getTransactionHistory(address: string, limit?: number): Promise<Transaction[]>;
  validateAddress(address: string): boolean;
  estimateFee(params: SendTransactionParams): Promise<string>;
}
```

### BitcoinChainAdapter

```typescript
const btcAdapter = new BitcoinChainAdapter(rpcUrl, isTestnet);
```

**Methods:**
- `generateAddress(publicKey, privateKey?)`: Generate P2WPKH address
- `getBalance(address)`: Get BTC balance
- `sendTransaction(params, privateKey)`: Send BTC transaction
- `validateAddress(address)`: Validate Bitcoin address format

### EthereumChainAdapter

```typescript
const ethAdapter = new EthereumChainAdapter(rpcUrl);
```

**Methods:**
- `generateAddress(publicKey, privateKey?)`: Generate EIP-55 checksum address
- `getBalance(address)`: Get ETH balance
- `getTokens(address)`: Get ERC-20 token balances
- `sendTransaction(params, privateKey)`: Send ETH or ERC-20 transaction
- `validateAddress(address)`: Validate Ethereum address

**ERC-20 Token Transfer:**
```typescript
const tx = await ethAdapter.sendTransaction({
  to: '0x742d35Cc6634C0532925a3b844Bc9e7595f0bEb',
  amount: '100',
  chainType: ChainType.ETH,
  token: '0xdAC17F958D2ee523a2206206994597C13D831ec7', // USDT
  gasLimit: '65000',
  gasPrice: '20'
}, privateKey);
```

### SolanaChainAdapter

```typescript
const solAdapter = new SolanaChainAdapter(rpcUrl);
```

**Methods:**
- `generateAddress(publicKey, privateKey?)`: Generate Base58 address
- `getBalance(address)`: Get SOL balance
- `getTokens(address)`: Get SPL token balances
- `sendTransaction(params, privateKey)`: Send SOL or SPL token transaction
- `validateAddress(address)`: Validate Solana address

### TronChainAdapter

```typescript
const trxAdapter = new TronChainAdapter(rpcUrl);
```

**Methods:**
- `generateAddress(publicKey, privateKey?)`: Generate Base58 address
- `getBalance(address)`: Get TRX balance
- `getTokens(address)`: Get TRC-20 token balances
- `sendTransaction(params, privateKey)`: Send TRX or TRC-20 transaction
- `validateAddress(address)`: Validate Tron address

## Type Definitions

### ChainType

```typescript
enum ChainType {
  BTC = 'BTC',
  ETH = 'ETH',
  TRX = 'TRX',
  SOL = 'SOL'
}
```

### WalletAccount

```typescript
interface WalletAccount {
  address: string;
  publicKey: string;
  privateKey?: string;
  derivationPath: string;
  chainType: ChainType;
  balance: string;
  index: number;
}
```

### Token

```typescript
interface Token {
  symbol: string;
  name: string;
  decimals: number;
  contractAddress: string;
  balance: string;
  logo?: string;
  chainType: ChainType;
}
```

### Transaction

```typescript
interface Transaction {
  hash: string;
  from: string;
  to: string;
  value: string;
  chainType: ChainType;
  timestamp: number;
  status: 'pending' | 'confirmed' | 'failed';
  gasUsed?: string;
  fee?: string;
}
```

### SendTransactionParams

```typescript
interface SendTransactionParams {
  to: string;
  amount: string;
  chainType: ChainType;
  token?: string; // Token contract address
  gasLimit?: string;
  gasPrice?: string;
}
```

## Electron IPC API

### Main Process → Renderer Process

#### wallet:generate

Generate new wallet.

```typescript
ipcRenderer.invoke('wallet:generate')
  .then(({ success, mnemonic, error }) => {
    if (success) {
      console.log('Mnemonic:', mnemonic);
    }
  });
```

#### wallet:restore

Restore wallet from mnemonic.

```typescript
ipcRenderer.invoke('wallet:restore', mnemonic)
  .then(({ success, error }) => {
    if (success) {
      console.log('Wallet restored');
    }
  });
```

#### wallet:export

Export wallet to keyfile.

```typescript
ipcRenderer.invoke('wallet:export', password)
  .then(({ success, path, error }) => {
    if (success) {
      console.log('Keyfile saved to:', path);
    }
  });
```

#### wallet:import

Import wallet from keyfile.

```typescript
ipcRenderer.invoke('wallet:import', password)
  .then(({ success, error }) => {
    if (success) {
      console.log('Wallet imported');
    }
  });
```

#### wallet:derive-account

Derive blockchain account.

```typescript
ipcRenderer.invoke('wallet:derive-account', ChainType.ETH, 0)
  .then(({ success, account, error }) => {
    if (success) {
      console.log('Address:', account.address);
    }
  });
```

#### chain:get-balance

Get account balance.

```typescript
ipcRenderer.invoke('chain:get-balance', ChainType.ETH, address)
  .then(({ success, balance, error }) => {
    if (success) {
      console.log('Balance:', balance);
    }
  });
```

#### chain:get-tokens

Get token balances.

```typescript
ipcRenderer.invoke('chain:get-tokens', ChainType.ETH, address)
  .then(({ success, tokens, error }) => {
    if (success) {
      console.log('Tokens:', tokens);
    }
  });
```

#### chain:send-transaction

Send transaction.

```typescript
const params = {
  to: '0x742d35Cc6634C0532925a3b844Bc9e7595f0bEb',
  amount: '0.1',
  accountIndex: 0
};

ipcRenderer.invoke('chain:send-transaction', params, ChainType.ETH)
  .then(({ success, transaction, error }) => {
    if (success) {
      console.log('TX Hash:', transaction.hash);
    }
  });
```

## Error Handling

All methods follow consistent error handling:

```typescript
try {
  const result = await wallet.generateWallet();
  // Handle success
} catch (error) {
  console.error('Error:', error.message);
  // Handle error
}
```

IPC methods return objects with `success` and `error` fields:

```typescript
const { success, data, error } = await ipcRenderer.invoke('method');

if (success) {
  // Use data
} else {
  // Handle error
  console.error(error);
}
```

## Security Considerations

1. **Never expose private keys in logs**
2. **Always validate addresses before transactions**
3. **Clear sensitive data from memory after use**
4. **Use strong passwords for keyfiles**
5. **Verify transaction parameters before signing**
6. **Handle errors securely without leaking info**

## Rate Limiting

When using public RPC endpoints:
- Implement request throttling
- Cache responses when possible
- Use paid plans for production
- Handle rate limit errors gracefully

## Best Practices

1. **Always check return values**
2. **Implement proper error handling**
3. **Validate all user inputs**
4. **Use TypeScript for type safety**
5. **Keep dependencies updated**
6. **Test thoroughly on testnet first**
