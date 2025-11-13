/**
 * DEE WALLET - Design Tokens
 *
 * Centralized design system tokens for consistent styling across the application.
 * Based on Tailwind CSS color palette with custom adjustments for crypto wallet UI.
 *
 * Usage:
 *   #include "DesignTokens.h"
 *
 *   QString style = QString("background-color: %1;").arg(DesignTokens::Colors::BG_PRIMARY);
 *   widget->setStyleSheet(style);
 */

#ifndef DESIGNTOKENS_H
#define DESIGNTOKENS_H

#include <QString>

namespace DesignTokens {

/**
 * Color Palette
 *
 * Based on Tailwind CSS slate and blue color scales
 * Dark theme optimized for cryptocurrency wallet interface
 */
namespace Colors {
    // ========================================================================
    // BACKGROUND COLORS
    // ========================================================================

    /** Primary background - Darkest (slate-900) */
    constexpr const char* BG_PRIMARY = "#0F172A";

    /** Secondary background - Cards, containers (slate-800) */
    constexpr const char* BG_SECONDARY = "#1E293B";

    /** Tertiary background - Borders, dividers (slate-700) */
    constexpr const char* BG_TERTIARY = "#334155";

    /** Interactive hover state (slate-600) */
    constexpr const char* BG_HOVER = "#475569";

    /** Pressed/active state (slate-600) */
    constexpr const char* BG_ACTIVE = "#64748B";

    /** Disabled state (slate-700) */
    constexpr const char* BG_DISABLED = "#334155";

    // ========================================================================
    // BRAND COLORS - Primary (Blue)
    // ========================================================================

    /** Primary brand color - Main actions (blue-500) */
    constexpr const char* BRAND_PRIMARY = "#3B82F6";

    /** Primary hover state (blue-600) */
    constexpr const char* BRAND_PRIMARY_HOVER = "#2563EB";

    /** Primary pressed/active state (blue-700) */
    constexpr const char* BRAND_PRIMARY_PRESSED = "#1D4ED8";

    /** Primary light variant (blue-400) */
    constexpr const char* BRAND_PRIMARY_LIGHT = "#60A5FA";

    // ========================================================================
    // SEMANTIC COLORS
    // ========================================================================

    /** Success color - Receive, positive actions (emerald-500) */
    constexpr const char* SUCCESS = "#10B981";

    /** Success hover state (emerald-600) */
    constexpr const char* SUCCESS_HOVER = "#059669";

    /** Success pressed state (emerald-700) */
    constexpr const char* SUCCESS_PRESSED = "#047857";

    /** Warning color (amber-500) */
    constexpr const char* WARNING = "#F59E0B";

    /** Warning hover state (amber-600) */
    constexpr const char* WARNING_HOVER = "#D97706";

    /** Error/danger color (red-500) */
    constexpr const char* ERROR = "#EF4444";

    /** Error hover state (red-600) */
    constexpr const char* ERROR_HOVER = "#DC2626";

    /** Info color (cyan-500) */
    constexpr const char* INFO = "#06B6D4";

    // ========================================================================
    // TEXT COLORS
    // ========================================================================

    /** Primary heading text (slate-100) */
    constexpr const char* TEXT_PRIMARY = "#F1F5F9";

    /** Body text, paragraphs (slate-200) */
    constexpr const char* TEXT_BODY = "#E2E8F0";

    /** Secondary text, labels (slate-300) */
    constexpr const char* TEXT_SECONDARY = "#CBD5E1";

    /** Muted text, hints (slate-400) */
    constexpr const char* TEXT_MUTED = "#94A3B8";

    /** Subtle text, disabled (slate-500) */
    constexpr const char* TEXT_SUBTLE = "#64748B";

    /** Disabled text (slate-600) */
    constexpr const char* TEXT_DISABLED = "#475569";

    /** White text for buttons */
    constexpr const char* TEXT_WHITE = "#FFFFFF";

    // ========================================================================
    // BORDER COLORS
    // ========================================================================

    /** Default border color (slate-700) */
    constexpr const char* BORDER_DEFAULT = "#334155";

    /** Hover border color */
    constexpr const char* BORDER_HOVER = BRAND_PRIMARY;

    /** Focus border color */
    constexpr const char* BORDER_FOCUS = BRAND_PRIMARY_LIGHT;

