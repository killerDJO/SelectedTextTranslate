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
    HoverIconButtonControl(CommonContext* context, View* parentWindow);

    void SetDimensions(Point position, Size size);

    void SetNormalIconResource(DWORD normalIconResource);
    DWORD GetNormalIconResource() const;

    void SetHoverIconResource(DWORD hoverIconResource);
    DWORD GetHoverIconResource() const;

    void SetBackgroundBrush(Brush* backgroundBrush);
    Brush* GetBackgroundBrush() const;

    void Initialize() override;
};