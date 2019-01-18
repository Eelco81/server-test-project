
'use strict';

import ChartGraph from "./chart-graph.js"
import $ from "jquery"

export default {
    props: ['sim'],
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
        this.revokeStartSubscription = this.sim.subscribe("sim-started", function(){
            self.sim.getSamplers(function(samplers){
                self.samplers = samplers;
            });
        });
        this.revokeStopSubscription = this.sim.subscribe("sim-stopped", function(){
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
                v-bind:sim="sim"
            ></chart-graph>
        </div>`
};
