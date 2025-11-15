/**
 * DEE WALLET - Style Helper
 *
 * Provides reusable stylesheet generation functions for consistent UI styling.
 * Uses DesignTokens for all color, spacing, and typography values.
 *
 * Usage:
 *   #include "StyleHelper.h"
 *
 *   button->setStyleSheet(StyleHelper::primaryButton());
 *   input->setStyleSheet(StyleHelper::inputField());
 */

#ifndef STYLEHELPER_H
#define STYLEHELPER_H

#include "DesignTokens.h"
#include <QString>

/**
 * StyleHelper - Centralized stylesheet generation
 *
 * All functions return ready-to-use Qt StyleSheet strings.
 * Customization can be done by modifying DesignTokens.h
 */
class StyleHelper {
public:
    // ========================================================================
    // BUTTONS
    // ========================================================================

    /**
     * Primary button style (blue, main actions)
     */
    static QString primaryButton() {
        return QString(R"(
            QPushButton {
                background-color: %1;
                color: %2;
                font-size: %3px;
                font-weight: %4;
                border: none;
                border-radius: %5px;
                padding: %6px %7px;
            }
            QPushButton:hover {
                background-color: %8;
            }
            QPushButton:pressed {
                background-color: %9;
            }
            QPushButton:disabled {
                background-color: %10;
                color: %11;
            }
        )")
            .arg(DesignTokens::Colors::BRAND_PRIMARY)
            .arg(DesignTokens::Colors::TEXT_WHITE)
            .arg(DesignTokens::Typography::FONT_SIZE_MD)
            .arg(DesignTokens::Typography::FONT_WEIGHT_SEMIBOLD)
            .arg(DesignTokens::BorderRadius::RADIUS_LG)
            .arg(DesignTokens::Spacing::SPACING_MD)
            .arg(DesignTokens::Spacing::SPACING_XL)
            .arg(DesignTokens::Colors::BRAND_PRIMARY_HOVER)
            .arg(DesignTokens::Colors::BRAND_PRIMARY_PRESSED)
            .arg(DesignTokens::Colors::BG_DISABLED)
            .arg(DesignTokens::Colors::TEXT_DISABLED);
    }

    /**
     * Secondary button style (outlined, secondary actions)
     */
    static QString secondaryButton() {
        return QString(R"(
            QPushButton {
                background-color: %1;
                color: %2;
                font-size: %3px;
                font-weight: %4;
                border: 1px solid %5;
                border-radius: %6px;
                padding: %7px %8px;
            }
            QPushButton:hover {
                background-color: %9;
                color: %10;
                border-color: %5;
            }
            QPushButton:pressed {
                background-color: %11;
            }
            QPushButton:disabled {
                background-color: %12;
                color: %13;
                border-color: %5;
            }
        )")
            .arg(DesignTokens::Colors::BG_SECONDARY)
            .arg(DesignTokens::Colors::TEXT_MUTED)
            .arg(DesignTokens::Typography::FONT_SIZE_MD)
            .arg(DesignTokens::Typography::FONT_WEIGHT_SEMIBOLD)
            .arg(DesignTokens::Colors::BORDER_DEFAULT)
            .arg(DesignTokens::BorderRadius::RADIUS_LG)
            .arg(DesignTokens::Spacing::SPACING_MD)
            .arg(DesignTokens::Spacing::SPACING_XL)
            .arg(DesignTokens::Colors::BG_TERTIARY)
            .arg(DesignTokens::Colors::TEXT_SECONDARY)
            .arg(DesignTokens::Colors::BG_HOVER)
            .arg(DesignTokens::Colors::BG_DISABLED)
            .arg(DesignTokens::Colors::TEXT_DISABLED);
    }

    /**
     * Success button style (green, receive/positive actions)
     */
    static QString successButton() {
        return QString(R"(
            QPushButton {
                background-color: %1;
                color: %2;
                font-size: %3px;
                font-weight: %4;
                border: none;
                border-radius: %5px;
                padding: %6px %7px;
            }
            QPushButton:hover {
                background-color: %8;
            }
            QPushButton:pressed {
                background-color: %9;
            }
            QPushButton:disabled {
                background-color: %10;
                color: %11;
            }
        )")
            .arg(DesignTokens::Colors::SUCCESS)
            .arg(DesignTokens::Colors::TEXT_WHITE)
            .arg(DesignTokens::Typography::FONT_SIZE_MD)
            .arg(DesignTokens::Typography::FONT_WEIGHT_SEMIBOLD)
            .arg(DesignTokens::BorderRadius::RADIUS_LG)
            .arg(DesignTokens::Spacing::SPACING_MD)
            .arg(DesignTokens::Spacing::SPACING_XL)
            .arg(DesignTokens::Colors::SUCCESS_HOVER)
            .arg(DesignTokens::Colors::SUCCESS_PRESSED)
            .arg(DesignTokens::Colors::BG_DISABLED)
            .arg(DesignTokens::Colors::TEXT_DISABLED);
    }

