$(document).ready(function(){

    let received = $('#received');
    let main_container = $('#main_container');
    let socket = new WebSocket("ws://" + server_ip + ":8080/ws");

    socket.onopen = function(){
      console.log("connected");
    };

    socket.onmessage = function (message) {

      let JSONObject = JSON.parse(message.data);
      console.log("receiving: " + JSONObject.message);
      console.log("receiving: " + JSONObject.hostname);
      console.log("receiving: " + JSONObject.type);

      if (JSONObject.type === 'new_connection') {
          let new_connection = '<div id="'+ JSONObject.hostname +'" class="module_container">'
            + '<div class="module_ip">'
            + JSONObject.hostname
            + '</div>'
            + '<div class="module_status green_status">'
            + '</div>'
            + '</div>';

          main_container.append(new_connection);
      }

      received.append(message.data);
      received.append($('<br/>'));
    };

    socket.onclose = function() {
      console.log("disconnected");
    };

    let sendMessage = function(message) {
      console.log("sending:" + message.data);
      socket.send(message.data);
    };

    // Send message to the server
    $("#cmd_send").click(function(ev){
      ev.preventDefault();

      let message = $('#cmd_value').val();
      sendMessage({ 'data' : message});
      $('#cmd_value').val("");
    });

    $('#clear').click(function(){
      received.empty();
    });
});
