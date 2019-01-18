
'use strict';

export default {
    props: ['sim'],
    data: function () {
        return {
            selectedConfig: "",
            configs: []
        }
    },
    mounted: function() {
        const self = this;
        this.sim.getConfigs(function(configs) {
            self.selectedConfig = configs[0];
            self.configs = configs;
        });
    },
    methods: {
        start: function() {
            this.sim.start(this.selectedConfig);
        },
        stop: function() {
            this.sim.stop();
        }
    },
    template: `
        <form class="sim-control">
            <div class="form-group">
                <select id="config-select" class="form-control" v-model="selectedConfig">
                    <option v-for="config in configs" v-bind:value="config">{{config}}</option>
                </select>
            </div>
            <button id="button-start" type="button" class="btn btn-secondary" v-on:click="start">Start</button>
            <button id="button-stop" type="button" class="btn btn-secondary" v-on:click="stop">Stop</button>
        </form>`
};