
'use strict';

import SimChart from "./sim-chart.js"
import SimXyChart from "./sim-xy-chart.js"
import simulator from "./../models/simulator.js"

export default {
    components: {
        "sim-chart" : SimChart,
        "sim-xy-chart" : SimXyChart
    },
    data: function() {
        return {
            samplers: []
        }
    },
    mounted: function() {
        const self = this;
        this.revokeStartSubscription = simulator.subscribe("sim-started", function(){
            simulator.getSamplers(function(samplers){
                self.samplers = samplers;
            });
        });
        this.revokeStopSubscription = simulator.subscribe("sim-stopped", function(){
            self.samplers = [];
        });
    },
    beforeDestroy: function() {
        this.revokeStartSubscription();
        this.revokeStopSubscription();
    },
    template: `
        <div>
            <div class="chart-graph" v-for="sampler in samplers" v-bind:key="sampler.id" >
                <sim-chart v-if="sampler.type==0" v-bind:sampler="sampler"></sim-chart>
                <sim-xy-chart v-else v-bind:sampler="sampler"></sim-chart>
            </div>
        </div>`
};
