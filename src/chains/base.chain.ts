/**
 * DEEWALLET - Base Chain Interface
 */

import { ChainType, WalletAccount, Token, Transaction, SendTransactionParams } from '../types/wallet.types';

export interface IChainAdapter {
  chainType: ChainType;

  /**
   * Generate address from public key
   */
  generateAddress(publicKey: string, privateKey?: string): string;

  /**
   * Get balance for address
   */
  getBalance(address: string): Promise<string>;

  /**
   * Get tokens for address (ERC-20, SPL, etc.)
   */
  getTokens(address: string): Promise<Token[]>;

  /**
   * Send transaction
   */
  sendTransaction(params: SendTransactionParams, privateKey: string): Promise<Transaction>;

  /**
   * Get transaction history
   */
  getTransactionHistory(address: string, limit?: number): Promise<Transaction[]>;

  /**
   * Validate address format
   */
  validateAddress(address: string): boolean;

  /**
   * Estimate transaction fee
   */
  estimateFee(params: SendTransactionParams): Promise<string>;
}

export abstract class BaseChainAdapter implements IChainAdapter {
  abstract chainType: ChainType;
  protected rpcUrl: string;

  constructor(rpcUrl: string) {
    this.rpcUrl = rpcUrl;
  }

  abstract generateAddress(publicKey: string, privateKey?: string): string;
  abstract getBalance(address: string): Promise<string>;
  abstract getTokens(address: string): Promise<Token[]>;
  abstract sendTransaction(params: SendTransactionParams, privateKey: string): Promise<Transaction>;
  abstract getTransactionHistory(address: string, limit?: number): Promise<Transaction[]>;
  abstract validateAddress(address: string): boolean;
  abstract estimateFee(params: SendTransactionParams): Promise<string>;
}
