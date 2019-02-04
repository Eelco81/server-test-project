
'use strict';

import Vue from 'vue';
import VueRouter from 'vue-router';

import routes from './views/routes.js'; 
import HeaderBar from './components/header-bar.js';
import SideBar from './components/side-bar.js'; 
import SimService from './models/sim-service.js'

Vue.use(VueRouter);

export default function (div) {
    
    const router = new VueRouter({ routes: routes, base: "/home/" });
    router.replace("sim-view"); // default route
    
    const simService = new SimService();
    
    return new Vue({
        
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
            'side-bar': SideBar
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
        
    }).$mount(div);
}