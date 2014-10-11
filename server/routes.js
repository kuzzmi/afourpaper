/**
 * Main application routes
 */

'use strict';

var errors = require('./components/errors');
var fs = require('fs');
module.exports = function(app) {

    // Insert routes below
    app.use('/api/things', require('./api/thing'));
    app.use('/api/users', require('./api/user'));

    var multer = require('multer');
    app.use(multer({
        dest: './uploads/'
    }));

    app.post('/api/upload', function(req, res, next) {
        console.log(req.headers, req.files, __dirname);
        var python = require('child_process').spawn(
            'python', [__dirname + '/imagetools/imageEncode.py', './' + req.files.file.path, req.files.file.originalname, './uploads']
        ).stdout.on('data', function(data) {
            var absPath = data.toString().trim();
            // res.download(absPath);
            res.download('./' + req.files.file.path);
        });
    });

    app.use('/auth', require('./auth'));

    // All undefined asset or api routes should return a 404
    app.route('/:url(api|auth|components|app|bower_components|assets)/*')
        .get(errors[404]);

    // All other routes should redirect to the index.html
    app.route('/*')
        .get(function(req, res) {
            res.sendfile(app.get('appPath') + '/index.html');
        });
};