var messageArea = document.getElementById("webViewArea");
var manualScrolling = false;
var newMessages = 0;
var bottomBar;
$('body,html').bind('scroll mousedown wheel DOMMouseScroll mousewheel keyup', function(e) {
    if (e.which > 0 || e.type == "mousedown" || e.type == "mousewheel") {
        $("html,body").stop();
        if (!manualScrolling && document.body.scrollHeight > window.innerHeight) {
            bottomBar = document.createElement('div');
            bottomBar.id = "bottomBar";
            bottomBar.className = "animated fadeInUp";
            document.body.appendChild(bottomBar);
            bottomBar.innerHTML = "<p id='bottomBarText'>New messages: " + newMessages + "</p>";
            // bottomBar.style.cursor = 'pointer';
            bottomBar.onclick = function() {
                manualScrolling = false;
                newMessages = 0;
                bottomBar.remove();
                window.scrollTo(0, document.body.scrollHeight);
            };
            manualScrolling = true;
        }


    }
});
// $('body,html').bind('scroll mouseup wheel DOMMouseScroll mousewheel keyup', function(e){
//  if ( e.which > 0 || e.type == "mouseup" || e.type == "mousewheel"){
//    manualScrolling = false;
//  }
// });
// function scroll_to(div){
//       $('html, body').animate({
//               scrollTop: $(div).offset().top
//       },1);
// }

function showMessage(message) {
    var div = document.createElement('div');
    div.className = 'message-container';
    document.body.appendChild(div);
    div.innerHTML = message;
    if (!manualScrolling) {
        window.scrollTo(0, document.body.scrollHeight);
    } else {
        newMessages++;
        bottomBar.innerHTML = "<p id='bottomBarText'>New messages: " + newMessages + "</p>";
    }

}
var port = getCookie("messages_port");
var wsUri = "ws://127.0.0.1:" + port;
var websocket = null;

function initWebSocket() {
    try {
        if (typeof MozWebSocket == 'function')
            WebSocket = MozWebSocket;
        if (websocket && websocket.readyState == 1)
            websocket.close();
        websocket = new WebSocket(wsUri);
        websocket.onopen = function(evt) {
            //debug("CONNECTED");
        };
        websocket.onclose = function(evt) {
            //debug("DISCONNECTED");
        };
        websocket.onmessage = function(evt) {
            showMessage(evt.data);
        };
        websocket.onerror = function(evt) {
            console.log('ERROR: ' + evt.data);
        };
    } catch (exception) {
        console.log('ERROR: ' + exception);
    }
}

function stopWebSocket() {
    if (websocket)
        websocket.close();
}

function checkSocket() {
    if (websocket != null) {
        var stateStr;
        switch (websocket.readyState) {
            case 0:
                {
                    stateStr = "CONNECTING";
                    break;
                }
            case 1:
                {
                    stateStr = "OPEN";
                    break;
                }
            case 2:
                {
                    stateStr = "CLOSING";
                    break;
                }
            case 3:
                {
                    stateStr = "CLOSED";
                    break;
                }
            default:
                {
                    stateStr = "UNKNOW";
                    break;
                }
        }
        debug("WebSocket state = " + websocket.readyState + " ( " + stateStr + " )");
    } else {
        debug("WebSocket is null");
    }
}

function getQueryVariable(variable) {
    var query = window.location.search.substring(1);
    var vars = query.split("&");
    for (var i = 0; i < vars.length; i++) {
        var pair = vars[i].split("=");
        if (pair[0] == variable) {
            return pair[1];
        }
    }
    alert('Query Variable ' + variable + ' not found');
}

function getCookie(name) {
    var matches = document.cookie.match(new RegExp(
        "(?:^|; )" + name.replace(/([\.$?*|{}\(\)\[\]\\\/\+^])/g, '\\$1') + "=([^;]*)"
    ));
    return matches ? decodeURIComponent(matches[1]) : undefined;
}
