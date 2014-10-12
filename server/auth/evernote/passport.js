exports.setup = function(User, config) {
    var passport = require('passport');
    var EvernoteStrategy = require('passport-evernote').Strategy;

    passport.serializeUser(function(user, done) {
        done(null, user);
    });

    passport.deserializeUser(function(user, done) {
        done(null, user);
    });

    passport.use(
        new EvernoteStrategy({
                requestTokenURL: 'https://sandbox.evernote.com/oauth',
                accessTokenURL: 'https://sandbox.evernote.com/oauth',
                userAuthorizationURL: 'https://sandbox.evernote.com/OAuth.action',
                consumerKey: config.evernote.clientID,
                consumerSecret: config.evernote.clientSecret,
                callbackURL: config.evernote.callbackURL
            },
            function(token, tokenSecret, profile, done) {

                // console.log(done);
                // done(null, profile);
                User.findOne({
                        'evernote.id': profile.id
                    },
                    function(err, user) {
                        if (err) {
                            return done(err);
                        }
                        if (!user) {
                            user = new User({
                                name: profile.displayName || 'Igor Kuzmenko',
                                email: 'kuzzmi@gmail.com',
                                role: 'user',
                                username: profile.username || 'kuzzmi',
                                provider: 'evernote'
                                // evernote: profile._json
                            });
                            console.log(user);
                            user.save(function(err) {
                                if (err) done(err);
                                return done(err, user);
                            });
                        } else {
                            return done(err, user);
                        }
                    })
            }));

};