/**
 * DEE WALLET - Token Detector Implementation
 */

#include "TokenDetector.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// Popular token lists
QVector<Token> TokenDetector::popularERC20Tokens = {
    {"USDT", "Tether USD", "0xdac17f958d2ee523a2206206994597c13d831ec7", "0", 6},
    {"USDC", "USD Coin", "0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48", "0", 6},
    {"DAI", "Dai Stablecoin", "0x6b175474e89094c44da98b954eedeac495271d0f", "0", 18},
    {"WETH", "Wrapped Ether", "0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2", "0", 18},
    {"UNI", "Uniswap", "0x1f9840a85d5af5bf1d1762f925bdaddc4201f984", "0", 18},
};

QVector<Token> TokenDetector::popularTRC20Tokens = {
    {"USDT", "Tether USD", "TR7NHqjeKQxGTCi8q8ZY4pL8otSzgjLj6t", "0", 6},
    {"USDC", "USD Coin", "TEkxiTehnzSmSe2XqrBj4w32RUN966rdz8", "0", 6},
};

QVector<Token> TokenDetector::popularSPLTokens = {
    {"USDT", "Tether USD", "Es9vMFrzaCERmJfrF4H2FYD4KCoNkY11McCe8BenwNYB", "0", 6},
    {"USDC", "USD Coin", "EPjFWdd5AufqSSqeM2qN1xzybapC8G4wEGGkZwyTDt1v", "0", 6},
    {"SOL", "Wrapped SOL", "So11111111111111111111111111111111111111112", "0", 9},
};

QVector<Token> TokenDetector::detectERC20Tokens(const QString &address)
{
    QVector<Token> detectedTokens;

    // Query popular ERC-20 tokens
    for (Token token : popularERC20Tokens) {
        // Encode balanceOf(address) call
        QString data = encodeERC20BalanceOf(address);

        // Make eth_call to get balance
        QNetworkAccessManager manager;
        QNetworkRequest request("https://eth.llamarpc.com");
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QJsonObject rpcRequest;
        rpcRequest["jsonrpc"] = "2.0";
        rpcRequest["method"] = "eth_call";

        QJsonArray params;
        QJsonObject txObj;
        txObj["to"] = token.contractAddress;
        txObj["data"] = data;
        params.append(txObj);
        params.append("latest");
        rpcRequest["params"] = params;
        rpcRequest["id"] = 1;

        QJsonDocument doc(rpcRequest);
        QByteArray requestData = doc.toJson(QJsonDocument::Compact);

        QNetworkReply *reply = manager.post(request, requestData);

        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(response);
            if (responseDoc.isObject()) {
                QString result = responseDoc.object()["result"].toString();

                // Parse hex balance
                bool ok;
                quint64 balance = result.toULongLong(&ok, 16);

                if (ok && balance > 0) {
                    token.balance = QString::number(balance / pow(10, token.decimals), 'f', token.decimals);
                    detectedTokens.append(token);
                }
            }
        }

        reply->deleteLater();
    }

    return detectedTokens;
}

QVector<Token> TokenDetector::detectTRC20Tokens(const QString &address)
{
    QVector<Token> detectedTokens;

    // TronGrid API for TRC-20 tokens
    QString url = QString("https://api.trongrid.io/v1/accounts/%1/tokens").arg(address);

    QNetworkAccessManager manager;
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        if (doc.isObject()) {
            QJsonArray data = doc.object()["data"].toArray();
            for (const QJsonValue &val : data) {
                QJsonObject tokenObj = val.toObject();

                Token token;
                token.symbol = tokenObj["symbol"].toString();
                token.name = tokenObj["name"].toString();
                token.contractAddress = tokenObj["token_id"].toString();
                token.balance = tokenObj["balance"].toString();
                token.decimals = tokenObj["decimals"].toInt();

                detectedTokens.append(token);
            }
        }
    }

    reply->deleteLater();
    return detectedTokens;
}

