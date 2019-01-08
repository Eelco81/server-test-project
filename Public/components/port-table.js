
'use strict';

Vue.component('port-table', {
    props: ['sim'],
    data: function() {
        return {
            ports: []
        }
    },
    mounted: function() {
        const self = this;
        this.sim.subscribe("sim-started", function(){
            self.sim.getPorts(function(ports){
                self.ports = ports;
            });
        });
    },
    template: `
        <ol> 
            <li v-for="port in ports" v-bind:key="port.path"> 
                <div>{{port.path}} + " " + {{port.value}}</div>
            </li>
        </ol>`
});
