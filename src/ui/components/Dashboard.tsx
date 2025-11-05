/**
 * DEEWALLET - Main Dashboard Component
 */

import React, { useState, useEffect } from 'react';
import { ChainType } from '../../types/wallet.types';
import { useWallet } from '../contexts/WalletContext';

const Dashboard: React.FC = () => {
  const { accounts, addAccount, updateBalance } = useWallet();
  const [selectedChain, setSelectedChain] = useState<ChainType>(ChainType.BTC);
  const [loading, setLoading] = useState(false);

  const chains = [
    { type: ChainType.BTC, name: 'Bitcoin', icon: '₿' },
    { type: ChainType.ETH, name: 'Ethereum', icon: 'Ξ' },
    { type: ChainType.SOL, name: 'Solana', icon: '◎' },
    { type: ChainType.TRX, name: 'Tron', icon: 'T' }
  ];

  const handleAddAccount = async (chainType: ChainType) => {
    setLoading(true);
    try {
      const chainAccounts = accounts.get(chainType) || [];
      const index = chainAccounts.length;

      const result = await (window as any).electron.invoke(
        'wallet:derive-account',
        chainType,
        index
      );

      if (result.success) {
        addAccount(chainType, result.account);

        // Fetch balance
        const balanceResult = await (window as any).electron.invoke(
          'chain:get-balance',
          chainType,
          result.account.address
        );

        if (balanceResult.success) {
          updateBalance(chainType, index, balanceResult.balance);
        }
      }
    } catch (error) {
      console.error('Failed to add account:', error);
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    // Auto-create first account for each chain on mount
    const initializeAccounts = async () => {
      for (const chain of chains) {
        if (!accounts.has(chain.type)) {
          await handleAddAccount(chain.type);
        }
      }
    };

    initializeAccounts();
  }, []); // eslint-disable-line react-hooks/exhaustive-deps

  const selectedAccounts = accounts.get(selectedChain) || [];

  return (
    <div className="dashboard">
      <header className="dashboard-header">
        <h1>DEEWALLET</h1>
        <div className="header-actions">
          <button className="btn-icon" title="Settings">⚙️</button>
        </div>
      </header>

      <div className="dashboard-content">
        <aside className="sidebar">
          <h3>Blockchains</h3>
          <ul className="chain-list">
            {chains.map(chain => (
              <li
                key={chain.type}
                className={selectedChain === chain.type ? 'active' : ''}
                onClick={() => setSelectedChain(chain.type)}
              >
                <span className="chain-icon">{chain.icon}</span>
                <span className="chain-name">{chain.name}</span>
              </li>
            ))}
          </ul>
        </aside>

        <main className="main-content">
          <div className="content-header">
            <h2>{chains.find(c => c.type === selectedChain)?.name}</h2>
            <button
              onClick={() => handleAddAccount(selectedChain)}
              disabled={loading}
              className="btn-primary"
            >
              + Add Account
            </button>
          </div>

          <div className="accounts-list">
            {selectedAccounts.length === 0 ? (
              <div className="empty-state">
                <p>No accounts yet</p>
                <button onClick={() => handleAddAccount(selectedChain)}>
                  Create First Account
                </button>
              </div>
            ) : (
              selectedAccounts.map((account, index) => (
                <div key={index} className="account-card">
                  <div className="account-header">
                    <h4>Account {index + 1}</h4>
                    <span className="account-balance">
                      {account.balance} {selectedChain}
                    </span>
                  </div>
                  <div className="account-address">
                    <code>{account.address}</code>
                    <button
                      className="btn-copy"
                      onClick={() => navigator.clipboard.writeText(account.address)}
                      title="Copy address"
                    >
                      📋
                    </button>
                  </div>
                  <div className="account-actions">
                    <button className="btn-action">Send</button>
                    <button className="btn-action">Receive</button>
                    <button className="btn-action">History</button>
                  </div>
                </div>
              ))
            )}
          </div>
        </main>
      </div>
    </div>
  );
};

export default Dashboard;
