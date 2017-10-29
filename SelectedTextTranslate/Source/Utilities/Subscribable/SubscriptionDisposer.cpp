#include "Utilities\Subscribable\SubscriptionDisposer.h"

SubscriptionDisposer::SubscriptionDisposer(function<void()> disposeFunction)
{
    this->disposeFunction = disposeFunction;
}

SubscriptionDisposer::~SubscriptionDisposer()
{
    disposeFunction();
}