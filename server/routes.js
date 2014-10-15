/**
 * Main application routes
 */

'use strict';

var errors = require('./components/errors');
var fs = require('fs');
var Evernote = require('evernote').Evernote;
module.exports = function(app) {

    // Insert routes below
    app.use('/api/things', require('./api/thing'));
    app.use('/api/users', require('./api/user'));

    var multer = require('multer');
    app.use(multer({
        dest: './uploads/'
    }));

    app.post('/api/upload/encode', function(req, res, next) {
        // if (req.cookies.token)
        // console.log(req.cookies.token);
        // return;
        console.log(req.headers, req.files, __dirname);
        var output = require("randomstring").generate(12);
        var python = require('child_process').spawn(
            __dirname + '/imagetools/encoder/bin/encode', ['./' + req.files.file.path, req.files.file.originalname, './uploads/' + output + '.png']
        ).stdout.on('data', function(data) {
            var absPath = '/home/kuzzmi/Projects/afourpaper/uploads/' + output + '.png';


            var base64data;

            fs.readFile(absPath, function(err, data) {
                base64data = new Buffer(data).toString('base64');

                // res.set({
                //     'Content-Transfer-Encoding': 'base64'
                // });

                res.send(base64data);
                // console.log(base64data);

                // setTimeout(function() {
                //     res.send(200);
                // }, 1000);

                res.on('finish', function() {
                    console.log('finished');
                    // res.end();
                    // fs.unlinkSync(absPath);
                    // fs.unlinkSync('./' + req.files.file.path);
                });
            });
        });
    });

    app.post('/api/upload/decode', function(req, res, next) {
        console.log(req.headers, req.files, __dirname);
        var python = require('child_process').spawn(
            __dirname + '/imagetools/encoder/bin/decode', ['./' + req.files.file.path, './uploads']
            // 'python', [__dirname + '/imagetools/imageDecode.py', './' + req.files.file.path, './uploads']
        ).stdout.on('data', function(data) {
            var absPath = data.toString().trim();
            var filename = absPath.split('/').pop();
            console.log(filename);

            var base64data;

            fs.readFile(absPath, function(err, data) {
                base64data = new Buffer(data).toString('base64');

                // res.set({
                //     'Content-Transfer-Encoding': 'base64'
                // });

                res.json({
                    data: base64data,
                    filename: filename
                });

                // res.on('finish', function() {
                //     res.end();
                // fs.unlinkSync(absPath);
                // fs.unlinkSync('./' + req.files.file.path);
                // });
            });
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