
'use strict';

Vue.component('chart-area', {
    props: ['sim'],
    data: function() {
        return {
            samplers: []
        }
    },
    mounted: function() {
        const self = this;
        this.sim.subscribe("sim-started", function(){
            self.sim.getSamplers(function(samplers){
                self.samplers = samplers;
            });
        });
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
});
