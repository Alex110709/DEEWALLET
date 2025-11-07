/**
 * DEE WALLET - Keyfile Manager Implementation
 */

#include "KeyfileManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QDateTime>
#include <openssl/evp.h>
#include <openssl/rand.h>

KeyfileManager::KeyfileManager()
{
}

KeyfileManager::~KeyfileManager()
{
}

bool KeyfileManager::encryptAndSave(const QString &filePath,
                                     const QByteArray &data,
                                     const QString &password)
{
    // Generate salt and IV
    QByteArray salt = generateSalt();
    QByteArray iv;
    QByteArray tag;

    // Derive encryption key from password
    QByteArray key = deriveKey(password, salt);

    // Encrypt data
    QByteArray ciphertext = encryptAES256GCM(data, key, iv, tag);

    // Create keyfile JSON
    QJsonObject keyfile;
    keyfile["version"] = 1;
    keyfile["createdAt"] = QDateTime::currentMSecsSinceEpoch();
    keyfile["updatedAt"] = QDateTime::currentMSecsSinceEpoch();

    QJsonObject crypto;
    crypto["cipher"] = "aes-256-gcm";
    crypto["ciphertext"] = QString::fromLatin1(ciphertext.toBase64());
    crypto["iv"] = QString::fromLatin1(iv.toBase64());
    crypto["tag"] = QString::fromLatin1(tag.toBase64());
    crypto["kdf"] = "pbkdf2";

    QJsonObject kdfParams;
    kdfParams["iterations"] = PBKDF2_ITERATIONS;
    kdfParams["salt"] = QString::fromLatin1(salt.toBase64());
    crypto["kdfparams"] = kdfParams;

    keyfile["crypto"] = crypto;

    // Write to file
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonDocument doc(keyfile);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    // Securely wipe key from memory
    OPENSSL_cleanse(key.data(), key.size());

    return true;
}

QByteArray KeyfileManager::loadAndDecrypt(const QString &filePath,
                                          const QString &password)
{
    // Read keyfile
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QByteArray();
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        return QByteArray();
    }

    QJsonObject keyfile = doc.object();
    QJsonObject crypto = keyfile["crypto"].toObject();

    // Extract encryption parameters
    QByteArray ciphertext = QByteArray::fromBase64(crypto["ciphertext"].toString().toLatin1());
    QByteArray iv = QByteArray::fromBase64(crypto["iv"].toString().toLatin1());
    QByteArray tag = QByteArray::fromBase64(crypto["tag"].toString().toLatin1());

    QJsonObject kdfParams = crypto["kdfparams"].toObject();
    QByteArray salt = QByteArray::fromBase64(kdfParams["salt"].toString().toLatin1());

    // Derive decryption key
    QByteArray key = deriveKey(password, salt);

    // Decrypt
    QByteArray plaintext = decryptAES256GCM(ciphertext, key, iv, tag);

    // Securely wipe key
    OPENSSL_cleanse(key.data(), key.size());

    return plaintext;
}

bool KeyfileManager::validateKeyfile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        return false;
    }

    QJsonObject keyfile = doc.object();

    // Check required fields
    if (!keyfile.contains("version") || !keyfile.contains("crypto")) {
        return false;
    }

    QJsonObject crypto = keyfile["crypto"].toObject();
    if (!crypto.contains("cipher") || !crypto.contains("ciphertext") ||
        !crypto.contains("kdf")) {
        return false;
    }

    return true;
}

QJsonObject KeyfileManager::getKeyfileMetadata(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QJsonObject();
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        return QJsonObject();
    }

    QJsonObject keyfile = doc.object();

    // Return safe metadata (no sensitive data)
    QJsonObject metadata;
    metadata["version"] = keyfile["version"];
    metadata["createdAt"] = keyfile["createdAt"];
    metadata["updatedAt"] = keyfile["updatedAt"];

    return metadata;
}

QByteArray KeyfileManager::deriveKey(const QString &password, const QByteArray &salt)
{
    QByteArray key(KEY_SIZE, 0);

    PKCS5_PBKDF2_HMAC(
        password.toUtf8().constData(),
        password.toUtf8().size(),
        reinterpret_cast<const unsigned char*>(salt.constData()),
        salt.size(),
        PBKDF2_ITERATIONS,
        EVP_sha256(),
        KEY_SIZE,
        reinterpret_cast<unsigned char*>(key.data())
    );

    return key;
}

QByteArray KeyfileManager::encryptAES256GCM(const QByteArray &plaintext,
                                            const QByteArray &key,
                                            QByteArray &iv,
                                            QByteArray &tag)
{
    iv = generateIV();
    tag.resize(TAG_SIZE);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        return QByteArray();
    }

    QByteArray ciphertext(plaintext.size() + EVP_MAX_BLOCK_LENGTH, 0);
    int len = 0;
    int ciphertext_len = 0;

    // Initialize encryption
    EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr,
                       reinterpret_cast<const unsigned char*>(key.constData()),
                       reinterpret_cast<const unsigned char*>(iv.constData()));

    // Encrypt
    EVP_EncryptUpdate(ctx,
                      reinterpret_cast<unsigned char*>(ciphertext.data()),
                      &len,
                      reinterpret_cast<const unsigned char*>(plaintext.constData()),
                      plaintext.size());
    ciphertext_len = len;

    // Finalize
    EVP_EncryptFinal_ex(ctx,
                        reinterpret_cast<unsigned char*>(ciphertext.data()) + len,
                        &len);
    ciphertext_len += len;

    // Get authentication tag
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, TAG_SIZE, tag.data());

    EVP_CIPHER_CTX_free(ctx);

    ciphertext.resize(ciphertext_len);
    return ciphertext;
}

QByteArray KeyfileManager::decryptAES256GCM(const QByteArray &ciphertext,
                                            const QByteArray &key,
                                            const QByteArray &iv,
                                            const QByteArray &tag)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        return QByteArray();
    }

    QByteArray plaintext(ciphertext.size() + EVP_MAX_BLOCK_LENGTH, 0);
    int len = 0;
    int plaintext_len = 0;

    // Initialize decryption
    EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr,
                       reinterpret_cast<const unsigned char*>(key.constData()),
                       reinterpret_cast<const unsigned char*>(iv.constData()));

    // Decrypt
    EVP_DecryptUpdate(ctx,
                      reinterpret_cast<unsigned char*>(plaintext.data()),
                      &len,
                      reinterpret_cast<const unsigned char*>(ciphertext.constData()),
                      ciphertext.size());
    plaintext_len = len;

    // Set authentication tag
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, TAG_SIZE,
                        const_cast<char*>(tag.constData()));

    // Finalize (this verifies the tag)
    int ret = EVP_DecryptFinal_ex(ctx,
                                   reinterpret_cast<unsigned char*>(plaintext.data()) + len,
                                   &len);

    EVP_CIPHER_CTX_free(ctx);

    if (ret <= 0) {
        // Authentication failed
        return QByteArray();
    }

    plaintext_len += len;
    plaintext.resize(plaintext_len);
    return plaintext;
}

QByteArray KeyfileManager::generateSalt()
{
    QByteArray salt(SALT_SIZE, 0);
    RAND_bytes(reinterpret_cast<unsigned char*>(salt.data()), SALT_SIZE);
    return salt;
}

QByteArray KeyfileManager::generateIV()
{
    QByteArray iv(IV_SIZE, 0);
    RAND_bytes(reinterpret_cast<unsigned char*>(iv.data()), IV_SIZE);
    return iv;
}
