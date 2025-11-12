/**
 * DEE WALLET - Loading Screen
 * Shows loading animation while processing
 */

#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

class LoadingScreen : public QWidget
{
    Q_OBJECT

public:
    explicit LoadingScreen(QWidget *parent = nullptr);
    void setMessage(const QString &message);
    void startAnimation();
    void stopAnimation();

private:
    void setupUI();
    void updateAnimation();

    QLabel *messageLabel;
    QLabel *spinnerLabel;
    QTimer *animationTimer;
    int animationFrame;
};

#endif // LOADINGSCREEN_H
