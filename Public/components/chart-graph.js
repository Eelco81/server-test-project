
'use strict';

import Chart from "./../models/chart.js"

export default {
    props: ['sampler', 'sim'],
    mounted: function() {
        
        const chart = new Chart(this.$el, this.sampler.id, this.sampler.type);
        chart.init(this.sampler.ports);
        chart.draw();
        
        this.revokeSubscription = this.sim.subscribe(this.sampler.id, function(data) {
            chart.update(data);
            chart.draw();
        });
    },
    beforeDestroy: function() {
        this.revokeSubscription();
    },
    template: '<div class="chart-graph"></div>'
};
