#pragma once
#include "View\Framework\Dto\WindowDescriptor.h"
#include "View\Framework\Dto\WindowNativeStateDescriptor.h"
#include "View\Framework\Enums\ViewStates.h"
#include "View\Framework\Rendering\DeviceContextBuffer.h"
#include "View\Framework\Rendering\RenderingContext.h"
#include "View\Framework\Rendering\Renderer.h"
#include "View\Framework\NativeWindowHolder.h"
#include "View\Framework\View\ViewContext.h"

class ViewContext;
class Renderer;

class View : public NativeWindowHolder
{
private:
    vector<View*> destroyBeforeDrawList;

    void DrawChildViews();
    void DestroyChildViews(vector<View*>& childViews) const;

protected:
    ViewContext* context;
    ScaleProvider* scaleProvider;
    ScrollProvider* scrollProvider;
    RenderingProvider* renderingProvider;
    RenderingContext* renderingContext;
    DeviceContextProvider* deviceContextProvider;

    WindowDescriptor descriptor;
    WindowNativeStateDescriptor nativeStateDescriptor;
    wstring viewName;

    Size contentSize;
    ViewStates viewState;

    DeviceContextBuffer* deviceContextBuffer;
    vector<View*> activeChildViews;

    DWORD GetScrollStyle() const;

    void DestroyChildViews();

    void ApplyNativeState(bool preserveScrolls);
    void ApplyViewPosition(bool preserveScrolls);
    Size RenderToBuffer();
    virtual Size RenderContent(Renderer* renderer) = 0;
    virtual Point GetInitialViewOffset();

    LRESULT ExecuteWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;
    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

    void AssertViewInitialized() const;
    void AssertViewNotInitialized() const;

public:
    View(ViewContext* context);
    ~View() override;

    WindowDescriptor GetDescriptor() const;
    virtual void SetDescriptor(WindowDescriptor descriptor);

    Size GetContentSize() const;
    Size GetSize() const;
    Size GetClientSize() const;
    Point GetPosition() const;
    Rect GetBoundingRect() const;

    void MakeVisible();
    void MakeHidden();
    bool IsVisible() const;

    virtual void Show();
    virtual void Hide();

    void AddChildView(View* childView);

    virtual void Render(bool preserveScrolls = false);
    void Draw(bool drawChildren = false);
    void Initialize() override;
    void InitializeAndRender(bool preserveScrolls = false);

    virtual void Resize();
};