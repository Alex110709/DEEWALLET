/**
 * DEEWALLET - Wallet Context Provider
 */

import React, { createContext, useContext, useState, useCallback } from 'react';
import { ChainType } from '../../types/wallet.types';

interface WalletAccount {
  address: string;
  publicKey: string;
  derivationPath: string;
  chainType: ChainType;
  balance: string;
  index: number;
}

interface WalletContextType {
  accounts: Map<ChainType, WalletAccount[]>;
  addAccount: (chainType: ChainType, account: WalletAccount) => void;
  updateBalance: (chainType: ChainType, index: number, balance: string) => void;
  clearAccounts: () => void;
}

const WalletContext = createContext<WalletContextType | undefined>(undefined);

export const WalletProvider: React.FC<{ children: React.ReactNode }> = ({ children }) => {
  const [accounts, setAccounts] = useState<Map<ChainType, WalletAccount[]>>(new Map());

  const addAccount = useCallback((chainType: ChainType, account: WalletAccount) => {
    setAccounts(prev => {
      const newAccounts = new Map(prev);
      const chainAccounts = newAccounts.get(chainType) || [];
      chainAccounts.push(account);
      newAccounts.set(chainType, chainAccounts);
      return newAccounts;
    });
  }, []);

  const updateBalance = useCallback((chainType: ChainType, index: number, balance: string) => {
    setAccounts(prev => {
      const newAccounts = new Map(prev);
      const chainAccounts = newAccounts.get(chainType) || [];
      if (chainAccounts[index]) {
        chainAccounts[index].balance = balance;
        newAccounts.set(chainType, chainAccounts);
      }
      return newAccounts;
    });
  }, []);

  const clearAccounts = useCallback(() => {
    setAccounts(new Map());
  }, []);

  return (
    <WalletContext.Provider value={{ accounts, addAccount, updateBalance, clearAccounts }}>
      {children}
    </WalletContext.Provider>
  );
};

export const useWallet = (): WalletContextType => {
  const context = useContext(WalletContext);
  if (!context) {
    throw new Error('useWallet must be used within WalletProvider');
  }
  return context;
};
