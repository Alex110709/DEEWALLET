/**
 * DEEWALLET - Solana Chain Adapter
 * Supports SOL and SPL tokens
 */

import {
  Connection,
  PublicKey,
  Keypair,
  Transaction,
  SystemProgram,
  sendAndConfirmTransaction,
  LAMPORTS_PER_SOL
} from '@solana/web3.js';
import {
  getAssociatedTokenAddress,
  createTransferInstruction,
  getAccount,
  TOKEN_PROGRAM_ID
} from '@solana/spl-token';
import { BaseChainAdapter } from './base.chain';
import { ChainType, Token, Transaction as WalletTransaction, SendTransactionParams } from '../types/wallet.types';

export class SolanaChainAdapter extends BaseChainAdapter {
  chainType = ChainType.SOL;
  private connection: Connection;

  constructor(rpcUrl: string) {
    super(rpcUrl);
    this.connection = new Connection(rpcUrl, 'confirmed');
  }

  /**
   * Generate Solana address from public key
   */
  generateAddress(publicKey: string, privateKey?: string): string {
    if (privateKey) {
      const keypair = Keypair.fromSecretKey(Buffer.from(privateKey, 'hex'));
      return keypair.publicKey.toBase58();
    }

    const pubKeyBuffer = Buffer.from(publicKey, 'hex');
    const pubKey = new PublicKey(pubKeyBuffer);
    return pubKey.toBase58();
  }

  /**
   * Get SOL balance
   */
  async getBalance(address: string): Promise<string> {
    try {
      const publicKey = new PublicKey(address);
      const balance = await this.connection.getBalance(publicKey);
      return (balance / LAMPORTS_PER_SOL).toString();
    } catch (error) {
      throw new Error(`Failed to get balance: ${(error as Error).message}`);
    }
  }

  /**
   * Get SPL tokens
   */
  async getTokens(address: string): Promise<Token[]> {
    try {
      const publicKey = new PublicKey(address);
      const tokenAccounts = await this.connection.getParsedTokenAccountsByOwner(
        publicKey,
        { programId: TOKEN_PROGRAM_ID }
      );

      const tokens: Token[] = [];

      for (const { account } of tokenAccounts.value) {
        const parsedInfo = account.data.parsed.info;
        const mintAddress = parsedInfo.mint;
        const balance = parsedInfo.tokenAmount.uiAmount;

        if (balance > 0) {
          // Fetch token metadata (simplified - in production use Metaplex)
          try {
            const mintPubkey = new PublicKey(mintAddress);
            const tokenInfo = await this.connection.getParsedAccountInfo(mintPubkey);

            tokens.push({
              symbol: mintAddress.slice(0, 8), // Placeholder
              name: `SPL Token ${mintAddress.slice(0, 8)}`,
              decimals: parsedInfo.tokenAmount.decimals,
              contractAddress: mintAddress,
              balance: balance.toString(),
              chainType: ChainType.SOL
            });
          } catch {
            continue;
          }
        }
      }

      return tokens;
    } catch (error) {
      throw new Error(`Failed to get tokens: ${(error as Error).message}`);
    }
  }

  /**
   * Send SOL or SPL token transaction
   */
  async sendTransaction(params: SendTransactionParams, privateKey: string): Promise<WalletTransaction> {
    try {
      const keypair = Keypair.fromSecretKey(Buffer.from(privateKey, 'hex'));
      const toPubkey = new PublicKey(params.to);

      let transaction: Transaction;
      let signature: string;

      if (params.token) {
        // SPL Token transfer
        const mintPubkey = new PublicKey(params.token);

        const fromTokenAccount = await getAssociatedTokenAddress(
          mintPubkey,
          keypair.publicKey
        );

        const toTokenAccount = await getAssociatedTokenAddress(
          mintPubkey,
          toPubkey
        );

        // Get token decimals
        const tokenAccountInfo = await getAccount(this.connection, fromTokenAccount);
        const mintInfo = await this.connection.getParsedAccountInfo(mintPubkey);
        const decimals = (mintInfo.value?.data as any).parsed.info.decimals;

        const amount = BigInt(Math.floor(parseFloat(params.amount) * Math.pow(10, decimals)));

        transaction = new Transaction().add(
          createTransferInstruction(
            fromTokenAccount,
            toTokenAccount,
            keypair.publicKey,
            amount,
            [],
            TOKEN_PROGRAM_ID
          )
        );

        signature = await sendAndConfirmTransaction(this.connection, transaction, [keypair]);
      } else {
        // SOL transfer
        transaction = new Transaction().add(
          SystemProgram.transfer({
            fromPubkey: keypair.publicKey,
            toPubkey,
            lamports: Math.floor(parseFloat(params.amount) * LAMPORTS_PER_SOL)
          })
        );

        signature = await sendAndConfirmTransaction(this.connection, transaction, [keypair]);
      }

      return {
        hash: signature,
        from: keypair.publicKey.toBase58(),
        to: params.to,
        value: params.amount,
        chainType: ChainType.SOL,
        timestamp: Date.now(),
        status: 'confirmed',
        fee: '0.000005' // Typical Solana fee
      };
    } catch (error) {
      throw new Error(`Transaction failed: ${(error as Error).message}`);
    }
  }

  /**
   * Get transaction history
   */
  async getTransactionHistory(address: string, limit: number = 10): Promise<WalletTransaction[]> {
    try {
      const publicKey = new PublicKey(address);
      const signatures = await this.connection.getSignaturesForAddress(publicKey, { limit });

      const transactions: WalletTransaction[] = [];

      for (const sigInfo of signatures) {
        const tx = await this.connection.getParsedTransaction(sigInfo.signature);

        if (tx && tx.meta && tx.transaction) {
          const instruction = tx.transaction.message.instructions[0];

          transactions.push({
            hash: sigInfo.signature,
            from: address,
            to: 'unknown', // Would need to parse instructions
            value: '0',
            chainType: ChainType.SOL,
            timestamp: (sigInfo.blockTime || 0) * 1000,
            status: tx.meta.err ? 'failed' : 'confirmed',
            fee: ((tx.meta.fee || 0) / LAMPORTS_PER_SOL).toString()
          });
        }
      }

      return transactions;
    } catch (error) {
      throw new Error(`Failed to get transaction history: ${(error as Error).message}`);
    }
  }

  /**
   * Validate Solana address
   */
  validateAddress(address: string): boolean {
    try {
      new PublicKey(address);
      return true;
    } catch {
      return false;
    }
  }

  /**
   * Estimate transaction fee
   */
  async estimateFee(params: SendTransactionParams): Promise<string> {
    try {
      // Solana has fixed fees
      const fee = 0.000005; // 5000 lamports typical fee
      return fee.toString();
    } catch (error) {
      throw new Error(`Failed to estimate fee: ${(error as Error).message}`);
    }
  }
}
