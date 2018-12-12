
$(document).ready(function(){
    
    const options = {
        title: 'Simulation stream',
        hAxis: {
            title: 'Time'
        },
        vAxis: {
            title: 'Value'
        },
        backgroundColor: '#f1f8e9'
    };
    let chartData;
    let chart;
    const ws = new WebSocket("ws://localhost:1704");
    //var ws = new WebSocket("ws://192.168.1.100:1704");
    
    ws.onmessage = function (evt) { 
        const data = JSON.parse (evt.data);
        if (data["event-id"] === "sim-started") {
            chartData.removeRows(0, chartData.getNumberOfRows());
            chart.draw(chartData, options);
        }
        else if (data["event-id"] === "sim-sampler") {
            chartData.addRow(data["event-data"]);
            if (chartData.getNumberOfRows() > 200) {
                chartData.removeRow(0);
            }
            chart.draw(chartData, options);
        }
    }
    
    const updateVersion = function() {
        $.get("api/version", function(data, status){
            $("#version_text").text(data.application + " " + data.version);
        });
    }
    
    const onGoogleChartLoaded = function (){
        chartData = new google.visualization.DataTable();
        chartData.addColumn('number', 'Time');
        chartData.addColumn('number', 'Force');
        chartData.addColumn('number', 'Position');
        chartData.addColumn('number', 'Velocity');
        chartData.addColumn('number', 'Acceleration');
        chartData.addRows([]);
        chart = new google.visualization.LineChart(document.getElementById('chart_div'));
        chart.draw(chartData, options);
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
