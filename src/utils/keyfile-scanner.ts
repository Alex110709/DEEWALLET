/**
 * DEE WALLET - Keyfile Scanner Utility
 * Automatically scans directory for wallet keyfiles
 */

import * as fs from 'fs/promises';
import * as path from 'path';
import { app } from 'electron';

export interface KeyfileInfo {
  filename: string;
  filepath: string;
  createdAt: number;
  updatedAt: number;
  size: number;
  isValid: boolean;
}

export class KeyfileScanner {
  private scanDirectory: string;

  constructor(directory?: string) {
    // Default to app directory
    this.scanDirectory = directory || app.getPath('userData');
  }

  /**
   * Scan directory for keyfiles
   */
  async scanForKeyfiles(): Promise<KeyfileInfo[]> {
    const keyfiles: KeyfileInfo[] = [];

    try {
      // Check if directory exists
      await fs.access(this.scanDirectory);

      // Read directory contents
      const files = await fs.readdir(this.scanDirectory);

      // Filter for .keyfile extension
      const keyfileNames = files.filter(file => file.endsWith('.keyfile'));

      // Get detailed info for each keyfile
      for (const filename of keyfileNames) {
        const filepath = path.join(this.scanDirectory, filename);

        try {
          const stats = await fs.stat(filepath);
          const content = await fs.readFile(filepath, 'utf-8');

          // Basic validation
          const isValid = await this.validateKeyfile(content);

          keyfiles.push({
            filename,
            filepath,
            createdAt: stats.birthtimeMs,
            updatedAt: stats.mtimeMs,
            size: stats.size,
            isValid
          });
        } catch (error) {
          console.error(`Error reading keyfile ${filename}:`, error);
          // Add to list but mark as invalid
          keyfiles.push({
            filename,
            filepath,
            createdAt: 0,
            updatedAt: 0,
            size: 0,
            isValid: false
          });
        }
      }

      // Sort by update time (newest first)
      keyfiles.sort((a, b) => b.updatedAt - a.updatedAt);

      return keyfiles;
    } catch (error) {
      console.error('Error scanning for keyfiles:', error);
      return [];
    }
  }

  /**
   * Validate keyfile structure
   */
  private async validateKeyfile(content: string): Promise<boolean> {
    try {
      const data = JSON.parse(content);

      // Check required fields
      if (!data.version || !data.id || !data.crypto) {
        return false;
      }

      // Check crypto fields
      if (!data.crypto.cipher || !data.crypto.ciphertext || !data.crypto.kdf) {
        return false;
      }

      return true;
    } catch {
      return false;
    }
  }

  /**
   * Get keyfile details
   */
  async getKeyfileDetails(filepath: string): Promise<any> {
    try {
      const content = await fs.readFile(filepath, 'utf-8');
      const data = JSON.parse(content);

      // Return safe info (no sensitive data)
      return {
        version: data.version,
        id: data.id,
        createdAt: data.createdAt,
        updatedAt: data.updatedAt,
        accounts: Object.keys(data.accounts || {})
      };
    } catch (error) {
      throw new Error(`Failed to read keyfile: ${(error as Error).message}`);
    }
  }

  /**
   * Set scan directory
   */
  setScanDirectory(directory: string): void {
    this.scanDirectory = directory;
  }

  /**
   * Get current scan directory
   */
  getScanDirectory(): string {
    return this.scanDirectory;
  }
}
