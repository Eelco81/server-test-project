
'use strict';

import Vue from 'vue';
import 'bootstrap';
import $ from 'jquery';

import 'bootstrap/dist/css/bootstrap.min.css';
import '@fortawesome/fontawesome-free/css/all.min.css';
import './css/style.css'

import ChartArea from "./components/chart-area.js";
import HeaderBar from "./components/header-bar.js";
import PortTable from "./components/port-table.js";
import SimControl from "./components/sim-control.js";
import SimChart from "./components/sim-chart.js";
import SimXyChart from "./components/sim-xy-chart.js";

import Simulator from "./models/simulator.js"

$(function() {
    
    
    const simulator = new Simulator();

    new Vue({
        el: '#vue-app',
        provide:  function () {
            return {
                getSimulator: this.getSimulator
            }
        },
        components: {
            "chart-area": ChartArea,
            "header-bar": HeaderBar,
            "port-table": PortTable,
            "sim-control": SimControl,
            "sim-chart": SimChart,
            "sim-xy-chart": SimXyChart
        },
        methods: {
            getSimulator: function() {
                return simulator;
            }
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
