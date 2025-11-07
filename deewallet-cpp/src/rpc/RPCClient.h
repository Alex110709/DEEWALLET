/**
 * DEE WALLET - RPC Client
 * Qt-based HTTP JSON-RPC client
 */

#ifndef RPCCLIENT_H
#define RPCCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonValue>

class RPCClient : public QObject {
    Q_OBJECT

public:
    explicit RPCClient(const QString &endpoint, QObject *parent = nullptr);

    // Synchronous call (blocking)
    QJsonValue call(const QString &method, const QJsonValue &params);

    // Asynchronous call
    void callAsync(const QString &method,
                   const QJsonValue &params,
                   std::function<void(const QJsonValue&)> callback);

signals:
    void requestCompleted(const QJsonValue &result);
    void requestFailed(const QString &error);

private:
    QNetworkAccessManager *networkManager;
    QString endpoint;
    int requestId;
};

#endif // RPCCLIENT_H
