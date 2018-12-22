
$(document).ready(function(){
    
    const options = {
        title: 'Simulation stream',
        legend: 'top',
        hAxis: { title: 'Time' },
        vAxis: { title: 'Value' }
    };
    let simLoaded = false;
    
    let charts = [ {}, {} ];
    
    const ws = new WebSocket("ws://localhost:1704");
    //var ws = new WebSocket("ws://192.168.1.100:1704");
    
    const configure = function() {
        
        charts[0].data.removeRows(0, charts[0].data.getNumberOfRows());
        charts[0].data.removeColumns(0, charts[0].data.getNumberOfColumns());
        
        charts[1].data.removeRows(0, charts[1].data.getNumberOfRows());
        charts[1].data.removeColumns(0, charts[1].data.getNumberOfColumns());
        
        $.get("api/simulation/samplers", function(data, status){
            
            charts[0].data.addColumn('number', 'Time');
            $.each(data.samplers[0].ports, function(i, port) {
                charts[0].data.addColumn('number', port.path);
            });
            
            charts[1].data.addColumn('number', 'Time');
            $.each(data.samplers[1].ports, function(i, port) {
                charts[1].data.addColumn('number', port.path);
            });
            
            simLoaded = true;
        });
        
    }
    
    ws.onmessage = function(evt) { 
    
        const data = JSON.parse(evt.data);
        
        if (data["event-id"] === "sim-started") {
            configure ();
        }
        else if (data["event-id"] === "sim-stopped") {
            simLoaded = false;
        }
        else if (data["event-id"] === "sim-sampler-0") {
            if (!simLoaded) return;
            charts[0].data.addRow(data["event-data"]);
            if (charts[0].data.getNumberOfRows() > 30) {
                charts[0].data.removeRow(0);
            }
            charts[0].chart.draw(charts[0].data, options);
        }
        else if (data["event-id"] === "sim-sampler-1") {
            if (!simLoaded) return;
            charts[1].data.addRow(data["event-data"]);
            if (charts[1].data.getNumberOfRows() > 30) {
                charts[1].data.removeRow(0);
            }
            charts[1].chart.draw(charts[1].data, options);
        }
    }
    
    const updateVersion = function() {
        $.get("api/version", function(data, status){
            $("#version_text").text(data.application + " " + data.version);
        });
    }
    
    const updateConfig = function() {
        $.get("api/simulation/config", function(data, status){
            $.each(data, function (i, item) {
                $('#config_select').append($('<option>', { 
                    value: item,
                    text : item 
                }));
            });
        });
    }
    
    const onGoogleChartLoaded = function (){
        
        charts[0].data = new google.visualization.DataTable();
        charts[0].chart = new google.visualization.LineChart(document.getElementById('chart_div_0'));
        charts[0].chart.draw(charts[0].data, options);
        
        charts[1].data = new google.visualization.DataTable();
        charts[1].chart = new google.visualization.LineChart(document.getElementById('chart_div_1'));
        charts[1].chart.draw(charts[1].data, options);
        
    }
    
    $('#button_start').click(function() {
        const id = $('#config_select').find(":selected").text();
        $.ajax({url:'api/simulation', type:'PUT', data: JSON.stringify({ id: id }) } );
    });
    
    $('#button_stop').click(function() {
        $.ajax({url:'api/simulation', type:'DELETE'});
    });
    
    updateVersion();
    updateConfig();
    
    google.charts.load('current', {packages: ['corechart', 'line']});   
    google.charts.setOnLoadCallback(onGoogleChartLoaded);
    
});
