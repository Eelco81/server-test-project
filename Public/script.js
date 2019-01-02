
$(document).ready(function(){

    google.charts.load('current', {packages: ['corechart', 'line']});
    google.charts.setOnLoadCallback(function(){
        
        let charts = [];
        
        const simulator = new Simulator();
        
        simulator.onStart = function() {
            
            /* Clear current chart selection */
            charts = [];
            $('#chart-area').empty();
            
            /* Create a chart for each sampler */
            $.each(simulator.samplers, function (i, sampler) {
                
                $('#chart-area').append($('<div>', { id: sampler.id, style: "width: 500px; height: 300px; float:left" }));
                
                chart = new Chart($('#' + sampler.id)[0], sampler.id);
                chart.init(sampler.ports);
                chart.draw();
                
                charts.push(chart);
            });
        }
        
        simulator.onSample = function(id, data) {
            const chart = $.grep(charts, function(chart) {
                return chart.id() === id;
            })[0];
            chart.update(data);
            chart.draw();
        }
        
        $('#button-start').click(function() {
            const id = $('#config-select').find(":selected").text();
            simulator.start(id);
        });
    
        $('#button-stop').click(function() {
            simulator.stop();
        });
        
        $.get("api/version", function(data, status){
            $("#version-text").text(data.application + " " + data.version);
        });
        
        simulator.getConfigs(function(configs) {
            $('#config-select').empty();
            $.each(configs, function (i, item) {
                $('#config-select').append($('<option>', { value: item,  text: item }));
            });
        });
        
    });
});
