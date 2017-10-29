#pragma once
#include "Utilities\Subscribable\SubscriptionDisposer.h"
#include "Utilities\Subscribable\SubscribableCallback.h"

template<class ... Types>
class Subscribable
{
private:
    int idGenerator = 0;
    vector<SubscribableCallback<Types...>> subscriptions;

    SubscriptionDisposer* Subscribe(SubscribableCallback<Types...> callback);
    void RemoveSubscription(int id);

public:
    SubscriptionDisposer* Subscribe(function<void(Types ... args)> callback);
    SubscriptionDisposer* Subscribe(Subscribable<Types ...>* subscribable);

    void UnsubscribeAll();

    void Notify(Types ... args);
};

template <class ... Types>
SubscriptionDisposer* Subscribable<Types...>::Subscribe(function<void(Types ... args)> callback)
{
    return Subscribe(SubscribableCallback<Types...>(++idGenerator, callback));
}

template <class ... Types>
SubscriptionDisposer* Subscribable<Types...>::Subscribe(Subscribable<Types ...>* subscribable)
{
    return Subscribe(SubscribableCallback<Types...>(++idGenerator, [subscribable](Types ... args) { subscribable->Notify(forward<Types>(args)...); }));
}

template <class ... Types>
SubscriptionDisposer* Subscribable<Types...>::Subscribe(SubscribableCallback<Types...> callback)
{
    subscriptions.push_back(callback);
    return new SubscriptionDisposer([this, callback]() { RemoveSubscription(callback.GetId()); });
}

template <class ... Types>
void Subscribable<Types...>::UnsubscribeAll()
{
    subscriptions.clear();
}

template <class ... Types>
void Subscribable<Types...>::RemoveSubscription(int id)
{
    for (auto iterator = subscriptions.begin(); iterator != subscriptions.end(); ++iterator)
    {
        if (iterator->GetId() == id)
        {
            subscriptions.erase(iterator);
            break;
        }
    }
}

template <class ... Types>
void Subscribable<Types...>::Notify(Types ... args)
{
    for (size_t i = 0; i < subscriptions.size(); ++i)
    {
        subscriptions[i].Invoke(forward<Types>(args)...);
    }
}