/**
 * DEE WALLET - Welcome Screen
 * Displays keyfile list and wallet creation/import options
 */

#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QVector>

struct KeyfileInfo {
    QString filename;
    QString filepath;
    qint64 createdAt;
    qint64 updatedAt;
    qint64 size;
    bool isValid;
};

class WelcomeScreen : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeScreen(QWidget *parent = nullptr);

signals:
    void walletCreated(const QString &mnemonic);
    void walletImported(const QString &mnemonic);
    void keyfileSelected(const QString &filepath);

private slots:
    void onCreateWallet();
    void onImportWallet();
    void onKeyfileDoubleClicked(QListWidgetItem *item);
    void refreshKeyfileList();

private:
    void setupUI();
    QVector<KeyfileInfo> scanKeyfiles();
    QString formatFileSize(qint64 bytes);
    QString formatDate(qint64 timestamp);

    QListWidget *keyfileList;
    QPushButton *createButton;
    QPushButton *importButton;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
};

#endif // WELCOMESCREEN_H
