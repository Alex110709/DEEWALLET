/**
 * DEE WALLET - RPC Client Implementation
 */

#include "RPCClient.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>

RPCClient::RPCClient(const QString &endpoint, QObject *parent)
    : QObject(parent)
    , networkManager(new QNetworkAccessManager(this))
    , endpoint(endpoint)
    , requestId(1)
{
}

QJsonValue RPCClient::call(const QString &method, const QJsonValue &params)
{
    QJsonObject request;
    request["jsonrpc"] = "2.0";
    request["id"] = requestId++;
    request["method"] = method;
    request["params"] = params;

    QNetworkRequest netRequest(endpoint);
    netRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonDocument doc(request);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    // Synchronous request
    QNetworkReply *reply = networkManager->post(netRequest, data);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QJsonValue result;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument responseDoc = QJsonDocument::fromJson(response);
        if (responseDoc.isObject()) {
            QJsonObject responseObj = responseDoc.object();
            result = responseObj["result"];
        }
    }

    reply->deleteLater();
    return result;
}

void RPCClient::callAsync(const QString &method,
                          const QJsonValue &params,
                          std::function<void(const QJsonValue&)> callback)
{
    QJsonObject request;
    request["jsonrpc"] = "2.0";
    request["id"] = requestId++;
    request["method"] = method;
    request["params"] = params;

    QNetworkRequest netRequest(endpoint);
    netRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonDocument doc(request);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    QNetworkReply *reply = networkManager->post(netRequest, data);

    connect(reply, &QNetworkReply::finished, this, [this, reply, callback]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(response);
            if (responseDoc.isObject()) {
                QJsonObject responseObj = responseDoc.object();
                QJsonValue result = responseObj["result"];
                callback(result);
                emit requestCompleted(result);
            }
        } else {
            emit requestFailed(reply->errorString());
        }
        reply->deleteLater();
    });
}
