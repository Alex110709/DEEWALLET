/**
 * DEEWALLET - Ethereum Chain Adapter
 * Supports ETH and ERC-20 tokens
 */

import { ethers } from 'ethers';
import { BaseChainAdapter } from './base.chain';
import { ChainType, Token, Transaction, SendTransactionParams } from '../types/wallet.types';

// ERC-20 ABI for token operations
const ERC20_ABI = [
  'function balanceOf(address owner) view returns (uint256)',
  'function decimals() view returns (uint8)',
  'function symbol() view returns (string)',
  'function name() view returns (string)',
  'function transfer(address to, uint256 amount) returns (bool)'
];

export class EthereumChainAdapter extends BaseChainAdapter {
  chainType = ChainType.ETH;
  private provider: ethers.JsonRpcProvider;

  constructor(rpcUrl: string) {
    super(rpcUrl);
    this.provider = new ethers.JsonRpcProvider(rpcUrl);
  }

  /**
   * Generate Ethereum address from public key
   */
  generateAddress(publicKey: string, privateKey?: string): string {
    if (privateKey) {
      const wallet = new ethers.Wallet(privateKey);
      return wallet.address;
    }

    // Generate from public key (uncompressed)
    const pubKeyBuffer = Buffer.from(publicKey, 'hex');
    const addressHash = ethers.keccak256(pubKeyBuffer.slice(1));
    return ethers.getAddress('0x' + addressHash.slice(-40));
  }

  /**
   * Get ETH balance
   */
  async getBalance(address: string): Promise<string> {
    try {
      const balance = await this.provider.getBalance(address);
      return ethers.formatEther(balance);
    } catch (error) {
      throw new Error(`Failed to get balance: ${(error as Error).message}`);
    }
  }

  /**
   * Get ERC-20 tokens
   */
  async getTokens(address: string): Promise<Token[]> {
    // This is a simplified version
    // In production, you'd query a token list or indexer service
    const tokens: Token[] = [];

    // Example popular tokens - in production, fetch from API or user's token list
    const knownTokens = [
      '0xdAC17F958D2ee523a2206206994597C13D831ec7', // USDT
      '0xA0b86991c6218b36c1d19D4a2e9Eb0cE3606eB48', // USDC
      '0x6B175474E89094C44Da98b954EedeAC495271d0F'  // DAI
    ];

    for (const tokenAddress of knownTokens) {
      try {
        const contract = new ethers.Contract(tokenAddress, ERC20_ABI, this.provider);
        const [balance, decimals, symbol, name] = await Promise.all([
          contract.balanceOf(address),
          contract.decimals(),
          contract.symbol(),
          contract.name()
        ]);

        if (balance > 0n) {
          tokens.push({
            symbol,
            name,
            decimals,
            contractAddress: tokenAddress,
            balance: ethers.formatUnits(balance, decimals),
            chainType: ChainType.ETH
          });
        }
      } catch (error) {
        // Skip if token query fails
        continue;
      }
    }

    return tokens;
  }

  /**
   * Send ETH or ERC-20 transaction
   */
  async sendTransaction(params: SendTransactionParams, privateKey: string): Promise<Transaction> {
    const wallet = new ethers.Wallet(privateKey, this.provider);

    try {
      let tx;

      if (params.token) {
        // ERC-20 token transfer
        const contract = new ethers.Contract(params.token, ERC20_ABI, wallet);
        const decimals = await contract.decimals();
        const amount = ethers.parseUnits(params.amount, decimals);

        tx = await contract.transfer(params.to, amount, {
          gasLimit: params.gasLimit,
          gasPrice: params.gasPrice ? ethers.parseUnits(params.gasPrice, 'gwei') : undefined
        });
      } else {
        // ETH transfer
        tx = await wallet.sendTransaction({
          to: params.to,
          value: ethers.parseEther(params.amount),
          gasLimit: params.gasLimit,
          gasPrice: params.gasPrice ? ethers.parseUnits(params.gasPrice, 'gwei') : undefined
        });
      }

      const receipt = await tx.wait();

      return {
        hash: receipt.hash,
        from: wallet.address,
        to: params.to,
        value: params.amount,
        chainType: ChainType.ETH,
        timestamp: Date.now(),
        status: receipt.status === 1 ? 'confirmed' : 'failed',
        gasUsed: receipt.gasUsed.toString(),
        fee: ethers.formatEther(receipt.gasUsed * receipt.gasPrice)
      };
    } catch (error) {
      throw new Error(`Transaction failed: ${(error as Error).message}`);
    }
  }

  /**
   * Get transaction history
   */
  async getTransactionHistory(address: string, limit: number = 10): Promise<Transaction[]> {
    // Note: This requires an indexer service like Etherscan API
    // Simplified implementation
    const transactions: Transaction[] = [];

    try {
      const currentBlock = await this.provider.getBlockNumber();
      const startBlock = Math.max(0, currentBlock - 1000);

      // In production, use Etherscan API or similar service
      // This is just a placeholder

      return transactions;
    } catch (error) {
      throw new Error(`Failed to get transaction history: ${(error as Error).message}`);
    }
  }

  /**
   * Validate Ethereum address
   */
  validateAddress(address: string): boolean {
    return ethers.isAddress(address);
  }

  /**
   * Estimate transaction fee
   */
  async estimateFee(params: SendTransactionParams): Promise<string> {
    try {
      const feeData = await this.provider.getFeeData();
      const gasPrice = feeData.gasPrice || ethers.parseUnits('20', 'gwei');

      let gasLimit: bigint;

      if (params.token) {
        // ERC-20 token transfer gas estimate
        gasLimit = BigInt(params.gasLimit || 65000);
      } else {
        // ETH transfer gas estimate
        gasLimit = BigInt(params.gasLimit || 21000);
      }

      const fee = gasPrice * gasLimit;
      return ethers.formatEther(fee);
    } catch (error) {
      throw new Error(`Failed to estimate fee: ${(error as Error).message}`);
    }
  }
}
