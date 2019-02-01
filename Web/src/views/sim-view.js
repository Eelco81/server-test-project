
'use strict';

import ChartArea from './../components/chart-area.js';
import PortTable from './../components/port-table.js';

export default {
    components: {
        'port-table': PortTable,
        'chart-area': ChartArea
    },
    template: `
        <div class="col-md-9 ml-sm-auto col-lg-10 px-4">
            <port-table></port-table>
            <chart-area></chart-area>
        </div>
    `
}
