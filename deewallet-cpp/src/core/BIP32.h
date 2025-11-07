/**
 * DEE WALLET - BIP32 HD Wallet Implementation
 * Hierarchical Deterministic Key Derivation
 */

#ifndef BIP32_H
#define BIP32_H

#include <QString>
#include <QByteArray>
#include "SecureMemory.h"

struct ExtendedKey {
    QByteArray key;          // 32 bytes (private) or 33 bytes (public)
    QByteArray chainCode;    // 32 bytes
    uint8_t depth;
    uint32_t fingerprint;
    uint32_t childNumber;
    bool isPrivate;
};

class BIP32 {
public:
    BIP32();
    ~BIP32();

    // Create master key from seed
    ExtendedKey generateMasterKey(const QByteArray &seed);

    // Derive child key
    ExtendedKey deriveChild(const ExtendedKey &parent, uint32_t index, bool hardened = false);

    // Derive from path (e.g., "m/44'/0'/0'/0/0")
    ExtendedKey derivePath(const ExtendedKey &master, const QString &path);

    // Get public key from private extended key
    QByteArray getPublicKey(const ExtendedKey &privateKey);

    // Serialize extended key (xprv/xpub format)
    QString serializeKey(const ExtendedKey &key);

    // Parse extended key
    ExtendedKey parseKey(const QString &serialized);

private:
    // HMAC-SHA512
    QByteArray hmacSHA512(const QByteArray &key, const QByteArray &data);

    // Serialize 32-bit integer (big-endian)
    QByteArray serializeUInt32(uint32_t value);

    // Parse derivation path
    QVector<uint32_t> parsePath(const QString &path);

    // Constants
    static constexpr uint32_t HARDENED_OFFSET = 0x80000000;
    static const char* MASTER_SECRET;
};

#endif // BIP32_H
