#include "Presentation\Framework\Views\View.h"
#include "Presentation\Framework\Views\ControlView.h"

ControlView::ControlView(CommonContext* context, View* parentView)
    : ChildView(context, parentView)
{
}