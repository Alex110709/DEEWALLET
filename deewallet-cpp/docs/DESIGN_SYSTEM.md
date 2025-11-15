# DEE WALLET - Design System Documentation

## 📐 Overview

DEE WALLET uses a centralized design system based on design tokens and reusable style helpers. This approach ensures consistency, maintainability, and scalability across the application.

**Design Philosophy:**
- **Dark Theme First** - Optimized for cryptocurrency users who prefer dark interfaces
- **Modern & Minimal** - Clean, card-based layouts with subtle animations
- **Consistent & Predictable** - Unified spacing, colors, and typography
- **Accessible** - High contrast, large touch targets, clear visual hierarchy

---

## 🎨 Design Tokens

All design values are centralized in `src/ui/DesignTokens.h`. This header file contains constants for colors, typography, spacing, and other design primitives.

### Color Palette

Our color palette is inspired by Tailwind CSS, specifically the Slate and Blue color scales, optimized for dark theme interfaces.

#### Background Colors

```cpp
DesignTokens::Colors::BG_PRIMARY      // #0F172A (slate-900) - Darkest background
DesignTokens::Colors::BG_SECONDARY    // #1E293B (slate-800) - Cards, containers
DesignTokens::Colors::BG_TERTIARY     // #334155 (slate-700) - Borders, dividers
DesignTokens::Colors::BG_HOVER        // #475569 (slate-600) - Interactive hover
DesignTokens::Colors::BG_ACTIVE       // #64748B (slate-500) - Pressed/active
DesignTokens::Colors::BG_DISABLED     // #334155 (slate-700) - Disabled state
```

#### Brand Colors (Primary Blue)

```cpp
DesignTokens::Colors::BRAND_PRIMARY         // #3B82F6 (blue-500) - Main brand
DesignTokens::Colors::BRAND_PRIMARY_HOVER   // #2563EB (blue-600) - Hover state
DesignTokens::Colors::BRAND_PRIMARY_PRESSED // #1D4ED8 (blue-700) - Pressed state
DesignTokens::Colors::BRAND_PRIMARY_LIGHT   // #60A5FA (blue-400) - Light variant
```

#### Semantic Colors

```cpp
// Success (Green)
DesignTokens::Colors::SUCCESS         // #10B981 (emerald-500) - Success actions
DesignTokens::Colors::SUCCESS_HOVER   // #059669 (emerald-600)
DesignTokens::Colors::SUCCESS_PRESSED // #047857 (emerald-700)

// Warning (Amber)
DesignTokens::Colors::WARNING         // #F59E0B (amber-500)
DesignTokens::Colors::WARNING_HOVER   // #D97706 (amber-600)

// Error (Red)
DesignTokens::Colors::ERROR           // #EF4444 (red-500)
DesignTokens::Colors::ERROR_HOVER     // #DC2626 (red-600)

// Info (Cyan)
DesignTokens::Colors::INFO            // #06B6D4 (cyan-500)
```

#### Text Colors

```cpp
DesignTokens::Colors::TEXT_PRIMARY    // #F1F5F9 (slate-100) - Headings
DesignTokens::Colors::TEXT_BODY       // #E2E8F0 (slate-200) - Body text
DesignTokens::Colors::TEXT_SECONDARY  // #CBD5E1 (slate-300) - Secondary text
DesignTokens::Colors::TEXT_MUTED      // #94A3B8 (slate-400) - Muted text
DesignTokens::Colors::TEXT_SUBTLE     // #64748B (slate-500) - Subtle text
DesignTokens::Colors::TEXT_DISABLED   // #475569 (slate-600) - Disabled text
DesignTokens::Colors::TEXT_WHITE      // #FFFFFF - White text for buttons
```

#### Border Colors

```cpp
DesignTokens::Colors::BORDER_DEFAULT  // #334155 (slate-700)
DesignTokens::Colors::BORDER_HOVER    // BRAND_PRIMARY
DesignTokens::Colors::BORDER_FOCUS    // BRAND_PRIMARY_LIGHT
DesignTokens::Colors::BORDER_ERROR    // ERROR
```

---

### Typography

#### Font Families

```cpp
DesignTokens::Typography::FONT_FAMILY  // -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif
DesignTokens::Typography::FONT_MONO    // 'Monaco', 'Courier New', monospace (for addresses/crypto data)
```

#### Font Sizes

