
module.exports = function(config) {
    
    config.set({
        
        basePath: 'test',
        frameworks: ['mocha'],
        singleRun: true,
        browsers: [ 'ChromeHeadless' ], //  'Chrome'
        files: [
            '*-test.js',
            '**/*-test.js'
        ],

        preprocessors: {
            '*-test.js': ['webpack'],
            '**/*-test.js': ['webpack']
        },

        webpack: {
            // you don't need to specify the entry option because
            // karma watches the test entry points
            // webpack watches dependencies
            // ... remainder of webpack configuration (or import)
        },

        webpackMiddleware: {
            // webpack-dev-middleware configuration
            // i.e.
            noInfo: true,
            // and use stats to turn off verbose output
            stats: {
                // options i.e. 
                chunks: false
            }
        }
    });
    
};