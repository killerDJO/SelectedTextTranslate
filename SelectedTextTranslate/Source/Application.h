#pragma once
#include "Presentation\Framework\Dto\LayoutDescriptor.h"
#include "Presentation\Framework\Providers\ScaleProvider.h"
#include "Infrastructure\Logging\Logger.h"

class Application
{
private:
    int BootstrapApplication(Logger* logger, HINSTANCE hInstance) const;
    LayoutDescriptor GetMainWindowDescriptor(ScaleProvider* scaleProvider) const;
    void Exit() const;

public:
    Application();

    int Run(HINSTANCE hInstance) const;
};
