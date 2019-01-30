
'use strict';

import EventEmitter from './event-emitter.js'
import $ from "jquery"

const logError = function (data) {
    console.log(data.responseJSON ? data.responseJSON.error : data.responseTEXT);
};

export default class Simulator extends EventEmitter {

    constructor() {
        
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
            error: logError
        });
    }
    
    stop() {
        $.ajax({
            url: this._root, 
            type: 'DELETE',
            error: logError
        });
    }
    
    getConfigs(callback) {
        $.ajax( {
            url: this._root + "/config", 
            type: 'GET',
            error: logError,
            success: callback
        });
    }
    
    getSamplers(callback) {
        $.ajax({
            url: this._root + "/samplers", 
            type: 'GET',
            error: logError,
            success: function(data){
                callback(data.samplers);
            }
        });
    }
    
    getPorts(callback) {
        $.ajax({
            url: this._root + "/ports", 
            type: 'GET',
            error: logError,
            success: function(data){
                callback(data.ports);
            }
        });
    }
    
    setPort(port) {
        $.ajax({
            url: this._root + "/ports/" + port.path,
            type: "PUT",
            data: JSON.stringify({ value: Number(port.value) }),
            error: logError,
            succes: function() {
                console.log("Succesfully set <" + port.path + ">");
            }
        });
    }
}