```cpp
DesignTokens::Typography::FONT_SIZE_XS    // 11px - Small hints, timestamps
DesignTokens::Typography::FONT_SIZE_SM    // 12px - Labels, secondary text
DesignTokens::Typography::FONT_SIZE_BASE  // 14px - Body text, buttons
DesignTokens::Typography::FONT_SIZE_MD    // 15px - Medium text
DesignTokens::Typography::FONT_SIZE_LG    // 16px - Section headers
DesignTokens::Typography::FONT_SIZE_XL    // 18px - Large text
DesignTokens::Typography::FONT_SIZE_2XL   // 24px - Page titles
DesignTokens::Typography::FONT_SIZE_3XL   // 28px - Large titles
DesignTokens::Typography::FONT_SIZE_4XL   // 36px - Hero text
DesignTokens::Typography::FONT_SIZE_5XL   // 48px - Extra large
```

#### Font Weights

```cpp
DesignTokens::Typography::FONT_WEIGHT_NORMAL     // 400
DesignTokens::Typography::FONT_WEIGHT_MEDIUM     // 500
DesignTokens::Typography::FONT_WEIGHT_SEMIBOLD   // 600
DesignTokens::Typography::FONT_WEIGHT_BOLD       // 700
DesignTokens::Typography::FONT_WEIGHT_EXTRABOLD  // 800
```

---

### Spacing

Based on a 4px grid system for consistent rhythm.

```cpp
DesignTokens::Spacing::SPACING_XXS   // 4px  - Minimal spacing
DesignTokens::Spacing::SPACING_XS    // 6px  - Very small spacing
DesignTokens::Spacing::SPACING_SM    // 8px  - Small spacing
DesignTokens::Spacing::SPACING_MD    // 12px - Medium spacing
DesignTokens::Spacing::SPACING_LG    // 16px - Large spacing
DesignTokens::Spacing::SPACING_XL    // 20px - Extra large spacing
DesignTokens::Spacing::SPACING_2XL   // 24px - 2x extra large
DesignTokens::Spacing::SPACING_3XL   // 30px - 3x extra large
DesignTokens::Spacing::SPACING_4XL   // 40px - 4x extra large
DesignTokens::Spacing::SPACING_5XL   // 50px - 5x extra large
```

---

### Border Radius

```cpp
DesignTokens::BorderRadius::RADIUS_NONE  // 0
DesignTokens::BorderRadius::RADIUS_SM    // 4px  - Small radius
DesignTokens::BorderRadius::RADIUS_MD    // 6px  - Medium radius (inputs)
DesignTokens::BorderRadius::RADIUS_LG    // 8px  - Large radius (buttons)
DesignTokens::BorderRadius::RADIUS_XL    // 10px - Extra large (cards)
DesignTokens::BorderRadius::RADIUS_2XL   // 12px - 2x extra large
DesignTokens::BorderRadius::RADIUS_FULL  // 9999 - Fully rounded
```

---

### Component Sizes

```cpp
// Button heights
DesignTokens::Sizes::BUTTON_HEIGHT_SM  // 36px
DesignTokens::Sizes::BUTTON_HEIGHT_MD  // 44px
DesignTokens::Sizes::BUTTON_HEIGHT_LG  // 48px

// Input heights
DesignTokens::Sizes::INPUT_HEIGHT_SM   // 36px
DesignTokens::Sizes::INPUT_HEIGHT_MD   // 40px
DesignTokens::Sizes::INPUT_HEIGHT_LG   // 48px

// Card heights
DesignTokens::Sizes::CARD_MIN_HEIGHT_SM  // 60px
DesignTokens::Sizes::CARD_MIN_HEIGHT_MD  // 90px
DesignTokens::Sizes::CARD_MIN_HEIGHT_LG  // 100px

// Scrollbar
DesignTokens::Sizes::SCROLLBAR_WIDTH     // 8px
```

---

### Animation

```cpp
DesignTokens::Animation::DURATION_FAST    // 100ms
DesignTokens::Animation::DURATION_NORMAL  // 150ms
DesignTokens::Animation::DURATION_SLOW    // 300ms
```

---

## 🎭 Style Helper

The `StyleHelper` class (in `src/ui/StyleHelper.h`) provides pre-built Qt StyleSheet strings for common components. This reduces code duplication and ensures consistency.

### Buttons

```cpp
// Primary action button (blue)
button->setStyleSheet(StyleHelper::primaryButton());

// Secondary action button (outlined)
button->setStyleSheet(StyleHelper::secondaryButton());

// Success button (green, for receive/positive actions)
button->setStyleSheet(StyleHelper::successButton());

// Danger button (red, for destructive actions)
button->setStyleSheet(StyleHelper::dangerButton());

// Ghost button (transparent, minimal)
button->setStyleSheet(StyleHelper::ghostButton());

// Custom button with specific colors
button->setStyleSheet(StyleHelper::customButton(bgColor, hoverColor, textColor));
```

### Input Fields