    /**
     * Danger/error button style (red, destructive actions)
     */
    static QString dangerButton() {
        return QString(R"(
            QPushButton {
                background-color: %1;
                color: %2;
                font-size: %3px;
                font-weight: %4;
                border: none;
                border-radius: %5px;
                padding: %6px %7px;
            }
            QPushButton:hover {
                background-color: %8;
            }
            QPushButton:pressed {
                background-color: %8;
            }
        )")
            .arg(DesignTokens::Colors::ERROR)
            .arg(DesignTokens::Colors::TEXT_WHITE)
            .arg(DesignTokens::Typography::FONT_SIZE_MD)
            .arg(DesignTokens::Typography::FONT_WEIGHT_SEMIBOLD)
            .arg(DesignTokens::BorderRadius::RADIUS_LG)
            .arg(DesignTokens::Spacing::SPACING_MD)
            .arg(DesignTokens::Spacing::SPACING_XL)
            .arg(DesignTokens::Colors::ERROR_HOVER);
    }

    /**
     * Ghost button style (transparent, minimal)
     */
    static QString ghostButton() {
        return QString(R"(
            QPushButton {
                background-color: transparent;
                color: %1;
                font-size: %2px;
                font-weight: %3;
                border: none;
                border-radius: %4px;
                padding: %5px %6px;
            }
            QPushButton:hover {
                background-color: %7;
                color: %8;
            }
        )")
            .arg(DesignTokens::Colors::TEXT_MUTED)
            .arg(DesignTokens::Typography::FONT_SIZE_MD)
            .arg(DesignTokens::Typography::FONT_WEIGHT_SEMIBOLD)
            .arg(DesignTokens::BorderRadius::RADIUS_LG)
            .arg(DesignTokens::Spacing::SPACING_MD)
            .arg(DesignTokens::Spacing::SPACING_XL)
            .arg(DesignTokens::Colors::BG_SECONDARY)
            .arg(DesignTokens::Colors::TEXT_BODY);
    }

    // ========================================================================
    // INPUT FIELDS
    // ========================================================================

    /**
     * Standard input field style
     */
    static QString inputField() {
        return QString(R"(
            QLineEdit {
                background-color: %1;
                color: %2;
                font-size: %3px;
                border: 1px solid %4;
                border-radius: %5px;
                padding: %6px %7px;
            }
            QLineEdit:focus {
                border: 1px solid %8;
                background-color: %1;
            }
            QLineEdit:disabled {
                background-color: %9;
                color: %10;
                border-color: %4;
            }
        )")
            .arg(DesignTokens::Colors::BG_SECONDARY)
            .arg(DesignTokens::Colors::TEXT_BODY)
            .arg(DesignTokens::Typography::FONT_SIZE_BASE)
            .arg(DesignTokens::Colors::BORDER_DEFAULT)
            .arg(DesignTokens::BorderRadius::RADIUS_MD)
            .arg(DesignTokens::Spacing::SPACING_MD)
            .arg(DesignTokens::Spacing::SPACING_LG)
            .arg(DesignTokens::Colors::BORDER_FOCUS)
            .arg(DesignTokens::Colors::BG_DISABLED)
            .arg(DesignTokens::Colors::TEXT_DISABLED);
    }

    /**
     * Monospace input field (for addresses, seeds)
     */
    static QString monoInputField() {
        return QString(R"(
            QLineEdit {
                background-color: %1;
                color: %2;
                font-size: %3px;
                font-family: %4;
                border: 1px solid %5;
                border-radius: %6px;
                padding: %7px %8px;
            }
            QLineEdit:focus {
                border: 1px solid %9;
            }
        )")
            .arg(DesignTokens::Colors::BG_SECONDARY)
            .arg(DesignTokens::Colors::TEXT_BODY)
            .arg(DesignTokens::Typography::FONT_SIZE_SM)
            .arg(DesignTokens::Typography::FONT_MONO)
            .arg(DesignTokens::Colors::BORDER_DEFAULT)
            .arg(DesignTokens::BorderRadius::RADIUS_MD)
            .arg(DesignTokens::Spacing::SPACING_MD)
            .arg(DesignTokens::Spacing::SPACING_LG)
            .arg(DesignTokens::Colors::BORDER_FOCUS);
    }

