/**
 * DEE WALLET - Address Encoding Utilities Implementation
 */

#include "AddressUtils.h"
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <QVector>
#include <algorithm>

const char* AddressUtils::BASE58_ALPHABET = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
const char* AddressUtils::BECH32_CHARSET = "qpzry9x8gf2tvdw0s3jn54khce6mua7l";

// Base58 Encoding
QString AddressUtils::encodeBase58(const QByteArray &data)
{
    if (data.isEmpty()) {
        return QString();
    }

    // Count leading zeros
    int leadingZeros = 0;
    for (int i = 0; i < data.size() && data[i] == 0; ++i) {
        leadingZeros++;
    }

    // Convert to base58
    QVector<uint8_t> digits((data.size() * 138 / 100) + 1);
    int digitsLen = 1;

    for (int i = 0; i < data.size(); ++i) {
        int carry = static_cast<uint8_t>(data[i]);
        for (int j = 0; j < digitsLen; ++j) {
            carry += static_cast<int>(digits[j]) << 8;
            digits[j] = static_cast<uint8_t>(carry % 58);
            carry /= 58;
        }

        while (carry > 0) {
            digits[digitsLen++] = static_cast<uint8_t>(carry % 58);
            carry /= 58;
        }
    }

    // Build result string
    QString result;
    for (int i = 0; i < leadingZeros; ++i) {
        result += '1';
    }

    for (int i = digitsLen - 1; i >= 0; --i) {
        result += BASE58_ALPHABET[digits[i]];
    }

    return result;
}

QByteArray AddressUtils::decodeBase58(const QString &encoded)
{
    if (encoded.isEmpty()) {
        return QByteArray();
    }

    // Count leading '1's
    int leadingZeros = 0;
    for (int i = 0; i < encoded.length() && encoded[i] == '1'; ++i) {
        leadingZeros++;
    }

    // Decode
    QVector<uint8_t> result((encoded.length() * 733 / 1000) + 1);
    int resultLen = 1;

    for (int i = 0; i < encoded.length(); ++i) {
        int carry = QString(BASE58_ALPHABET).indexOf(encoded[i]);
        if (carry < 0) {
            return QByteArray(); // Invalid character
        }

        for (int j = 0; j < resultLen; ++j) {
            carry += static_cast<int>(result[j]) * 58;
            result[j] = static_cast<uint8_t>(carry & 0xff);
            carry >>= 8;
        }

        while (carry > 0) {
            result[resultLen++] = static_cast<uint8_t>(carry & 0xff);
            carry >>= 8;
        }
    }

    // Build output
    QByteArray output;
    for (int i = 0; i < leadingZeros; ++i) {
        output.append('\x00');
    }

    for (int i = resultLen - 1; i >= 0; --i) {
        output.append(static_cast<char>(result[i]));
    }

    return output;
}

// Base58Check Encoding
QString AddressUtils::encodeBase58Check(const QByteArray &data, uint8_t version)
{
    QByteArray payload;
    payload.append(static_cast<char>(version));
    payload.append(data);

    // Calculate checksum
    QByteArray checksum = sha256d(payload).left(4);

    // Append checksum
    payload.append(checksum);

    return encodeBase58(payload);
}

QByteArray AddressUtils::decodeBase58Check(const QString &encoded)
{
    QByteArray decoded = decodeBase58(encoded);
    if (decoded.size() < 5) {
        return QByteArray();
    }

    // Verify checksum
    QByteArray payload = decoded.left(decoded.size() - 4);
    QByteArray checksum = decoded.right(4);
    QByteArray expectedChecksum = sha256d(payload).left(4);

    if (checksum != expectedChecksum) {
        return QByteArray(); // Invalid checksum
    }

    // Return payload without version byte
    return payload.mid(1);
}

