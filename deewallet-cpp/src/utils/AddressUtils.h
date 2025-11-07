/**
 * DEE WALLET - Address Encoding Utilities
 * Base58, Base58Check, Bech32 encoding for cryptocurrency addresses
 */

#ifndef ADDRESSUTILS_H
#define ADDRESSUTILS_H

#include <QString>
#include <QByteArray>

class AddressUtils {
public:
    // Base58 encoding (Bitcoin, Tron)
    static QString encodeBase58(const QByteArray &data);
    static QByteArray decodeBase58(const QString &encoded);

    // Base58Check encoding (Bitcoin legacy addresses)
    static QString encodeBase58Check(const QByteArray &data, uint8_t version = 0);
    static QByteArray decodeBase58Check(const QString &encoded);

    // Bech32 encoding (Bitcoin SegWit addresses)
    static QString encodeBech32(const QString &hrp, const QByteArray &data);
    static QByteArray decodeBech32(const QString &address, QString &hrp);

    // RIPEMD160 hash
    static QByteArray ripemd160(const QByteArray &data);

    // SHA256 double hash (Bitcoin)
    static QByteArray sha256d(const QByteArray &data);

    // Keccak256 hash (Ethereum)
    static QByteArray keccak256(const QByteArray &data);

private:
    static const char* BASE58_ALPHABET;
    static const char* BECH32_CHARSET;

    // Bech32 helpers
    static uint32_t bech32Polymod(const QVector<uint8_t> &values);
    static QVector<uint8_t> bech32HrpExpand(const QString &hrp);
    static QVector<uint8_t> convertBits(const QByteArray &data, int fromBits, int toBits, bool pad);
};

#endif // ADDRESSUTILS_H
