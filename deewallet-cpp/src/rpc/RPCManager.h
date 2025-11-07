/**
 * DEE WALLET - RPC Manager
 * Manages multiple RPC endpoints with health checking and failover
 */

#ifndef RPCMANAGER_H
#define RPCMANAGER_H

#include <QString>
#include <QVector>
#include <QMap>

struct RPCEndpoint {
    QString name;
    QString url;
    int priority;
    int rateLimit;
    bool isHealthy;
    qint64 lastCheck;
    int failureCount;
};

class RPCManager {
public:
    RPCManager();
    ~RPCManager();

    // Endpoint management
    void loadEndpoints(const QString &configPath);
    QString getEndpoint(const QString &chainType);
    bool setEndpoint(const QString &chainType, const QString &url);
    QVector<RPCEndpoint> getAvailableEndpoints(const QString &chainType);

    // Health checking
    void startHealthCheck();
    void stopHealthCheck();
    void reportFailure(const QString &chainType, const QString &error);

    // Statistics
    QMap<QString, QString> getStats();

private:
    void checkEndpointHealth(const QString &chainType, RPCEndpoint &endpoint);
    void switchToNextEndpoint(const QString &chainType);

    QMap<QString, QVector<RPCEndpoint>> endpoints;
    QMap<QString, int> currentEndpointIndex;
    bool healthCheckRunning;
};

#endif // RPCMANAGER_H
