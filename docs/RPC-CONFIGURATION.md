# DEEWALLET - RPC Configuration Guide

## Overview

DEEWALLET uses a sophisticated RPC management system with **automatic failover** to ensure reliable connectivity to blockchain networks. The system maintains multiple public RPC endpoints for each supported chain and automatically switches to backup endpoints when failures are detected.

## Features

- ✅ **Multiple RPC Endpoints** - 20+ public RPC endpoints across all chains
- ✅ **Automatic Failover** - Switches to backup RPC when primary fails
- ✅ **Health Monitoring** - Continuous health checks every minute
- ✅ **Priority System** - Endpoints ranked by reliability
- ✅ **Rate Limiting** - Built-in rate limit awareness
- ✅ **Custom Endpoints** - Add your own RPC endpoints
- ✅ **Network Statistics** - Monitor RPC performance

## Supported Public RPCs

### Bitcoin (BTC)

**Mainnet:**
1. **Blockstream** - `https://blockstream.info/api` (Priority 1)
2. **Blockchain.com** - `https://blockchain.info` (Priority 2)
3. **Mempool.space** - `https://mempool.space/api` (Priority 3)
4. **BlockCypher** - `https://api.blockcypher.com/v1/btc/main` (Priority 4)

**Testnet:**
1. **Blockstream Testnet** - `https://blockstream.info/testnet/api`
2. **Mempool.space Testnet** - `https://mempool.space/testnet/api`

### Ethereum (ETH)

**Mainnet:**
1. **Cloudflare** - `https://cloudflare-eth.com` (Priority 1, 100 req/s)
2. **Ankr** - `https://rpc.ankr.com/eth` (Priority 2, 50 req/s)
3. **1RPC** - `https://1rpc.io/eth` (Priority 3, 50 req/s)
4. **LlamaNodes** - `https://eth.llamarpc.com` (Priority 4, 30 req/s)
5. **BlastAPI** - `https://eth-mainnet.public.blastapi.io` (Priority 5, 50 req/s)
6. **PublicNode** - `https://ethereum.publicnode.com` (Priority 7, 100 req/s)

**Testnet (Sepolia/Goerli):**
1. **Sepolia (Ankr)** - `https://rpc.ankr.com/eth_sepolia`
2. **Goerli (Ankr)** - `https://rpc.ankr.com/eth_goerli`

### Solana (SOL)

**Mainnet:**
1. **Solana Official** - `https://api.mainnet-beta.solana.com` (Priority 1, 100 req/s)
2. **Ankr** - `https://rpc.ankr.com/solana` (Priority 2, 50 req/s)
3. **Chainstack** - `https://solana-mainnet.core.chainstack.com` (Priority 3, 30 req/s)
4. **GetBlock** - `https://sol.getblock.io/mainnet` (Priority 4, 40 req/s)
5. **Serum** - `https://solana-api.projectserum.com` (Priority 5, 50 req/s)
6. **QuickNode** - `https://solana-mainnet.rpc.extrnode.com` (Priority 6, 20 req/s)

**Testnet:**
1. **Solana Devnet** - `https://api.devnet.solana.com`
2. **Solana Testnet** - `https://api.testnet.solana.com`

### Tron (TRX)

**Mainnet:**
1. **TronGrid (Official)** - `https://api.trongrid.io` (Priority 1, 100 req/s)
2. **TronStack** - `https://api.tronstack.io` (Priority 2, 50 req/s)
3. **Ankr** - `https://rpc.ankr.com/tron` (Priority 3, 50 req/s)
4. **GetBlock** - `https://trx.getblock.io/mainnet` (Priority 4, 40 req/s)

**Testnet:**
1. **Shasta Testnet** - `https://api.shasta.trongrid.io`
2. **Nile Testnet** - `https://nile.trongrid.io`

## How It Works

### 1. Priority-Based Selection

Endpoints are ranked by priority (1 = highest). The system automatically selects the highest priority healthy endpoint.

```typescript
{
  "name": "Cloudflare",
  "url": "https://cloudflare-eth.com",
  "priority": 1,
  "rateLimit": 100
}
```

### 2. Health Monitoring

Every 60 seconds, the system checks:
- Endpoint availability
- Response time
- Error rates

```typescript
// Health check runs automatically
setInterval(healthCheck, 60000); // 1 minute
```

### 3. Automatic Failover

When an endpoint fails 3 times:
1. System marks endpoint as unhealthy
2. Switches to next priority endpoint
3. Continues attempting until healthy endpoint found

```typescript
if (failureCount >= 3) {
  switchToNextEndpoint();
}
```

### 4. Recovery

After 5 minutes of no failures, the failure counter resets:

```typescript
if (timeSinceLastFailure > 300000) {
  resetFailureCount();
}
```

## Configuration

### Default Configuration

Located at: `config/rpc-endpoints.json`

```json
{
  "mainnet": {
    "ETH": [
      {
        "name": "Cloudflare",
        "url": "https://cloudflare-eth.com",
        "priority": 1,
        "rateLimit": 100
      }
    ]
  }
}
```

### Adding Custom Endpoints

#### Method 1: Edit Config File

Add to `config/rpc-endpoints.json`:

```json
{
  "mainnet": {
    "ETH": [
      {
        "name": "My Custom RPC",
        "url": "https://my-rpc.example.com",
        "priority": 10,
        "rateLimit": 50
      }
    ]
  }
}
```

#### Method 2: Programmatically

