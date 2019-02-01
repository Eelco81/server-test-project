
'use strict';

import chai from 'chai';
import sinon from 'sinon';
import { mount } from '@vue/test-utils'

// Unit under test
import ChartArea from './../../src/components/chart-area.js';
import Simulator from './../mocks/simulator-mock.js';

describe('chart-area', function(){
    
    let simService, component;
    
    beforeEach(function() {
        simService = new Simulator();
        component = mount(ChartArea, { mocks: { simService: sinon.stub().returns(simService) } });
    });
    
    it('should initialize the component', function(){
        chai.expect(component.vm.samplers).to.deep.equal([]);
    });
    
    it('should listen to simulator start and stop events', function(){
        simService.getSamplers.yields([ 'sampler1', 'sampler2' ]);
        simService.emit('sim-started');
        chai.expect(component.vm.samplers).to.deep.equal([ 'sampler1', 'sampler2' ]);
        simService.emit('sim-stopped');
        chai.expect(component.vm.samplers).to.deep.equal([]);
    });
    
    it('should destroy the component', function(){
        component.vm.$destroy();
        simService.getSamplers.yields([ 'sampler1', 'sampler2' ]);
        simService.emit('sim-started');
        chai.expect(component.vm.samplers).to.deep.equal([]);
    });
    
});
