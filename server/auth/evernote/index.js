'use strict';

var express = require('express');
var passport = require('passport');
var auth = require('../auth.service');
var http = require('http');
var https = require('https');

var router = express.Router();

router
    .get('/', passport.authenticate('evernote'))

.get('/callback', passport.authenticate('evernote', {
    failueRedirect: '/',
    session: false
}), auth.setTokenCookie);

module.exports = router;