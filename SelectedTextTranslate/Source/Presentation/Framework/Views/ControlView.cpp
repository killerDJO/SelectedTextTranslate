#include "Presentation\Framework\Views\View.h"
#include "Presentation\Framework\Views\ControlView.h"

ControlView::ControlView(ViewContext* context, View* parentView)
    : ChildView(context, parentView)
{
}