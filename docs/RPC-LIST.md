# DEEWALLET - Complete RPC Endpoints List

## 📡 Public RPC Endpoints

This document lists all configured public RPC endpoints available in DEEWALLET.

---

## Bitcoin (BTC)

### Mainnet Endpoints

| Priority | Provider | URL | Rate Limit | Features |
|----------|----------|-----|------------|----------|
| 1 | Blockstream | `https://blockstream.info/api` | 10 req/s | Block explorer, reliable |
| 2 | Blockchain.com | `https://blockchain.info` | 10 req/s | Historical data |
| 3 | Mempool.space | `https://mempool.space/api` | 10 req/s | Fee estimation, mempool |
| 4 | BlockCypher | `https://api.blockcypher.com/v1/btc/main` | 5 req/s | Webhooks support |

### Testnet Endpoints

| Provider | URL | Network |
|----------|-----|---------|
| Blockstream | `https://blockstream.info/testnet/api` | Testnet |
| Mempool.space | `https://mempool.space/testnet/api` | Testnet |

---

## Ethereum (ETH)

### Mainnet Endpoints

| Priority | Provider | URL | Rate Limit | Features |
|----------|----------|-----|------------|----------|
| 1 | Cloudflare | `https://cloudflare-eth.com` | 100 req/s | Fast, reliable |
| 2 | Ankr | `https://rpc.ankr.com/eth` | 50 req/s | Multi-chain |
| 3 | 1RPC | `https://1rpc.io/eth` | 50 req/s | Privacy-focused |
| 4 | LlamaNodes | `https://eth.llamarpc.com` | 30 req/s | MEV protection |
| 5 | BlastAPI | `https://eth-mainnet.public.blastapi.io` | 50 req/s | Low latency |
| 6 | Infura Public | `https://mainnet.infura.io/v3/...` | 10 req/s | Limited public |
| 7 | PublicNode | `https://ethereum.publicnode.com` | 100 req/s | Community-run |

### Testnet Endpoints (Sepolia/Goerli)

| Provider | URL | Network | Rate Limit |
|----------|-----|---------|------------|
| Infura Sepolia | `https://sepolia.infura.io/v3/...` | Sepolia | 10 req/s |
| Ankr Sepolia | `https://rpc.ankr.com/eth_sepolia` | Sepolia | 50 req/s |
| Ankr Goerli | `https://rpc.ankr.com/eth_goerli` | Goerli | 50 req/s |

---

## Solana (SOL)

### Mainnet Endpoints

| Priority | Provider | URL | Rate Limit | Features |
|----------|----------|-----|------------|----------|
| 1 | Official | `https://api.mainnet-beta.solana.com` | 100 req/s | Official mainnet |
| 2 | Ankr | `https://rpc.ankr.com/solana` | 50 req/s | Fast response |
| 3 | Chainstack | `https://solana-mainnet.core.chainstack.com` | 30 req/s | Enterprise-grade |
| 4 | GetBlock | `https://sol.getblock.io/mainnet` | 40 req/s | Reliable |
| 5 | Serum | `https://solana-api.projectserum.com` | 50 req/s | DEX-optimized |
| 6 | QuickNode | `https://solana-mainnet.rpc.extrnode.com` | 20 req/s | Fast |

### Testnet/Devnet Endpoints

| Provider | URL | Network | Rate Limit |
|----------|-----|---------|------------|
| Official Devnet | `https://api.devnet.solana.com` | Devnet | 100 req/s |
| Official Testnet | `https://api.testnet.solana.com` | Testnet | 100 req/s |

---

## Tron (TRX)

### Mainnet Endpoints

| Priority | Provider | URL | Rate Limit | Features |
|----------|----------|-----|------------|----------|
| 1 | TronGrid (Official) | `https://api.trongrid.io` | 100 req/s | Official, reliable |
| 2 | TronStack | `https://api.tronstack.io` | 50 req/s | Alternative |
| 3 | Ankr | `https://rpc.ankr.com/tron` | 50 req/s | Multi-chain |
| 4 | GetBlock | `https://trx.getblock.io/mainnet` | 40 req/s | Reliable |
| 5 | NileGrid | `https://api.shasta.trongrid.io` | 30 req/s | Backup |

### Testnet Endpoints

| Provider | URL | Network | Rate Limit |
|----------|-----|---------|------------|
| Shasta | `https://api.shasta.trongrid.io` | Shasta Testnet | 100 req/s |
| Nile | `https://nile.trongrid.io` | Nile Testnet | 100 req/s |

---

## 🔧 Using Custom/Paid RPCs

For production use, consider using paid RPC services for:
- Higher rate limits
- Better reliability
- Lower latency
- Advanced features

### Recommended Providers

#### Multi-Chain
- **Ankr** - https://www.ankr.com/rpc/
  - Free tier: 500M requests/month
  - Supports BTC, ETH, SOL, TRX

- **GetBlock** - https://getblock.io/
  - Free tier: 40K requests/day
  - 50+ blockchains

#### Ethereum-Specific
- **Alchemy** - https://www.alchemy.com/
  - Free: 300M compute units/month
  - Enhanced APIs, WebSockets

