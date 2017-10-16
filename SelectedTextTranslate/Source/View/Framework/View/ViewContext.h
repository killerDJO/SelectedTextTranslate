#pragma once
#include "Infrastructure\ErrorHandling\ErrorHandler.h"
#include "CompositionRoot.h"

class ScrollProvider;
class RenderingContext;

class ViewContext
{
    HINSTANCE hInstance;
    ErrorHandler* errorHandler;
    CompositionRoot* compositionRoot;

public:
    ViewContext(
        HINSTANCE hInstance,
        CompositionRoot* compositionRoot,
        ErrorHandler* errorHandler);

    HINSTANCE GetInstance() const;
    ErrorHandler* GetErrorHandler() const;

    template <typename TService>
    TService* Get() const;
};

inline ViewContext::ViewContext(HINSTANCE hInstance, CompositionRoot* compositionRoot, ErrorHandler* errorHandler)
{
    this->hInstance = hInstance;
    this->compositionRoot = compositionRoot;
    this->errorHandler = errorHandler;
}

inline HINSTANCE ViewContext::GetInstance() const
{
    return hInstance;
}

inline ErrorHandler* ViewContext::GetErrorHandler() const
{
    return errorHandler;
}

template <typename TService>
TService* ViewContext::Get() const
{
    return compositionRoot->GetService<TService>();
}