
'use strict';

import Vue from 'vue';
import chai from 'chai';

// Unit under test
import ChartArea from "./../../src/components/chart-area.js"
import Simulator from "./../mocks/simulator-mock.js"

describe("chart-area", function(){
    
    let simulator, component;
    
    beforeEach(function() {
        simulator = new Simulator();
        const Component = Vue.extend(ChartArea);
        Component.prototype.getSimulator = function () { return simulator; }
        component = new Component().$mount();
    });
    
    it("should initialize the component", function(){
        chai.expect(component.samplers).to.deep.equal([]);
    });
    
    it("should listen to simulator start and stop events", function(){
        simulator.getSamplers.yields([ 'sampler1', 'sampler2' ]);
        simulator.emit("sim-started");
        chai.expect(component.samplers).to.deep.equal([ 'sampler1', 'sampler2' ]);
        simulator.emit("sim-stopped");
        chai.expect(component.samplers).to.deep.equal([]);
    });
    
    it("should destroy the component", function(){
        component.$destroy();
        simulator.getSamplers.yields([ 'sampler1', 'sampler2' ]);
        simulator.emit("sim-started");
        chai.expect(component.samplers).to.deep.equal([]);
    });
    
});
