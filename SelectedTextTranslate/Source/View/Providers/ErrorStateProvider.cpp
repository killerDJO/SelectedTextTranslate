#include "View\Providers\ErrorStateProvider.h"

ErrorStateProvider::ErrorStateProvider(AppController* appController)
{
    this->appController = appController;
}

void ErrorStateProvider::ShowError(wstring message) const
{
    appController->ShowError(message);
}

ErrorStateProvider::~ErrorStateProvider()
{
}