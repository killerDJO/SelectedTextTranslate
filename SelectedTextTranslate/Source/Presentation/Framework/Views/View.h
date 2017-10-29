#pragma once
#include "Presentation\Framework\Dto\LayoutDescriptor.h"
#include "Presentation\Framework\Dto\ViewStateDescriptor.h"
#include "Presentation\Framework\Rendering\DeviceContextBuffer.h"
#include "Presentation\Framework\Rendering\RenderingContext.h"
#include "Presentation\Framework\Rendering\Renderer.h"
#include "Presentation\Framework\NativeWindowHolder.h"
#include "Presentation\Framework\Providers\ScrollProvider.h"
#include "Presentation\Framework\ViewContext.h"

class ViewContext;
class Renderer;
class ScrollProvider;

class View : public NativeWindowHolder
{
private:
    vector<View*> destroyedChildViews;
    vector<View*> activeChildViews;

    void DrawChildViews();
    void DestroyChildViews(vector<View*>& childViews) const;

protected:
    ViewContext* Context;
    ScaleProvider* ScaleProvider;
    ScrollProvider* ScrollProvider;
    RenderingProvider* RenderingProvider;
    RenderingContext* RenderingContext;
    DeviceContextProvider* DeviceContextProvider;

    LayoutDescriptor Layout;
    ViewStateDescriptor State;
    wstring Name;

    DeviceContextBuffer* DeviceContextBuffer;

    void DestroyChildViews();

    DWORD GetWindowStyle() const override;
    Rect GetWindowRectangle() const override;

    virtual Point GetInitialViewOffset() const;
    void ApplyViewState(bool preserveScrolls);
    void ApplyViewPosition(bool preserveScrolls);

    Size RenderToBuffer();
    virtual Size RenderContent(Renderer* renderer) = 0;

    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;
    LRESULT ExecuteWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;
    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

    void AssertViewInitialized() const;
    void AssertViewNotInitialized() const;

public:
    View(ViewContext* context);
    ~View() override;

    LayoutDescriptor GetLayout() const;

    Size GetContentSize() const;
    Size GetAvailableClientSize() const;
    Size GetCurrentClientSize() const;
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