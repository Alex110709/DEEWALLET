/**
 * DEEWALLET - RPC Manager with Auto-Fallback
 * Manages multiple RPC endpoints with health checking and automatic failover
 */

import { ChainType, NetworkType } from '../types/wallet.types';
import * as fs from 'fs';
import * as path from 'path';

export interface RPCEndpoint {
  name: string;
  url: string;
  priority: number;
  rateLimit: number;
  lastUsed?: number;
  failureCount?: number;
  avgResponseTime?: number;
  isHealthy?: boolean;
}

export interface RPCConfig {
  mainnet: {
    [key in ChainType]: RPCEndpoint[];
  };
  testnet: {
    [key in ChainType]: RPCEndpoint[];
  };
}

export class RPCManager {
  private config: RPCConfig;
  private currentEndpoints: Map<ChainType, RPCEndpoint>;
  private healthCheckInterval: NodeJS.Timeout | null = null;
  private readonly HEALTH_CHECK_INTERVAL = 60000; // 1 minute
  private readonly MAX_FAILURES = 3;
  private readonly FAILURE_RESET_TIME = 300000; // 5 minutes

  constructor(private network: NetworkType = NetworkType.MAINNET) {
    this.config = this.loadConfig();
    this.currentEndpoints = new Map();
    this.initializeEndpoints();
    this.startHealthCheck();
  }

  /**
   * Load RPC configuration from file
   */
  private loadConfig(): RPCConfig {
    try {
      const configPath = path.join(__dirname, '../../config/rpc-endpoints.json');
      const configData = fs.readFileSync(configPath, 'utf-8');
      return JSON.parse(configData);
    } catch (error) {
      console.error('Failed to load RPC config, using defaults:', error);
      return this.getDefaultConfig();
    }
  }

  /**
   * Get default RPC configuration
   */
  private getDefaultConfig(): RPCConfig {
    return {
      mainnet: {
        [ChainType.BTC]: [
          { name: 'Blockstream', url: 'https://blockstream.info/api', priority: 1, rateLimit: 10 }
        ],
        [ChainType.ETH]: [
          { name: 'Cloudflare', url: 'https://cloudflare-eth.com', priority: 1, rateLimit: 100 }
        ],
        [ChainType.SOL]: [
          { name: 'Solana Mainnet', url: 'https://api.mainnet-beta.solana.com', priority: 1, rateLimit: 100 }
        ],
        [ChainType.TRX]: [
          { name: 'TronGrid', url: 'https://api.trongrid.io', priority: 1, rateLimit: 100 }
        ]
      },
      testnet: {
        [ChainType.BTC]: [
          { name: 'Blockstream Testnet', url: 'https://blockstream.info/testnet/api', priority: 1, rateLimit: 10 }
        ],
        [ChainType.ETH]: [
          { name: 'Sepolia', url: 'https://rpc.ankr.com/eth_sepolia', priority: 1, rateLimit: 50 }
        ],
        [ChainType.SOL]: [
          { name: 'Solana Devnet', url: 'https://api.devnet.solana.com', priority: 1, rateLimit: 100 }
        ],
        [ChainType.TRX]: [
          { name: 'Shasta', url: 'https://api.shasta.trongrid.io', priority: 1, rateLimit: 100 }
        ]
      }
    };
  }

  /**
   * Initialize endpoints for all chains
   */
  private initializeEndpoints(): void {
    const networkConfig = this.network === NetworkType.MAINNET
      ? this.config.mainnet
      : this.config.testnet;

    Object.values(ChainType).forEach(chainType => {
      const endpoints = networkConfig[chainType];
      if (endpoints && endpoints.length > 0) {
        // Sort by priority and select the first one
        const sortedEndpoints = [...endpoints].sort((a, b) => a.priority - b.priority);
        const bestEndpoint = {
          ...sortedEndpoints[0],
          failureCount: 0,
          isHealthy: true,
          avgResponseTime: 0
        };
        this.currentEndpoints.set(chainType, bestEndpoint);
      }
    });
  }

  /**
   * Get current RPC URL for a chain
   */
  getEndpoint(chainType: ChainType): string {
    const endpoint = this.currentEndpoints.get(chainType);
    if (!endpoint) {
      throw new Error(`No RPC endpoint configured for ${chainType}`);
    }
    return endpoint.url;
  }

  /**
   * Get all available endpoints for a chain
   */
  getAvailableEndpoints(chainType: ChainType): RPCEndpoint[] {
    const networkConfig = this.network === NetworkType.MAINNET
      ? this.config.mainnet
      : this.config.testnet;

    return networkConfig[chainType] || [];
  }

  /**
   * Report RPC failure and attempt failover
   */
  async reportFailure(chainType: ChainType, error?: Error): Promise<void> {
    const currentEndpoint = this.currentEndpoints.get(chainType);
    if (!currentEndpoint) return;

    currentEndpoint.failureCount = (currentEndpoint.failureCount || 0) + 1;
    console.warn(`RPC failure for ${chainType} (${currentEndpoint.name}):`, error?.message);

    // If failures exceed threshold, switch to next endpoint
    if (currentEndpoint.failureCount >= this.MAX_FAILURES) {
      console.log(`Switching RPC endpoint for ${chainType}...`);
      await this.switchToNextEndpoint(chainType);
    }
  }

