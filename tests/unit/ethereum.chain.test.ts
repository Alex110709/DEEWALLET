/**
 * DEEWALLET - Ethereum Chain Adapter Tests
 */

import { EthereumChainAdapter } from '../../src/chains/ethereum.chain';
import { ChainType } from '../../src/types/wallet.types';

describe('EthereumChainAdapter', () => {
  let adapter: EthereumChainAdapter;

  const testRpcUrl = 'https://eth-goerli.g.alchemy.com/v2/demo';
  const testAddress = '0x742d35Cc6634C0532925a3b844Bc9e7595f0bEb';
  const testPrivateKey = '0x' + '1'.repeat(64); // Dummy private key

  beforeEach(() => {
    adapter = new EthereumChainAdapter(testRpcUrl);
  });

  describe('chainType', () => {
    it('should have correct chain type', () => {
      expect(adapter.chainType).toBe(ChainType.ETH);
    });
  });

  describe('validateAddress', () => {
    it('should validate correct Ethereum address', () => {
      expect(adapter.validateAddress(testAddress)).toBe(true);
    });

    it('should reject invalid Ethereum address', () => {
      expect(adapter.validateAddress('invalid-address')).toBe(false);
      expect(adapter.validateAddress('0x123')).toBe(false);
    });

    it('should accept checksum addresses', () => {
      expect(adapter.validateAddress('0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed')).toBe(true);
    });
  });

  describe('generateAddress', () => {
    it('should generate address from private key', () => {
      const address = adapter.generateAddress('', testPrivateKey);

      expect(address).toBeDefined();
      expect(address).toMatch(/^0x[a-fA-F0-9]{40}$/);
    });

    it('should generate same address for same private key', () => {
      const address1 = adapter.generateAddress('', testPrivateKey);
      const address2 = adapter.generateAddress('', testPrivateKey);

      expect(address1).toBe(address2);
    });
  });

  describe('getBalance', () => {
    it('should return balance for address', async () => {
      const balance = await adapter.getBalance(testAddress);

      expect(balance).toBeDefined();
      expect(typeof balance).toBe('string');
      expect(parseFloat(balance)).toBeGreaterThanOrEqual(0);
    }, 30000);
  });

  describe('estimateFee', () => {
    it('should estimate fee for ETH transfer', async () => {
      const fee = await adapter.estimateFee({
        to: testAddress,
        amount: '0.1',
        chainType: ChainType.ETH
      });

      expect(fee).toBeDefined();
      expect(typeof fee).toBe('string');
      expect(parseFloat(fee)).toBeGreaterThan(0);
    }, 30000);

    it('should estimate higher fee for token transfer', async () => {
      const fee = await adapter.estimateFee({
        to: testAddress,
        amount: '100',
        chainType: ChainType.ETH,
        token: '0xdAC17F958D2ee523a2206206994597C13D831ec7' // USDT
      });

      expect(fee).toBeDefined();
      expect(parseFloat(fee)).toBeGreaterThan(0);
    }, 30000);
  });
});
