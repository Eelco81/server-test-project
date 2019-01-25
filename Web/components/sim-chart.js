
'use strict';

import simulator from "./../models/simulator.js"
import Plotly from "plotly.js/dist/plotly-basic.min.js"

const MAX_SAMPLES = 100;

export default {
    props: ['sampler'],
    mounted: function() {
        
        const self = this;
        
        const data = [];
        const indices = [];
        
        if (this.sampler.type == 0) {
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
        }
        else {
            data.push({
                y: [],
                type: 'scatter',
                name: 'Data',
                mode: 'lines'
            });
            indices.push(0);
        }
        
        const layout = {
            title: this.sampler.id,
            xaxis: {
                title: 'Time',
                showgrid: true
            },
            yaxis: {
                title: 'Value',
                showline: true
            },
            dragmode: false,
            hovermode: false
        };
        
        Plotly.newPlot(this.$el, data, layout, {displayModeBar: false});
        
        this.revokeSubscription = simulator.subscribe(this.sampler.id, function(sampleData) {
           
           const time = sampleData.shift();
           
           if (self.sampler.type == 0) {
               
                $.each(data, function(j,item) {
                    if (item.y.length > MAX_SAMPLES) {
                        item.x.shift();
                        item.y.shift();
                    }
                });
                
                Plotly.extendTraces(self.$el, {
                    x: sampleData.map(function(sample){return [ time ];}),
                    y: sampleData.map(function(sample){return [ sample ];})
               }, indices);
           }
           else {
               Plotly.restyle(self.$el, {
                    y: [ sampleData ]
               }, indices);
           }
        });
    },
    beforeDestroy: function() {
        this.revokeSubscription();
        Plotly.purge(this.$el);
    },
    template: '<div></div>'
};
