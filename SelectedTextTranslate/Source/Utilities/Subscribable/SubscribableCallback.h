#pragma once

template<class ... Types>
class SubscribableCallback
{
private:
    function<void(Types ... args)> callbackFunction;
    int id;

public:
    SubscribableCallback(int id, function<void(Types ... args)> callbackFunction);

    void Invoke(Types... args);
    int GetId() const;
};

template <class ... Types>
SubscribableCallback<Types...>::SubscribableCallback(int id, function<void(Types... args)> callbackFunction)
{
    this->id = id;
    this->callbackFunction = callbackFunction;
}

template <class ... Types>
void SubscribableCallback<Types...>::Invoke(Types... args)
{
    callbackFunction(forward<Types>(args)...);
}

template <class ... Types>
int SubscribableCallback<Types...>::GetId() const
{
    return id;
}
