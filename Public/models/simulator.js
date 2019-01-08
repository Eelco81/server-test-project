
'use strict';

const logSimulatorError = function (data) {
    console.log(data.responseJSON ? data.responseJSON.error : data.responseTEXT);
};

class Simulator extends EventEmitter {

    constructor(){
        
        super();
        
        const self = this;
        
        this._root = "api/simulation";
        
        const websock = new WebSocket("ws://localhost:1704");
        websock.onmessage = function(evt) { 
        
            const data = JSON.parse(evt.data);
            const eventId = data["event-id"];
            const eventData = data["event-data"];
            
            self.emit(eventId, eventData);
        }
    }
    
    start(id) {
        $.ajax({
            url: this._root, 
            type: 'PUT', 
            data: JSON.stringify({ id: id }),
            error: logSimulatorError
        });
    }
    
    stop() {
        $.ajax({
            url: this._root, 
            type: 'DELETE',
            error: logSimulatorError
        });
    }
    
    getConfigs(callback) {
        $.ajax( {
            url: this._root + "/config", 
            type: 'GET',
            error: logSimulatorError,
            success: callback
        });
    }
    
    getSamplers(callback) {
        $.ajax({
            url: this._root + "/samplers", 
            type: 'GET',
            error: logSimulatorError,
            success: function(data){
                callback(data.samplers);
            }
        });
    }
    
    getPorts(callback) {
        $.ajax({
            url: this._root + "/ports", 
            type: 'GET',
            error: logSimulatorError,
            success: function(data){
                callback(data.ports);
            }
        });
    }
    
}
