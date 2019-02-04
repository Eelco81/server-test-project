
module.exports = function(config) {
    
    config.set({
        
        basePath: 'test',
        frameworks: [ 'mocha' ],
        singleRun: true,
        browsers: [ 'ChromeHeadless' ], //  'Chrome'
        files: [
            '*-test.js',
            '**/*-test.js'
        ],
        
        preprocessors: {
            '*-test.js': [ 'webpack' ],
            '**/*-test.js': [ 'webpack' ]
        },
        
        webpack: {
        },
        
        webpackMiddleware: {
            noInfo: true,
            stats: {
                chunks: false
            }
        }
        
    });
    
};