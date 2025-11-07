/**
 * DEE WALLET - Electron Preload Script
 * Exposes safe IPC methods to renderer process
 */

import { contextBridge, ipcRenderer } from 'electron';

contextBridge.exposeInMainWorld('electron', {
  invoke: (channel: string, ...args: any[]) => {
    const validChannels = [
      'wallet:generate',
      'wallet:restore',
      'wallet:export',
      'wallet:import',
      'wallet:derive-account',
      'chain:get-balance',
      'chain:get-tokens',
      'chain:send-transaction',
      'chain:get-transaction-history',
      'keyfile:scan',
      'keyfile:get-details',
      'keyfile:import-from-path',
      'rpc:get-stats',
      'rpc:set-endpoint',
      'rpc:get-endpoints'
    ];

    if (validChannels.includes(channel)) {
      return ipcRenderer.invoke(channel, ...args);
    }

    throw new Error(`Invalid IPC channel: ${channel}`);
  }
});
