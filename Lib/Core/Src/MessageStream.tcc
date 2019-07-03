
template<typename Input_t, typename Output_t>
void OS::MessageStream<Input_t,Output_t>::Pipe (OS::MessageStream<Input_t,Output_t>::Callback inCallback) {
    sNext.Connect (inCallback);
}

template<typename Input_t, typename Output_t>
template <typename T>
void OS::MessageStream<Input_t,Output_t>::Pipe (T* inObject, void(T::*inMethod)(const Output_t&)) {
    Pipe ([inObject, inMethod] (const Output_t& data) { (inObject->*inMethod) (data); });
}

template<typename Input_t, typename Output_t>
template<typename T>
OS::MessageStream<Output_t,T>& OS::MessageStream<Input_t,Output_t>::Pipe (MessageStream<Output_t,T>& inStream) {
    Pipe ([&] (const Output_t& data) { inStream.Write (data); });
    return inStream;
}

template<typename Input_t, typename Output_t>
void OS::MessageStream<Input_t, Output_t>::Pipe (OS::Signal<const Output_t&>& inSignal) {
    sNext.Forward (inSignal);
}

template<typename Input_t, typename Output_t>
OS::MessageStream<Input_t,Output_t>& OS::MessageStream<Input_t,Output_t>::Clear () {
    sNext.DisconnectAll ();
    return *this;
}

