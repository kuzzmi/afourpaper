'use strict';

angular.module('afourpaperApp')
    .controller('MainCtrl', function(
        $window, $timeout, $scope, $http, socket, $upload, Auth) {
        $scope.awesomeThings = [];

        $http.get('/api/things').success(function(awesomeThings) {
            $scope.awesomeThings = awesomeThings;
            socket.syncUpdates('thing', $scope.awesomeThings);
        });

        $scope.isLoggedIn = Auth.isLoggedIn;
        $scope.getCurrentUser = Auth.getCurrentUser;

        $scope.addThing = function() {
            if ($scope.newThing === '') {
                return;
            }
            $http.post('/api/things', {
                name: $scope.newThing
            });
            $scope.newThing = '';
        };

        $scope.logout = function() {
            Auth.logout();
            $location.path('/');
        };

        $scope.loginOauth = function(provider) {
            $window.location.href = '/auth/' + provider;
        };

        $scope.deleteThing = function(thing) {
            $http.delete('/api/things/' + thing._id);
        };

        $scope.$on('$destroy', function() {
            socket.unsyncUpdates('thing');
        });

        $scope.myModelObj = null;

        $scope.$watch('myModelObj', function(a, b) {
            console.log(a, b);
        });

        var circles = angular.element('.promo-circles .col-lg-4');
        $window.onscroll = function() {
            angular.forEach(circles, function(value, key) {
                $timeout(function() {
                    angular.element(value).css('opacity', 1);
                }, key * 500);
            });
            // console.log(circles);
            // for (var i = circles.length - 1; i >= 0; i--) {
            //     console.log(i);
            //     if (!circles[i])
            //         return;
            //     $timeout(function() {
            //         circles[i].css('opacity', 1);
            //     }, 250 * i);
            // };
            // .css('opacity', 1);
        };


        $scope.onEncodeFileSelect = function($files) {
            //$files: an array of files selected, each file has name, size, and type.
            console.log($files);
            for (var i = 0; i < $files.length; i++) {
                var file = $files[i];
                $scope.upload = $upload.upload({
                    url: 'api/upload/encode', //upload.php script, node.js route, or servlet url
                    //method: 'POST' or 'PUT',
                    //headers: {'header-key': 'header-value'},
                    //withCredentials: true,
                    data: {
                        myObj: $scope.myModelObj
                    },
                    file: file, // or list of files ($files) for html5 only
                    //fileName: 'doc.jpg' or ['1.jpg', '2.jpg', ...] // to modify the name of the file(s)
                    // customize file formData name ('Content-Disposition'), server side file variable name. 
                    //fileFormDataName: myFile, //or a list of names for multiple files (html5). Default is 'file' 
                    // customize how data is added to formData. See #40#issuecomment-28612000 for sample code
                    //formDataAppender: function(formData, key, val){}
                }).progress(function(evt) {
                    console.log('percent: ' + parseInt(100.0 * evt.loaded / evt.total));
                }).success(function(data, status, headers, config) {
                    // file is uploaded successfully
                    var element = angular.element('<a/>');
                    element.attr({
                        href: 'data:attachment/png;base64,' + encodeURIComponent(data),
                        target: '_blank',
                        download: 'encrypted.png'
                    })[0].click();
                });
                //.error(...)
                //.then(success, error, progress); 
                // access or attach event listeners to the underlying XMLHttpRequest.
                //.xhr(function(xhr){xhr.upload.addEventListener(...)})
            }
            /* alternative way of uploading, send the file binary with the file's content-type.
             Could be used to upload files to CouchDB, imgur, etc... html5 FileReader is needed. 
             It could also be used to monitor the progress of a normal http post/put request with large data*/
            // $scope.upload = $upload.http({...})  see 88#issuecomment-31366487 for sample code.
        };


        $scope.onDecodeFileSelect = function($files) {
            //$files: an array of files selected, each file has name, size, and type.
            console.log($files);
            for (var i = 0; i < $files.length; i++) {
                var file = $files[i];
                $scope.upload = $upload.upload({
                    url: 'api/upload/decode', //upload.php script, node.js route, or servlet url
                    //method: 'POST' or 'PUT',
                    //headers: {'header-key': 'header-value'},
                    //withCredentials: true,
                    data: {
                        myObj: $scope.myModelObj
                    },
                    file: file, // or list of files ($files) for html5 only
                    //fileName: 'doc.jpg' or ['1.jpg', '2.jpg', ...] // to modify the name of the file(s)
                    // customize file formData name ('Content-Disposition'), server side file variable name. 
                    //fileFormDataName: myFile, //or a list of names for multiple files (html5). Default is 'file' 
                    // customize how data is added to formData. See #40#issuecomment-28612000 for sample code
                    //formDataAppender: function(formData, key, val){}
                }).progress(function(evt) {
                    console.log('percent: ' + parseInt(100.0 * evt.loaded / evt.total));
                }).success(function(data, status, headers, config) {
                    // file is uploaded successfully
                    console.log(data);
                    var element = angular.element('<a/>');
                    element.attr({
                        href: 'data:attachment/png;base64,' + encodeURIComponent(data.data),
                        target: '_blank',
                        download: data.filename
                    })[0].click();
                });
                //.error(...)
                //.then(success, error, progress); 
                // access or attach event listeners to the underlying XMLHttpRequest.
                //.xhr(function(xhr){xhr.upload.addEventListener(...)})
            }
            /* alternative way of uploading, send the file binary with the file's content-type.
             Could be used to upload files to CouchDB, imgur, etc... html5 FileReader is needed. 
             It could also be used to monitor the progress of a normal http post/put request with large data*/
            // $scope.upload = $upload.http({...})  see 88#issuecomment-31366487 for sample code.
        };
    });