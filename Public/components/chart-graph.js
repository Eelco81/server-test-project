
'use strict';

Vue.component('chart-graph', {
    props: ['sampler', 'sim'],
    mounted: function() {
        
        const chart = new Chart($('#' + this.sampler.id)[0], this.sampler.id, this.sampler.type);
        chart.init(this.sampler.ports);
        chart.draw();
        
        this.sim.subscribe(this.sampler.id, function(data) {
            chart.update(data);
            chart.draw();
        });
    },
    template: '<div v-bind:id="sampler.id" style="width: 500px; height: 300px; float:left"></div>'
});
