#pragma once
#include "Infrastructure\ErrorHandling\ErrorHandler.h"
#include "Infrastructure\CompositionRoot.h"

class ScrollProvider;
class RenderingContext;

class CommonContext
{
    HINSTANCE hInstance;
    ErrorHandler* errorHandler;
    CompositionRoot* compositionRoot;

public:
    CommonContext(
        HINSTANCE hInstance,
        CompositionRoot* compositionRoot,
        ErrorHandler* errorHandler);

    HINSTANCE GetInstance() const;
    ErrorHandler* GetErrorHandler() const;

    template <typename TService>
    TService* Get() const;
};

inline CommonContext::CommonContext(HINSTANCE hInstance, CompositionRoot* compositionRoot, ErrorHandler* errorHandler)
{
    this->hInstance = hInstance;
    this->compositionRoot = compositionRoot;
    this->errorHandler = errorHandler;
}

inline HINSTANCE CommonContext::GetInstance() const
{
    return hInstance;
}

inline ErrorHandler* CommonContext::GetErrorHandler() const
{
    return errorHandler;
}

template <typename TService>
TService* CommonContext::Get() const
{
    return compositionRoot->GetService<TService>();
}