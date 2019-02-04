
'use strict';

import LicenceModal from "./licence-modal.js"

export default {
    inject: [ 'simService' ],
    components: {
        "licence-modal": LicenceModal
    },
    data: function () {
        return {
            selectedConfig: "",
            configs: []
        }
    },
    mounted: function() {
        const self = this;
        self.simService().getConfigs(function(configs) {
            self.selectedConfig = configs[0];
            self.configs = configs;
        });
    },
    methods: {
        start: function() {
            this.simService().start(this.selectedConfig);
        },
        stop: function() {
            this.simService().stop();
        },
        visitGithub: function() {
            window.open('https://github.com/Eelco81/server-test-project', '_blank');
        }
    },
    template: `
        <form class="form-inline">
            <div class="form-group">
                <select id="config-select" class="form-control mr-sm-2" v-model="selectedConfig">
                    <option v-for="config in configs" v-bind:value="config">{{config}}</option>
                </select>
            </div>
            <button id="button-start" type="button" class="btn btn-outline-light mr-sm-2" v-on:click="start">Start</button>
            <button id="button-stop" type="button" class="btn btn-outline-light mr-sm-2" v-on:click="stop">Stop</button>
            <button id="button-info" type="button" class="btn btn-outline-light mr-sm-2" v-on:click="visitGithub">
                <i class="fab fa-github"></i>
            </button>
            <licence-modal></licence-modal>
        </form>`
};