- **Infura** - https://infura.io/
  - Free: 100K requests/day
  - IPFS support

- **QuickNode** - https://www.quicknode.com/
  - Free: 100K requests/day
  - Very fast response times

#### Solana-Specific
- **Helius** - https://www.helius.dev/
  - Free: 100K requests/day
  - DAS API, Webhooks

- **GenesysGo** - https://genesysgo.com/
  - Premium infrastructure
  - Shadow Drive integration

#### Bitcoin-Specific
- **Blockchair** - https://blockchair.com/
  - API for multiple chains
  - Analytics features

- **Bitaps** - https://bitaps.com/
  - Bitcoin-focused
  - Free tier available

---

## 📊 RPC Performance Comparison

Based on community testing (approximate values):

### Ethereum

| Provider | Avg Latency | Reliability | Rate Limit | Cost |
|----------|-------------|-------------|------------|------|
| Cloudflare | 50-100ms | ⭐⭐⭐⭐⭐ | 100 req/s | Free |
| Ankr | 80-150ms | ⭐⭐⭐⭐ | 50 req/s | Free |
| Alchemy (Paid) | 30-70ms | ⭐⭐⭐⭐⭐ | 1000+ req/s | $49/mo |
| Infura (Paid) | 40-80ms | ⭐⭐⭐⭐⭐ | Unlimited | $50/mo |

### Solana

| Provider | Avg Latency | Reliability | Rate Limit | Cost |
|----------|-------------|-------------|------------|------|
| Official | 100-200ms | ⭐⭐⭐ | 100 req/s | Free |
| Helius (Paid) | 50-100ms | ⭐⭐⭐⭐⭐ | 1000+ req/s | $29/mo |
| QuickNode | 60-120ms | ⭐⭐⭐⭐ | Custom | $9+/mo |

### Bitcoin

| Provider | Avg Latency | Reliability | Rate Limit | Cost |
|----------|-------------|-------------|------------|------|
| Blockstream | 100-200ms | ⭐⭐⭐⭐⭐ | 10 req/s | Free |
| Mempool.space | 80-150ms | ⭐⭐⭐⭐ | 10 req/s | Free |

---

## 🔄 Failover Strategy

DEEWALLET automatically manages RPC failover:

1. **Primary Selection**
   - Selects highest priority healthy endpoint
   - Monitors response time and errors

2. **Health Monitoring**
   - Checks every 60 seconds
   - Tracks failure count
   - Measures response time

3. **Automatic Switching**
   - After 3 failures, switches to next endpoint
   - Rotates through available endpoints
   - Continues until healthy endpoint found

4. **Recovery**
   - After 5 minutes without failures
   - Resets failure counter
   - May switch back to primary

---

## 🛠️ Adding Your Own RPC

### Method 1: Edit Configuration File

1. Open `config/rpc-endpoints.json`
2. Add your endpoint:

```json
{
  "mainnet": {
    "ETH": [
      {
        "name": "My Alchemy",
        "url": "https://eth-mainnet.g.alchemy.com/v2/YOUR_KEY",
        "priority": 1,
        "rateLimit": 1000
      }
    ]
  }
}
```

### Method 2: Through Settings UI

(Coming in future version)

1. Open Settings
2. Click "RPC Endpoints"
3. Add new endpoint
4. Test and save

---

## 📈 Monitoring RPC Health

### View Current Status

From the app, you can:
- See which RPC is currently active
- View failure counts
- Check average response times
- Switch between endpoints manually

### Console Monitoring

```typescript
// Get RPC stats
const stats = await window.electron.invoke('rpc:get-stats');
console.log(stats);

// Output:
// {
//   ETH: {
//     endpoint: 'https://cloudflare-eth.com',
//     name: 'Cloudflare',
//     failures: 0,
//     avgResponseTime: 85,
//     isHealthy: true
//   }
// }
```

---

## ⚠️ Rate Limit Best Practices

1. **Monitor Usage**
   - Track your request count
   - Stay within limits
   - Use caching when possible

2. **Implement Backoff**
   - Reduce request frequency when limited
   - Wait before retrying
   - Distribute load across endpoints

3. **Upgrade When Needed**
   - Production apps need higher limits
   - Paid plans offer better reliability
   - Consider dedicated nodes for heavy usage

---

## 🔐 Security Considerations

1. **API Key Protection**
   - Never commit API keys to git
   - Use environment variables
   - Rotate keys regularly

2. **Verify SSL**
   - Only use HTTPS endpoints
   - Verify certificates
   - Don't ignore SSL errors

3. **Validate Responses**
   - Check response integrity
   - Verify block hashes
   - Don't trust blindly

---

## 📚 Additional Resources

- [Ethereum RPC Providers](https://ethereumnodes.com/)
- [Chainlist](https://chainlist.org/) - Network configurations
- [Solana RPC Endpoints](https://solana.com/rpc)
- [Tron Documentation](https://developers.tron.network/)

---

**Last Updated**: 2024-11

**Note**: RPC endpoints may change. Always verify current URLs and availability.
