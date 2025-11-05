/**
 * DEEWALLET - Main Application Component
 */

import React, { useState } from 'react';
import { WalletProvider } from './contexts/WalletContext';
import WelcomeScreen from './components/WelcomeScreen';
import Dashboard from './components/Dashboard';

const App: React.FC = () => {
  const [hasWallet, setHasWallet] = useState(false);

  return (
    <WalletProvider>
      <div className="app">
        {hasWallet ? (
          <Dashboard />
        ) : (
          <WelcomeScreen onWalletCreated={() => setHasWallet(true)} />
        )}
      </div>
    </WalletProvider>
  );
};

export default App;
