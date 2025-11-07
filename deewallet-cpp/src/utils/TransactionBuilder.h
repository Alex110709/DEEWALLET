/**
 * DEE WALLET - Transaction Builder Utilities
 * Helper functions for creating blockchain transactions
 */

#ifndef TRANSACTIONBUILDER_H
#define TRANSACTIONBUILDER_H

#include <QString>
#include <QByteArray>
#include <QVector>

// Bitcoin UTXO
struct UTXO {
    QString txid;
    uint32_t vout;
    QString scriptPubKey;
    qint64 value;  // satoshis
};

// Ethereum transaction
struct EthereumTx {
    QString nonce;
    QString gasPrice;
    QString gasLimit;
    QString to;
    QString value;
    QString data;
    int chainId;
};

class TransactionBuilder {
public:
    // Bitcoin transaction building
    static QByteArray buildBitcoinTransaction(const QVector<UTXO> &inputs,
                                             const QString &toAddress,
                                             qint64 amount,
                                             qint64 fee,
                                             const QString &changeAddress);

    static QByteArray signBitcoinInput(const QByteArray &tx,
                                      int inputIndex,
                                      const QByteArray &privateKey,
                                      const QString &scriptPubKey);

    // Ethereum RLP encoding
    static QByteArray encodeRLP(const EthereumTx &tx);
    static QByteArray signEthereumTransaction(const EthereumTx &tx,
                                             const QByteArray &privateKey);

    // Tron protobuf
    static QByteArray buildTronTransaction(const QString &from,
                                          const QString &to,
                                          qint64 amount);
    static QByteArray signTronTransaction(const QByteArray &txBytes,
                                         const QByteArray &privateKey);

    // Solana transaction
    static QByteArray buildSolanaTransaction(const QString &from,
                                            const QString &to,
                                            qint64 lamports);
    static QByteArray signSolanaTransaction(const QByteArray &message,
                                           const QByteArray &privateKey);

    // Utility functions
    static QByteArray encodeVarint(quint64 value);
    static QByteArray encodeRLPString(const QByteArray &data);
    static QByteArray encodeRLPList(const QVector<QByteArray> &items);
    static QString bytesToHex(const QByteArray &bytes);
    static QByteArray hexToBytes(const QString &hex);
};

#endif // TRANSACTIONBUILDER_H
