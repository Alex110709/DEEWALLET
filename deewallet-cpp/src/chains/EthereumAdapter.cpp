/**
 * DEE WALLET - Ethereum Chain Adapter Implementation
 */

#include "EthereumAdapter.h"
#include "../utils/Keccak256.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

EthereumAdapter::EthereumAdapter(const QString &rpcUrl)
{
    this->rpcUrl = rpcUrl;
}

QString EthereumAdapter::deriveAddress(const QByteArray &publicKey)
{
    // Ethereum address derivation from public key
    // 1. Take the last 64 bytes of the public key (uncompressed, without 0x04 prefix)
    // 2. Keccak256 hash
    // 3. Take last 20 bytes
    // 4. Prefix with 0x

    QByteArray pubKeyData = publicKey;

    // If compressed (33 bytes), we need to decompress
    // For now, assume it's already uncompressed (65 bytes with 0x04 prefix)
    if (pubKeyData.size() == 65 && pubKeyData[0] == 0x04) {
        // Remove 0x04 prefix
        pubKeyData = pubKeyData.mid(1);
    } else if (pubKeyData.size() == 64) {
        // Already without prefix
    } else {
        return QString(); // Invalid public key format
    }

    // Keccak256 hash of public key
    QByteArray hash = Keccak256::hash(pubKeyData);

    // Take last 20 bytes
    QByteArray addressBytes = hash.right(20);

    // Convert to hex string with 0x prefix
    QString address = "0x" + addressBytes.toHex();

    return address;
}

bool EthereumAdapter::validateAddress(const QString &address)
{
    if (!address.startsWith("0x")) {
        return false;
    }

    if (address.length() != 42) {
        return false;
    }

    // Check if all characters are hex
    QString hexPart = address.mid(2);
    for (QChar c : hexPart) {
        if (!c.isDigit() && !(c >= 'a' && c <= 'f') && !(c >= 'A' && c <= 'F')) {
            return false;
        }
    }

    return true;
}

QString EthereumAdapter::getBalance(const QString &address)
{
    // Use Infura public RPC endpoint
    QString endpoint = rpcUrl.isEmpty() ? "https://eth.llamarpc.com" : rpcUrl;

    QNetworkAccessManager manager;
    QNetworkRequest request(endpoint);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Prepare JSON-RPC request
    QJsonObject rpcRequest;
    rpcRequest["jsonrpc"] = "2.0";
    rpcRequest["method"] = "eth_getBalance";
    QJsonArray params;
    params.append(address);
    params.append("latest");
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
            QString hexBalance = responseObj["result"].toString();
            // Convert hex Wei to ETH
            bool ok;
            quint64 wei = hexBalance.toULongLong(&ok, 16);
            if (ok) {
                double eth = wei / 1e18;
                balance = QString::number(eth, 'f', 8);
            }
        }
    }

    reply->deleteLater();
    return balance;
}

QVector<Token> EthereumAdapter::getTokens(const QString &address)
{
    // TODO: Implement ERC-20 token balance queries
    return parseERC20Balances(address);
}

QString EthereumAdapter::createTransaction(const QString &from,
                                           const QString &to,
                                           const QString &amount,
                                           const QString &gasPrice)
{
    // TODO: Implement RLP-encoded transaction creation
    return QString();
}

QString EthereumAdapter::signTransaction(const QString &rawTx, const QByteArray &privateKey)
{
    // TODO: Implement ECDSA signing with secp256k1
    return QString();
}

QString EthereumAdapter::broadcastTransaction(const QString &signedTx)
{
    // TODO: Implement eth_sendRawTransaction RPC call
    return QString();
}

QVector<Transaction> EthereumAdapter::getTransactionHistory(const QString &address, int limit)
{
    // TODO: Implement transaction history query (Etherscan API or RPC)
    return QVector<Transaction>();
}

QString EthereumAdapter::estimateFee(const QString &from, const QString &to, const QString &amount)
{
    // TODO: Implement eth_gasPrice and eth_estimateGas RPC calls
    return "0.001";
}

QString EthereumAdapter::encodeERC20Transfer(const QString &to, const QString &amount)
{
    // TODO: Implement ERC-20 transfer ABI encoding
    // Method: transfer(address,uint256)
    return QString();
}

QVector<Token> EthereumAdapter::parseERC20Balances(const QString &address)
{
    // TODO: Query popular ERC-20 contracts
    return QVector<Token>();
}
