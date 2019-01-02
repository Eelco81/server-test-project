
function Simulator() {
    
    let self = this;
    let loaded = false;
    
    const root = "api/simulation";
    
    const logError = function (data) {
        console.log(data.responseJSON ? data.responseJSON.error : data.responseTEXT);
    };
    
    const getSamplers = function(callback) {
        $.ajax({
            url: root + "/samplers", 
            type: 'GET',
            error: logError,
            success: function(data){
                callback(data.samplers);
            }
        });
    }
    
    const websock = new WebSocket("ws://localhost:1704");
    websock.onmessage = function(evt) { 
    
        const data = JSON.parse(evt.data);
        const eventId = data["event-id"];
        const eventData = data["event-data"];
        
        if (eventId === "sim-started") {
           getSamplers(function(samplers) {
                loaded = true;
                self.samplers = samplers;
                self.onStart();
            });
        }
        else if (eventId === "sim-stopped") {
            loaded = false;
            self.onStop();
        }
        else if (loaded) {
            self.onSample(eventId, eventData);
        }
    }
    
    this.onStart = function() {}
    this.onStop = function() {}
    this.onSample = function() {}
    
    this.samplers = [];
    
    this.start = function(id) {
        $.ajax({
            url: root, 
            type: 'PUT', 
            data: JSON.stringify({ id: id }),
            error: logError
        });
    }
    
    this.stop = function() {
        $.ajax({
            url: root, 
            type: 'DELETE',
            error: logError
        });
    }
    
    this.getConfigs = function(callback) {
        $.ajax( {
            url: root + "/config", 
            type: 'GET',
            error: logError,
            success: callback
        });
    }
    
}