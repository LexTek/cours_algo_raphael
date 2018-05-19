let ball = {'x': 150, 'y': 150, 'w': 10, 'h': 10};
let rq = {};
rq['1'] = {'x': 25, 'y': 150, 'w': 10, 'h': 40};
rq['2'] = {'x': 275, 'y': 150, 'w': 10, 'h': 40};
let sc = {'1': 0, '2': 0};
var socket = io();

console.log('join_game', index);
socket.emit('join_game', index);

socket.on('pos_changed', function(pos) {
    //console.log('received pos :', pos);
    ball['x'] = pos.x;
    ball['y'] = pos.y;
});

socket.on('pos_changed_rq', function(pos) {
    //console.log('received rq pos :', pos);
    rq[pos.id]['y'] = pos.y;
});

socket.on('score_changed', function(scc) {
    console.log('received score_changed :', scc);
    sc['1'] = scc['sc1'];
    sc['2'] = scc['sc2'];
});

socket.on('kick', function(msg) {
    alert(msg);
    window.location.href = '/game';
});

document.addEventListener('keydown', function(event) {
    let mvy = 0;
    if(event.keyCode == 38) {
        mvy = -10
        console.log('up')
    }
    else if(event.keyCode == 40) {
        mvy = 10
        console.log('down')
    }
    socket.emit('move', {'y' : mvy});
});

var game_canvas = document.getElementById("game")
var width = game_canvas.getAttribute('width');
var height = game_canvas.getAttribute('height')

let refresh = () => {
    var ctx = game_canvas.getContext("2d")
    ctx.textBaseline = 'middle';
    ctx.textAlign = "center";

    ctx.fillStyle = "#FFFFFF"
    ctx.fillRect(0,0,width,height)

    ctx.strokeStyle="red"
    ctx.rect(0,0,width,height)
    ctx.stroke();

    ctx.fillStyle = "#FF0000";
    ctx.fillRect(ball['x'] - ball['w']/2,ball['y'] - ball['h']/2,ball['w'],ball['h']);

    ctx.fillStyle = "#00FF00";
    ctx.fillRect(rq['1']['x'] - rq['1']['w']/2,rq['1']['y'] - rq['1']['h']/2,rq['1']['w'],rq['1']['h']);

    ctx.fillStyle = "#0000FF";
    ctx.fillRect(rq['2']['x'] - rq['2']['w']/2,rq['2']['y'] - rq['2']['h']/2,rq['2']['w'],rq['2']['h']);

    ctx.fillText(sc['1'] + ' : ' + sc['2'],150,20); 

    window.requestAnimationFrame(refresh)
}

window.requestAnimationFrame(refresh)
