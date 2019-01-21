
'use strict';

import simulator from "./../models/simulator.js"

export default {
    data: function() {
        return {
            ports: []
        }
    },
    mounted: function() {
        const self = this;
        this.revokeSubscription = simulator.subscribe("sim-started", function(){
            simulator.getPorts(function(ports){
                self.ports = ports.filter(function(port) { 
                    return port.path.match(/\.par\./);
                });
            });
        });
    },
    beforeDestroy: function() {
        this.revokeSubscription();
    },
    methods: {
        submit: function(port) {
            simulator.setPort(port);
        }
    },
    template: `
        <div class="port-table">
            <table class="table table-striped">
                <thead>
                    <tr>
                        <th scope="col">Path</th>
                        <th scope="col">Value</th>
                    </tr>
                </thead>
                <tbody>
                    <tr v-for="port in ports" v-bind:key="port.path">
                        <td>{{port.path}}</td>
                        <td>
                            <input type="number" v-model="port.value"/>
                            <button type="button" class="btn btn-secondary" v-on:click="submit(port)">
                                <span class="glyphicon glyphicon-star" aria-hidden="true"></span>
                            </button>
                        </td>
                    </tr>
                </tbody>
            </table>
        </div>
        `
};
