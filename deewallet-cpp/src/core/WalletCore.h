/**
 * DEE WALLET - Wallet Core
 * HD Wallet (BIP39/BIP32/BIP44) implementation
 */

#ifndef WALLETCORE_H
#define WALLETCORE_H

#include <QString>
#include <QByteArray>
#include <memory>

class WalletCore
{
public:
    WalletCore();
    ~WalletCore();

    // BIP39 Mnemonic generation
    QString generateMnemonic(int wordCount = 12);
    bool validateMnemonic(const QString &mnemonic);

    // HD Wallet operations
    bool restoreFromMnemonic(const QString &mnemonic);
    void clear();

    // Key derivation (BIP32/BIP44)
    QByteArray derivePrivateKey(const QString &path);
    QByteArray derivePublicKey(const QString &path);

    // Address generation
    QString deriveAddress(const QString &chainType, uint32_t accountIndex = 0);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

#endif // WALLETCORE_H
