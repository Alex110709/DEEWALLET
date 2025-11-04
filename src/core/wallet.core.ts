/**
 * DEEWALLET - Core Wallet Management System
 * HD Wallet implementation with BIP39/BIP44 support
 */

import * as bip39 from 'bip39';
import { BIP32Factory } from 'bip32';
import * as ecc from 'tiny-secp256k1';
import CryptoJS from 'crypto-js';
import { randomBytes } from 'crypto';
import { ChainType, KeyfileData, WalletAccount, WalletConfig } from '../types/wallet.types';

const bip32 = BIP32Factory(ecc);

export class WalletCore {
  private mnemonic: string | null = null;
  private seed: Buffer | null = null;
  private config: WalletConfig;

  constructor(config: WalletConfig) {
    this.config = config;
  }

  /**
   * Generate new HD wallet with mnemonic phrase
   */
  async generateWallet(strength: 128 | 256 = 256): Promise<string> {
    this.mnemonic = bip39.generateMnemonic(strength);
    this.seed = await bip39.mnemonicToSeed(this.mnemonic);
    return this.mnemonic;
  }

  /**
   * Restore wallet from mnemonic phrase
   */
  async restoreFromMnemonic(mnemonic: string): Promise<boolean> {
    if (!bip39.validateMnemonic(mnemonic)) {
      throw new Error('Invalid mnemonic phrase');
    }
    this.mnemonic = mnemonic;
    this.seed = await bip39.mnemonicToSeed(mnemonic);
    return true;
  }

  /**
   * Derive account for specific chain
   */
  deriveAccount(chainType: ChainType, accountIndex: number = 0): WalletAccount {
    if (!this.seed) {
      throw new Error('Wallet not initialized');
    }

    const derivationPath = this.getDerivationPath(chainType, accountIndex);
    const root = bip32.fromSeed(this.seed);
    const child = root.derivePath(derivationPath);

    if (!child.privateKey) {
      throw new Error('Failed to derive private key');
    }

    return {
      address: this.getAddress(chainType, child),
      publicKey: child.publicKey.toString('hex'),
      privateKey: child.privateKey.toString('hex'),
      derivationPath,
      chainType,
      balance: '0',
      index: accountIndex
    };
  }

  /**
   * Get derivation path for chain
   */
  private getDerivationPath(chainType: ChainType, accountIndex: number): string {
    const basePaths = {
      [ChainType.BTC]: `m/44'/0'/0'/0/${accountIndex}`,
      [ChainType.ETH]: `m/44'/60'/0'/0/${accountIndex}`,
      [ChainType.TRX]: `m/44'/195'/0'/0/${accountIndex}`,
      [ChainType.SOL]: `m/44'/501'/0'/0/${accountIndex}`
    };
    return this.config.derivationPaths[chainType] || basePaths[chainType];
  }

  /**
   * Get address from derived key (implementation depends on chain)
   */
  private getAddress(chainType: ChainType, node: any): string {
    // This will be implemented by chain-specific modules
    // Placeholder for now
    return node.publicKey.toString('hex');
  }

  /**
   * Export wallet to encrypted keyfile
   */
  async exportToKeyfile(password: string): Promise<KeyfileData> {
    if (!this.mnemonic) {
      throw new Error('No wallet to export');
    }

    const salt = randomBytes(32).toString('hex');
    const iv = randomBytes(16).toString('hex');

    // Derive key from password using PBKDF2
    const key = CryptoJS.PBKDF2(password, salt, {
      keySize: 256 / 32,
      iterations: 100000
    });

    // Encrypt mnemonic
    const encrypted = CryptoJS.AES.encrypt(this.mnemonic, key.toString(), {
      iv: CryptoJS.enc.Hex.parse(iv)
    });

    const keyfileData: KeyfileData = {
      version: 1,
      id: randomBytes(16).toString('hex'),
      crypto: {
        cipher: 'aes-256-cbc',
        ciphertext: encrypted.ciphertext.toString(CryptoJS.enc.Hex),
        cipherparams: {
          iv: iv
        },
        kdf: 'pbkdf2',
        kdfparams: {
          salt: salt,
          iterations: 100000
        }
      },
      accounts: {},
      createdAt: Date.now(),
      updatedAt: Date.now()
    };

    return keyfileData;
  }

  /**
   * Import wallet from encrypted keyfile
   */
  async importFromKeyfile(keyfileData: KeyfileData, password: string): Promise<boolean> {
    try {
      // Derive key from password
      const key = CryptoJS.PBKDF2(
        password,
        keyfileData.crypto.kdfparams.salt,
        {
          keySize: 256 / 32,
          iterations: keyfileData.crypto.kdfparams.iterations
        }
      );

      // Decrypt mnemonic
      const decrypted = CryptoJS.AES.decrypt(
        { ciphertext: CryptoJS.enc.Hex.parse(keyfileData.crypto.ciphertext) } as any,
        key.toString(),
        {
          iv: CryptoJS.enc.Hex.parse(keyfileData.crypto.cipherparams.iv)
        }
      );

      const mnemonic = decrypted.toString(CryptoJS.enc.Utf8);

      if (!mnemonic || !bip39.validateMnemonic(mnemonic)) {
        throw new Error('Invalid password or corrupted keyfile');
      }

      return await this.restoreFromMnemonic(mnemonic);
    } catch (error) {
      throw new Error('Failed to decrypt keyfile: ' + (error as Error).message);
    }
  }

  /**
   * Get current mnemonic (for backup purposes)
   */
  getMnemonic(): string | null {
    return this.mnemonic;
  }

  /**
   * Clear sensitive data from memory
   */
  clear(): void {
    this.mnemonic = null;
    this.seed = null;
  }
}
