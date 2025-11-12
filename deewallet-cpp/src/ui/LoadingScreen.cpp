/**
 * DEE WALLET - Loading Screen Implementation
 * Exodus-inspired design
 */

#include "LoadingScreen.h"
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QApplication>
#include <QDebug>

LoadingScreen::LoadingScreen(QWidget *parent)
    : QWidget(parent)
    , messageLabel(new QLabel("처리 중...", this))
    , spinnerLabel(new QLabel(this))
    , animationTimer(new QTimer(this))
    , animationFrame(0)
{
    setupUI();
    connect(animationTimer, &QTimer::timeout, this, &LoadingScreen::updateAnimation);
}

void LoadingScreen::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(30);
    
    // Spinner label - simpler and cleaner
    spinnerLabel->setStyleSheet(R"(
        QLabel {
            background: transparent;
            font-size: 48px;
            color: #3B82F6;
            border: none;
        }
    )");
    spinnerLabel->setAlignment(Qt::AlignCenter);
    spinnerLabel->setMinimumSize(80, 80);
    mainLayout->addWidget(spinnerLabel);
    
    // Message label - cleaner typography
    messageLabel->setStyleSheet(R"(
        QLabel {
            background: transparent;
            font-size: 16px;
            font-weight: 500;
            color: #94A3B8;
            border: none;
        }
    )");
    messageLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(messageLabel);
    
    // Solid dark background
    setStyleSheet(R"(
        QWidget {
            background-color: #0F172A;
        }
    )");
}

void LoadingScreen::setMessage(const QString &message)
{
    messageLabel->setText(message);
}

void LoadingScreen::startAnimation()
{
    animationFrame = 0;
    animationTimer->stop(); // Stop if already running
    animationTimer->start(150); // 150ms per frame for smoother animation
    updateAnimation();
    qDebug() << "[LoadingScreen] ✓ Animation started, timer active:" << animationTimer->isActive();
}

void LoadingScreen::stopAnimation()
{
    animationTimer->stop();
    qDebug() << "[LoadingScreen] ✗ Animation stopped at frame:" << animationFrame;
}

void LoadingScreen::updateAnimation()
{
    // Simple dots animation - easier to see
    const QStringList spinnerFrames = {
        "●○○○○", "○●○○○", "○○●○○", "○○○●○", "○○○○●",
        "○○○●○", "○○●○○", "○●○○○"
    };
    
    int index = animationFrame % spinnerFrames.size();
    QString frame = spinnerFrames[index];
    
    spinnerLabel->setText(frame);
    spinnerLabel->repaint(); // Force immediate repaint
    
    qDebug() << "[LoadingScreen] Frame" << animationFrame << ":" << frame 
             << "| Timer active:" << animationTimer->isActive()
             << "| Visible:" << isVisible();
    
    animationFrame++;
}
