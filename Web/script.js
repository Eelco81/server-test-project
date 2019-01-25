
'use strict';

import Vue from 'vue';
import 'bootstrap';
import $ from 'jquery';

import 'bootstrap/dist/css/bootstrap.min.css';
import '@fortawesome/fontawesome-free/css/all.min.css';
import './style.css'

import ChartArea from "./components/chart-area.js";
import ChartGraph from "./components/chart-graph.js";
import HeaderBar from "./components/header-bar.js";
import PortTable from "./components/port-table.js";
import SimControl from "./components/sim-control.js";
import SimChart from "./components/sim-chart.js";

$(function() {
    
    new Vue({
        el: '#vue-app',
        components: {
            "chart-area": ChartArea,
            "chart-graph": ChartGraph,
            "header-bar": HeaderBar,
            "port-table": PortTable,
            "sim-control": SimControl,
            "sim-chart": SimChart
        },
        template:`
            <div>
                <header-bar></header-bar>
                <port-table></port-table>
                <chart-area></chart-area>
            </div>
        `
    });
    
});
