
$(document).ready(function(){
    
    const options = {
        title: 'Simulation stream',
        hAxis: { title: 'Time' },
        vAxis: { title: 'Value' }
    };
    
    let charts = [ {}, {} ];
    
    const ws = new WebSocket("ws://localhost:1704");
    //var ws = new WebSocket("ws://192.168.1.100:1704");
    
    ws.onmessage = function(evt) { 
    
        const data = JSON.parse(evt.data);
        
        if (data["event-id"] === "sim-started") {
            charts[0].data.removeRows(0, charts[0].data.getNumberOfRows());
            charts[0].chart.draw(charts[0].data, options);
            charts[1].data.removeRows(0, charts[1].data.getNumberOfRows());
            charts[1].chart.draw(charts[1].data, options);
        }
        else if (data["event-id"] === "sim-sampler-0") {
            charts[0].data.addRow(data["event-data"]);
            if (charts[0].data.getNumberOfRows() > 200) {
                charts[0].data.removeRow(0);
            }
            charts[0].chart.draw(charts[0].data, options);
        }
        else if (data["event-id"] === "sim-sampler-1") {
            charts[1].data.addRow(data["event-data"]);
            if (charts[1].data.getNumberOfRows() > 200) {
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
    
    const onGoogleChartLoaded = function (){
        
        charts[0].data = new google.visualization.DataTable();
        charts[0].chart = new google.visualization.LineChart(document.getElementById('chart_div_0'));
        charts[0].chart.draw(charts[0].data, options);
        
        charts[1].data = new google.visualization.DataTable();
        charts[1].chart = new google.visualization.LineChart(document.getElementById('chart_div_1'));
        charts[1].chart.draw(charts[1].data, options);
        
        charts[0].data.addColumn('number', 'Time');
        charts[0].data.addColumn('number', 'Position');
        charts[0].data.addColumn('number', 'Velocity');
        charts[0].data.addColumn('number', 'Acceleration');
        
        charts[1].data.addColumn('number', 'Time');
        charts[1].data.addColumn('number', 'Force');
        charts[1].data.addColumn('number', 'Control');
    }
    
    $('#button_start').click(function() {
        $.ajax({url:'api/simulation', type:'PUT', data: JSON.stringify({ id: "basic"}) } );
    });
    
    $('#button_stop').click(function() {
        $.ajax({url:'api/simulation', type:'DELETE'});
    });
    
    updateVersion();
    
    google.charts.load('current', {packages: ['corechart', 'line']});   
    google.charts.setOnLoadCallback(onGoogleChartLoaded);
    
});
