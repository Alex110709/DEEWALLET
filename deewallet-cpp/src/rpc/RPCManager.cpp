/**
 * DEE WALLET - RPC Manager Implementation
 */

#include "RPCManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

RPCManager::RPCManager()
    : healthCheckRunning(false)
{
}

RPCManager::~RPCManager()
{
    stopHealthCheck();
}

void RPCManager::loadEndpoints(const QString &configPath)
{
    QFile file(configPath);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject mainnet = root["mainnet"].toObject();

    for (const QString &chain : mainnet.keys()) {
        QJsonArray endpointArray = mainnet[chain].toArray();
        QVector<RPCEndpoint> chainEndpoints;

        for (const QJsonValue &val : endpointArray) {
            QJsonObject obj = val.toObject();
            RPCEndpoint ep;
            ep.name = obj["name"].toString();
            ep.url = obj["url"].toString();
            ep.priority = obj["priority"].toInt();
            ep.rateLimit = obj["rateLimit"].toInt();
            ep.isHealthy = true;
            ep.lastCheck = 0;
            ep.failureCount = 0;

            chainEndpoints.append(ep);
        }

        endpoints[chain] = chainEndpoints;
        currentEndpointIndex[chain] = 0;
    }
}

QString RPCManager::getEndpoint(const QString &chainType)
{
    if (!endpoints.contains(chainType) || endpoints[chainType].isEmpty()) {
        return QString();
    }

    int index = currentEndpointIndex[chainType];
    return endpoints[chainType][index].url;
}

bool RPCManager::setEndpoint(const QString &chainType, const QString &url)
{
    if (!endpoints.contains(chainType)) {
        return false;
    }

    QVector<RPCEndpoint> &chainEndpoints = endpoints[chainType];
    for (int i = 0; i < chainEndpoints.size(); ++i) {
        if (chainEndpoints[i].url == url) {
            currentEndpointIndex[chainType] = i;
            return true;
        }
    }

    return false;
}

QVector<RPCEndpoint> RPCManager::getAvailableEndpoints(const QString &chainType)
{
    if (!endpoints.contains(chainType)) {
        return QVector<RPCEndpoint>();
    }

    return endpoints[chainType];
}

void RPCManager::startHealthCheck()
{
    // TODO: Implement periodic health checking
    healthCheckRunning = true;
}

void RPCManager::stopHealthCheck()
{
    healthCheckRunning = false;
}

void RPCManager::reportFailure(const QString &chainType, const QString &error)
{
    if (!endpoints.contains(chainType)) {
        return;
    }

    int index = currentEndpointIndex[chainType];
    RPCEndpoint &ep = endpoints[chainType][index];
    ep.failureCount++;

    // Switch to next endpoint after 3 failures
    if (ep.failureCount >= 3) {
        switchToNextEndpoint(chainType);
    }
}

void RPCManager::checkEndpointHealth(const QString &chainType, RPCEndpoint &endpoint)
{
    // TODO: Implement actual health check (e.g., eth_blockNumber)
}

void RPCManager::switchToNextEndpoint(const QString &chainType)
{
    if (!endpoints.contains(chainType)) {
        return;
    }

    QVector<RPCEndpoint> &chainEndpoints = endpoints[chainType];
    int &index = currentEndpointIndex[chainType];

    // Find next healthy endpoint
    int startIndex = index;
    do {
        index = (index + 1) % chainEndpoints.size();
        if (chainEndpoints[index].isHealthy) {
            chainEndpoints[index].failureCount = 0;
            return;
        }
    } while (index != startIndex);
}

QMap<QString, QString> RPCManager::getStats()
{
    QMap<QString, QString> stats;

    for (const QString &chain : endpoints.keys()) {
        int index = currentEndpointIndex[chain];
        if (index < endpoints[chain].size()) {
            stats[chain] = endpoints[chain][index].name;
        }
    }

    return stats;
}
