#pragma once
#include "Presentation\Framework\Views\ChildView.h"

class ControlView : public ChildView
{
public:
    ControlView(CommonContext* context, View* parentView);
};