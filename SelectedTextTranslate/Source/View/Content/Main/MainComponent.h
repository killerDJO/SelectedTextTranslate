#pragma once
#include "View\Framework\View\Component.h"
#include "View\Providers\HotkeyProvider.h"

class MainComponent : public Component
{
public:
    MainComponent(ViewContext* context, HotkeyProvider* hotkeyProvider);
    ~MainComponent();
};
