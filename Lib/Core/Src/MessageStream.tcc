
template<typename Input_t, typename Output_t>
void OS::MessageStream<Input_t,Output_t>::Pipe (OS::MessageStream<Input_t,Output_t>::Callback inCallback) {
    mCallbacks.push_back (inCallback);
}

template<typename Input_t, typename Output_t>
void OS::MessageStream<Input_t,Output_t>::Emit (const Output_t& inMessage) {
    for (auto callback : mCallbacks) {
        callback (inMessage);
    }
}

template<typename Input_t, typename Output_t>
OS::MessageStream<Input_t,Output_t>& OS::MessageStream<Input_t,Output_t>::Clear () {
    mCallbacks.clear ();
    return *this;
}

template<typename T>
void OS::ForwardStream<T>::Write (const T& inMessage) {
    this->Emit (inMessage);
}