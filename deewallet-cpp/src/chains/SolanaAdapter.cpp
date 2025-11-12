/**
 * DEE WALLET - Solana Chain Adapter Implementation
 */

#include "SolanaAdapter.h"
#include "../utils/AddressUtils.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

SolanaAdapter::SolanaAdapter(const QString &rpcUrl)
{
    this->rpcUrl = rpcUrl;
}

QString SolanaAdapter::deriveAddress(const QByteArray &publicKey)
{
    // Solana addresses are Ed25519 public keys encoded in Base58
    // Since we use secp256k1, we'll use the x-coordinate (32 bytes)
    
    QByteArray pubKeyData = publicKey;
    
    // Remove 0x04 prefix if present (uncompressed key)
    if (pubKeyData.size() == 65 && pubKeyData[0] == 0x04) {
        pubKeyData = pubKeyData.mid(1);
    }
    
    // Take first 32 bytes (x-coordinate of secp256k1 public key)
    if (pubKeyData.size() >= 32) {
        pubKeyData = pubKeyData.left(32);
    } else {
        return QString();
    }

    // Base58 encoding (no checksum for Solana)
    QString address = AddressUtils::encodeBase58(pubKeyData);

    return address;
}

bool SolanaAdapter::validateAddress(const QString &address)
{
    // Solana addresses are Base58-encoded 32-byte keys (typically 32-44 chars)
    return address.length() >= 32 && address.length() <= 44;
}

QString SolanaAdapter::getBalance(const QString &address)
{
    // Use Solana RPC endpoint
    QString endpoint = rpcUrl.isEmpty() ? "https://api.mainnet-beta.solana.com" : rpcUrl;

    QNetworkAccessManager manager;
    QNetworkRequest request(endpoint);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Prepare JSON-RPC request
    QJsonObject rpcRequest;
    rpcRequest["jsonrpc"] = "2.0";
    rpcRequest["method"] = "getBalance";
    QJsonArray params;
    params.append(address);
    rpcRequest["params"] = params;
    rpcRequest["id"] = 1;

    QJsonDocument doc(rpcRequest);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    QNetworkReply *reply = manager.post(request, data);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString balance = "0.0";
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(response);
        if (responseDoc.isObject()) {
            QJsonObject responseObj = responseDoc.object();
            QJsonObject result = responseObj["result"].toObject();
            qint64 lamports = result["value"].toVariant().toLongLong();
            double sol = lamports / 1e9; // Convert lamports to SOL
            balance = QString::number(sol, 'f', 9);
        }
    }

    reply->deleteLater();
    return balance;
}

QVector<Token> SolanaAdapter::getTokens(const QString &address)
{
    // TODO: Implement SPL token account queries
    return parseSPLTokens(address);
}

QString SolanaAdapter::createTransaction(const QString &from,
                                         const QString &to,
                                         const QString &amount,
                                         const QString &gasPrice)
{
    // TODO: Implement Solana transaction creation
    // Solana uses a different transaction format than EVM chains
    return QString();
}

QString SolanaAdapter::signTransaction(const QString &rawTx, const QByteArray &privateKey)
{
    // TODO: Implement Ed25519 signing (NOT secp256k1)
    // Use libsodium's crypto_sign_detached
    return QString();
}

QString SolanaAdapter::broadcastTransaction(const QString &signedTx)
{
    // TODO: Implement sendTransaction RPC call
    return QString();
}

QVector<Transaction> SolanaAdapter::getTransactionHistory(const QString &address, int limit)
{
    // TODO: Implement getSignaturesForAddress RPC call
    return QVector<Transaction>();
}

QString SolanaAdapter::estimateFee(const QString &from, const QString &to, const QString &amount)
{
    // Solana uses lamports, typical fee is 5000 lamports = 0.000005 SOL
    return "0.000005";
}

QVector<Token> SolanaAdapter::parseSPLTokens(const QString &address)
{
    // TODO: Query SPL token accounts for this address
    // Use getTokenAccountsByOwner RPC method
    return QVector<Token>();
}
