/**
 * DEE WALLET - Main Window
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

class WelcomeScreen;
class WalletDetailScreen;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onWalletCreated(const QString &mnemonic);
    void onWalletImported(const QString &mnemonic);
    void onBackToWelcome();

private:
    void setupUI();
    void showWelcomeScreen();
    void showWalletScreen(const QString &mnemonic);

    QStackedWidget *stackedWidget;
    WelcomeScreen *welcomeScreen;
    WalletDetailScreen *walletDetailScreen;
    QString currentMnemonic;
};

#endif // MAINWINDOW_H
