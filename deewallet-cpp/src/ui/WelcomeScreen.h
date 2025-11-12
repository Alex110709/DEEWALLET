/**
 * DEE WALLET - Welcome Screen
 * Displays keyfile list and wallet creation/import options
 */

#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include <QScrollArea>
#include <QVBoxLayout>

struct KeyfileInfo {
    QString filename;
    QString filepath;
    qint64 createdAt;
    qint64 updatedAt;
    qint64 size;
    bool isValid;
};

// Custom clickable card widget for keyfiles
class KeyfileCard : public QWidget
{
    Q_OBJECT

public:
    explicit KeyfileCard(const KeyfileInfo &info, QWidget *parent = nullptr);

signals:
    void clicked(const QString &filepath);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    KeyfileInfo keyfileInfo;
    QLabel *nameLabel;
    QLabel *dateLabel;
    bool isHovered;
    
    void updateStyle();
};

class WelcomeScreen : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeScreen(QWidget *parent = nullptr);

signals:
    void walletCreated(const QString &mnemonic);
    void walletImported(const QString &mnemonic);
    void loadingStarted(const QString &filepath, const QString &password);

private slots:
    void onCreateWallet();
    void onImportWallet();
    void onKeyfileClicked(const QString &filepath);
    void refreshKeyfileList();

private:
    void setupUI();
    QVector<KeyfileInfo> scanKeyfiles();
    QString formatDate(qint64 timestamp);
    void showPasswordDialog(const QString &filepath);

    QScrollArea *scrollArea;
    QWidget *keyfileContainer;
    QVBoxLayout *keyfileLayout;
    QPushButton *createButton;
    QPushButton *importButton;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
};

#endif // WELCOMESCREEN_H
