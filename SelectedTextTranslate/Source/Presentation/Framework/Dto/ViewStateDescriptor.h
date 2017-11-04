#pragma once
#include "Presentation\Framework\Dto\Positioning\Point.h"
#include "Presentation\Framework\Dto\Positioning\Size.h"
#include "Presentation\Framework\Dto\Positioning\Rect.h"
#include "Presentation\Framework\Enums\ViewStates.h"
#include "Presentation\Framework\Rendering\DeviceContextBuffer.h"
#include "Presentation\Framework\Dto\LayoutDescriptor.h"
#include "Presentation\Framework\Rendering\Renderer.h"

class Renderer;

class ViewStateDescriptor
{
private:
    LayoutDescriptor layout;

    Size windowSize;

    //In case of the Scrollable window used to detirmine size of content to scroll. For other window types always equal to the window size.
    Size contentSize;

    Point position;
    bool isVisible;

    ViewStates viewState;

    DeviceContextProvider* deviceContextProvider;
    DeviceContextBuffer* deviceContextBuffer;

    void StretchToSize(Size size);

public:
    ViewStateDescriptor(DeviceContextProvider* deviceContextProvider);
    ~ViewStateDescriptor();

    void SetLayout(LayoutDescriptor layout);
    LayoutDescriptor GetLayout() const;

    void ResetToLayout();

    void UpdateContent(Renderer* renderer);
    DeviceContextBuffer* GetDeviceContextBuffer() const;

    Size GetWindowSize() const;
    
    Rect GetBoundingRect() const;

    Point GetPosition() const;
    void SetPosition(Point position);

    Size GetContentSize() const;
    void SetContentSize(Size contentSize);

    ViewStates GetViewState() const;
    void SetViewState(ViewStates viewState);

    void MakeVisible();
    void MakeHidden();
    bool IsVisible() const;
};
