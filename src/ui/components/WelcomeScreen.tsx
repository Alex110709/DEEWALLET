/**
 * DEEWALLET - Welcome Screen Component
 */

import React, { useState } from 'react';

interface WelcomeScreenProps {
  onWalletCreated: () => void;
}

const WelcomeScreen: React.FC<WelcomeScreenProps> = ({ onWalletCreated }) => {
  const [view, setView] = useState<'welcome' | 'create' | 'import'>('welcome');
  const [mnemonic, setMnemonic] = useState<string>('');
  const [password, setPassword] = useState<string>('');
  const [confirmPassword, setConfirmPassword] = useState<string>('');
  const [error, setError] = useState<string>('');

  const handleCreateWallet = async () => {
    try {
      const result = await (window as any).electron.invoke('wallet:generate');
      if (result.success) {
        setMnemonic(result.mnemonic);
        setView('create');
      } else {
        setError(result.error);
      }
    } catch (err) {
      setError((err as Error).message);
    }
  };

  const handleSaveWallet = async () => {
    if (password !== confirmPassword) {
      setError('Passwords do not match');
      return;
    }

    if (password.length < 12) {
      setError('Password must be at least 12 characters');
      return;
    }

    try {
      const result = await (window as any).electron.invoke('wallet:export', password);
      if (result.success) {
        onWalletCreated();
      } else {
        setError(result.error);
      }
    } catch (err) {
      setError((err as Error).message);
    }
  };

  const handleImportWallet = async () => {
    try {
      const result = await (window as any).electron.invoke('wallet:import', password);
      if (result.success) {
        onWalletCreated();
      } else {
        setError(result.error);
      }
    } catch (err) {
      setError((err as Error).message);
    }
  };

  if (view === 'welcome') {
    return (
      <div className="welcome-screen">
        <div className="welcome-container">
          <h1>Welcome to DEEWALLET</h1>
          <p>Secure Multi-Chain Cryptocurrency Wallet</p>

          <div className="welcome-buttons">
            <button onClick={handleCreateWallet} className="btn-primary">
              Create New Wallet
            </button>
            <button onClick={() => setView('import')} className="btn-secondary">
              Import Wallet
            </button>
          </div>

          {error && <div className="error-message">{error}</div>}
        </div>
      </div>
    );
  }

  if (view === 'create') {
    return (
      <div className="welcome-screen">
        <div className="welcome-container">
          <h2>Your Recovery Phrase</h2>
          <p className="warning">
            ⚠️ Write down these 24 words in order and store them safely.
            Never share them with anyone!
          </p>

          <div className="mnemonic-grid">
            {mnemonic.split(' ').map((word, index) => (
              <div key={index} className="mnemonic-word">
                <span className="word-number">{index + 1}.</span>
                <span className="word-text">{word}</span>
              </div>
            ))}
          </div>

          <div className="password-section">
            <h3>Set Wallet Password</h3>
            <input
              type="password"
              placeholder="Password (min 12 characters)"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
              className="input-field"
            />
            <input
              type="password"
              placeholder="Confirm Password"
              value={confirmPassword}
              onChange={(e) => setConfirmPassword(e.target.value)}
              className="input-field"
            />
          </div>

          <button onClick={handleSaveWallet} className="btn-primary">
            Save Wallet
          </button>

          {error && <div className="error-message">{error}</div>}
        </div>
      </div>
    );
  }

  if (view === 'import') {
    return (
      <div className="welcome-screen">
        <div className="welcome-container">
          <h2>Import Wallet</h2>
          <p>Select your encrypted keyfile and enter the password</p>

          <input
            type="password"
            placeholder="Wallet Password"
            value={password}
            onChange={(e) => setPassword(e.target.value)}
            className="input-field"
          />

          <button onClick={handleImportWallet} className="btn-primary">
            Import from Keyfile
          </button>

          <button onClick={() => setView('welcome')} className="btn-secondary">
            Back
          </button>

          {error && <div className="error-message">{error}</div>}
        </div>
      </div>
    );
  }

  return null;
};

export default WelcomeScreen;
