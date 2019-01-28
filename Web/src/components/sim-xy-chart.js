
import simulator from "./../models/simulator.js"
import ChartLayout from "./../models/chart-layout.js"
import Plotly from "plotly.js/dist/plotly-basic.min.js"

const MAX_SAMPLES = 100;

export default {
    props: ['sampler'],
    mounted: function() {
        
        const element = this.$el;
        
        const data = [{
            y: [],
            type: 'scatter',
            name: 'Data',
            mode: 'lines'
        }];
        
        Plotly.newPlot(element, data, ChartLayout(this.sampler.id), {displayModeBar: false});
        
        this.revokeSubscription = simulator.subscribe(this.sampler.id, function(sampleData) {
          
           const time = sampleData.shift();
           Plotly.restyle(element, { y: [ sampleData ] }, [0]);
           
        });
    },
    beforeDestroy: function() {
        this.revokeSubscription();
        Plotly.purge(this.$el);
    },
    template: '<div></div>'
};
