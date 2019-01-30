
'use strict';

import SimChart from "./sim-chart.js"
import SimXyChart from "./sim-xy-chart.js"

export default {
    inject: [ 'simService' ],
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
        self.revokeStartSubscription = self.simService().subscribe("sim-started", function(){
            self.simService().getSamplers(function(samplers){
                self.samplers = samplers;
            });
        });
        this.revokeStopSubscription = this.simService().subscribe("sim-stopped", function(){
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
                <sim-xy-chart v-else v-bind:sampler="sampler"></sim-xy-chart>
            </div>
        </div>`
};
