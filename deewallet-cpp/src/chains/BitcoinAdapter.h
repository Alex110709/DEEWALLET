/**
 * DEE WALLET - Bitcoin Chain Adapter
 */

#ifndef BITCOINADAPTER_H
#define BITCOINADAPTER_H

#include "ChainAdapter.h"

class BitcoinAdapter : public ChainAdapter {
public:
    explicit BitcoinAdapter(const QString &rpcUrl, bool isTestnet = false);

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

private:
    bool isTestnet;
};

#endif // BITCOINADAPTER_H
