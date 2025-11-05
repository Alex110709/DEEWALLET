/**
 * DEEWALLET - Electron Main Process
 */

import { app, BrowserWindow, ipcMain, dialog } from 'electron';
import * as path from 'path';
import * as fs from 'fs/promises';
import { WalletCore } from './core/wallet.core';
import { BitcoinChainAdapter } from './chains/bitcoin.chain';
import { EthereumChainAdapter } from './chains/ethereum.chain';
import { SolanaChainAdapter } from './chains/solana.chain';
import { TronChainAdapter } from './chains/tron.chain';
import { ChainType, NetworkType, WalletConfig } from './types/wallet.types';

let mainWindow: BrowserWindow | null = null;
let walletCore: WalletCore | null = null;

const defaultConfig: WalletConfig = {
  network: NetworkType.MAINNET,
  rpcEndpoints: {
    BTC: 'https://blockstream.info/api',
    ETH: 'https://eth-mainnet.g.alchemy.com/v2/demo',
    SOL: 'https://api.mainnet-beta.solana.com',
    TRX: 'https://api.trongrid.io'
  },
  derivationPaths: {}
};

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1200,
    height: 800,
    minWidth: 900,
    minHeight: 600,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      nodeIntegration: false,
      contextIsolation: true
    },
    titleBarStyle: 'hidden',
    trafficLightPosition: { x: 15, y: 15 }
  });

  if (process.env.NODE_ENV === 'development') {
    mainWindow.loadURL('http://localhost:5173');
    mainWindow.webContents.openDevTools();
  } else {
    mainWindow.loadFile(path.join(__dirname, '../renderer/index.html'));
  }

  mainWindow.on('closed', () => {
    mainWindow = null;
    if (walletCore) {
      walletCore.clear();
    }
  });
}

app.whenReady().then(() => {
  createWindow();

  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) {
      createWindow();
    }
  });
});

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

// IPC Handlers

ipcMain.handle('wallet:generate', async () => {
  try {
    walletCore = new WalletCore(defaultConfig);
    const mnemonic = await walletCore.generateWallet();
    return { success: true, mnemonic };
  } catch (error) {
    return { success: false, error: (error as Error).message };
  }
});

ipcMain.handle('wallet:restore', async (_, mnemonic: string) => {
  try {
    walletCore = new WalletCore(defaultConfig);
    await walletCore.restoreFromMnemonic(mnemonic);
    return { success: true };
  } catch (error) {
    return { success: false, error: (error as Error).message };
  }
});

ipcMain.handle('wallet:export', async (_, password: string) => {
  try {
    if (!walletCore) {
      throw new Error('No wallet initialized');
    }

    const keyfileData = await walletCore.exportToKeyfile(password);

    const { filePath } = await dialog.showSaveDialog({
      title: 'Save Keyfile',
      defaultPath: 'deewallet.keyfile',
      filters: [{ name: 'Keyfile', extensions: ['keyfile'] }]
    });

    if (filePath) {
      await fs.writeFile(filePath, JSON.stringify(keyfileData, null, 2));
      return { success: true, path: filePath };
    }

    return { success: false, error: 'Export cancelled' };
  } catch (error) {
    return { success: false, error: (error as Error).message };
  }
});

ipcMain.handle('wallet:import', async (_, password: string) => {
  try {
    const { filePaths } = await dialog.showOpenDialog({
      title: 'Import Keyfile',
      filters: [{ name: 'Keyfile', extensions: ['keyfile'] }],
      properties: ['openFile']
    });

    if (filePaths.length === 0) {
      return { success: false, error: 'Import cancelled' };
    }

    const keyfileContent = await fs.readFile(filePaths[0], 'utf-8');
    const keyfileData = JSON.parse(keyfileContent);

    walletCore = new WalletCore(defaultConfig);
    await walletCore.importFromKeyfile(keyfileData, password);

    return { success: true };
  } catch (error) {
    return { success: false, error: (error as Error).message };
  }
});

ipcMain.handle('wallet:derive-account', async (_, chainType: ChainType, index: number) => {
  try {
    if (!walletCore) {
      throw new Error('No wallet initialized');
    }

    const account = walletCore.deriveAccount(chainType, index);

    // Don't send private key to renderer
    const safeAccount = {
      address: account.address,
      publicKey: account.publicKey,
      derivationPath: account.derivationPath,
      chainType: account.chainType,
      balance: account.balance,
      index: account.index
    };

    return { success: true, account: safeAccount };
  } catch (error) {
    return { success: false, error: (error as Error).message };
  }
});

ipcMain.handle('chain:get-balance', async (_, chainType: ChainType, address: string) => {
  try {
    const adapter = getChainAdapter(chainType);
    const balance = await adapter.getBalance(address);
    return { success: true, balance };
  } catch (error) {
    return { success: false, error: (error as Error).message };
  }
});

ipcMain.handle('chain:get-tokens', async (_, chainType: ChainType, address: string) => {
  try {
    const adapter = getChainAdapter(chainType);
    const tokens = await adapter.getTokens(address);
    return { success: true, tokens };
  } catch (error) {
    return { success: false, error: (error as Error).message };
  }
});

ipcMain.handle('chain:send-transaction', async (_, params: any, chainType: ChainType) => {
  try {
    if (!walletCore) {
      throw new Error('No wallet initialized');
    }

    const account = walletCore.deriveAccount(chainType, params.accountIndex || 0);

    if (!account.privateKey) {
      throw new Error('Private key not available');
    }

    const adapter = getChainAdapter(chainType);
    const transaction = await adapter.sendTransaction(
      {
        to: params.to,
        amount: params.amount,
        chainType,
        token: params.token,
        gasLimit: params.gasLimit,
        gasPrice: params.gasPrice
      },
      account.privateKey
    );

    return { success: true, transaction };
  } catch (error) {
    return { success: false, error: (error as Error).message };
  }
});

ipcMain.handle('chain:get-transaction-history', async (_, chainType: ChainType, address: string, limit?: number) => {
  try {
    const adapter = getChainAdapter(chainType);
    const transactions = await adapter.getTransactionHistory(address, limit);
    return { success: true, transactions };
  } catch (error) {
    return { success: false, error: (error as Error).message };
  }
});

function getChainAdapter(chainType: ChainType) {
  const rpcUrl = defaultConfig.rpcEndpoints[chainType];

  switch (chainType) {
    case ChainType.BTC:
      return new BitcoinChainAdapter(rpcUrl, defaultConfig.network === NetworkType.TESTNET);
    case ChainType.ETH:
      return new EthereumChainAdapter(rpcUrl);
    case ChainType.SOL:
      return new SolanaChainAdapter(rpcUrl);
    case ChainType.TRX:
      return new TronChainAdapter(rpcUrl);
    default:
      throw new Error(`Unsupported chain type: ${chainType}`);
  }
}