```cpp
// Standard text input
lineEdit->setStyleSheet(StyleHelper::inputField());

// Monospace input (for addresses, seeds)
lineEdit->setStyleSheet(StyleHelper::monoInputField());

// Text area
textEdit->setStyleSheet(StyleHelper::textArea());
```

### Cards

```cpp
// Standard card container
card->setStyleSheet(StyleHelper::card());

// Interactive card with hover effect
card->setStyleSheet(StyleHelper::interactiveCard());
```

### Labels

```cpp
// Page title / h1
label->setStyleSheet(StyleHelper::titleLabel());

// Section heading / h2
label->setStyleSheet(StyleHelper::headingLabel());

// Subheading / h3
label->setStyleSheet(StyleHelper::subheadingLabel());

// Body text
label->setStyleSheet(StyleHelper::bodyLabel());

// Muted/secondary text
label->setStyleSheet(StyleHelper::mutedLabel());

// Small hint text
label->setStyleSheet(StyleHelper::hintLabel());

// Monospace label (for addresses)
label->setStyleSheet(StyleHelper::monoLabel());

// Highlighted balance/amount
label->setStyleSheet(StyleHelper::balanceLabel());
```

### Scroll Areas

```cpp
scrollArea->setStyleSheet(StyleHelper::scrollArea());
```

### Tables

```cpp
tableWidget->setStyleSheet(StyleHelper::tableWidget());
```

### Utilities

```cpp
// Transparent background for container widgets
container->setStyleSheet(StyleHelper::transparentBackground());
```

---

## 📚 Usage Examples

### Example 1: Creating a Primary Button

**Before (old approach):**
```cpp
button->setStyleSheet(R"(
    QPushButton {
        background-color: #3B82F6;
        color: white;
        font-size: 15px;
        font-weight: 600;
        border: none;
        border-radius: 8px;
    }
    QPushButton:hover {
        background-color: #2563EB;
    }
    QPushButton:pressed {
        background-color: #1D4ED8;
    }
)");
```

**After (new approach):**
```cpp
#include "StyleHelper.h"
#include "DesignTokens.h"

button->setMinimumHeight(DesignTokens::Sizes::BUTTON_HEIGHT_LG);
button->setStyleSheet(StyleHelper::primaryButton());
```

### Example 2: Creating a Custom Card

```cpp
#include "StyleHelper.h"
#include "DesignTokens.h"

auto *card = new QWidget();
card->setMinimumHeight(DesignTokens::Sizes::CARD_MIN_HEIGHT_MD);
card->setStyleSheet(StyleHelper::interactiveCard());

auto *layout = new QVBoxLayout(card);
layout->setSpacing(DesignTokens::Spacing::SPACING_MD);
layout->setContentsMargins(
    DesignTokens::Spacing::SPACING_XL,
    DesignTokens::Spacing::SPACING_LG,
    DesignTokens::Spacing::SPACING_XL,
    DesignTokens::Spacing::SPACING_LG
);
```

### Example 3: Creating Custom Styled Label

```cpp
auto *label = new QLabel("Balance", this);
label->setStyleSheet(QString(R"(
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
    .arg(DesignTokens::Colors::TEXT_BODY));
```

---

## 🔧 Customization

### Modifying Colors

To change the color scheme globally:

1. Open `src/ui/DesignTokens.h`
2. Modify the color constants in the `Colors` namespace
3. Rebuild the application

All components using the design tokens will automatically update.

### Adding New Styles

To add a new reusable style:

1. Open `src/ui/StyleHelper.h`
2. Add a new static method returning a `QString` with the stylesheet
3. Use design tokens for all values
4. Document the new function

Example:
```cpp
// In StyleHelper.h
static QString warningButton() {
    return QString(R"(
        QPushButton {
            background-color: %1;
            color: %2;
            font-size: %3px;
            font-weight: %4;
            border: none;
            border-radius: %5px;
        }
        QPushButton:hover {
            background-color: %6;
        }
    )")
        .arg(DesignTokens::Colors::WARNING)
        .arg(DesignTokens::Colors::TEXT_WHITE)
        .arg(DesignTokens::Typography::FONT_SIZE_MD)
        .arg(DesignTokens::Typography::FONT_WEIGHT_SEMIBOLD)
        .arg(DesignTokens::BorderRadius::RADIUS_LG)
        .arg(DesignTokens::Colors::WARNING_HOVER);
}
```

---

## ✅ Best Practices

### DO ✅

- **Always use design tokens** for colors, spacing, typography
- **Use StyleHelper functions** for common components
- **Create reusable styles** when a pattern is used 3+ times
- **Test in dark environments** as that's our primary theme
- **Maintain high contrast** for accessibility (WCAG AA minimum)
- **Use semantic color names** (SUCCESS, ERROR, WARNING) instead of color values

