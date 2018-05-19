var games_list = document.getElementById("gamesu")

let update_list = (games) => {
    while (games_list.firstChild) {
        games_list.removeChild(games_list.firstChild);
    }
    for(let i of games) {
        let list = document.createElement('li')
        let link = document.createElement('a')
        let node = document.createTextNode('Game ' + i)
        link.href = 'game/'+i
        link.appendChild(node)
        list.appendChild(link)
        games_list.appendChild(list)
    }
}

//update_list(['1','2','17']);
var socket = io('/lobby');

socket.on('list_changed', function(g) {
    update_list(g);
});
