
'use strict';

Vue.component('header-bar', {
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
        <header style="height:30px; background-color:navy; color:white;">
            <div>{{ name }}</div> 
        </header>`
});
