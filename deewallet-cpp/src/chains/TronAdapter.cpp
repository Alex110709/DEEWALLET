/**
 * DEE WALLET - Tron Chain Adapter Implementation
 */

#include "TronAdapter.h"
#include "../utils/AddressUtils.h"
#include "../utils/Keccak256.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

TronAdapter::TronAdapter(const QString &rpcUrl)
{
    this->rpcUrl = rpcUrl;
}

QString TronAdapter::deriveAddress(const QByteArray &publicKey)
{
    // Tron address generation (similar to Ethereum)
    // 1. Keccak256 hash of public key (like Ethereum)
    // 2. Take last 20 bytes
    // 3. Add 0x41 prefix (mainnet)
    // 4. Base58Check encoding

    QByteArray pubKeyData = publicKey;

    // Remove 0x04 prefix if present
    if (pubKeyData.size() == 65 && pubKeyData[0] == 0x04) {
        pubKeyData = pubKeyData.mid(1);
    } else if (pubKeyData.size() != 64) {
        return QString(); // Invalid public key
    }

    // Keccak256 hash (Tron uses Keccak256 like Ethereum)
    QByteArray hash = Keccak256::hash(pubKeyData);

    // Take last 20 bytes
    QByteArray addressBytes = hash.right(20);

    // Base58Check encoding with 0x41 prefix (Tron mainnet)
    QString address = AddressUtils::encodeBase58Check(addressBytes, 0x41);

    return address;
}

bool TronAdapter::validateAddress(const QString &address)
{
    return address.startsWith("T") && address.length() == 34;
}

QString TronAdapter::getBalance(const QString &address)
{
    // Use TronGrid API for balance query
    QString endpoint = rpcUrl.isEmpty() ? "https://api.trongrid.io" : rpcUrl;
    QString url = endpoint + "/v1/accounts/" + address;

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
            QJsonArray data = obj["data"].toArray();
            if (!data.isEmpty()) {
                QJsonObject accountData = data[0].toObject();
                qint64 sunBalance = accountData["balance"].toVariant().toLongLong();
                double trx = sunBalance / 1000000.0; // Convert SUN to TRX
                balance = QString::number(trx, 'f', 6);
            }
        }
    }

    reply->deleteLater();
    return balance;
}

QVector<Token> TronAdapter::getTokens(const QString &address)
{
    // TODO: Implement TRC-20 token balance queries
    return QVector<Token>();
}

QString TronAdapter::createTransaction(const QString &from,
                                       const QString &to,
                                       const QString &amount,
                                       const QString &gasPrice)
{
    // TODO: Implement Tron transaction creation
    return QString();
}

QString TronAdapter::signTransaction(const QString &rawTx, const QByteArray &privateKey)
{
    // TODO: Implement ECDSA signing (same curve as Ethereum)
    return QString();
}

QString TronAdapter::broadcastTransaction(const QString &signedTx)
{
    // TODO: Implement TronGrid broadcast
    return QString();
}

QVector<Transaction> TronAdapter::getTransactionHistory(const QString &address, int limit)
{
    // TODO: Implement transaction history query
    return QVector<Transaction>();
}

QString TronAdapter::estimateFee(const QString &from, const QString &to, const QString &amount)
{
    // Tron typically uses bandwidth/energy, not gas
    return "0.0";
}
