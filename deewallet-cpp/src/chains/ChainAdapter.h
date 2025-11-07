/**
 * DEE WALLET - Chain Adapter Interface
 * Abstract base class for blockchain adapters
 */

#ifndef CHAINADAPTER_H
#define CHAINADAPTER_H

#include <QString>
#include <QByteArray>
#include <QVector>

struct Token {
    QString symbol;
    QString name;
    QString contractAddress;
    QString balance;
    int decimals;
};

struct Transaction {
    QString hash;
    QString from;
    QString to;
    QString value;
    QString chainType;
    qint64 timestamp;
    QString status; // "pending", "confirmed", "failed"
    QString fee;
};

class ChainAdapter {
public:
    virtual ~ChainAdapter() = default;

    // Address generation
    virtual QString deriveAddress(const QByteArray &publicKey) = 0;
    virtual bool validateAddress(const QString &address) = 0;

    // Balance queries
    virtual QString getBalance(const QString &address) = 0;
    virtual QVector<Token> getTokens(const QString &address) = 0;

    // Transaction operations
    virtual QString createTransaction(const QString &from,
                                      const QString &to,
                                      const QString &amount,
                                      const QString &gasPrice = QString()) = 0;

    virtual QString signTransaction(const QString &rawTx,
                                    const QByteArray &privateKey) = 0;

    virtual QString broadcastTransaction(const QString &signedTx) = 0;

    virtual QVector<Transaction> getTransactionHistory(const QString &address,
                                                       int limit = 10) = 0;

    // Fee estimation
    virtual QString estimateFee(const QString &from,
                               const QString &to,
                               const QString &amount) = 0;

protected:
    QString rpcUrl;
};

#endif // CHAINADAPTER_H
