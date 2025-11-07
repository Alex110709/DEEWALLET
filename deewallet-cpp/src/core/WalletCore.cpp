/**
 * DEE WALLET - Wallet Core Implementation
 */

#include "WalletCore.h"
#include "BIP39.h"
#include "BIP32.h"
#include "../chains/BitcoinAdapter.h"
#include "../chains/EthereumAdapter.h"
#include "../chains/TronAdapter.h"
#include "../chains/SolanaAdapter.h"
#include <openssl/crypto.h>

class WalletCore::Impl
{
public:
    BIP39 bip39;
    BIP32 bip32;
    ExtendedKey masterKey;
    bool isInitialized = false;
};

WalletCore::WalletCore()
    : pImpl(std::make_unique<Impl>())
{
}

WalletCore::~WalletCore()
{
    clear();
}

QString WalletCore::generateMnemonic(int wordCount)
{
    return pImpl->bip39.generateMnemonic(wordCount);
}

bool WalletCore::validateMnemonic(const QString &mnemonic)
{
    return pImpl->bip39.validateMnemonic(mnemonic);
}

bool WalletCore::restoreFromMnemonic(const QString &mnemonic)
{
    if (!validateMnemonic(mnemonic)) {
        return false;
    }

    // Generate seed from mnemonic (BIP39)
    QByteArray seed = pImpl->bip39.mnemonicToSeed(mnemonic);

    // Generate master key (BIP32)
    pImpl->masterKey = pImpl->bip32.generateMasterKey(seed);
    pImpl->isInitialized = true;

    // Securely wipe seed
    OPENSSL_cleanse(seed.data(), seed.size());

    return true;
}

void WalletCore::clear()
{
    if (pImpl->isInitialized) {
        // Securely wipe master key
        OPENSSL_cleanse(pImpl->masterKey.key.data(), pImpl->masterKey.key.size());
        OPENSSL_cleanse(pImpl->masterKey.chainCode.data(), pImpl->masterKey.chainCode.size());
        pImpl->isInitialized = false;
    }
}

QByteArray WalletCore::derivePrivateKey(const QString &path)
{
    if (!pImpl->isInitialized) {
        return QByteArray();
    }

    ExtendedKey derived = pImpl->bip32.derivePath(pImpl->masterKey, path);
    return derived.key;
}

QByteArray WalletCore::derivePublicKey(const QString &path)
{
    if (!pImpl->isInitialized) {
        return QByteArray();
    }

    ExtendedKey derived = pImpl->bip32.derivePath(pImpl->masterKey, path);
    return pImpl->bip32.getPublicKey(derived);
}

QString WalletCore::deriveAddress(const QString &chainType, uint32_t accountIndex)
{
    // BIP44 paths:
    // Bitcoin:  m/44'/0'/0'/0/0
    // Ethereum: m/44'/60'/0'/0/0
    // Tron:     m/44'/195'/0'/0/0
    // Solana:   m/44'/501'/0'/0/0

    QString path;
    if (chainType == "BTC" || chainType == "bitcoin") {
        path = QString("m/44'/0'/%1'/0/0").arg(accountIndex);
    } else if (chainType == "ETH" || chainType == "ethereum") {
        path = QString("m/44'/60'/%1'/0/0").arg(accountIndex);
    } else if (chainType == "TRX" || chainType == "tron") {
        path = QString("m/44'/195'/%1'/0/0").arg(accountIndex);
    } else if (chainType == "SOL" || chainType == "solana") {
        path = QString("m/44'/501'/%1'/0/0").arg(accountIndex);
    } else {
        return QString();
    }

    QByteArray publicKey = derivePublicKey(path);
    if (publicKey.isEmpty()) {
        return QString();
    }

    // Use chain adapters to convert public key to address
    if (chainType == "BTC" || chainType == "bitcoin") {
        BitcoinAdapter adapter("", false);
        return adapter.deriveAddress(publicKey);
    } else if (chainType == "ETH" || chainType == "ethereum") {
        EthereumAdapter adapter("");
        return adapter.deriveAddress(publicKey);
    } else if (chainType == "TRX" || chainType == "tron") {
        TronAdapter adapter("");
        return adapter.deriveAddress(publicKey);
    } else if (chainType == "SOL" || chainType == "solana") {
        SolanaAdapter adapter("");
        return adapter.deriveAddress(publicKey);
    }

    return QString();
}
