/**
 * DEE WALLET - Create Wallet Dialog
 */

#ifndef CREATEWALLETDIALOG_H
#define CREATEWALLETDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QGridLayout>
#include <QLabel>
#include <QVector>

class CreateWalletDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateWalletDialog(QWidget *parent = nullptr);
    QString getMnemonic() const { return createdMnemonic; }

private slots:
    void onNextClicked();
    void onCreateWallet();
    void onBackClicked();

private:
    void setupUI();
    void setupPasswordPage();
    void setupMnemonicPage();
    void showMnemonicWords(const QString &mnemonic);

    QStackedWidget *stackedWidget;
    
    // Password page widgets
    QWidget *passwordPage;
    QLineEdit *passwordInput;
    QLineEdit *confirmPasswordInput;
    QPushButton *nextButton;
    QPushButton *cancelButton;
    
    // Mnemonic page widgets
    QWidget *mnemonicPage;
    QGridLayout *mnemonicGrid;
    QVector<QLabel*> wordLabels;
    QPushButton *createButton;
    QPushButton *backButton;
    
    QString createdMnemonic;
};

#endif // CREATEWALLETDIALOG_H
