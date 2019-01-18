
'use strict';

import {GoogleCharts} from 'google-charts'

class Chart {
    
    constructor(elem, id, type) {
        
        this.maxSamples = 100;
        this.id = id;
        this.type = type;
        this._options = {
            title: id,
            legend: { position: 'top', maxLines: 1000 },
            hAxis: { title: 'Time' },
            vAxis: { title: 'Value' }
        };
        this._data = new GoogleCharts.api.visualization.DataTable();
        this._chart = new GoogleCharts.api.visualization.LineChart(elem);
    }
    
    init(ports) {
        this._data.removeRows(0, this._data.getNumberOfRows());
        this._data.removeColumns(0, this._data.getNumberOfColumns());
        if (this.type === 0) {
            this._data.addColumn('number', 'Time');
            const self = this;
            $.each(ports, function(j, port) {
                self._data.addColumn('number', port.path);
            });
        }
        else {
            this._data.addColumn('number', 'Index');
            this._data.addColumn('number', 'Value');
        }
    }
    
    update(samples) {
        if (this.type === 0) {
            this._data.addRow(samples);
            if (this._data.getNumberOfRows() > this.maxSamples) {
                this._data.removeRow(0);
            }
        }
        else {
            this._data.removeRows(0, this._data.getNumberOfRows());
            const self = this;
            $.each(samples, function(i, sample) {
                if (i > 0) {
                    self._data.addRow([i,sample]);
                }
            });
        }
    }
    
    draw() {
        this._chart.draw(this._data, this._options);
    }
    
};

export { Chart as default }
