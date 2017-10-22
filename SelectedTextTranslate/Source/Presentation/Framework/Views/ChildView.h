#pragma once
#include "Presentation\Framework\Views\View.h"

class ChildView : public View
{
protected:
    View* parentView;
    bool isLayered;

    Point GetInitialViewOffset() override;
    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

public:
    ChildView(CommonContext* context, View* parentView);

    void Initialize() override;

    void EnableLayeredMode();
};