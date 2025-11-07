/**
 * DEE WALLET - BIP39 Mnemonic Implementation
 * Generates and validates 12/24 word recovery phrases
 */

#ifndef BIP39_H
#define BIP39_H

#include <QString>
#include <QByteArray>
#include <QVector>

class BIP39 {
public:
    BIP39();
    ~BIP39();

    // Generate mnemonic from entropy
    QString generateMnemonic(int wordCount = 12);

    // Convert mnemonic to seed (PBKDF2)
    QByteArray mnemonicToSeed(const QString &mnemonic, const QString &passphrase = "");

    // Validate mnemonic
    bool validateMnemonic(const QString &mnemonic);

    // Get word by index
    QString getWord(int index);

    // Get word index
    int getWordIndex(const QString &word);

private:
    // BIP39 wordlist (2048 words)
    static const char* const WORDLIST[];
    static const int WORDLIST_SIZE = 2048;

    // Generate entropy
    QByteArray generateEntropy(int bits);

    // Calculate checksum
    QByteArray calculateChecksum(const QByteArray &entropy);

    // Entropy to mnemonic
    QString entropyToMnemonic(const QByteArray &entropy);

    // Mnemonic to entropy
    QByteArray mnemonicToEntropy(const QString &mnemonic);
};

#endif // BIP39_H
