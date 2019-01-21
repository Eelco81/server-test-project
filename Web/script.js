
'use strict';

import Vue from 'vue'
import {GoogleCharts} from 'google-charts'

import $ from 'jquery'
import ChartArea from "./components/chart-area.js"
import ChartGraph from "./components/chart-graph.js"
import HeaderBar from "./components/header-bar.js"
import PortTable from "./components/port-table.js"
import SimControl from "./components/sim-control.js"

$(function() {
    GoogleCharts.load(function(){
        new Vue({
            el: '#vue-app',
            components: {
                "chart-area": ChartArea,
                "chart-graph": ChartGraph,
                "header-bar": HeaderBar,
                "port-table": PortTable,
                "sim-control": SimControl
            }
        });
    });
});
