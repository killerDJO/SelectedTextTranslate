#pragma once
#include "Windows\Controls\Buttons\Base\HoverButtonWindow.h"

/// <summary>
/// The button that changes its icon on hover.
/// </summary>
/// <seealso cref="HoverButtonWindow" />
class HoverIconButtonWindow : public HoverButtonWindow
{
private:
    /// <summary>
    /// The identifier of metafile resource for normal state.
    /// </summary>
    DWORD normalIconResource;

    /// <summary>
    /// The identifier of metafile resource for hovered state.
    /// </summary>
    DWORD hoverIconResource;

    /// <summary>
    /// Renders the icon onto device context.
    /// </summary>
    /// <param name="deviceContext">The device context.</param>
    /// <param name="iconResource">The icon resource.</param>
    void RenderStateDeviceContext(HDC deviceContext, DWORD iconResource);

    /// <summary>
    /// Loads the metafile (WMF or EMF) from resource.
    /// </summary>
    /// <param name="resourceId">The resource identifier.</param>
    /// <returns>The loaded metafile.</returns>
    Metafile* LoadMetafileFromResource(DWORD resourceId) const;

    /// <summary>
    /// The cache which holds rendered on device context icon. Cache key: (resource identifier, width, height).
    /// </summary>
    static map<tuple<DWORD, int, int>, HDC> iconsCache;

protected:
    /// <summary>
    /// Renders the normal and hover states device contexts.
    /// </summary>
    void RenderStatesDeviceContext() override;

public:
    /// <summary>
    /// Initializes a new instance of the <see cref="HoverIconButtonWindow"/> class.
    /// </summary>
    /// <param name="context">The window context.</param>
    /// <param name="descriptor">The window descriptor.</param>
    /// <param name="parentWindow">The parent window.</param>
    /// <param name="normalIconResource">The normal icon resource identifier.</param>
    /// <param name="hoverIconResource">The hover icon resource identifier.</param>
    /// <param name="clickCallback">The click callback.</param>
    HoverIconButtonWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, DWORD normalIconResource, DWORD hoverIconResource, function<void()> clickCallback);
    
    /// <summary>
    /// Finalizes an instance of the <see cref="HoverIconButtonWindow"/> class.
    /// </summary>
    ~HoverIconButtonWindow();
};