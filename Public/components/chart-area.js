
'use strict';

import $ from "jquery"

import ChartGraph from "./chart-graph.js"
import simulator from "./../models/simulator.js"

export default {
    components: {
        "chart-graph": ChartGraph
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
            <chart-graph 
                v-for="sampler in samplers" 
                v-bind:key="sampler.id" 
                v-bind:sampler="sampler"
            ></chart-graph>
        </div>`
};