### DON'T ❌

- **Don't hardcode color values** like `#3B82F6` directly in components
- **Don't hardcode spacing values** - use Spacing tokens
- **Don't create inline styles** for common patterns - add to StyleHelper
- **Don't mix old and new styling approaches** in the same file
- **Don't override design tokens** with inline values
- **Don't use random font sizes** - stick to the typographic scale

---

## 🎯 Component Patterns

### Interactive Cards

Cards that respond to hover/click should:
- Use `StyleHelper::interactiveCard()`
- Set cursor to `Qt::PointingHandCursor`
- Have minimum height from `DesignTokens::Sizes::CARD_MIN_HEIGHT_*`
- Use standard padding: `SPACING_XL` horizontal, `SPACING_LG` vertical

### Buttons

Primary actions (create, send, confirm):
- Use `StyleHelper::primaryButton()`
- Blue background
- Height: `BUTTON_HEIGHT_LG` (48px)

Secondary actions (cancel, back):
- Use `StyleHelper::secondaryButton()`
- Outlined style
- Height: `BUTTON_HEIGHT_MD` (44px) or `BUTTON_HEIGHT_SM` (36px)

Positive actions (receive, success):
- Use `StyleHelper::successButton()`
- Green background
- Height: `BUTTON_HEIGHT_LG` (48px)

### Typography Hierarchy

```
Hero Title       → StyleHelper::titleLabel() (36px, bold)
Page Title       → StyleHelper::headingLabel() (24px, bold)
Section Heading  → StyleHelper::subheadingLabel() (16px, semibold)
Body Text        → StyleHelper::bodyLabel() (14px, normal)
Muted Text       → StyleHelper::mutedLabel() (12px)
Hint Text        → StyleHelper::hintLabel() (11px)
```

### Spacing Scale

```
Tight spacing:   SPACING_XXS (4px), SPACING_XS (6px)
Default spacing: SPACING_SM (8px), SPACING_MD (12px)
Loose spacing:   SPACING_LG (16px), SPACING_XL (20px)
Section spacing: SPACING_2XL (24px), SPACING_3XL (30px)
Page margins:    SPACING_4XL (40px), SPACING_5XL (50px)
```

---

## 🔄 Migration Guide

### Migrating Existing Components

When updating an existing component to use the design system:

1. **Add includes:**
   ```cpp
   #include "StyleHelper.h"
   #include "DesignTokens.h"
   ```

2. **Replace hardcoded colors** with design tokens:
   ```cpp
   // Before
   setStyleSheet("background-color: #1E293B;");

   // After
   setStyleSheet(QString("background-color: %1;")
       .arg(DesignTokens::Colors::BG_SECONDARY));
   ```

3. **Replace inline styles** with StyleHelper:
   ```cpp
   // Before
   button->setStyleSheet("QPushButton { background-color: #3B82F6; ... }");

   // After
   button->setStyleSheet(StyleHelper::primaryButton());
   ```

4. **Replace hardcoded sizes** with design tokens:
   ```cpp
   // Before
   button->setMinimumHeight(48);

   // After
   button->setMinimumHeight(DesignTokens::Sizes::BUTTON_HEIGHT_LG);
   ```

---

## 📊 Design System Benefits

### Before Design System

❌ **Problems:**
- Colors scattered across 10+ files
- Inconsistent spacing and typography
- Difficult to maintain and update
- No single source of truth
- Code duplication
- Hard to ensure accessibility

### After Design System

✅ **Benefits:**
- **Consistency:** All components use the same colors, spacing, typography
- **Maintainability:** Change colors globally from one place
- **Scalability:** Easy to add new components following the same patterns
- **Developer Experience:** Faster development with pre-built styles
- **Documentation:** Clear guidelines for future development
- **Accessibility:** Enforced color contrast and sizing standards

---

## 🚀 Future Enhancements

Potential improvements to consider:

1. **Theme Switching:** Add light theme support
2. **Icon System:** Centralized icon management (SVG or icon fonts)
3. **Animation Library:** Pre-built transition and animation utilities
4. **Component Library:** More complex pre-built components (modals, dropdowns, etc.)
5. **Design Tokens Export:** Generate design tokens for web/mobile versions
6. **Accessibility Tools:** Automated contrast checking and screen reader support

---

## 📞 Support

For questions about the design system:
- Review this documentation
- Check `src/ui/DesignTokens.h` for available tokens
- Check `src/ui/StyleHelper.h` for available styles
- See example usage in `WelcomeScreen.cpp`, `WalletDetailScreen.cpp`, `ChainDetailScreen.cpp`

---

**Last Updated:** 2025-11-13
**Version:** 1.0.0
**Maintainer:** DEE WALLET Team
