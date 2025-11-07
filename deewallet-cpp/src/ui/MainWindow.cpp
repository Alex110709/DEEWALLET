/**
 * DEE WALLET - Main Window Implementation
 */

#include "MainWindow.h"
#include "WelcomeScreen.h"
#include "WalletDetailScreen.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , stackedWidget(new QStackedWidget(this))
    , welcomeScreen(nullptr)
    , walletDetailScreen(nullptr)
{
    setupUI();
    showWelcomeScreen();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setCentralWidget(stackedWidget);

    // Set minimum size
    setMinimumSize(900, 600);

    // Apply dark theme stylesheet
    setStyleSheet(R"(
        QMainWindow {
            background-color: #1a1a1a;
        }
        QWidget {
            color: #ffffff;
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif;
        }
        QPushButton {
            background-color: #0066ff;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 12px 24px;
            font-size: 14px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #0052cc;
        }
        QPushButton:pressed {
            background-color: #003d99;
        }
        QPushButton#secondaryButton {
            background-color: #2a2a2a;
        }
        QPushButton#secondaryButton:hover {
            background-color: #3a3a3a;
        }
    )");
}

void MainWindow::showWelcomeScreen()
{
    if (!welcomeScreen) {
        welcomeScreen = new WelcomeScreen(this);
        connect(welcomeScreen, &WelcomeScreen::walletCreated,
                this, &MainWindow::onWalletCreated);
        connect(welcomeScreen, &WelcomeScreen::walletImported,
                this, &MainWindow::onWalletImported);
        stackedWidget->addWidget(welcomeScreen);
    }

    stackedWidget->setCurrentWidget(welcomeScreen);
}

void MainWindow::showWalletScreen(const QString &mnemonic)
{
    if (!walletDetailScreen) {
        walletDetailScreen = new WalletDetailScreen(mnemonic, this);
        connect(walletDetailScreen, &WalletDetailScreen::backToWelcome,
                this, &MainWindow::onBackToWelcome);
        stackedWidget->addWidget(walletDetailScreen);
    } else {
        // Update existing screen with new mnemonic
        walletDetailScreen->deleteLater();
        walletDetailScreen = new WalletDetailScreen(mnemonic, this);
        connect(walletDetailScreen, &WalletDetailScreen::backToWelcome,
                this, &MainWindow::onBackToWelcome);
        stackedWidget->addWidget(walletDetailScreen);
    }

    currentMnemonic = mnemonic;
    stackedWidget->setCurrentWidget(walletDetailScreen);
}

void MainWindow::onWalletCreated(const QString &mnemonic)
{
    showWalletScreen(mnemonic);
}

void MainWindow::onWalletImported(const QString &mnemonic)
{
    showWalletScreen(mnemonic);
}

void MainWindow::onBackToWelcome()
{
    currentMnemonic.clear();
    showWelcomeScreen();
}
