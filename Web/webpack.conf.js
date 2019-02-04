
const path = require('path');
const webpack = require('webpack')

module.exports = {
    
    entry: './src/script.js',
    
    devtool: "source-map",
    
    output: {
        filename: 'main.js',
        path: path.resolve(__dirname, "dist")
    },
    
    plugins: [
        new webpack.ProvidePlugin({
            $: 'jquery',
            jQuery: 'jquery'
        })
    ],
    
    resolve: {
        alias: {
            'vue': 'vue/dist/vue.min.js',
            'vue-router': 'vue-router/dist/vue-router.min.js'
        }
    },
    
    module: {
        rules: [
            {
                test: /\.css$/,
                use: ['style-loader', 'css-loader']
            },
            { 
                test: /\.woff(2)?(\?v=[0-9]\.[0-9]\.[0-9])?$/, 
                loader: "url-loader" 
            },
            { 
                test: /\.(ttf|eot|svg)(\?v=[0-9]\.[0-9]\.[0-9])?$/, 
                loader: "url-loader" 
            }
        ]
    }
    
};
