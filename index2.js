var clientId = "client_id_" + Math.floor(Math.random() * 1000000 + 1);
client = new Paho.MQTT.Client("driver.cloudmqtt.com", 38651, clientId);

client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

const patron = document.getElementById("patrones");

var options = {
  useSSL: true,
  userName: "clxxwiek",
  password: "PtsiiKX9xhVm",
  onSuccess: onConnect,
  onFailure: doFail,
};

client.connect(options);

function onConnect() {
  console.log("Conexion etsitosa!!");
  client.subscribe("salida");
}

function onMessageArrived(message) {
  const mensajito = message.payloadString;
  //console.log("Un mensaje ha llegado: " + message.payloadString);
  const mensaje = document.createElement("p");
  mensaje.innerHTML = `Un mensaje ha llegado: ${mensajito}`;
  valores.appendChild(mensaje);
}

function doFail(e) {
  console.log(e);
}

function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost: " + responseObject.errorMessage);
  }
}

function command(topic, value) {
  console.log(value);
  message = new Paho.MQTT.Message(value + " ");
  message.destinationName = topic;
  client.send(message);
}

const selectpatron = () => {
  command("selectPresetSM", patron.value);
};

const setPreview = () => {
    const videoContainer = document.querySelector("#display_animation");
    while(videoContainer.lastElementChild){
    
        videoContainer.removeChild(videoContainer.lastElementChild);
        console.log(videoContainer)
    }
    const video = document.createElement("video");
    video.src = `assets/${patron.value}.mp4`;
    video.autoplay = true;
    video.loop = true
    videoContainer.append(video);
}
setPreview();
