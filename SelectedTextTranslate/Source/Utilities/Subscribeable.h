#pragma once

template<class ... Types>
class Subscribeable
{
private:
    vector<function<void(Types ... args)>> callbacks;
    vector<Subscribeable<Types ...>*> subscribeables;

public:
    Subscribeable();
    ~Subscribeable();

    void Subscribe(function<void(Types ... args)> callback);
    void Subscribe(Subscribeable<Types ...>* subscribeable);

    void UnsubscribeAll();

    void Notify(Types ... args);
};

template <class ... Types>
Subscribeable<Types...>::Subscribeable()
{
    callbacks = vector<function<void(Types ... args)>>();
    subscribeables = vector<Subscribeable<Types ...>*>();
}

template <class ... Types>
void Subscribeable<Types...>::Subscribe(function<void(Types ... args)> callback)
{
    callbacks.push_back(callback);
}

template <class ... Types>
void Subscribeable<Types...>::Subscribe(Subscribeable<Types ...>* subscribeable)
{
    for (size_t i = 0; i < subscribeables.size(); ++i)
    {
        if (subscribeables[i] == subscribeable)
        {
            return;
        }
    }

    subscribeables.push_back(subscribeable);
}

template <class ... Types>
void Subscribeable<Types...>::UnsubscribeAll()
{
    callbacks.clear();
    subscribeables.clear();
}

template <class ... Types>
void Subscribeable<Types...>::Notify(Types ... args)
{
    for (size_t i = 0; i < callbacks.size(); ++i)
    {
        callbacks[i](forward<Types>(args)...);
    }

    for (size_t i = 0; i < subscribeables.size(); ++i)
    {
        subscribeables[i]->Notify(forward<Types>(args)...);
    }
}

template <class ... Types>
Subscribeable<Types...>::~Subscribeable()
{
}