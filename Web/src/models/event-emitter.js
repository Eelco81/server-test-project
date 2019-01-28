
'use strict';

export default class EventEmitter {
    
    constructor() {
        this._events = {};
    }
    
    subscribe(eventName, fn) {
        const self = this;
        if(!this._events[eventName]) {
            this._events[eventName] = [];
        }
        this._events[eventName].push(fn);
        
        return function unsubscribe() {
            self._events[eventName] = self._events[eventName].filter(function(evFn) { return evFn !== fn; });
        }
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

//export { EventEmitter as default }