// Bech32 Encoding
uint32_t AddressUtils::bech32Polymod(const QVector<uint8_t> &values)
{
    uint32_t chk = 1;
    for (uint8_t value : values) {
        uint32_t top = chk >> 25;
        chk = (chk & 0x1ffffff) << 5 ^ value;
        if (top & 1)  chk ^= 0x3b6a57b2;
        if (top & 2)  chk ^= 0x26508e6d;
        if (top & 4)  chk ^= 0x1ea119fa;
        if (top & 8)  chk ^= 0x3d4233dd;
        if (top & 16) chk ^= 0x2a1462b3;
    }
    return chk;
}

QVector<uint8_t> AddressUtils::bech32HrpExpand(const QString &hrp)
{
    QVector<uint8_t> result;
    for (QChar c : hrp) {
        result.append(c.toLatin1() >> 5);
    }
    result.append(0);
    for (QChar c : hrp) {
        result.append(c.toLatin1() & 31);
    }
    return result;
}

QVector<uint8_t> AddressUtils::convertBits(const QByteArray &data, int fromBits, int toBits, bool pad)
{
    QVector<uint8_t> result;
    int acc = 0;
    int bits = 0;
    int maxv = (1 << toBits) - 1;

    for (unsigned char byte : data) {
        acc = (acc << fromBits) | byte;
        bits += fromBits;
        while (bits >= toBits) {
            bits -= toBits;
            result.append((acc >> bits) & maxv);
        }
    }

    if (pad) {
        if (bits > 0) {
            result.append((acc << (toBits - bits)) & maxv);
        }
    } else if (bits >= fromBits || ((acc << (toBits - bits)) & maxv)) {
        return QVector<uint8_t>(); // Invalid padding
    }

    return result;
}

QString AddressUtils::encodeBech32(const QString &hrp, const QByteArray &data)
{
    // Convert witness version (0) and program
    QVector<uint8_t> values;
    values.append(0); // Witness version 0 (P2WPKH)

    QVector<uint8_t> converted = convertBits(data, 8, 5, true);
    values.append(converted);

    // Calculate checksum
    QVector<uint8_t> polymodInput = bech32HrpExpand(hrp);
    polymodInput.append(values);
    polymodInput.append(QVector<uint8_t>(6, 0));

    uint32_t polymod = bech32Polymod(polymodInput) ^ 1;

    // Add checksum
    for (int i = 0; i < 6; ++i) {
        values.append((polymod >> (5 * (5 - i))) & 31);
    }

    // Build result
    QString result = hrp + "1";
    for (uint8_t value : values) {
        result += BECH32_CHARSET[value];
    }

    return result;
}

QByteArray AddressUtils::decodeBech32(const QString &address, QString &hrp)
{
    // TODO: Implement Bech32 decoding
    return QByteArray();
}

// Hash functions
QByteArray AddressUtils::ripemd160(const QByteArray &data)
{
    unsigned char hash[RIPEMD160_DIGEST_LENGTH];
    RIPEMD160(reinterpret_cast<const unsigned char*>(data.constData()),
              data.size(),
              hash);
    return QByteArray(reinterpret_cast<const char*>(hash), RIPEMD160_DIGEST_LENGTH);
}

QByteArray AddressUtils::sha256d(const QByteArray &data)
{
    unsigned char hash1[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(data.constData()),
           data.size(),
           hash1);

    unsigned char hash2[SHA256_DIGEST_LENGTH];
    SHA256(hash1, SHA256_DIGEST_LENGTH, hash2);

    return QByteArray(reinterpret_cast<const char*>(hash2), SHA256_DIGEST_LENGTH);
}

QByteArray AddressUtils::keccak256(const QByteArray &data)
{
    // Keccak256 implementation (Ethereum uses Keccak, not SHA3)
    // For now, return SHA3-256 as placeholder
    // TODO: Use proper Keccak256 implementation
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(data.constData()),
           data.size(),
           hash);
    return QByteArray(reinterpret_cast<const char*>(hash), SHA256_DIGEST_LENGTH);
}
