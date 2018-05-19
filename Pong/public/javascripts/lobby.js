//var getJSON = function(url, callback) {
//    var xhr = new XMLHttpRequest();
//    xhr.open('GET', url, true);
//    xhr.responseType = 'json';
//    xhr.onload = function() {
//        var status = xhr.status;
//        if (status === 200) {
//            callback(null, xhr.response);
//        } else {
//            callback(status, xhr.response);
//        }
//    };
//    xhr.send();
//};
//
//getJSON('127.0.0.1:3000/game',
//    function(err, data) {
//        if (err !== null) {
//            alert('Something went wrong: ' + err);
//        } else {
//            alert('You games count: ' + data.games);
//        }
//    });
