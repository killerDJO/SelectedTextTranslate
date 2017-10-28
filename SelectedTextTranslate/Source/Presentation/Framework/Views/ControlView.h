#pragma once
#include "Presentation\Framework\Views\ChildView.h"

class ControlView : public ChildView
{
public:
    ControlView(ViewContext* context, View* parentView);
};