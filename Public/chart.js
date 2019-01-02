
function Chart(elem, id) {
    
    const maxSamples = 100;
    const options = {
        title: id,
        legend: { position: 'top', maxLines: 1000 },
        hAxis: { title: 'Time' },
        vAxis: { title: 'Value' }
    };
    const data = new google.visualization.DataTable();
    const chart = new google.visualization.LineChart(elem);
    
    this.id = function() {
        return id;
    }
    
    this.init = function(ports) {
        data.removeRows(0, data.getNumberOfRows());
        data.removeColumns(0, data.getNumberOfColumns());
        data.addColumn('number', 'Time');
        $.each(ports, function(j, port) {
            data.addColumn('number', port.path);
        });
    }
    
    this.draw = function() {
        chart.draw(data, options);
    }
    
    this.update = function(samples) {
        data.addRow(samples);
        if (data.getNumberOfRows() > maxSamples) {
            data.removeRow(0);
        }
    }
    
};