    /**
     * Text area / QTextEdit style
     */
    static QString textArea() {
        return QString(R"(
            QTextEdit {
                background-color: %1;
                color: %2;
                font-size: %3px;
                border: 1px solid %4;
                border-radius: %5px;
                padding: %6px;
            }
            QTextEdit:focus {
                border: 1px solid %7;
            }
        )")
            .arg(DesignTokens::Colors::BG_SECONDARY)
            .arg(DesignTokens::Colors::TEXT_BODY)
            .arg(DesignTokens::Typography::FONT_SIZE_BASE)
            .arg(DesignTokens::Colors::BORDER_DEFAULT)
            .arg(DesignTokens::BorderRadius::RADIUS_MD)
            .arg(DesignTokens::Spacing::SPACING_MD)
            .arg(DesignTokens::Colors::BORDER_FOCUS);
    }

    // ========================================================================
    // CARDS
    // ========================================================================

    /**
     * Standard card container
     */
    static QString card() {
        return QString(R"(
            QWidget {
                background-color: %1;
                border: 1px solid %2;
                border-radius: %3px;
            }
        )")
            .arg(DesignTokens::Colors::BG_SECONDARY)
            .arg(DesignTokens::Colors::BORDER_DEFAULT)
            .arg(DesignTokens::BorderRadius::RADIUS_XL);
    }

    /**
     * Interactive/clickable card with hover effect
     */
    static QString interactiveCard() {
        return QString(R"(
            QWidget {
                background-color: %1;
                border: 1px solid %2;
                border-radius: %3px;
            }
            QWidget:hover {
                background-color: %4;
                border: 1px solid %5;
            }
        )")
            .arg(DesignTokens::Colors::BG_SECONDARY)
            .arg(DesignTokens::Colors::BORDER_DEFAULT)
            .arg(DesignTokens::BorderRadius::RADIUS_XL)
            .arg(DesignTokens::Colors::BG_TERTIARY)
            .arg(DesignTokens::Colors::BORDER_HOVER);
    }

    // ========================================================================
    // LABELS
    // ========================================================================

    /**
     * Page title / h1
     */
    static QString titleLabel() {
        return QString(R"(
            QLabel {
                background: transparent;
                font-size: %1px;
                font-weight: %2;
                color: %3;
                border: none;
            }
        )")
            .arg(DesignTokens::Typography::FONT_SIZE_4XL)
            .arg(DesignTokens::Typography::FONT_WEIGHT_BOLD)
            .arg(DesignTokens::Colors::TEXT_PRIMARY);
    }

    /**
     * Section heading / h2
     */
    static QString headingLabel() {
        return QString(R"(
            QLabel {
                background: transparent;
                font-size: %1px;
                font-weight: %2;
                color: %3;
                border: none;
            }
        )")
            .arg(DesignTokens::Typography::FONT_SIZE_2XL)
            .arg(DesignTokens::Typography::FONT_WEIGHT_BOLD)
            .arg(DesignTokens::Colors::TEXT_PRIMARY);
    }

    /**
     * Subheading / h3
     */
    static QString subheadingLabel() {
        return QString(R"(
            QLabel {
                background: transparent;
                font-size: %1px;
                font-weight: %2;
                color: %3;
                border: none;
            }
        )")
            .arg(DesignTokens::Typography::FONT_SIZE_LG)
            .arg(DesignTokens::Typography::FONT_WEIGHT_SEMIBOLD)
            .arg(DesignTokens::Colors::TEXT_MUTED);
    }

    /**
     * Body text
     */
    static QString bodyLabel() {
        return QString(R"(
            QLabel {
                background: transparent;
                font-size: %1px;
                font-weight: %2;
                color: %3;
                border: none;
            }
        )")
            .arg(DesignTokens::Typography::FONT_SIZE_BASE)
            .arg(DesignTokens::Typography::FONT_WEIGHT_NORMAL)
            .arg(DesignTokens::Colors::TEXT_BODY);
    }

    /**
     * Muted/secondary text
     */
    static QString mutedLabel() {
        return QString(R"(
            QLabel {
                background: transparent;
                font-size: %1px;
                color: %2;
                border: none;
            }
        )")
            .arg(DesignTokens::Typography::FONT_SIZE_SM)
            .arg(DesignTokens::Colors::TEXT_MUTED);
    }

