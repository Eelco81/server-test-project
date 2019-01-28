
'use strict';

export default {
    inject: [ 'getSimulator' ],
    data: function() {
        return {
            ports: []
        }
    },
    mounted: function() {
        const self = this;
        self.revokeSubscription = self.getSimulator().subscribe("sim-started", function(){
            self.getSimulator().getPorts(function(ports){
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
            this.getSimulator.setPort(port);
        }
    },
    template: `
        <div class="port-table">
            <table class="table table-striped table-sm">
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
                            <form class="form-inline">
                                <input type="number" class="form-control form-control-sm mr-sm-2" v-model="port.value"/>
                                <button type="button" class="btn btn-secondary btn-sm" v-on:click="submit(port)">
                                    <i class="fas fa-sign-in-alt"></i>
                                </button>
                            </form>
                        </td>
                    </tr>
                </tbody>
            </table>
        </div>
        `
};
