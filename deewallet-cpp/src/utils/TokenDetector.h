/**
 * DEE WALLET - Token Detector
 * Detect and manage ERC-20, TRC-20, and SPL tokens
 */

#ifndef TOKENDETECTOR_H
#define TOKENDETECTOR_H

#include <QString>
#include <QVector>
#include "../chains/ChainAdapter.h"

class TokenDetector {
public:
    // Detect ERC-20 tokens for Ethereum address
    static QVector<Token> detectERC20Tokens(const QString &address);

    // Detect TRC-20 tokens for Tron address
    static QVector<Token> detectTRC20Tokens(const QString &address);

    // Detect SPL tokens for Solana address
    static QVector<Token> detectSPLTokens(const QString &address);

    // Get popular token list
    static QVector<Token> getPopularTokens(const QString &chainType);

    // Add custom token
    static bool addCustomToken(const QString &chainType,
                              const QString &contractAddress,
                              const QString &symbol,
                              const QString &name,
                              int decimals);

    // ERC-20 ABI encoding
    static QString encodeERC20BalanceOf(const QString &address);
    static QString encodeERC20Transfer(const QString &to, const QString &amount);

    // TRC-20 ABI encoding (same as ERC-20)
    static QString encodeTRC20BalanceOf(const QString &address);
    static QString encodeTRC20Transfer(const QString &to, const QString &amount);

private:
    static QVector<Token> popularERC20Tokens;
    static QVector<Token> popularTRC20Tokens;
    static QVector<Token> popularSPLTokens;
};

#endif // TOKENDETECTOR_H