```typescript
import { RPCManager } from './utils/rpc-manager';

const rpcManager = new RPCManager();

rpcManager.addCustomEndpoint(ChainType.ETH, {
  name: 'My Custom RPC',
  url: 'https://my-rpc.example.com',
  priority: 10,
  rateLimit: 50
});
```

### Manually Switch Endpoint

```typescript
// Switch to specific endpoint
rpcManager.setEndpoint(
  ChainType.ETH,
  'https://eth.llamarpc.com'
);
```

## Using Your Own RPC

### Why Use Your Own RPC?

Public RPCs have limitations:
- **Rate limits** - Limited requests per second
- **Reliability** - Shared infrastructure
- **Privacy** - Your requests are visible
- **Features** - Some endpoints have limited functionality

### Recommended Providers

#### Ethereum
- **Alchemy** - https://www.alchemy.com/
  - Free tier: 300M compute units/month
  - Enhanced APIs
  - $0 to start

- **Infura** - https://infura.io/
  - Free tier: 100,000 requests/day
  - Reliable infrastructure
  - $0 to start

- **QuickNode** - https://www.quicknode.com/
  - Free tier: 100,000 requests/day
  - High performance
  - $0 to start

#### Solana
- **Helius** - https://www.helius.dev/
  - Free tier: 100,000 requests/day
  - WebSocket support
  - $0 to start

- **QuickNode** - https://www.quicknode.com/
  - Multi-chain support
  - DAS API support
  - Starting at $9/month

#### Bitcoin
- **GetBlock** - https://getblock.io/
  - Free tier: 40,000 requests/day
  - Multiple networks
  - $0 to start

#### Tron
- **TronGrid (API Key)** - https://www.trongrid.io/
  - Free tier with registration
  - Official Tron RPC
  - $0 to start

### Adding API Keys

Edit `config/rpc-endpoints.json`:

```json
{
  "mainnet": {
    "ETH": [
      {
        "name": "Alchemy",
        "url": "https://eth-mainnet.g.alchemy.com/v2/YOUR_API_KEY",
        "priority": 1,
        "rateLimit": 1000
      }
    ],
    "SOL": [
      {
        "name": "Helius",
        "url": "https://rpc.helius.xyz/?api-key=YOUR_API_KEY",
        "priority": 1,
        "rateLimit": 1000
      }
    ]
  }
}
```

## Monitoring RPC Performance

### View Statistics

Get current RPC stats:

```typescript
const stats = rpcManager.getStats();

// Output:
// {
//   ETH: {
//     endpoint: 'https://cloudflare-eth.com',
//     name: 'Cloudflare',
//     failures: 0,
//     avgResponseTime: 150,
//     isHealthy: true
//   }
// }
```

### IPC API (from Renderer)

```typescript
// Get RPC stats
const { stats } = await window.electron.invoke('rpc:get-stats');

// Get available endpoints
const { endpoints } = await window.electron.invoke(
  'rpc:get-endpoints',
  ChainType.ETH
);

// Switch endpoint
await window.electron.invoke(
  'rpc:set-endpoint',
  ChainType.ETH,
  'https://eth.llamarpc.com'
);
```

## Troubleshooting

### All Endpoints Failing

If all endpoints fail:

1. **Check Internet Connection**
   ```bash
   ping 8.8.8.8
   ```

2. **Test RPC Directly**
   ```bash
   curl -X POST https://cloudflare-eth.com \
     -H "Content-Type: application/json" \
     -d '{"jsonrpc":"2.0","method":"eth_blockNumber","params":[],"id":1}'
   ```

3. **Check Firewall**
   - Ensure outbound HTTPS (443) is allowed
   - Check corporate firewall settings

4. **Try Different Network**
   - Switch between WiFi/Ethernet
   - Try mobile hotspot

### Slow Performance

1. **Check Response Times**
   ```typescript
   const stats = rpcManager.getStats();
   console.log(stats.get(ChainType.ETH).avgResponseTime);
   ```

2. **Switch to Faster Endpoint**
   - Use geographically closer RPC
   - Upgrade to paid tier

3. **Reduce Request Frequency**
   - Implement caching
   - Batch requests when possible

### Rate Limiting

If you hit rate limits:

1. **Use Multiple Endpoints**
   - System automatically rotates
   - Add more endpoints to config

2. **Upgrade to Paid Plan**
   - Most providers offer affordable tiers
   - Significantly higher limits

3. **Implement Caching**
   - Cache balance queries
   - Store recent transactions

## Best Practices

1. **Always Have Backups**
   - Configure at least 3 endpoints per chain
   - Mix free and paid providers

2. **Use API Keys for Production**
   - Much higher rate limits
   - Better reliability
   - Enhanced features

3. **Monitor Health**
   - Check RPC stats regularly
   - Set up alerts for failures

4. **Geographic Distribution**
   - Use RPCs close to your users
   - Consider multi-region setup

5. **Test Before Production**
   - Always test on testnet first
   - Verify endpoint compatibility

## Security Considerations

1. **Never Share API Keys**
   - Keep keys in config files
   - Add config to `.gitignore`
   - Use environment variables

2. **Use HTTPS Only**
   - All endpoints must use HTTPS
   - Verify SSL certificates

3. **Validate Responses**
   - Check response signatures
   - Verify data integrity
   - Don't trust blindly

## Support

For RPC-related issues:
- Check provider status pages
- Contact provider support
- Report issues on GitHub

---

**Note**: Public RPCs may change or become unavailable. Always maintain backup options!
