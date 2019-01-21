
'use strict';

import Chart from "./../models/chart.js"
import simulator from "./../models/simulator.js"

export default {
    props: ['sampler'],
    mounted: function() {
        
        const chart = new Chart(this.$el, this.sampler.id, this.sampler.type);
        chart.init(this.sampler.ports);
        chart.draw();
        
        this.revokeSubscription = simulator.subscribe(this.sampler.id, function(data) {
            chart.update(data);
            chart.draw();
        });
    },
    beforeDestroy: function() {
        this.revokeSubscription();
    },
    template: '<div class="chart-graph"></div>'
};
