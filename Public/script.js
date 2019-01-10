
'use strict';

$(function() {

    google.charts.load('current', {packages: ['corechart', 'line']});

    google.charts.setOnLoadCallback(function(){
        
        new Vue({
            el: '#vue-app',
            data: {
                sim: new Simulator()
            } 
        });
        
    });

});
