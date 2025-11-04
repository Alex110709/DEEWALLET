/**
 * DEEWALLET - Multi-chain Wallet Type Definitions
 */

export enum ChainType {
  BTC = 'BTC',
  ETH = 'ETH',
  TRX = 'TRX',
  SOL = 'SOL'
}

export enum NetworkType {
  MAINNET = 'mainnet',
  TESTNET = 'testnet'
}

export interface WalletAccount {
  address: string;
  publicKey: string;
  privateKey?: string;
  derivationPath: string;
  chainType: ChainType;
  balance: string;
  index: number;
}

export interface Token {
  symbol: string;
  name: string;
  decimals: number;
  contractAddress: string;
  balance: string;
  logo?: string;
  chainType: ChainType;
}

export interface KeyfileData {
  version: number;
  id: string;
  crypto: {
    cipher: string;
    ciphertext: string;
    cipherparams: {
      iv: string;
    };
    kdf: string;
    kdfparams: {
      salt: string;
      iterations: number;
    };
  };
  accounts: {
    [chainType: string]: {
      address: string;
      derivationPath: string;
      index: number;
    }[];
  };
  createdAt: number;
  updatedAt: number;
}

export interface WalletConfig {
  network: NetworkType;
  rpcEndpoints: {
    [chainType: string]: string;
  };
  derivationPaths: {
    [chainType: string]: string;
  };
}

export interface Transaction {
  hash: string;
  from: string;
  to: string;
  value: string;
  chainType: ChainType;
  timestamp: number;
  status: 'pending' | 'confirmed' | 'failed';
  gasUsed?: string;
  fee?: string;
}

export interface SendTransactionParams {
  to: string;
  amount: string;
  chainType: ChainType;
  token?: string; // token contract address for ERC-20/SPL tokens
  gasLimit?: string;
  gasPrice?: string;
}
