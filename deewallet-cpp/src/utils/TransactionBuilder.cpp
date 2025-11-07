/**
 * DEE WALLET - Transaction Builder Implementation
 */

#include "TransactionBuilder.h"
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/sha.h>
#include <QDataStream>

QByteArray TransactionBuilder::buildBitcoinTransaction(const QVector<UTXO> &inputs,
                                                       const QString &toAddress,
                                                       qint64 amount,
                                                       qint64 fee,
                                                       const QString &changeAddress)
{
    // Simplified Bitcoin transaction building
    // Real implementation requires:
    // - UTXO selection
    // - Witness data for SegWit
    // - Proper script generation
    // - Complete serialization

    QByteArray tx;
    QDataStream stream(&tx, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    // Version
    stream << (quint32)2;

    // Input count
    stream << (quint8)inputs.size();

    // Inputs (simplified)
    for (const UTXO &utxo : inputs) {
        // Previous txid (reversed)
        QByteArray txid = hexToBytes(utxo.txid);
        std::reverse(txid.begin(), txid.end());
        stream.writeRawData(txid.constData(), 32);

        // Vout
        stream << utxo.vout;

        // ScriptSig (empty for unsigned)
        stream << (quint8)0;

        // Sequence
        stream << (quint32)0xffffffff;
    }

    // Output count (2: recipient + change)
    stream << (quint8)2;

    // Recipient output
    stream << amount;
    // ScriptPubKey (simplified)
    stream << (quint8)0;  // Will be filled with actual script

    // Change output
    qint64 changeAmount = 0;
    for (const UTXO &utxo : inputs) {
        changeAmount += utxo.value;
    }
    changeAmount -= (amount + fee);

    stream << changeAmount;
    stream << (quint8)0;  // Change script

    // Locktime
    stream << (quint32)0;

    return tx;
}

QByteArray TransactionBuilder::signBitcoinInput(const QByteArray &tx,
                                               int inputIndex,
                                               const QByteArray &privateKey,
                                               const QString &scriptPubKey)
{
    // Simplified SegWit signing
    // Real implementation requires:
    // - Proper sighash calculation (BIP143)
    // - ECDSA signature with secp256k1
    // - DER encoding
    // - Witness data construction

    // TODO: Implement full BIP143 SegWit signing
    return QByteArray();
}

QByteArray TransactionBuilder::encodeRLP(const EthereumTx &tx)
{
    // RLP encoding for Ethereum transactions
    QVector<QByteArray> fields;

    // Nonce
    fields.append(hexToBytes(tx.nonce));

    // Gas price
    fields.append(hexToBytes(tx.gasPrice));

    // Gas limit
    fields.append(hexToBytes(tx.gasLimit));

    // To address
    fields.append(hexToBytes(tx.to));

    // Value
    fields.append(hexToBytes(tx.value));

    // Data
    fields.append(hexToBytes(tx.data.isEmpty() ? "0x" : tx.data));

    // Chain ID (for EIP-155)
    if (tx.chainId > 0) {
        fields.append(encodeVarint(tx.chainId));
        fields.append(QByteArray());  // r
        fields.append(QByteArray());  // s
    }

    return encodeRLPList(fields);
}

QByteArray TransactionBuilder::signEthereumTransaction(const EthereumTx &tx,
                                                       const QByteArray &privateKey)
{
    // Encode transaction
    QByteArray rlp = encodeRLP(tx);

    // Keccak256 hash
    unsigned char hash[32];
    SHA256(reinterpret_cast<const unsigned char*>(rlp.constData()), rlp.size(), hash);

    // TODO: Implement ECDSA signature with secp256k1
    // Real implementation requires:
    // - secp256k1 curve
    // - Recoverable signature (v, r, s)
    // - Proper RLP encoding with signature

    return QByteArray();
}

QByteArray TransactionBuilder::buildTronTransaction(const QString &from,
                                                   const QString &to,
                                                   qint64 amount)
{
    // Tron uses Protobuf encoding
    // This is a simplified placeholder
    // Real implementation requires:
    // - Protobuf Transaction message
    // - Contract type (TransferContract)
    // - Proper serialization

    // TODO: Implement Protobuf encoding
    return QByteArray();
}

QByteArray TransactionBuilder::signTronTransaction(const QByteArray &txBytes,
                                                  const QByteArray &privateKey)
{
    // Tron signature (SHA256 + ECDSA)
    unsigned char hash[32];
    SHA256(reinterpret_cast<const unsigned char*>(txBytes.constData()),
           txBytes.size(), hash);

    // TODO: Implement ECDSA signature
    return QByteArray();
}

QByteArray TransactionBuilder::buildSolanaTransaction(const QString &from,
                                                     const QString &to,
                                                     qint64 lamports)
{
    // Solana transaction message
    // Real implementation requires:
    // - CompactArray for accounts
    // - Recent blockhash
    // - Instructions
    // - Proper serialization

    // TODO: Implement Solana transaction building
    return QByteArray();
}

QByteArray TransactionBuilder::signSolanaTransaction(const QByteArray &message,
                                                    const QByteArray &privateKey)
{
    // Solana uses Ed25519 signatures
    // TODO: Implement Ed25519 signing with libsodium
    return QByteArray();
}

// Utility functions
QByteArray TransactionBuilder::encodeVarint(quint64 value)
{
    QByteArray result;
    while (value >= 0x80) {
        result.append((value & 0x7f) | 0x80);
        value >>= 7;
    }
    result.append(value & 0x7f);
    return result;
}

QByteArray TransactionBuilder::encodeRLPString(const QByteArray &data)
{
    if (data.size() == 1 && (unsigned char)data[0] < 0x80) {
        return data;
    } else if (data.size() < 56) {
        QByteArray result;
        result.append(0x80 + data.size());
        result.append(data);
        return result;
    } else {
        QByteArray lengthBytes = QByteArray::number(data.size(), 16).rightJustified(2, '0');
        QByteArray result;
        result.append(0xb7 + lengthBytes.size());
        result.append(hexToBytes(lengthBytes));
        result.append(data);
        return result;
    }
}

QByteArray TransactionBuilder::encodeRLPList(const QVector<QByteArray> &items)
{
    QByteArray encoded;
    for (const QByteArray &item : items) {
        encoded.append(encodeRLPString(item));
    }

    if (encoded.size() < 56) {
        QByteArray result;
        result.append(0xc0 + encoded.size());
        result.append(encoded);
        return result;
    } else {
        QByteArray lengthBytes = QByteArray::number(encoded.size(), 16).rightJustified(2, '0');
        QByteArray result;
        result.append(0xf7 + lengthBytes.size());
        result.append(hexToBytes(lengthBytes));
        result.append(encoded);
        return result;
    }
}

QString TransactionBuilder::bytesToHex(const QByteArray &bytes)
{
    return bytes.toHex();
}

QByteArray TransactionBuilder::hexToBytes(const QString &hex)
{
    QString cleanHex = hex;
    if (cleanHex.startsWith("0x")) {
        cleanHex = cleanHex.mid(2);
    }
    return QByteArray::fromHex(cleanHex.toLatin1());
}
