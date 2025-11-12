#include <QCoreApplication>
#include <QDebug>
#include "src/core/KeyfileManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    KeyfileManager keyfileManager;
    QString filepath = "build/test_wallet.keyfile";
    QString password = "test123";
    
    qDebug() << "Testing keyfile decryption...";
    qDebug() << "File:" << filepath;
    qDebug() << "Password:" << password;
    qDebug() << "";
    
    QByteArray decrypted = keyfileManager.loadAndDecrypt(filepath, password);
    
    if (decrypted.isEmpty()) {
        qDebug() << "\n❌ FAILED: Decryption returned empty result";
        return 1;
    }
    
    qDebug() << "\n✅ SUCCESS: Decryption successful!";
    qDebug() << "Decrypted data:";
    qDebug() << QString::fromUtf8(decrypted);
    
    return 0;
}
