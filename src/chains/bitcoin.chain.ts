/**
 * DEEWALLET - Bitcoin Chain Adapter
 */

import * as bitcoin from 'bitcoinjs-lib';
import axios from 'axios';
import { BaseChainAdapter } from './base.chain';
import { ChainType, Token, Transaction, SendTransactionParams } from '../types/wallet.types';

export class BitcoinChainAdapter extends BaseChainAdapter {
  chainType = ChainType.BTC;
  private network: bitcoin.Network;
  private apiUrl: string;

  constructor(rpcUrl: string, isTestnet: boolean = false) {
    super(rpcUrl);
    this.network = isTestnet ? bitcoin.networks.testnet : bitcoin.networks.bitcoin;
    this.apiUrl = isTestnet
      ? 'https://blockstream.info/testnet/api'
      : 'https://blockstream.info/api';
  }

  /**
   * Generate Bitcoin address from public key
   */
  generateAddress(publicKey: string, privateKey?: string): string {
    const pubKeyBuffer = Buffer.from(publicKey, 'hex');

    // Generate P2WPKH (Native SegWit) address
    const { address } = bitcoin.payments.p2wpkh({
      pubkey: pubKeyBuffer,
      network: this.network
    });

    if (!address) {
      throw new Error('Failed to generate Bitcoin address');
    }

    return address;
  }

  /**
   * Get BTC balance
   */
  async getBalance(address: string): Promise<string> {
    try {
      const response = await axios.get(`${this.apiUrl}/address/${address}`);
      const satoshis = response.data.chain_stats.funded_txo_sum - response.data.chain_stats.spent_txo_sum;
      return (satoshis / 100000000).toString(); // Convert satoshis to BTC
    } catch (error) {
      throw new Error(`Failed to get balance: ${(error as Error).message}`);
    }
  }

  /**
   * Get tokens (BTC doesn't have native tokens)
   */
  async getTokens(address: string): Promise<Token[]> {
    return []; // BTC doesn't support tokens natively
  }

  /**
   * Send BTC transaction
   */
  async sendTransaction(params: SendTransactionParams, privateKey: string): Promise<Transaction> {
    try {
      const keyPair = bitcoin.ECPair.fromPrivateKey(
        Buffer.from(privateKey, 'hex'),
        { network: this.network }
      );

      const { address: fromAddress } = bitcoin.payments.p2wpkh({
        pubkey: keyPair.publicKey,
        network: this.network
      });

      if (!fromAddress) {
        throw new Error('Failed to derive sender address');
      }

      // Get UTXOs
      const utxosResponse = await axios.get(`${this.apiUrl}/address/${fromAddress}/utxo`);
      const utxos = utxosResponse.data;

      if (utxos.length === 0) {
        throw new Error('No UTXOs available');
      }

      const psbt = new bitcoin.Psbt({ network: this.network });
      const amountSatoshis = Math.floor(parseFloat(params.amount) * 100000000);
      const feeRate = parseInt(params.gasPrice || '10'); // sat/vB

      let inputAmount = 0;

      // Add inputs
      for (const utxo of utxos) {
        const txHex = await this.getTxHex(utxo.txid);

        psbt.addInput({
          hash: utxo.txid,
          index: utxo.vout,
          witnessUtxo: {
            script: Buffer.from(utxo.scriptpubkey, 'hex'),
            value: utxo.value
          }
        });

        inputAmount += utxo.value;

        // Rough fee estimation: 2 outputs + inputs
        const estimatedSize = utxos.length * 68 + 2 * 31 + 10;
        const estimatedFee = estimatedSize * feeRate;

        if (inputAmount >= amountSatoshis + estimatedFee) {
          break;
        }
      }

      // Add recipient output
      psbt.addOutput({
        address: params.to,
        value: amountSatoshis
      });

      // Calculate actual fee and change
      const estimatedSize = psbt.txInputs.length * 68 + 2 * 31 + 10;
      const fee = estimatedSize * feeRate;
      const change = inputAmount - amountSatoshis - fee;

      // Add change output if necessary
      if (change > 546) { // Dust limit
        psbt.addOutput({
          address: fromAddress,
          value: change
        });
      }

      // Sign all inputs
      psbt.signAllInputs(keyPair);
      psbt.finalizeAllInputs();

      // Broadcast transaction
      const txHex = psbt.extractTransaction().toHex();
      const broadcastResponse = await axios.post(`${this.apiUrl}/tx`, txHex, {
        headers: { 'Content-Type': 'text/plain' }
      });

      return {
        hash: broadcastResponse.data,
        from: fromAddress,
        to: params.to,
        value: params.amount,
        chainType: ChainType.BTC,
        timestamp: Date.now(),
        status: 'pending',
        fee: (fee / 100000000).toString()
      };
    } catch (error) {
      throw new Error(`Transaction failed: ${(error as Error).message}`);
    }
  }

  /**
   * Get transaction hex
   */
  private async getTxHex(txid: string): Promise<string> {
    const response = await axios.get(`${this.apiUrl}/tx/${txid}/hex`);
    return response.data;
  }

  /**
   * Get transaction history
   */
  async getTransactionHistory(address: string, limit: number = 10): Promise<Transaction[]> {
    try {
      const response = await axios.get(`${this.apiUrl}/address/${address}/txs`);
      const txs = response.data.slice(0, limit);

      return txs.map((tx: any) => ({
        hash: tx.txid,
        from: tx.vin[0]?.prevout?.scriptpubkey_address || 'unknown',
        to: tx.vout[0]?.scriptpubkey_address || 'unknown',
        value: (tx.vout[0]?.value / 100000000).toString(),
        chainType: ChainType.BTC,
        timestamp: tx.status.block_time * 1000,
        status: tx.status.confirmed ? 'confirmed' : 'pending',
        fee: (tx.fee / 100000000).toString()
      }));
    } catch (error) {
      throw new Error(`Failed to get transaction history: ${(error as Error).message}`);
    }
  }

  /**
   * Validate Bitcoin address
   */
  validateAddress(address: string): boolean {
    try {
      bitcoin.address.toOutputScript(address, this.network);
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
      // Get recommended fee rates
      const response = await axios.get(`${this.apiUrl}/fee-estimates`);
      const feeRate = response.data['6'] || 10; // 6 block target or default 10 sat/vB

      // Estimate transaction size (1 input, 2 outputs)
      const estimatedSize = 1 * 68 + 2 * 31 + 10;
      const fee = estimatedSize * feeRate;

      return (fee / 100000000).toString(); // Convert to BTC
    } catch (error) {
      throw new Error(`Failed to estimate fee: ${(error as Error).message}`);
    }
  }
}
