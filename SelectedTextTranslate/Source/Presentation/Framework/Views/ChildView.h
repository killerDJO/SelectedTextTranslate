#pragma once
#include "Presentation\Framework\Views\View.h"

class ChildView : public View
{
protected:
    View* ParentView;
    bool IsLayered;

    Point GetInitialViewOffset() override;
    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

public:
    ChildView(CommonContext* context, View* parentView);

    void Initialize() override;

    void EnableLayeredMode();
};