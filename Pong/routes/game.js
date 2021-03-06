var express = require('express');
var router = express.Router();

let ball_collides_rq = (ball, rq) => {
    return ball['x'] < rq['x'] + rq['w'] && rq['x'] < ball['x'] + ball['w'] &&
           ball['y'] < rq['y'] + rq['h'] && rq['y'] < ball['y'] + ball['h'];
};

module.exports = function(io, games) {

    var nsp_l = io.of('/lobby');
    nsp_l.on('connection', function(socket) {
        console.log("connected lobby");
    });

    var nsp_g = io.of('/game');
    nsp_g.on('connection', function(socket) {
        console.log("connected game");

        let socket_id = socket.id;
        let index;
        let ball;
        let rq;
        let st;
        let id;

        socket.on('join_game', function(index_g) {
            console.log("join_game", index_g, games);
            if(games[index_g] != undefined) {
                console.log("index OK");
                if(games[index_g]['player1'] == undefined) {
                    console.log("add player1", socket.id);
                    index = index_g;
                    games[index]['player1'] = socket.id;
                    ball = games[index]['ball'];
                    rq = games[index]['rq1'];
                    st = games[index]['state1'];
                    id = '1';
                    socket.join(index);
                    console.log('joined :', index);
                } else if(games[index_g]['player2'] == undefined) {
                    console.log("add player2", socket.id);
                    index = index_g;
                    games[index]['player2'] = socket.id;
                    ball = games[index]['ball'];
                    rq = games[index]['rq2'];
                    st = games[index]['state2'];
                    id = '2';
                    socket.join(index);
                    console.log('joined :', index);
                    games[index]['started'] = true;

                    //send update game list
                    let act_games = [];
                    for(let v of Object.keys(games)) {
                        if(!games[v].started)
                            act_games.push(v);
                    }
                    nsp_l.emit('list_changed', act_games);

                    games[index]['interval'] = setInterval(function() {
                        let dx = ball['v'] * Math.cos(ball['a']) * 60./1000;
                        let dy = ball['v'] * Math.sin(ball['a']) * 60./1000;
                        //console.log('dx', dx, 'dy', dy, games[index]['state1'], games[index]['state2']);
                        ball['x'] += dx;
                        ball['y'] += dy;
                        if(ball['x'] - ball['w']/2 < 0) {
                            ball['a'] = Math.PI - ball['a'];
                            if(ball['a'] > Math.PI) {
                                ball['a'] -= 2*Math.PI;
                            }
                            if(ball['a'] < -Math.PI) {
                                ball['a'] += 2*Math.PI;
                            }
                            games[index]['sc2']++;
                            nsp_g.to(index).emit('score_changed', {'sc1' : games[index]['sc1'], 'sc2' : games[index]['sc2']});
                        }
                        if(ball['x'] + ball['w']/2 > 300) {
                            ball['a'] = Math.PI - ball['a'];
                            if(ball['a'] > Math.PI) {
                                ball['a'] -= 2*Math.PI;
                            }
                            if(ball['a'] < -Math.PI) {
                                ball['a'] += 2*Math.PI;
                            }
                            games[index]['sc1']++;
                            nsp_g.to(index).emit('score_changed', {'sc1' : games[index]['sc1'], 'sc2' : games[index]['sc2']});
                        }
                        if(ball['y'] - ball['h']/2 < 0 || ball['y'] + ball['h']/2 > 300) {
                            ball['a'] = - ball['a'];
                        }
                        //console.log(ball['a']/Math.PI);
                        if(ball_collides_rq(ball, games[index]['rq1']) || ball_collides_rq(ball, games[index]['rq2'])) {
                            let p_collide = '';
                            if(ball_collides_rq(ball, games[index]['rq1']))
                                p_collide = 'state1';
                            else
                                p_collide = 'state2';
                            ball['a'] = (Math.PI - ball['a']);//%2*Math.PI;
                            if(ball['a'] > Math.PI) {
                                ball['a'] -= 2*Math.PI;
                            }
                            if(ball['a'] < -Math.PI) {
                                ball['a'] += 2*Math.PI;
                            }
                            if(dy > 0) {//balle descends
                                if(games[index][p_collide]['down']) {
                                    ball['a'] = (4*ball['a'] + Math.PI/2)/5;
                                } else if(games[index][p_collide]['up']) {
                                    if(-dx < 0) {
                                        ball['a'] = (2*ball['a'] + Math.PI)/3;
                                    } else {
                                        ball['a'] = (2*ball['a'] + 0)/3;
                                    }
                                }
                            } else { //balle monte
                                if(ball['a'] <= 0) {
                                    console.log("monte : ", ball['a']/Math.PI);
                                }
                                if(games[index][p_collide]['up']) {
                                    ball['a'] = (4*ball['a'] - Math.PI/2)/5;
                                } else if(games[index][p_collide]['down']) {
                                    if(-dx < 0) {
                                        ball['a'] = (2*ball['a'] - Math.PI)/3;
                                    } else {
                                        ball['a'] = (2*ball['a'] - 0)/3;
                                    }
                                }
                            }
                        }
                        nsp_g.to(index).emit('pos_changed', {'x' : ball['x'], 'y' : ball['y']});
                    }, 60);
                } else {
                    socket.emit('kick', 'No place in this game');
                }
            } else {
                socket.emit('kick', 'Game removed');
            }
        });

        socket.on('move', function(mv) {
            //console.log("move", mv);
            if(index != undefined) {
                rq['y'] += mv.y;
                //console.log(rq)
                nsp_g.to(index).emit('pos_changed_rq', {'id' : id, 'y' : rq['y']});
            }
        });

        socket.on('state', function(state) {
            if(index != undefined) {
                st['up'] = state['up'];
                st['down'] = state['down'];
            }
        });

        socket.on('disconnect', function(socket) {
            console.log("disconnected", games[index], socket_id);
            if(index != undefined && games[index] != undefined && (games[index]['player1'] == socket_id || games[index]['player2'] == socket_id)) {
                console.log("clearing room", index);
                nsp_g.to(index).emit('kick', 'A player leaved the game');
                nsp_g.in(index).clients((error, socketIds) => {
                    if (error) throw error;
                    socketIds.forEach(socketId => nsp_g.sockets[socketId].leave(index));
                });

                clearInterval(games[index]['interval']);
                delete games[index];
                index = undefined;
                //send update game list
                let act_games = [];
                for(let v of Object.keys(games)) {
                    if(!games[v].started)
                        act_games.push(v);
                }
                nsp_l.emit('list_changed', act_games);
            }
        });
    });

    router.post('/', function(req, res, next) {
        let max = '-1';
        console.log(Object.keys(games));
        for(let v of Object.keys(games)) {
            if(Number(max) < Number(v)) {
                console.log(v);
                max = v;
            }
        }
        let new_id = Number(max) + 1;
        console.log(new_id)
        games[String(new_id)] = {'ball' : 
            {'x' : 150,
                'y' : 150,
                'w' : 10,
                'h' : 10,
                'v' : 100,
                'a' : Math.PI/3},
            'player1' : undefined,
            'sc1'     : 0,
            'rq1'     : {'x': 25, 'y': 150, 'w': 10, 'h': 40},
            'state1'  : {'up': false, 'down': false},
            'player2' : undefined,
            'sc2'     : 0,
            'rq2'     : {'x': 275, 'y': 150, 'w': 10, 'h': 40},
            'state2'  : {'up': false, 'down': false},
            'player2' : undefined,
            'interval': undefined,
            'started' : false};

        let act_games = [];
        for(let v of Object.keys(games)) {
            if(!games[v].started)
                act_games.push(v);
        }
        res.render('lobby', { games: act_games });
        nsp_l.emit('list_changed', act_games);
    });

    router.get('/:id', function(req, res, next) {
        let id = req.params.id;

        if(id in games) {
            res.render('game', { title: 'Super jeu kitutou', index: id });
        } else {
            console.log("bad");
        }
    });

    /* GET home page. */
    router.get('/', function(req, res, next) {
        let act_games = [];
        for(let v of Object.keys(games)) {
            if(!games[v].started)
                act_games.push(v);
        }
        console.log('games', act_games);
        res.render('lobby', {games : act_games});
    });
    return router;
}
