/**
 * DEE WALLET - Main Window
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

class WelcomeScreen;
class WalletDetailScreen;
class ChainDetailScreen;
class LoadingScreen;

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
    void onChainClicked(const QString &chainName, const QString &chainSymbol, const QString &mnemonic);
    void onBackToWalletDetail();
    void onLoadingStarted(const QString &filepath, const QString &password);

private:
    void setupUI();
    void showWelcomeScreen();
    void showWalletScreen(const QString &mnemonic);
    void showChainDetailScreen(const QString &chainName, const QString &chainSymbol, const QString &mnemonic);

    QStackedWidget *stackedWidget;
    WelcomeScreen *welcomeScreen;
    WalletDetailScreen *walletDetailScreen;
    ChainDetailScreen *chainDetailScreen;
    LoadingScreen *loadingScreen;
    QString currentMnemonic;
};

#endif // MAINWINDOW_H
