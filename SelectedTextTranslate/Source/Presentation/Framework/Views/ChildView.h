#pragma once
#include "Presentation\Framework\Views\View.h"

class ChildView : public View
{
protected:
    View* ParentView;
    bool IsLayered;

    Point GetInitialViewOffset() const override;
    DWORD GetWindowStyle() const override;
    DWORD GetExtendedWindowStyles() const override;
    HWND GetWindowParent() const override;

    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

public:
    ChildView(ViewContext* context, View* parentView);

    void Initialize() override;

    void EnableLayeredMode();
};