    /**
     * Small hint text
     */
    static QString hintLabel() {
        return QString(R"(
            QLabel {
                background: transparent;
                font-size: %1px;
                color: %2;
                border: none;
            }
        )")
            .arg(DesignTokens::Typography::FONT_SIZE_XS)
            .arg(DesignTokens::Colors::TEXT_SUBTLE);
    }

    /**
     * Monospace label (for addresses)
     */
    static QString monoLabel() {
        return QString(R"(
            QLabel {
                background: transparent;
                font-size: %1px;
                font-family: %2;
                color: %3;
                border: none;
            }
        )")
            .arg(DesignTokens::Typography::FONT_SIZE_XS)
            .arg(DesignTokens::Typography::FONT_MONO)
            .arg(DesignTokens::Colors::TEXT_MUTED);
    }

    /**
     * Highlighted balance/amount text
     */
    static QString balanceLabel() {
        return QString(R"(
            QLabel {
                background: transparent;
                font-size: %1px;
                font-weight: %2;
                color: %3;
                border: none;
            }
        )")
            .arg(DesignTokens::Typography::FONT_SIZE_XL)
            .arg(DesignTokens::Typography::FONT_WEIGHT_BOLD)
            .arg(DesignTokens::Colors::BRAND_PRIMARY);
    }

    // ========================================================================
    // SCROLL AREAS
    // ========================================================================

    /**
     * Scroll area with custom scrollbar
     */
    static QString scrollArea() {
        return QString(R"(
            QScrollArea {
                border: none;
                background-color: transparent;
            }
            QScrollBar:vertical {
                background: %1;
                width: %2px;
                border-radius: %3px;
            }
            QScrollBar::handle:vertical {
                background: %4;
                border-radius: %3px;
            }
            QScrollBar::handle:vertical:hover {
                background: %5;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                height: 0px;
            }
        )")
            .arg(DesignTokens::Colors::BG_SECONDARY)
            .arg(DesignTokens::Sizes::SCROLLBAR_WIDTH)
            .arg(DesignTokens::BorderRadius::RADIUS_SM)
            .arg(DesignTokens::Colors::BRAND_PRIMARY)
            .arg(DesignTokens::Colors::BRAND_PRIMARY_HOVER);
    }

    // ========================================================================
    // TABLES
    // ========================================================================

    /**
     * Table widget style
     */
    static QString tableWidget() {
        return QString(R"(
            QTableWidget {
                background-color: %1;
                border: 1px solid %2;
                border-radius: %3px;
                color: %4;
                gridline-color: %2;
            }
            QTableWidget::item {
                padding: %5px;
                border: none;
            }
            QTableWidget::item:selected {
                background-color: %6;
            }
            QHeaderView::section {
                background-color: %7;
                color: %8;
                padding: %5px;
                border: none;
                font-weight: %9;
            }
        )")
            .arg(DesignTokens::Colors::BG_SECONDARY)
            .arg(DesignTokens::Colors::BORDER_DEFAULT)
            .arg(DesignTokens::BorderRadius::RADIUS_LG)
            .arg(DesignTokens::Colors::TEXT_BODY)
            .arg(DesignTokens::Spacing::SPACING_SM)
            .arg(DesignTokens::Colors::BG_TERTIARY)
            .arg(DesignTokens::Colors::BG_PRIMARY)
            .arg(DesignTokens::Colors::TEXT_MUTED)
            .arg(DesignTokens::Typography::FONT_WEIGHT_SEMIBOLD);
    }

    // ========================================================================
    // UTILITY FUNCTIONS
    // ========================================================================

    /**
     * Transparent background (for container widgets)
     */
    static QString transparentBackground() {
        return "QWidget { background-color: transparent; }";
    }

    /**
     * Create custom button with specific colors
     */
    static QString customButton(const QString &bgColor,
                                const QString &hoverColor,
                                const QString &textColor = DesignTokens::Colors::TEXT_WHITE) {
        return QString(R"(
            QPushButton {
                background-color: %1;
                color: %2;
                font-size: %3px;
                font-weight: %4;
                border: none;
                border-radius: %5px;
                padding: %6px %7px;
            }
            QPushButton:hover {
                background-color: %8;
            }
        )")
            .arg(bgColor)
            .arg(textColor)
            .arg(DesignTokens::Typography::FONT_SIZE_MD)
            .arg(DesignTokens::Typography::FONT_WEIGHT_SEMIBOLD)
            .arg(DesignTokens::BorderRadius::RADIUS_LG)
            .arg(DesignTokens::Spacing::SPACING_MD)
            .arg(DesignTokens::Spacing::SPACING_XL)
            .arg(hoverColor);
    }
};

#endif // STYLEHELPER_H
