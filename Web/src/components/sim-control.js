
'use strict';

export default {
    inject: [ 'simService' ],
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
            <button id="button-info" type="button" class="btn btn-outline-light mr-sm-2" >
                <i class="fas fa-info"></i>
            </button>
        </form>`
};