    /** Error border color */
    constexpr const char* BORDER_ERROR = ERROR;
}

/**
 * Typography Settings
 */
namespace Typography {
    /** Font family for general UI text */
    constexpr const char* FONT_FAMILY = "-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif";

    /** Monospace font for addresses and crypto data */
    constexpr const char* FONT_MONO = "'Monaco', 'Courier New', monospace";

    // Font sizes (in pixels)
    constexpr int FONT_SIZE_XS = 11;    // Small hints, timestamps
    constexpr int FONT_SIZE_SM = 12;    // Labels, secondary text
    constexpr int FONT_SIZE_BASE = 14;  // Body text, buttons
    constexpr int FONT_SIZE_MD = 15;    // Medium text
    constexpr int FONT_SIZE_LG = 16;    // Section headers
    constexpr int FONT_SIZE_XL = 18;    // Large text
    constexpr int FONT_SIZE_2XL = 24;   // Page titles
    constexpr int FONT_SIZE_3XL = 28;   // Large titles
    constexpr int FONT_SIZE_4XL = 36;   // Hero text
    constexpr int FONT_SIZE_5XL = 48;   // Extra large

    // Font weights
    constexpr int FONT_WEIGHT_NORMAL = 400;
    constexpr int FONT_WEIGHT_MEDIUM = 500;
    constexpr int FONT_WEIGHT_SEMIBOLD = 600;
    constexpr int FONT_WEIGHT_BOLD = 700;
    constexpr int FONT_WEIGHT_EXTRABOLD = 800;
}

/**
 * Spacing Values (in pixels)
 *
 * Based on 4px grid system
 */
namespace Spacing {
    constexpr int SPACING_XXS = 4;   // Minimal spacing
    constexpr int SPACING_XS = 6;    // Very small spacing
    constexpr int SPACING_SM = 8;    // Small spacing
    constexpr int SPACING_MD = 12;   // Medium spacing
    constexpr int SPACING_LG = 16;   // Large spacing
    constexpr int SPACING_XL = 20;   // Extra large spacing
    constexpr int SPACING_2XL = 24;  // 2x extra large
    constexpr int SPACING_3XL = 30;  // 3x extra large
    constexpr int SPACING_4XL = 40;  // 4x extra large
    constexpr int SPACING_5XL = 50;  // 5x extra large
}

/**
 * Border Radius Values (in pixels)
 */
namespace BorderRadius {
    constexpr int RADIUS_NONE = 0;
    constexpr int RADIUS_SM = 4;    // Small radius
    constexpr int RADIUS_MD = 6;    // Medium radius (inputs)
    constexpr int RADIUS_LG = 8;    // Large radius (buttons)
    constexpr int RADIUS_XL = 10;   // Extra large (cards)
    constexpr int RADIUS_2XL = 12;  // 2x extra large
    constexpr int RADIUS_FULL = 9999; // Fully rounded
}

/**
 * Component Sizes
 */
namespace Sizes {
    // Button heights
    constexpr int BUTTON_HEIGHT_SM = 36;
    constexpr int BUTTON_HEIGHT_MD = 44;
    constexpr int BUTTON_HEIGHT_LG = 48;

    // Input heights
    constexpr int INPUT_HEIGHT_SM = 36;
    constexpr int INPUT_HEIGHT_MD = 40;
    constexpr int INPUT_HEIGHT_LG = 48;

    // Card heights
    constexpr int CARD_MIN_HEIGHT_SM = 60;
    constexpr int CARD_MIN_HEIGHT_MD = 90;
    constexpr int CARD_MIN_HEIGHT_LG = 100;

    // Scrollbar width
    constexpr int SCROLLBAR_WIDTH = 8;
}

/**
 * Animation Durations (in milliseconds)
 */
namespace Animation {
    constexpr int DURATION_FAST = 100;
    constexpr int DURATION_NORMAL = 150;
    constexpr int DURATION_SLOW = 300;
}

/**
 * Z-Index Layers
 */
namespace ZIndex {
    constexpr int BASE = 0;
    constexpr int DROPDOWN = 1000;
    constexpr int STICKY = 1020;
    constexpr int FIXED = 1030;
    constexpr int MODAL_BACKDROP = 1040;
    constexpr int MODAL = 1050;
    constexpr int POPOVER = 1060;
    constexpr int TOOLTIP = 1070;
}

} // namespace DesignTokens

#endif // DESIGNTOKENS_H
