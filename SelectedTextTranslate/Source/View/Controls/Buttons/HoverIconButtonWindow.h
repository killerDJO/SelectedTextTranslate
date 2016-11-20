#pragma once
#include "View\Controls\Buttons\Base\HoverButtonWindow.h"

class HoverIconButtonWindow : public HoverButtonWindow
{
private:
    DWORD normalIconResource;
    DWORD hoverIconResource;
    HBRUSH backgroundBrush;

    void RenderStateDeviceContext(HDC deviceContext, DWORD iconResource);

    Metafile* LoadMetafileFromResource(DWORD resourceId) const;

    static map<tuple<DWORD, int, int>, HDC> iconsCache;

protected:
    void RenderStatesDeviceContext() override;

public:
    HoverIconButtonWindow(WindowContext* context, Window* parentWindow);
    ~HoverIconButtonWindow();

    void SetDescriptor(WindowDescriptor descriptor) override;
    void SetDimensions(Point position, Size size);

    void SetNormalIconResource(DWORD normalIconResource);
    DWORD GetNormalIconResource() const;
    void SetHoverIconResource(DWORD hoverIconResource);
    DWORD GetHoverIconResource() const;
    void SetBackgroundBrush(HBRUSH backgroundBrush);
    HBRUSH GetBackgroundBrush() const;

    void Initialize() override;
};