#pragma once
#include "Windows\Framework\Enums\FontSizes.h"
#include "Windows\Framework\Enums\Colors.h"
#include "Windows\Framework\Providers\ScaleProvider.h"
#include "Windows\Framework\Renderer.h"

class Renderer;
class DeviceContextProvider;

/// <summary>
/// The rendering context. Used to created renderer, fonts, brushes and determine font metrics.
/// </summary>
class RenderingContext
{
private:
    /// <summary>
    /// The scale provider.
    /// </summary>
    ScaleProvider* scaleProvider;

    /// <summary>
    /// The device context provider.
    /// </summary>
    DeviceContextProvider* deviceContextProvider;

public:
    /// <summary>
    /// Initializes a new instance of the <see cref="RenderingContext"/> class.
    /// </summary>
    /// <param name="scaleProvider">The scale provider.</param>
    /// <param name="deviceContextProvider">The device context provider.</param>
    RenderingContext(ScaleProvider* scaleProvider, DeviceContextProvider* deviceContextProvider);

    /// <summary>
    /// Finalizes an instance of the <see cref="RenderingContext"/> class.
    /// </summary>
    ~RenderingContext();

    /// <summary>
    /// Creates the custom font.
    /// </summary>
    /// <param name="windowHandle">The window handle.</param>
    /// <param name="fontSize">Size of the font.</param>
    /// <param name="isItalic">if set to <c>true</c> font will be italic.</param>
    /// <param name="isUnderscored">if set to <c>true</c> font will be underscored.</param>
    /// <returns>Created font.</returns>
    HFONT CreateCustomFont(HWND windowHandle, FontSizes fontSize, bool isItalic = false, bool isUnderscored = false) const;
    
    /// <summary>
    /// Creates the custom brush.
    /// </summary>
    /// <param name="color">The color.</param>
    /// <returns>Created brush.</returns>
    HBRUSH CreateCustomBrush(Colors color) const;

    /// <summary>
    /// Gets the size which rendered text would occupy.
    /// </summary>
    /// <param name="deviceContext">The device context.</param>
    /// <param name="text">The text.</param>
    /// <param name="font">The font.</param>
    /// <returns>The size which rendered text would occupy.</returns>
    Size GetTextSize(HDC deviceContext, const wchar_t* text, HFONT font) const;

    /// <summary>
    /// Gets the font metrics.
    /// </summary>
    /// <param name="deviceContext">The device context.</param>
    /// <param name="font">The font.</param>
    /// <returns>The TEXTMETRIC structure.</returns>
    TEXTMETRIC GetFontMetrics(HDC deviceContext, HFONT font) const;

    /// <summary>
    /// Gets the renderer. ReleaseRenderer method must be called after renderer is no longer required.
    /// </summary>
    /// <returns></returns>
    Renderer* GetRenderer();

    /// <summary>
    /// Releases the renderer.
    /// </summary>
    /// <param name="renderer">The renderer.</param>
    void ReleaseRenderer(Renderer* renderer) const;
};