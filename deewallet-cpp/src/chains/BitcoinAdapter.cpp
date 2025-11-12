/**
 * DEE WALLET - Bitcoin Chain Adapter Implementation
 */

#include "BitcoinAdapter.h"
#include "../utils/AddressUtils.h"
#include <openssl/sha.h>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

BitcoinAdapter::BitcoinAdapter(const QString &rpcUrl, bool isTestnet)
    : isTestnet(isTestnet)
{
    this->rpcUrl = rpcUrl;
}

QString BitcoinAdapter::deriveAddress(const QByteArray &publicKey)
{
    // P2WPKH (Native SegWit) address generation
    // bc1q... for mainnet, tb1q... for testnet

    qDebug() << "[BitcoinAdapter] Input public key size:" << publicKey.size();
    if (!publicKey.isEmpty()) {
        qDebug() << "[BitcoinAdapter] First byte:" << QString("0x%1").arg((unsigned char)publicKey[0], 2, 16, QChar('0'));
    }

    QByteArray compressedKey;
    
    // Convert uncompressed (65 bytes) to compressed (33 bytes)
    if (publicKey.size() == 65 && publicKey[0] == 0x04) {
        qDebug() << "[BitcoinAdapter] Converting uncompressed to compressed key";
        // Uncompressed key format: 0x04 || X (32 bytes) || Y (32 bytes)
        // Compressed key format: prefix (0x02 or 0x03) || X (32 bytes)
        
        compressedKey.resize(33);
        
        // Determine prefix based on Y coordinate parity
        unsigned char yLast = static_cast<unsigned char>(publicKey[64]);
        compressedKey[0] = (yLast & 1) ? 0x03 : 0x02;
        
        // Copy X coordinate (bytes 1-32 from uncompressed key)
        for (int i = 0; i < 32; i++) {
            compressedKey[i + 1] = publicKey[i + 1];
        }
        qDebug() << "[BitcoinAdapter] Compressed key prefix:" << QString("0x%1").arg((unsigned char)compressedKey[0], 2, 16, QChar('0'));
    } else if (publicKey.size() == 33) {
        // Already compressed
        qDebug() << "[BitcoinAdapter] Key is already compressed";
        compressedKey = publicKey;
    } else {
        qDebug() << "[BitcoinAdapter] ERROR: Invalid public key size!";
        return QString(); // Invalid public key size
    }

    // Step 1: SHA256 hash of compressed public key
    unsigned char sha256Hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(compressedKey.constData()),
           compressedKey.size(),
           sha256Hash);

    QByteArray sha256Data(reinterpret_cast<const char*>(sha256Hash), SHA256_DIGEST_LENGTH);

    // Step 2: RIPEMD160 hash
    QByteArray pubKeyHash = AddressUtils::ripemd160(sha256Data);

    // Step 3: Bech32 encoding
    QString hrp = isTestnet ? "tb" : "bc";
    QString address = AddressUtils::encodeBech32(hrp, pubKeyHash);

    return address;
}

bool BitcoinAdapter::validateAddress(const QString &address)
{
    // Check for Bech32 (SegWit)
    if (address.startsWith("bc1") || address.startsWith("tb1")) {
        return address.length() >= 42 && address.length() <= 62;
    }

    // Check for legacy addresses (P2PKH: 1..., P2SH: 3...)
    if (address.startsWith("1") || address.startsWith("3")) {
        return address.length() >= 26 && address.length() <= 35;
    }

    return false;
}

QString BitcoinAdapter::getBalance(const QString &address)
{
    // Use Blockstream API for balance query
    QString url = QString("https://blockstream.info/api/address/%1").arg(address);

    QNetworkAccessManager manager;
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString balance = "0.0";
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            QJsonObject chainStats = obj["chain_stats"].toObject();
            qint64 fundedTxoSum = chainStats["funded_txo_sum"].toVariant().toLongLong();
            qint64 spentTxoSum = chainStats["spent_txo_sum"].toVariant().toLongLong();
            qint64 satoshis = fundedTxoSum - spentTxoSum;
            double btc = satoshis / 100000000.0;
            balance = QString::number(btc, 'f', 8);
        }
    }

    reply->deleteLater();
    return balance;
}

QVector<Token> BitcoinAdapter::getTokens(const QString &address)
{
    // Bitcoin doesn't have native tokens
    return QVector<Token>();
}

QString BitcoinAdapter::createTransaction(const QString &from,
                                          const QString &to,
                                          const QString &amount,
                                          const QString &gasPrice)
{
    // TODO: Implement UTXO-based transaction creation
    return QString();
}

QString BitcoinAdapter::signTransaction(const QString &rawTx, const QByteArray &privateKey)
{
    // TODO: Implement ECDSA signing
    return QString();
}

QString BitcoinAdapter::broadcastTransaction(const QString &signedTx)
{
    // TODO: Implement RPC broadcast
    return QString();
}

QVector<Transaction> BitcoinAdapter::getTransactionHistory(const QString &address, int limit)
{
    // TODO: Implement transaction history query
    return QVector<Transaction>();
}

QString BitcoinAdapter::estimateFee(const QString &from, const QString &to, const QString &amount)
{
    // TODO: Implement fee estimation
    return "0.0001";
}
