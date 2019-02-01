
'use strict';

import Vue from 'vue';
import VueRouter from 'vue-router';

import 'bootstrap';
import $ from 'jquery';

import 'bootstrap/dist/css/bootstrap.min.css';
import '@fortawesome/fontawesome-free/css/all.min.css';
import './css/style.css'

import routes from './views/routes.js'; 
import HeaderBar from './components/header-bar.js';
import SideBar from './components/side-bar.js'; 
import ChartArea from './components/chart-area.js';
import PortTable from './components/port-table.js';
import SimControl from './components/sim-control.js';
import SimChart from './components/sim-chart.js';
import SimXyChart from './components/sim-xy-chart.js';

import SimService from './models/sim-service.js'

$(function() {
    
    Vue.use(VueRouter);
    const router = new VueRouter({ routes: routes });
    router.replace("sim-view");
    
    const simService = new SimService();
    
    new Vue({
        el: '#vue-app',
        router: router,
        provide: function () {
            return {
                simService: function() {
                    return simService;
                }
            }
        },
        components: {
            'header-bar': HeaderBar,
            'side-bar': SideBar,
            'chart-area': ChartArea,
            'port-table': PortTable,
            'sim-control': SimControl,
            'sim-chart': SimChart,
            'sim-xy-chart': SimXyChart
        },
        template:`
            <div>
                <header-bar></header-bar>
                <div class="container-fluid">
                    <div class="row">
                        <side-bar></side-bar>
                        <router-view></router-view>
                    </div>
                </div>
            </div>
        `
    });
    
});
