
'use strict';

import _ from 'lodash';

export default {
    
    data: function () {
        return {
            items: [
                {
                    name: "nlohmann/json",
                    url: "https://github.com/nlohmann/json",
                    licence: "https://github.com/nlohmann/json/blob/develop/LICENSE.MIT"
                },
                {
                    name: "sha1",
                    url: "https://github.com/983/SHA1",
                    licence: "https://github.com/983/SHA1/blob/master/LICENSE"
                },
                {
                    name: "vue.js",
                    url: "https://vuejs.org/",
                    licence: "https://github.com/vuejs/vue/blob/dev/LICENSE"
                },
                {
                    name: "bootstrap",
                    url: "https://getbootstrap.com/",
                    licence: "https://github.com/twbs/bootstrap/blob/v4-dev/LICENSE"
                },
                { 
                    name: "font awesome",
                    url: "https://fontawesome.com/",
                    licence: "https://fontawesome.com/license/free"
                },
                {
                    name: "plotly.js",
                    url: "https://plot.ly/javascript/",
                    licence: "https://github.com/plotly/plotly.js/blob/master/LICENSE"
                }
            ]
        }
    },
    
    methods: {
        goTo: function(url) {
            window.open(url, '_blank');
        },
        getClass: function(url) {
            const classObj = {
                "fab fa-vuejs": url.includes("vuejs"),
                "fab fa-github": url.includes("github"),
                "fab fa-font-awesome": url.includes("fontawesome")
            }
            classObj["fas fa-link"] = !_.includes(_.values(classObj), true);
            
            return classObj; 
        }
    },
    
    template: `
        <div>

            <button id="button-github" type="button" class="btn btn-outline-light" data-toggle="modal" data-target="#licence-modal">
                <i class="fas fa-info"></i>
            </button>
            
            <div class="modal fade" id="licence-modal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLongTitle" aria-hidden="true">
                <div class="modal-dialog" role="document">
                    <div class="modal-content">
                    
                        <div class="modal-header">
                            <h5 class="modal-title" id="exampleModalLongTitle">3rd party components</h5>
                        </div>
                        
                        <div class="modal-body">
                            
                            <table class="table table-striped table-sm">
                                <tbody>
                                
                                    <tr v-for="item in items" v-bind:key="item.name" class="justify-content-between">
                                        <td> {{ item.name }} </td>
                                        <td> 
                                            <button id="button-info" type="button" class="btn btn-secondary btn-sm" v-on:click="goTo(item.url)">
                                                <i v-bind:class="getClass(item.url)"></i>
                                            </button>
                                        </td>
                                        <td>
                                            <button id="button-info" type="button" class="btn btn-secondary btn-sm" v-on:click="goTo(item.licence)">
                                                <i class="fas fa-file-contract"></i>
                                            </button>
                                        </td>
                                    </tr>
                                    
                                </tbody>
                            </table>
                        
                        </div>
                        
                        <div class="modal-footer">
                            <button type="button" class="btn btn-secondary" data-dismiss="modal">Close</button>
                        </div>
                        
                    </div>
                </div>
            </div>
        </div>`
};