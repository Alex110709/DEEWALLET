/**
 * Simple test program to generate a valid keyfile
 */
#include "core/KeyfileManager.h"
#include "core/BIP39.h"
#include <QCoreApplication>
#include <QDebug>
#include <QJsonDocument>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    // Generate a test mnemonic
    BIP39 bip39;
    std::string mnemonic = bip39.generateMnemonic(256); // 24 words
    
    qDebug() << "Generated mnemonic:" << QString::fromStdString(mnemonic);
    
    // Create keyfile with password "test123"
    KeyfileManager keyfileManager;
    QString keyfilePath = "build/test_wallet.keyfile";
    QString password = "test123";
    
    QByteArray mnemonicData = QByteArray::fromStdString(mnemonic);
    
    bool success = keyfileManager.encryptAndSave(keyfilePath, mnemonicData, password);
    
    if (success) {
        qDebug() << "✓ Keyfile created successfully:" << keyfilePath;
        qDebug() << "Password: test123";
        
        // Test decryption
        QByteArray decrypted = keyfileManager.loadAndDecrypt(keyfilePath, password);
        if (!decrypted.isEmpty()) {
            qDebug() << "✓ Decryption test passed!";
            qDebug() << "Decrypted mnemonic:" << QString::fromUtf8(decrypted);
        } else {
            qDebug() << "✗ Decryption test failed!";
        }
    } else {
        qDebug() << "✗ Failed to create keyfile";
    }
    
    return 0;
}
