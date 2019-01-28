
'use strict';

import SimControl from "./sim-control.js"

export default {
    components: {
        "sim-control": SimControl
    },
    data: function() {
        return {
            version: "",
            app: ""
        }
    },
    mounted: function () {
        const self = this;
        $.get("api/version", function(data){
            self.version = data.version;
            self.app = data.application;
        });
    },
    computed: {
        name: function() {
            return this.app + " " + this.version;
        }
    },
    template: `
        <header>
            <nav class="navbar navbar-expand-lg navbar-dark bg-dark justify-content-between">
                <a class="navbar-brand" href="#"> <i class="fas fa-angle-double-right"></i> {{name}} </a>
                <sim-control></sim-control>
            </nav>
        </header>
        `
};
