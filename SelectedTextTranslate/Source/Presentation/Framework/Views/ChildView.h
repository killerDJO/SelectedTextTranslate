#pragma once
#include "Presentation\Framework\Views\View.h"

class ChildView : public View
{
private:
    LRESULT ProcessMove(WPARAM wParam, LPARAM lParam);
    LRESULT ProcessButtonDown(WPARAM wParam, LPARAM lParam);

protected:
    View* ParentView;
    bool IsLayered;

    void SpecifyWindow(NativeWindowHolder* window) override;
    Point GetInitialViewOffset() const override;

public:
    ChildView(ViewContext* context, View* parentView);

    void Initialize() override;

    void EnableLayeredMode();
};