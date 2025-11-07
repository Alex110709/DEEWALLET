/**
 * DEE WALLET - Ethereum Chain Adapter
 */

#ifndef ETHEREUMADAPTER_H
#define ETHEREUMADAPTER_H

#include "ChainAdapter.h"

class EthereumAdapter : public ChainAdapter {
public:
    explicit EthereumAdapter(const QString &rpcUrl);

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
    // ERC-20 helpers
    QString encodeERC20Transfer(const QString &to, const QString &amount);
    QVector<Token> parseERC20Balances(const QString &address);
};

#endif // ETHEREUMADAPTER_H
