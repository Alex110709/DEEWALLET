/**
 * DEEWALLET - WalletCore Unit Tests
 */

import { WalletCore } from '../../src/core/wallet.core';
import { ChainType, NetworkType } from '../../src/types/wallet.types';

describe('WalletCore', () => {
  let walletCore: WalletCore;

  const testConfig = {
    network: NetworkType.TESTNET,
    rpcEndpoints: {
      BTC: 'https://blockstream.info/testnet/api',
      ETH: 'https://eth-goerli.g.alchemy.com/v2/demo',
      SOL: 'https://api.devnet.solana.com',
      TRX: 'https://api.shasta.trongrid.io'
    },
    derivationPaths: {}
  };

  beforeEach(() => {
    walletCore = new WalletCore(testConfig);
  });

  afterEach(() => {
    walletCore.clear();
  });

  describe('generateWallet', () => {
    it('should generate a valid 24-word mnemonic', async () => {
      const mnemonic = await walletCore.generateWallet(256);

      expect(mnemonic).toBeDefined();
      expect(typeof mnemonic).toBe('string');
      expect(mnemonic.split(' ').length).toBe(24);
    });

    it('should generate a valid 12-word mnemonic', async () => {
      const mnemonic = await walletCore.generateWallet(128);

      expect(mnemonic).toBeDefined();
      expect(mnemonic.split(' ').length).toBe(12);
    });

    it('should generate different mnemonics each time', async () => {
      const mnemonic1 = await walletCore.generateWallet();
      const mnemonic2 = await walletCore.generateWallet();

      expect(mnemonic1).not.toBe(mnemonic2);
    });
  });

  describe('restoreFromMnemonic', () => {
    it('should restore wallet from valid mnemonic', async () => {
      const mnemonic = await walletCore.generateWallet();
      const newWalletCore = new WalletCore(testConfig);

      const result = await newWalletCore.restoreFromMnemonic(mnemonic);

      expect(result).toBe(true);
      newWalletCore.clear();
    });

    it('should reject invalid mnemonic', async () => {
      await expect(
        walletCore.restoreFromMnemonic('invalid mnemonic phrase test')
      ).rejects.toThrow('Invalid mnemonic phrase');
    });
  });

  describe('deriveAccount', () => {
    beforeEach(async () => {
      await walletCore.generateWallet();
    });

    it('should derive BTC account', () => {
      const account = walletCore.deriveAccount(ChainType.BTC, 0);

      expect(account).toBeDefined();
      expect(account.chainType).toBe(ChainType.BTC);
      expect(account.address).toBeDefined();
      expect(account.publicKey).toBeDefined();
      expect(account.privateKey).toBeDefined();
      expect(account.derivationPath).toContain("m/44'/0'");
    });

    it('should derive ETH account', () => {
      const account = walletCore.deriveAccount(ChainType.ETH, 0);

      expect(account).toBeDefined();
      expect(account.chainType).toBe(ChainType.ETH);
      expect(account.derivationPath).toContain("m/44'/60'");
    });

    it('should derive different accounts for different indices', () => {
      const account1 = walletCore.deriveAccount(ChainType.ETH, 0);
      const account2 = walletCore.deriveAccount(ChainType.ETH, 1);

      expect(account1.address).not.toBe(account2.address);
      expect(account1.privateKey).not.toBe(account2.privateKey);
    });

    it('should throw error when wallet not initialized', () => {
      const uninitializedWallet = new WalletCore(testConfig);

      expect(() => uninitializedWallet.deriveAccount(ChainType.BTC, 0))
        .toThrow('Wallet not initialized');
    });
  });

  describe('exportToKeyfile', () => {
    beforeEach(async () => {
      await walletCore.generateWallet();
    });

    it('should export wallet to encrypted keyfile', async () => {
      const keyfile = await walletCore.exportToKeyfile('test-password-123');

      expect(keyfile).toBeDefined();
      expect(keyfile.version).toBe(1);
      expect(keyfile.crypto.cipher).toBe('aes-256-cbc');
      expect(keyfile.crypto.kdf).toBe('pbkdf2');
      expect(keyfile.crypto.kdfparams.iterations).toBe(100000);
      expect(keyfile.crypto.ciphertext).toBeDefined();
    });

    it('should create different keyfiles with different passwords', async () => {
      const keyfile1 = await walletCore.exportToKeyfile('password1');
      const keyfile2 = await walletCore.exportToKeyfile('password2');

      expect(keyfile1.crypto.ciphertext).not.toBe(keyfile2.crypto.ciphertext);
      expect(keyfile1.crypto.cipherparams.iv).not.toBe(keyfile2.crypto.cipherparams.iv);
    });
  });

  describe('importFromKeyfile', () => {
    let originalMnemonic: string;
    let keyfile: any;

    beforeEach(async () => {
      originalMnemonic = await walletCore.generateWallet();
      keyfile = await walletCore.exportToKeyfile('test-password-123');
    });

    it('should import wallet from keyfile with correct password', async () => {
      const newWalletCore = new WalletCore(testConfig);
      const result = await newWalletCore.importFromKeyfile(keyfile, 'test-password-123');

      expect(result).toBe(true);
      expect(newWalletCore.getMnemonic()).toBe(originalMnemonic);

      newWalletCore.clear();
    });

    it('should reject keyfile with wrong password', async () => {
      const newWalletCore = new WalletCore(testConfig);

      await expect(
        newWalletCore.importFromKeyfile(keyfile, 'wrong-password')
      ).rejects.toThrow('Failed to decrypt keyfile');

      newWalletCore.clear();
    });
  });

  describe('getMnemonic', () => {
    it('should return null when wallet not initialized', () => {
      expect(walletCore.getMnemonic()).toBeNull();
    });

    it('should return mnemonic after wallet generation', async () => {
      const mnemonic = await walletCore.generateWallet();

      expect(walletCore.getMnemonic()).toBe(mnemonic);
    });
  });

  describe('clear', () => {
    it('should clear sensitive data from memory', async () => {
      await walletCore.generateWallet();

      expect(walletCore.getMnemonic()).not.toBeNull();

      walletCore.clear();

      expect(walletCore.getMnemonic()).toBeNull();
    });
  });
});
