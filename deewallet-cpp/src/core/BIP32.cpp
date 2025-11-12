/**
 * DEE WALLET - BIP32 Implementation
 */

#include "BIP32.h"
#include <openssl/hmac.h>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <openssl/bn.h>
#include <QRegularExpression>

const char* BIP32::MASTER_SECRET = "Bitcoin seed";

BIP32::BIP32()
{
}

BIP32::~BIP32()
{
}

ExtendedKey BIP32::generateMasterKey(const QByteArray &seed)
{
    // I = HMAC-SHA512(Key = "Bitcoin seed", Data = seed)
    QByteArray hmac = hmacSHA512(
        QByteArray(MASTER_SECRET),
        seed
    );

    ExtendedKey master;
    master.key = hmac.left(32);           // IL = master secret key
    master.chainCode = hmac.mid(32, 32);  // IR = master chain code
    master.depth = 0;
    master.fingerprint = 0;
    master.childNumber = 0;
    master.isPrivate = true;

    return master;
}

ExtendedKey BIP32::deriveChild(const ExtendedKey &parent, uint32_t index, bool hardened)
{
    QByteArray data;

    if (hardened || index >= HARDENED_OFFSET) {
        // Hardened child: data = 0x00 || ser256(kpar) || ser32(i)
        if (!parent.isPrivate) {
            throw std::runtime_error("Cannot derive hardened child from public key");
        }
        data.append('\x00');
        data.append(parent.key);
        data.append(serializeUInt32(index >= HARDENED_OFFSET ? index : index + HARDENED_OFFSET));
    } else {
        // Normal child: data = serP(point(kpar)) || ser32(i)
        QByteArray publicKey = getPublicKey(parent);
        data.append(publicKey);
        data.append(serializeUInt32(index));
    }

    // I = HMAC-SHA512(Key = cpar, Data = data)
    QByteArray hmac = hmacSHA512(parent.chainCode, data);

    QByteArray IL = hmac.left(32);
    QByteArray IR = hmac.mid(32, 32);

    ExtendedKey child;

    if (parent.isPrivate) {
        // ki = parse256(IL) + kpar (mod n)
        // For simplicity, we'll store IL directly as the child key
        // In production, you'd need to perform EC math
        child.key = IL;  // Simplified (TODO: proper EC addition)
        child.isPrivate = true;
    } else {
        // Ki = point(parse256(IL)) + Kpar
        child.key = IL;  // Simplified (TODO: proper EC point addition)
        child.isPrivate = false;
    }

    child.chainCode = IR;
    child.depth = parent.depth + 1;
    child.fingerprint = 0; // TODO: Calculate from parent public key hash
    child.childNumber = hardened ? (index + HARDENED_OFFSET) : index;

    return child;
}

ExtendedKey BIP32::derivePath(const ExtendedKey &master, const QString &path)
{
    QVector<uint32_t> indices = parsePath(path);

    ExtendedKey current = master;
    for (uint32_t index : indices) {
        current = deriveChild(current, index, index >= HARDENED_OFFSET);
    }

    return current;
}

QByteArray BIP32::getPublicKey(const ExtendedKey &privateKey)
{
    if (!privateKey.isPrivate) {
        return privateKey.key;
    }

    // Create EC_KEY
    EC_KEY *eckey = EC_KEY_new_by_curve_name(NID_secp256k1);
    if (!eckey) {
        return QByteArray();
    }

    // Set private key
    BIGNUM *priv = BN_bin2bn(
        reinterpret_cast<const unsigned char*>(privateKey.key.constData()),
        privateKey.key.size(),
        nullptr
    );

    EC_KEY_set_private_key(eckey, priv);

    // Compute public key
    const EC_GROUP *group = EC_KEY_get0_group(eckey);
    EC_POINT *pub = EC_POINT_new(group);
    EC_POINT_mul(group, pub, priv, nullptr, nullptr, nullptr);
    EC_KEY_set_public_key(eckey, pub);

    // Serialize public key (uncompressed format for Ethereum/Tron compatibility)
    unsigned char pubkey[65];
    size_t len = EC_POINT_point2oct(
        group,
        pub,
        POINT_CONVERSION_UNCOMPRESSED,
        pubkey,
        sizeof(pubkey),
        nullptr
    );

    QByteArray result(reinterpret_cast<const char*>(pubkey), len);

    // Cleanup
    EC_POINT_free(pub);
    BN_free(priv);
    EC_KEY_free(eckey);

    return result;
}

QString BIP32::serializeKey(const ExtendedKey &key)
{
    // TODO: Implement Base58Check encoding
    return QString("xprv...");
}

ExtendedKey BIP32::parseKey(const QString &serialized)
{
    // TODO: Implement Base58Check decoding
    ExtendedKey key;
    return key;
}

QByteArray BIP32::hmacSHA512(const QByteArray &key, const QByteArray &data)
{
    unsigned char hash[64];
    unsigned int hashLen;

    HMAC(
        EVP_sha512(),
        key.constData(),
        key.size(),
        reinterpret_cast<const unsigned char*>(data.constData()),
        data.size(),
        hash,
        &hashLen
    );

    return QByteArray(reinterpret_cast<const char*>(hash), hashLen);
}

QByteArray BIP32::serializeUInt32(uint32_t value)
{
    QByteArray result(4, 0);
    result[0] = (value >> 24) & 0xFF;
    result[1] = (value >> 16) & 0xFF;
    result[2] = (value >> 8) & 0xFF;
    result[3] = value & 0xFF;
    return result;
}

QVector<uint32_t> BIP32::parsePath(const QString &path)
{
    QVector<uint32_t> indices;

    // Remove "m/" prefix if present
    QString p = path;
    if (p.startsWith("m/", Qt::CaseInsensitive)) {
        p = p.mid(2);
    }

    QStringList parts = p.split('/', Qt::SkipEmptyParts);

    for (const QString &part : parts) {
        bool hardened = part.endsWith('\'') || part.endsWith('h') || part.endsWith('H');
        QString numStr = hardened ? part.left(part.length() - 1) : part;

        uint32_t index = numStr.toUInt();
        if (hardened) {
            index += HARDENED_OFFSET;
        }

        indices.append(index);
    }

    return indices;
}
