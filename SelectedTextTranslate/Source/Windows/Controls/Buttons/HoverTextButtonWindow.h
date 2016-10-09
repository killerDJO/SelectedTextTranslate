#pragma once
#include "Windows\Controls\Buttons\Base\HoverButtonWindow.h"

/// <summary>
/// The button that changes its text color on hover.
/// </summary>
/// <seealso cref="HoverButtonWindow" />
class HoverTextButtonWindow : public HoverButtonWindow
{
private:
    /// <summary>
    /// The font of displayed text.
    /// </summary>
    HFONT font;

    /// <summary>
    /// The normal state color.
    /// </summary>
    Colors normalColor;

    /// <summary>
    /// The hover state color.
    /// </summary>
    Colors hoverColor;

    /// <summary>
    /// The text to display.
    /// </summary>
    wstring text;

    /// <summary>
    /// Renders the text with specified color onto device context.
    /// </summary>
    /// <param name="deviceContext">The device context.</param>
    /// <param name="color">The color of text to render.</param>
    void RenderStateDeviceContext(HDC deviceContext, Colors color) const;

protected:

    /// <summary>
    /// Renders the normal and hover states device contexts.
    /// </summary>
    void RenderStatesDeviceContext() override;

public:
    /// <summary>
    /// Initializes a new instance of the <see cref="HoverTextButtonWindow"/> class.
    /// </summary>
    /// <param name="context">The window context.</param>
    /// <param name="descriptor">The window descriptor.</param>
    /// <param name="parentWindow">The parent window.</param>
    /// <param name="font">The font to use.</param>
    /// <param name="normalColor">Text color for normal state.</param>
    /// <param name="hoverColor">Text color for hover state.</param>
    /// <param name="text">The text to display.</param>
    /// <param name="clickCallback">The click callback.</param>
    HoverTextButtonWindow(
        WindowContext* context,
        WindowDescriptor descriptor,
        Window* parentWindow,
        HFONT font,
        Colors normalColor,
        Colors hoverColor,
        wstring text,
        function<void()> clickCallback);

    /// <summary>
    /// Finalizes an instance of the <see cref="HoverTextButtonWindow"/> class.
    /// </summary>
    ~HoverTextButtonWindow();
};