
class EventEmitter {
    
    constructor() {
        this._events = {};
    }
    
    subscribe(eventName, fn) {
        if(!this._events[eventName]) {
            this._events[eventName] = [];
        }
        this._events[eventName].push(fn);
    }
    
    emit(eventName, data1, data2) {
        const event = this._events[eventName];
        if(event) {
            event.forEach(function(fn) {
                fn.call(null, data1, data2);
            });
        }
    }
    
}