QVector<Token> TokenDetector::detectSPLTokens(const QString &address)
{
    QVector<Token> detectedTokens;

    // Solana getTokenAccountsByOwner RPC call
    QString endpoint = "https://api.mainnet-beta.solana.com";

    QNetworkAccessManager manager;
    QNetworkRequest request(endpoint);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject rpcRequest;
    rpcRequest["jsonrpc"] = "2.0";
    rpcRequest["method"] = "getTokenAccountsByOwner";

    QJsonArray params;
    params.append(address);

    QJsonObject filterObj;
    filterObj["programId"] = "TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA";
    params.append(filterObj);

    QJsonObject encodingObj;
    encodingObj["encoding"] = "jsonParsed";
    params.append(encodingObj);

    rpcRequest["params"] = params;
    rpcRequest["id"] = 1;

    QJsonDocument doc(rpcRequest);
    QByteArray requestData = doc.toJson(QJsonDocument::Compact);

    QNetworkReply *reply = manager.post(request, requestData);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(response);
        if (responseDoc.isObject()) {
            QJsonObject result = responseDoc.object()["result"].toObject();
            QJsonArray accounts = result["value"].toArray();

            for (const QJsonValue &val : accounts) {
                QJsonObject account = val.toObject();
                QJsonObject accountData = account["account"].toObject()["data"].toObject();
                QJsonObject parsed = accountData["parsed"].toObject();
                QJsonObject info = parsed["info"].toObject();

                Token token;
                token.contractAddress = info["mint"].toString();
                token.balance = info["tokenAmount"].toObject()["uiAmountString"].toString();
                token.decimals = info["tokenAmount"].toObject()["decimals"].toInt();
                token.symbol = "SPL";  // Need additional lookup for symbol
                token.name = "SPL Token";

                detectedTokens.append(token);
            }
        }
    }

    reply->deleteLater();
    return detectedTokens;
}

QVector<Token> TokenDetector::getPopularTokens(const QString &chainType)
{
    if (chainType == "ETH" || chainType == "Ethereum") {
        return popularERC20Tokens;
    } else if (chainType == "TRX" || chainType == "Tron") {
        return popularTRC20Tokens;
    } else if (chainType == "SOL" || chainType == "Solana") {
        return popularSPLTokens;
    }

    return QVector<Token>();
}

bool TokenDetector::addCustomToken(const QString &chainType,
                                  const QString &contractAddress,
                                  const QString &symbol,
                                  const QString &name,
                                  int decimals)
{
    Token token;
    token.symbol = symbol;
    token.name = name;
    token.contractAddress = contractAddress;
    token.balance = "0";
    token.decimals = decimals;

    if (chainType == "ETH") {
        popularERC20Tokens.append(token);
        return true;
    } else if (chainType == "TRX") {
        popularTRC20Tokens.append(token);
        return true;
    } else if (chainType == "SOL") {
        popularSPLTokens.append(token);
        return true;
    }

    return false;
}

QString TokenDetector::encodeERC20BalanceOf(const QString &address)
{
    // balanceOf(address) = 0x70a08231 + padded address
    QString cleanAddress = address;
    if (cleanAddress.startsWith("0x")) {
        cleanAddress = cleanAddress.mid(2);
    }

    return "0x70a08231" + cleanAddress.rightJustified(64, '0');
}

QString TokenDetector::encodeERC20Transfer(const QString &to, const QString &amount)
{
    // transfer(address,uint256) = 0xa9059cbb + padded address + padded amount
    QString cleanAddress = to;
    if (cleanAddress.startsWith("0x")) {
        cleanAddress = cleanAddress.mid(2);
    }

    QString amountHex = QString::number(amount.toULongLong(), 16).rightJustified(64, '0');

    return "0xa9059cbb" + cleanAddress.rightJustified(64, '0') + amountHex;
}

QString TokenDetector::encodeTRC20BalanceOf(const QString &address)
{
    return encodeERC20BalanceOf(address);
}

QString TokenDetector::encodeTRC20Transfer(const QString &to, const QString &amount)
{
    return encodeERC20Transfer(to, amount);
}