  /**
   * Switch to next available endpoint
   */
  private async switchToNextEndpoint(chainType: ChainType): Promise<void> {
    const availableEndpoints = this.getAvailableEndpoints(chainType);
    const currentEndpoint = this.currentEndpoints.get(chainType);

    if (availableEndpoints.length <= 1) {
      console.warn(`No alternative RPC endpoints available for ${chainType}`);
      // Reset failure count to retry
      if (currentEndpoint) {
        currentEndpoint.failureCount = 0;
      }
      return;
    }

    // Find next healthy endpoint
    const currentIndex = availableEndpoints.findIndex(ep => ep.url === currentEndpoint?.url);
    let nextIndex = (currentIndex + 1) % availableEndpoints.length;
    let attempts = 0;

    while (attempts < availableEndpoints.length) {
      const nextEndpoint = availableEndpoints[nextIndex];
      const isHealthy = await this.checkEndpointHealth(chainType, nextEndpoint);

      if (isHealthy) {
        this.currentEndpoints.set(chainType, {
          ...nextEndpoint,
          failureCount: 0,
          isHealthy: true
        });
        console.log(`Switched ${chainType} RPC to: ${nextEndpoint.name} (${nextEndpoint.url})`);
        return;
      }

      nextIndex = (nextIndex + 1) % availableEndpoints.length;
      attempts++;
    }

    console.error(`All RPC endpoints failed for ${chainType}`);
  }

  /**
   * Check endpoint health
   */
  private async checkEndpointHealth(
    chainType: ChainType,
    endpoint: RPCEndpoint
  ): Promise<boolean> {
    try {
      const startTime = Date.now();

      // Simple health check based on chain type
      let response: any;

      switch (chainType) {
        case ChainType.ETH:
          response = await fetch(endpoint.url, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
              jsonrpc: '2.0',
              method: 'eth_blockNumber',
              params: [],
              id: 1
            })
          });
          break;

        case ChainType.SOL:
          response = await fetch(endpoint.url, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
              jsonrpc: '2.0',
              method: 'getHealth',
              id: 1
            })
          });
          break;

        case ChainType.BTC:
          response = await fetch(`${endpoint.url}/blocks/tip/height`);
          break;

        case ChainType.TRX:
          response = await fetch(`${endpoint.url}/wallet/getnowblock`);
          break;

        default:
          return false;
      }

      const responseTime = Date.now() - startTime;

      if (response.ok) {
        endpoint.avgResponseTime = responseTime;
        return true;
      }

      return false;
    } catch (error) {
      console.error(`Health check failed for ${endpoint.name}:`, error);
      return false;
    }
  }

  /**
   * Start periodic health checks
   */
  private startHealthCheck(): void {
    if (this.healthCheckInterval) {
      clearInterval(this.healthCheckInterval);
    }

    this.healthCheckInterval = setInterval(async () => {
      for (const [chainType, endpoint] of this.currentEndpoints) {
        const isHealthy = await this.checkEndpointHealth(chainType, endpoint);

        if (!isHealthy) {
          await this.reportFailure(chainType);
        } else if (endpoint.failureCount && endpoint.failureCount > 0) {
          // Reset failure count if endpoint is healthy again
          const timeSinceLastFailure = Date.now() - (endpoint.lastUsed || 0);
          if (timeSinceLastFailure > this.FAILURE_RESET_TIME) {
            endpoint.failureCount = 0;
            console.log(`Reset failure count for ${chainType} - ${endpoint.name}`);
          }
        }
      }
    }, this.HEALTH_CHECK_INTERVAL);
  }

  /**
   * Stop health checks
   */
  stopHealthCheck(): void {
    if (this.healthCheckInterval) {
      clearInterval(this.healthCheckInterval);
      this.healthCheckInterval = null;
    }
  }

  /**
   * Get RPC statistics
   */
  getStats(): Map<ChainType, {
    endpoint: string;
    name: string;
    failures: number;
    avgResponseTime: number;
    isHealthy: boolean;
  }> {
    const stats = new Map();

    for (const [chainType, endpoint] of this.currentEndpoints) {
      stats.set(chainType, {
        endpoint: endpoint.url,
        name: endpoint.name,
        failures: endpoint.failureCount || 0,
        avgResponseTime: endpoint.avgResponseTime || 0,
        isHealthy: endpoint.isHealthy || false
      });
    }

    return stats;
  }

  /**
   * Manually set endpoint for a chain
   */
  setEndpoint(chainType: ChainType, endpointUrl: string): boolean {
    const availableEndpoints = this.getAvailableEndpoints(chainType);
    const endpoint = availableEndpoints.find(ep => ep.url === endpointUrl);

    if (endpoint) {
      this.currentEndpoints.set(chainType, {
        ...endpoint,
        failureCount: 0,
        isHealthy: true
      });
      console.log(`Manually set ${chainType} RPC to: ${endpoint.name}`);
      return true;
    }

    return false;
  }

  /**
   * Add custom endpoint
   */
  addCustomEndpoint(
    chainType: ChainType,
    endpoint: Omit<RPCEndpoint, 'failureCount' | 'isHealthy'>
  ): void {
    const networkConfig = this.network === NetworkType.MAINNET
      ? this.config.mainnet
      : this.config.testnet;

    if (!networkConfig[chainType]) {
      networkConfig[chainType] = [];
    }

    networkConfig[chainType].push(endpoint);
    console.log(`Added custom endpoint for ${chainType}: ${endpoint.name}`);
  }
}
