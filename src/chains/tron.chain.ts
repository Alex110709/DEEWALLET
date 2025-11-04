/**
 * DEEWALLET - Tron Chain Adapter
 * Supports TRX and TRC-20 tokens
 */

import TronWeb from 'tronweb';
import { BaseChainAdapter } from './base.chain';
import { ChainType, Token, Transaction, SendTransactionParams } from '../types/wallet.types';

export class TronChainAdapter extends BaseChainAdapter {
  chainType = ChainType.TRX;
  private tronWeb: typeof TronWeb;

  constructor(rpcUrl: string) {
    super(rpcUrl);
    this.tronWeb = new TronWeb({
      fullHost: rpcUrl
    });
  }

  /**
   * Generate Tron address from public key
   */
  generateAddress(publicKey: string, privateKey?: string): string {
    if (privateKey) {
      const address = this.tronWeb.address.fromPrivateKey(privateKey);
      return address;
    }

    // Generate from public key
    const pubKeyBuffer = Buffer.from(publicKey, 'hex');
    const address = this.tronWeb.address.fromHex(
      this.tronWeb.sha256(pubKeyBuffer).slice(-40)
    );

    return address;
  }

  /**
   * Get TRX balance
   */
  async getBalance(address: string): Promise<string> {
    try {
      const balance = await this.tronWeb.trx.getBalance(address);
      return this.tronWeb.fromSun(balance);
    } catch (error) {
      throw new Error(`Failed to get balance: ${(error as Error).message}`);
    }
  }

  /**
   * Get TRC-20 tokens
   */
  async getTokens(address: string): Promise<Token[]> {
    try {
      const tokens: Token[] = [];

      // Popular TRC-20 tokens
      const knownTokens = [
        { address: 'TR7NHqjeKQxGTCi8q8ZY4pL8otSzgjLj6t', symbol: 'USDT', name: 'Tether USD' },
        { address: 'TEkxiTehnzSmSe2XqrBj4w32RUN966rdz8', symbol: 'USDC', name: 'USD Coin' }
      ];

      for (const tokenInfo of knownTokens) {
        try {
          const contract = await this.tronWeb.contract().at(tokenInfo.address);
          const balance = await contract.balanceOf(address).call();
          const decimals = await contract.decimals().call();

          if (balance > 0) {
            tokens.push({
              symbol: tokenInfo.symbol,
              name: tokenInfo.name,
              decimals: Number(decimals),
              contractAddress: tokenInfo.address,
              balance: (Number(balance) / Math.pow(10, Number(decimals))).toString(),
              chainType: ChainType.TRX
            });
          }
        } catch {
          continue;
        }
      }

      return tokens;
    } catch (error) {
      throw new Error(`Failed to get tokens: ${(error as Error).message}`);
    }
  }

  /**
   * Send TRX or TRC-20 transaction
   */
  async sendTransaction(params: SendTransactionParams, privateKey: string): Promise<Transaction> {
    try {
      this.tronWeb.setPrivateKey(privateKey);
      const fromAddress = this.tronWeb.address.fromPrivateKey(privateKey);

      let tx;

      if (params.token) {
        // TRC-20 token transfer
        const contract = await this.tronWeb.contract().at(params.token);
        const decimals = await contract.decimals().call();
        const amount = Math.floor(parseFloat(params.amount) * Math.pow(10, Number(decimals)));

        tx = await contract.transfer(params.to, amount).send({
          feeLimit: params.gasLimit || 100_000_000
        });
      } else {
        // TRX transfer
        const amountSun = this.tronWeb.toSun(params.amount);

        tx = await this.tronWeb.trx.sendTransaction(params.to, amountSun, {
          feeLimit: params.gasLimit || 100_000_000
        });
      }

      return {
        hash: tx,
        from: fromAddress,
        to: params.to,
        value: params.amount,
        chainType: ChainType.TRX,
        timestamp: Date.now(),
        status: 'pending',
        fee: '0' // Will be updated when confirmed
      };
    } catch (error) {
      throw new Error(`Transaction failed: ${(error as Error).message}`);
    }
  }

  /**
   * Get transaction history
   */
  async getTransactionHistory(address: string, limit: number = 10): Promise<Transaction[]> {
    try {
      const transactions: Transaction[] = [];

      // Get TRX transactions
      const txs = await this.tronWeb.trx.getTransactionsRelated(address, 'all', limit);

      for (const tx of txs) {
        if (tx.raw_data && tx.raw_data.contract) {
          const contract = tx.raw_data.contract[0];

          if (contract.type === 'TransferContract') {
            transactions.push({
              hash: tx.txID,
              from: this.tronWeb.address.fromHex(contract.parameter.value.owner_address),
              to: this.tronWeb.address.fromHex(contract.parameter.value.to_address),
              value: this.tronWeb.fromSun(contract.parameter.value.amount),
              chainType: ChainType.TRX,
              timestamp: tx.raw_data.timestamp,
              status: tx.ret && tx.ret[0].contractRet === 'SUCCESS' ? 'confirmed' : 'failed',
              fee: this.tronWeb.fromSun(tx.ret && tx.ret[0].fee || 0)
            });
          }
        }
      }

      return transactions;
    } catch (error) {
      throw new Error(`Failed to get transaction history: ${(error as Error).message}`);
    }
  }

  /**
   * Validate Tron address
   */
  validateAddress(address: string): boolean {
    return this.tronWeb.isAddress(address);
  }

  /**
   * Estimate transaction fee
   */
  async estimateFee(params: SendTransactionParams): Promise<string> {
    try {
      // Tron fees are generally fixed
      if (params.token) {
        // TRC-20 transfer fee
        return '0'; // Uses energy/bandwidth
      } else {
        // TRX transfer fee
        return '0'; // Usually free if bandwidth available
      }
    } catch (error) {
      throw new Error(`Failed to estimate fee: ${(error as Error).message}`);
    }
  }
}
