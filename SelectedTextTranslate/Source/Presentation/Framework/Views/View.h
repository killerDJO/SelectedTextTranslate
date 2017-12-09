#pragma once
#include "Presentation\Framework\Dto\ViewStateDescriptor.h"
#include "Presentation\Framework\Rendering\RenderingContext.h"
#include "Presentation\Framework\Rendering\Renderer.h"
#include "Presentation\Framework\NativeWindowHolder.h"
#include "Presentation\Framework\Providers\ScrollProvider.h"
#include "Presentation\Framework\ViewContext.h"

class ViewContext;
class Renderer;
class ScrollProvider;
class ViewStateDescriptor;

class View
{
private:
    vector<View*> destroyedChildViews;
    vector<View*> activeChildViews;

    DWORD GetScrollStyle() const;
    Rect GetWindowBoundingRect() const;

    void ApplyViewState(bool preserveScrolls);

    void RenderToBuffer();
    Size ComputeContentSize(Renderer* renderer);

    void DrawFromBuffer();
    void DrawChildViews();
    void DestroyChildViews(vector<View*>& childViews) const;

protected:
    ViewContext* Context;
    ScaleProvider* ScaleProvider;
    ScrollProvider* ScrollProvider;
    RenderingProvider* RenderingProvider;
    RenderingContext* RenderingContext;
    DeviceContextProvider* DeviceContextProvider;

    NativeWindowHolder* Window;
    ViewStateDescriptor* ViewState;
    wstring Name;

    void DestroyChildViews();
    void AssertViewInitialized() const;
    void AssertViewNotInitialized() const;

    virtual void SpecifyWindow(NativeWindowHolder* window);
    virtual Point GetInitialViewOffset() const;
    virtual void RenderContent(Renderer* renderer) = 0;

    //TODO: move to private
    void ApplyViewPosition(bool preserveScrolls);

public:
    View(ViewContext* context);
    virtual ~View();

    NativeWindowHolder* GetWindow() const;

    Size GetContentSize() const;
    Size GetAvailableClientSize() const;
    Rect GetBoundingRect() const;

    void MakeVisible() const;
    void MakeHidden() const;
    bool IsVisible() const;

    void AddChildView(View* childView);

    virtual void Initialize();
    virtual void Render(bool preserveScrolls = false);
    void InitializeAndRender(bool preserveScrolls = false);
    void Draw(bool drawChildren = false);
};