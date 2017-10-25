#pragma once
#include "Presentation\Framework\Dto\LayoutDescriptor.h"
#include "Presentation\Framework\Dto\ViewStateDescriptor.h"
#include "Presentation\Framework\Rendering\DeviceContextBuffer.h"
#include "Presentation\Framework\Rendering\RenderingContext.h"
#include "Presentation\Framework\Rendering\Renderer.h"
#include "Presentation\Framework\NativeWindowHolder.h"
#include "Presentation\Framework\CommonContext.h"

class CommonContext;
class Renderer;

class View : public NativeWindowHolder
{
private:
    vector<View*> destroyedChildViews;

    void DrawChildViews();
    void DestroyChildViews(vector<View*>& childViews) const;

protected:
    CommonContext* context;
    ScaleProvider* scaleProvider;
    ScrollProvider* scrollProvider;
    RenderingProvider* renderingProvider;
    RenderingContext* renderingContext;
    DeviceContextProvider* deviceContextProvider;

    LayoutDescriptor layoutDescriptor;
    ViewStateDescriptor viewStateDescriptor;
    wstring viewName;

    DeviceContextBuffer* deviceContextBuffer;
    vector<View*> activeChildViews;

    DWORD GetScrollStyle() const;

    void DestroyChildViews();

    void ApplyViewState(bool preserveScrolls);
    void ApplyViewPosition(bool preserveScrolls);
    Size RenderToBuffer();
    virtual Size RenderContent(Renderer* renderer) = 0;
    virtual Point GetInitialViewOffset();

    LRESULT ExecuteWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;
    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

    void AssertViewInitialized() const;
    void AssertViewNotInitialized() const;

public:
    View(CommonContext* context);
    ~View() override;

    LayoutDescriptor GetLayout() const;

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