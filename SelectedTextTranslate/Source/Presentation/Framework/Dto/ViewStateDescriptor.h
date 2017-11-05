#pragma once
#include "Presentation\Framework\Dto\Positioning\Point.h"
#include "Presentation\Framework\Dto\Positioning\Size.h"
#include "Presentation\Framework\Dto\Positioning\Rect.h"
#include "Presentation\Framework\Enums\ViewStatus.h"
#include "Presentation\Framework\Rendering\DeviceContextBuffer.h"
#include "Presentation\Framework\Dto\LayoutDescriptor.h"
#include "Presentation\Framework\Rendering\Renderer.h"

class Renderer;

class ViewStateDescriptor
{
private:
    LayoutDescriptor layout;

    Size viewSize;

    //In case of the Scrollable view used to detirmine size of content to scroll. For other window types always equal to the window size.
    Size contentSize;

    Point position;
    bool isVisible;

    ViewStatus viewStatus;

    DeviceContextProvider* deviceContextProvider;
    DeviceContextBuffer* deviceContextBuffer;

    void StretchToSize(Size size);
    void AssertLayoutIsSet() const;

public:
    ViewStateDescriptor(DeviceContextProvider* deviceContextProvider);
    ~ViewStateDescriptor();

    void SetLayout(LayoutDescriptor layout);
    LayoutDescriptor GetLayout() const;

    void ResetToLayout();

    void UpdateDeviceContext(Renderer* renderer);
    void SetContentSize(Size newContentSize);

    DeviceContextBuffer* GetDeviceContextBuffer() const;

    Size GetViewSize() const;
    Size GetContentSize() const;
    Rect GetBoundingRect() const;

    Point GetPosition() const;
    void SetPosition(Point position);

    ViewStatus GetViewStatus() const;
    void SetViewStatus(ViewStatus viewStatus);

    void MakeVisible();
    void MakeHidden();
    bool IsVisible() const;
};
