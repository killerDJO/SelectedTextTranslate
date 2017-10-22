#pragma once
#include "Presentation\Framework\Dto\WindowDescriptor.h"
#include "Presentation\Framework\Providers\ScaleProvider.h"
#include "Infrastructure\Logging\Logger.h"

class Application
{
private:
    int BootstrapApplication(Logger* logger, HINSTANCE hInstance) const;
    WindowDescriptor GetMainWindowDescriptor(ScaleProvider* scaleProvider) const;

public:
    Application();

    int Run(HINSTANCE hInstance) const;
};
