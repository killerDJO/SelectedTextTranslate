#pragma once
#include "Presentation\Controls\Buttons\Base\HoverButtonControl.h"

class HoverIconButtonControl : public HoverButtonControl
{
private:
    DWORD normalIconResource;
    DWORD hoverIconResource;
    Brush* backgroundBrush;

    void RenderStateDeviceContext(HDC deviceContext, DWORD iconResource) const;

    Gdiplus::Metafile* LoadMetafileFromResource(DWORD resourceId) const;

    static map<tuple<DWORD, int, int>, HDC> iconsCache;

protected:
    void RenderStatesDeviceContexts() override;

public:
    HoverIconButtonControl(ViewContext* context, View* parentWindow);

    HoverIconButtonControl* SetDimensions(Point position, Size size);

    HoverIconButtonControl* SetNormalIconResource(DWORD normalIconResource);
    DWORD GetNormalIconResource() const;

    HoverIconButtonControl* SetHoverIconResource(DWORD hoverIconResource);
    DWORD GetHoverIconResource() const;

    HoverIconButtonControl* SetBackgroundBrush(Brush* backgroundBrush);
    Brush* GetBackgroundBrush() const;

    void Initialize() override;
};