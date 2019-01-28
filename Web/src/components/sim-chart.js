
'use strict';

import ChartLayout from "./../models/chart-layout.js"
import Plotly from "plotly.js/dist/plotly-basic.min.js"

const MAX_SAMPLES = 100;

export default {
    inject: [ 'getSimulator' ],
    props: [ 'sampler' ],
    mounted: function() {
        
        const element = this.$el;
        
        const data = [];
        const indices = [];
        
        $.each(this.sampler.ports, function(j,port) {
            data.push({
                x: [],
                y: [],
                type: 'scatter',
                name: port.path,
                mode: 'lines'
            });
            indices.push(j);
        });
        
        Plotly.newPlot(element, data, ChartLayout(this.sampler.id), {displayModeBar: false});
        
        this.revokeSubscription = this.getSimulator().subscribe(this.sampler.id, function(sampleData) {
           
            const time = sampleData.shift();
            
            $.each(data, function(j,item) {
                if (item.y.length > MAX_SAMPLES) {
                    item.x.shift();
                    item.y.shift();
                }
            });
                
            Plotly.extendTraces(element, {
                x: sampleData.map(function(sample){return [ time ];}),
                y: sampleData.map(function(sample){return [ sample ];})
            }, indices);
       
        });
    },
    beforeDestroy: function() {
        this.revokeSubscription();
        Plotly.purge(this.$el);
    },
    template: '<div></div>'
};
