#include <Arduino.h>
#include <PubSubClient.h>
//publicaion de los temas
#include <WiFi.h>
//conexion a wifi

const char *ssid = "INFINITUM2FD7_2.4";
const char *password = "74QPn4UkhY";
WiFiClient espClient;
PubSubClient client(espClient);

//establecer conexion con el server
const char *mqtt_server = "driver.cloudmqtt.com";
const int mqtt_port = 18612;
const char *mqtt_user = "mfzjcshg";
const char *mqtt_pass = "W_P4G074jmif";

//control de los mensajes
long lastMsg = 0;
char msg[50];  //arreglo para los mensajes
int value = 0; //para los valores

void callback(char *topic, byte *payload, unsigned int length)
{ //entra un topico
  Serial.print("Mensaje recibido bajo el topico -> ");
  Serial.print(topic);
  Serial.print("\n");
  //interrupcion imprimir el payload
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]); //payload para los mensajes
  }
  Serial.println();
}

void setup_wifi()
{

  Serial.println();
  Serial.print("Conectado a -> ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println(".");
  }

  Serial.println("");
  Serial.println("Conexión Exitosa!");
  Serial.println("Mi ip es -> ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  //genera una cadena como id para conexion al servidor
  //generaa un identificador del cliente se le anexa un numero aleatorio  al identificador
  //por si la conexion no esta conectada o si alguien esta conectado con el mismo id del cliente genera otro id
  //verificar que estemos conectados con el broker
  while (!client.connected())
  {
    Serial.println("Intentando Conexion MQTT");
    String clientId = "iot_1_"; //genera una cadena para conexion con cliente
    clientId = clientId + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass))
    {
      Serial.println("Conexion a MQTT Exitosa!!");
      client.publish("RRX", "Primer Mensaje"); //envir mensaejs  eindicar a que topico tiene dos parametros
      client.subscribe("Entrada");
    }
    else
    {
      Serial.println("Fallo Conexion");
      Serial.print(client.state()); //reconectar con otro id
      Serial.print("Se intentara de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  //tema,paylod contenido de la informacion,lenght longitud del paylos
}

void loop()
{
  //si nuestro cliente se encuentra conectado si esta o no esta
  if (client.connected() == false)
  {
    reconnect(); //reconectarnos al cliente qtt
  }

  client.loop();

  if (millis() - lastMsg > 2000) //va a estar ejecutandose continimente hasta que la sentenncia se cumpla evaluar ell ultimo mnesaje con el tiempo de la plac
  {
    lastMsg = millis();
    value++;
    String mes = "Valor -> " + String(value);
    mes.toCharArray(msg, 50);
    client.publish("RRX", msg);
    Serial.println("Mensaje enviado -> " + String(value));
  }
}