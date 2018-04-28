let x = 0;
let y = 0;
var socket = io();

socket.on('pos_changed', function(pos) {
    x = pos.x;
    y = pos.y;
})

document.addEventListener('keydown', function(event) {
    let mvx = 0;
    let mvy = 0;
    if(event.keyCode == 37) {
        mvx = -10
        console.log('left', x)
    }
    else if(event.keyCode == 39) {
        mvx = 10
        console.log('right', x)
    }
    if(event.keyCode == 38) {
        mvy = -10
        console.log('up', x)
    }
    else if(event.keyCode == 40) {
        mvy = 10
        console.log('down', x)
    }
    socket.emit('move', {'x' : mvx, 'y' : mvy});
})
var game_canvas = document.getElementById("game")
var width = game_canvas.getAttribute('width');
var height = game_canvas.getAttribute('height')

let refresh = () => {
    var ctx = game_canvas.getContext("2d")
    ctx.fillStyle = "#FFFFFF"
    ctx.fillRect(0,0,width,height)

    ctx.strokeStyle="red"
    ctx.rect(0,0,width,height)
    ctx.stroke()

    ctx.fillStyle = "#FF0000";
    ctx.fillRect(x,y,10,10);
    window.requestAnimationFrame(refresh)
}

window.requestAnimationFrame(refresh)