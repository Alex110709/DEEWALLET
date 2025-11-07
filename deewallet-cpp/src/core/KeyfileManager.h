/**
 * DEE WALLET - Keyfile Manager
 * AES-256-GCM encryption with PBKDF2 key derivation
 */

#ifndef KEYFILEMANAGER_H
#define KEYFILEMANAGER_H

#include <QString>
#include <QJsonObject>

class KeyfileManager
{
public:
    KeyfileManager();
    ~KeyfileManager();

    // Encryption/Decryption
    bool encryptAndSave(const QString &filePath,
                        const QByteArray &data,
                        const QString &password);

    QByteArray loadAndDecrypt(const QString &filePath,
                              const QString &password);

    // Keyfile validation
    bool validateKeyfile(const QString &filePath);
    QJsonObject getKeyfileMetadata(const QString &filePath);

private:
    QByteArray deriveKey(const QString &password, const QByteArray &salt);
    QByteArray encryptAES256GCM(const QByteArray &plaintext,
                                const QByteArray &key,
                                QByteArray &iv,
                                QByteArray &tag);
    QByteArray decryptAES256GCM(const QByteArray &ciphertext,
                                const QByteArray &key,
                                const QByteArray &iv,
                                const QByteArray &tag);
    QByteArray generateSalt();
    QByteArray generateIV();

    static constexpr int PBKDF2_ITERATIONS = 100000;
    static constexpr int KEY_SIZE = 32; // 256 bits
    static constexpr int SALT_SIZE = 16;
    static constexpr int IV_SIZE = 12;
    static constexpr int TAG_SIZE = 16;
};

#endif // KEYFILEMANAGER_H
