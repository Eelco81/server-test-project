
template<typename Input_t, typename Output_t>
void OS::MessageStream<Input_t,Output_t>::Pipe (OS::MessageStream<Input_t,Output_t>::Callback inCallback) {
    mCallbacks.push_back (inCallback);
}

template<typename Input_t, typename Output_t>
void OS::MessageStream<Input_t,Output_t>::Done (const Output_t& inMessage) {
    for (auto callback : mCallbacks) {
        callback (inMessage);
    }
}

template<typename Input_t, typename Output_t>
OS::MessageStream<Input_t,Output_t>& OS::MessageStream<Input_t,Output_t>::Clear () {
    mCallbacks.clear ();
    return *this;
}
