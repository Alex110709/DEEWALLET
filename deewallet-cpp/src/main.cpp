/**
 * DEE WALLET - C++ Multi-chain Cryptocurrency Wallet
 * Main Entry Point
 */

#include <QApplication>
#include <QDir>
#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application metadata
    QApplication::setApplicationName("DEE WALLET");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("DEEWALLET Team");

    // Create main window
    MainWindow mainWindow;
    mainWindow.setWindowTitle("DEE WALLET");
    mainWindow.resize(1200, 800);
    mainWindow.show();

    return app.exec();
}
