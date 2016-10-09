#pragma once
#include "Windows\Framework\RenderingContext.h"
#include "Windows\Framework\Providers\DeviceContextProvider.h"
#include "Windows\Framework\DeviceContextBuffer.h"

class RenderingContext;

/// <summary>
/// Renderer is used to render content onto device context. It supports automatic scaling and delayed rendering.
/// </summary>
class Renderer
{
private:
    /// <summary>
    /// The rendering context.
    /// </summary>
    RenderingContext* renderingContext;

    /// <summary>
    /// The scale provider.
    /// </summary>
    ScaleProvider* scaleProvider;

    /// <summary>
    /// The fake device context to get font metrics.
    /// </summary>
    HDC emptyDeviceContext;

    /// <summary>
    /// The scaled size of rendered content.
    /// </summary>
    Size scaledSize;

    /// <summary>
    /// The list of render actions. In order to render current state these actions should be executed against device context.
    /// </summary>
    vector<function<void(HDC)>> renderActions;

    /// <summary>
    /// Clears the device context.
    /// </summary>
    /// <param name="deviceContext">The device context.</param>
    /// <param name="deviceContextSize">The size to clear.</param>
    void ClearDeviceContext(HDC deviceContext, Size deviceContextSize) const;

public:
    /// <summary>
    /// Initializes a new instance of the <see cref="Renderer"/> class.
    /// </summary>
    /// <param name="renderingContext">The rendering context.</param>
    /// <param name="deviceContextProvider">The device context provider.</param>
    /// <param name="scaleProvider">The scale provider.</param>
    Renderer(RenderingContext* renderingContext, DeviceContextProvider* deviceContextProvider, ScaleProvider* scaleProvider);
    
    /// <summary>
    /// Finalizes an instance of the <see cref="Renderer"/> class.
    /// </summary>
    ~Renderer();

    /// <summary>
    /// Prints the text.
    /// </summary>
    /// <param name="text">The text.</param>
    /// <param name="font">The font.</param>
    /// <param name="color">The color.</param>
    /// <param name="position">The left text baseline position.</param>
    /// <returns>The right position of printed text baseline.</returns>
    Point PrintText(const wchar_t* text, HFONT font, Colors color, Point position);

    /// <summary>
    /// Draws the rectangle.
    /// </summary>
    /// <param name="rect">The rectangle.</param>
    /// <param name="brush">The brush.</param>
    void DrawRect(Rect rect, HBRUSH brush);

    /// <summary>
    /// Gets the font ascent.
    /// </summary>
    /// <param name="font">The font.</param>
    /// <returns>The font ascent value.</returns>
    int GetFontAscent(HFONT font) const;

    /// <summary>
    /// Gets the height of the font stroke.
    /// </summary>
    /// <param name="font">The font.</param>
    /// <returns>The font stroke height.</returns>
    int GetFontStrokeHeight(HFONT font) const;

    /// <summary>
    /// Gets the scaled size of the rendered content.
    /// </summary>
    /// <returns>The scaled size.</returns>
    Size GetScaledSize() const;

    /// <summary>
    /// Gets the unscaled size of the rendered content.
    /// </summary>
    /// <returns>The unscaled size.</returns>
    Size GetSize() const;

    /// <summary>
    /// Increases the width of the rendered content. Useful to add padding.
    /// </summary>
    /// <param name="widthToAdd">The width to add.</param>
    void IncreaseWidth(int widthToAdd);

    /// <summary>
    /// Increases the height of the rendered content. Useful to add padding.
    /// </summary>
    /// <param name="heightToAdd">The height to add.</param>
    void IncreaseHeight(int heightToAdd);

    /// <summary>
    /// Renders the current state onto device context.
    /// </summary>
    /// <param name="deviceContext">The device context.</param>
    /// <param name="deviceContextSize">Size of the device context.</param>
    void Render(HDC deviceContext, Size deviceContextSize);

    /// <summary>
    /// Renders the current state onto device context buffer.
    /// </summary>
    /// <param name="deviceContextBuffer">The device context buffer.</param>
    void Render(DeviceContextBuffer* deviceContextBuffer);
};