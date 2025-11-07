/**
 * DEE WALLET - Tron Chain Adapter
 */

#ifndef TRONADAPTER_H
#define TRONADAPTER_H

#include "ChainAdapter.h"

class TronAdapter : public ChainAdapter {
public:
    explicit TronAdapter(const QString &rpcUrl);

    QString deriveAddress(const QByteArray &publicKey) override;
    bool validateAddress(const QString &address) override;
    QString getBalance(const QString &address) override;
    QVector<Token> getTokens(const QString &address) override;
    QString createTransaction(const QString &from,
                             const QString &to,
                             const QString &amount,
                             const QString &gasPrice) override;
    QString signTransaction(const QString &rawTx, const QByteArray &privateKey) override;
    QString broadcastTransaction(const QString &signedTx) override;
    QVector<Transaction> getTransactionHistory(const QString &address, int limit) override;
    QString estimateFee(const QString &from, const QString &to, const QString &amount) override;
};

#endif // TRONADAPTER_H
