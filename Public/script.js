
$(document).ready(function(){
    
    var options = {
        title: 'Simulation stream',
        hAxis: {
            title: 'Time'
        },
        vAxis: {
            title: 'Value'
        },
        backgroundColor: '#f1f8e9'
    };
    var chartData;
    var chart;
    var ws = new WebSocket("ws://localhost:1704");
    //var ws = new WebSocket("ws://192.168.1.100:1704");
    
    ws.onmessage = function (evt) { 
        const data = JSON.parse (evt.data)
        chartData.addRow(data.values);
        if (chartData.getNumberOfRows() > 100) {
            chartData.removeRow(0);
        }
        chart.draw(chartData, options);
    }
    
    var updateVersion = function() {
        $.get("api/version", function(data, status){
            $("#version_text").text(data.application + " " + data.version);
        });
    }
    
    var onGoogleChartLoaded = function (){
        chartData = new google.visualization.DataTable();
        chartData.addColumn('number', 'Time');
        chartData.addColumn('number', 'Signal');
        chartData.addColumn('number', 'Signal2');
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
