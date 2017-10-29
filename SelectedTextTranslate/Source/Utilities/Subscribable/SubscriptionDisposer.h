#pragma once

class SubscriptionDisposer
{
private:
    function<void()> disposeFunction;

public:
    SubscriptionDisposer(function<void()> disposeFunction);
    ~SubscriptionDisposer();
};