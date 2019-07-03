
template <class ...Args>
class OS::Signal<Args...>::Implementation : public AbstractSignal {

public:

    void Emit (Args... args) {
        for (auto& callback : mCallbacks) {
            callback.second(args...);
        }
    }

    Connection Connect (Callback inCallback) {
        auto listedCb = std::pair<uint64_t, Callback>(mIndex++, inCallback);
        mCallbacks.push_back (listedCb);
        return Connection (shared_from_this (), listedCb.first);
    }

    virtual void Disconnect (uint64_t inIndex) override {
        const auto remover = [inIndex](const auto& listedCb) { return listedCb.first == inIndex; };
        mCallbacks.erase (std::remove_if (mCallbacks.begin (), mCallbacks.end (), remover), mCallbacks.end ());
    }

    void DisconnectAll() {
        mCallbacks.clear ();
    }

private:
    std::vector<std::pair<uint64_t, Callback>> mCallbacks = {};
    uint64_t mIndex = 0u;
};

template <class ...Args>
OS::Signal<Args...>::Signal () :
    mImpl (std::make_shared<Implementation> ())
{
}

template <class ...Args>
OS::Signal<Args...>::Signal (const OS::Signal<Args...>& inSignal) :
    OS::Signal<Args...>::Signal ()
{
}

template <class ...Args>
void OS::Signal<Args...>::Emit(Args...args) {
    mImpl->Emit (args...);
}

template <class ...Args>
OS::Connection OS::Signal<Args...>::Connect (OS::Signal<Args...>::Callback inCallback) {
    return mImpl->Connect (inCallback);
}

template <class ...Args>
template <typename T>
OS::Connection OS::Signal<Args...>::Connect (T* inObject, void(T::*inMethod)(Args...)) {
    return mImpl->Connect ([inObject, inMethod](Args...args) { (inObject->*inMethod) (args...); });
}

template <class ...Args>
OS::Connection OS::Signal<Args...>::Forward (OS::Signal<Args...>& inSignal) {
    return mImpl->Connect ([&](Args...args) { inSignal.Emit (args...); });
}
/*
template <class ...Args>
template <typename T>
OS::MessageStream<std::tuple<Args...>, T>& OS::Signal<Args...>::Pipe(OS::MessageStream<std::tuple<Args...>, T>& inStream) {

}
*/

template <class ...Args>
void OS::Signal<Args...>::DisconnectAll () {
    return mImpl->DisconnectAll ();
}