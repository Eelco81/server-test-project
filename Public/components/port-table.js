
'use strict';

export default {
    props: ['sim'],
    data: function() {
        return {
            ports: []
        }
    },
    mounted: function() {
        const self = this;
        this.revokeSubscription = this.sim.subscribe("sim-started", function(){
            self.sim.getPorts(function(ports){
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
            this.sim.setPort(port);
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
                            <button type="button" class="btn btn-secondary" v-on:click="submit(port)">Submit</button>
                        </td>
                    </tr>
                </tbody>
            </table>
        </div>
        `
};
