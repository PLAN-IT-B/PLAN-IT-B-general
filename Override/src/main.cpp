#include "WiFi.h"
#include "PubSubClient.h" //pio lib install "knolleary/PubSubClient"

#define SSID          "NETGEAR68"
#define PWD           "excitedtuba713"
#define MQTT_SERVER   "192.168.1.2"
#define MQTT_PORT     1883

#define LED_PIN       2

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void callback(char *topic, byte *message, unsigned int length);

void setup_wifi()
{
  delay(10);
  Serial.println("Connecting to WiFi..");

  WiFi.begin(SSID, PWD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup()
{

  Serial.begin(115200);

  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);




}

void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();


}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // CREATE UNIQUE client ID!
    // in Mosquitto broker enable anom. access
    if (client.connect("Override"))
    {
      Serial.println("connected");
      // Subscribe
      // Vul hieronder in naar welke directories je gaat luisteren.
      //Voor de communicatie tussen de puzzels, check "Datacommunicatie.docx". (terug tevinden in dezelfde repository) 
      client.subscribe("controlpanel/#");
      client.subscribe("garbage/#");
      client.subscribe("controlpanel/#");
      client.subscribe("TrappenMaar/#");
      client.subscribe("Wristbands/#");
      }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000)
  {
    lastMsg = now;
  }

  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '0'){
      client.publish("controlpanel/reset","Reset escaperoom");
      Serial.println("reset");
    }

    if(temp == '1'){
      client.publish("garbage/eindcode","1234");
      Serial.println("eindcode garbage gestuurd");
    }
    else if(temp == 'g'){
      client.publish("TrappenMaar/zone","groen");
      Serial.println("groen gestuurd");
  }
    else if(temp == 'o'){
      client.publish("TrappenMaar/zone","oranje");
      Serial.println("oranje gestuurd");
}
    else if(temp == 'r'){
      client.publish("TrappenMaar/zone","rood");
      Serial.println("rood gestuurd");
  }
    else if(temp == 'h'){
      client.publish("Wristbands","hartslagsensor uit");
      Serial.println("hartslagsensor uit");
      }
    else if(temp == '2'){
      client.publish("Wristbands","Stop Wristbands");
      Serial.println("stop wristbands");
      }
    
    else if(temp == 'i'){
      client.publish("Wristbands","hartslagsensor aan");
      Serial.println("hartslagsensor aan");
      }
    else if(temp == 'w'){
      client.publish("Wristbands","Herstart Wristbands");
      Serial.println("herstart wristbands");
      }
    
    else if(temp == '3'){
      client.publish("Wristbands","Reset Wristbands");
      Serial.println("reset wristbands");
      }
    else if(temp =='4'){
      client.publish("garbage/status","Garbage code is correct ingegeven");
      Serial.println("Garbage code correct ingegeven");
    }
    else if(temp =='5'){
      client.publish("treingame/4decijfer","Trein-code 0");
      Serial.println("Cijfer treingame is doorgestuurd");
    }
    else if(temp=='6'){
      client.publish("wristbands/code","Wristband-code 0000");
      Serial.println("Code wristbands is doorgestuurd");
    }
    else if(temp=='7'){
      client.publish("TrappenMaar/buffer","Reset TrappenMaar");
      Serial.println("buffer resetten");
    }
    else if(temp=='p'){
      client.publish("eindpuzzel/timer","partnerruil");
      Serial.println("Partnerruil");
    }
    else if(temp=='s'){
      client.publish("controlpanel/status","Trappenmaar Ready");
      client.publish("controlpanel/status","Traingame Ready");
      for (int i = 0; i < 4; i++)
      {
      client.publish("controlpanel/status","Wristbands Ready");
      }     
      client.publish("controlpanel/status","Garbage Ready");
      client.publish("controlpanel/status","UV-slot Ready");
      Serial.println("Elke status ready");
    }
    else if(temp=='f'){
      client.publish("TrappenMaar/buffer","grote fout");
      Serial.println("buffer omlaag");
    }
    else if(temp=='a'){
      client.publish("TrappenMaar/buffer","omhoog");
      Serial.println("buffer omhoog");
    }
    else if(temp == 'l'){
      client.publish("TrappenMaar/zone","vol");
      Serial.println("vol gestuurd");
  }
    else if(temp == 'm'){
      client.publish("TrappenMaar/zone","niet vol");
      Serial.println("niet vol gestuurd");
  }
   
      
  }

  /*
    GEBRUIKTE TEKENS
    0   "reset"
    1   "eindcode garbage gestuurd"(1234)
    2   "stop wristbands"
    3   "reset wristbands"
    4   "Garbage code is correct ingegeven"
    5   "Treingame cijfer"
    6   "Code wristbands"
    7   
    8
    9
    
    a   "Buffer omhoog"
    b
    c
    d
    e
    f   grote fout
    g   "groen gestuurd"
    h   "hartslagsensor uit"
    i   "hartslagsensor aan"
    j
    k
    l   "vol"
    m   "niet vol"
    n
    o   "oranje gestuurd"
    p   "partnerruil"
    q
    r   "rood gestuurd"
    s   status ready
    t
    u
    v
    w   "herstart wristbands"
    x
    y
    z
  */
  
  
  
 
  

}