#pragma once
#include "Controllers\AppController.h"

class AppController;

class ErrorStateProvider
{
private:
    AppController* appController;

public:
    ErrorStateProvider(AppController* appController);
    ~ErrorStateProvider();

    void ShowError(wstring message) const;
};