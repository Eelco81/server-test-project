
'use strict';

import EventEmitter from './../../src/models/event-emitter.js'

import sinon from 'sinon'

export default class SimService extends EventEmitter {
    
    constructor() {
        
        super();
        
        this.start = sinon.stub();
        this.stop = sinon.stub();
        this.getSamplers = sinon.stub();
        this.getConfigs = sinon.stub();
        this.getPorts = sinon.stub();
        this.setPort = sinon.stub();
        
    }
    
};

