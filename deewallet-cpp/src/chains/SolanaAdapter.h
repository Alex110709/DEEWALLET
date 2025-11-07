/**
 * DEE WALLET - Solana Chain Adapter
 */

#ifndef SOLANAADAPTER_H
#define SOLANAADAPTER_H

#include "ChainAdapter.h"

class SolanaAdapter : public ChainAdapter {
public:
    explicit SolanaAdapter(const QString &rpcUrl);

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
    // SPL Token helpers
    QVector<Token> parseSPLTokens(const QString &address);
};

#endif // SOLANAADAPTER_H
