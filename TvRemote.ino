#include <i18n.h>
#include <IRac.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRtext.h>
#include <IRtimer.h>
#include <IRutils.h>
#include <ir_Amcor.h>
#include <ir_Argo.h>
#include <ir_Coolix.h>
#include <ir_Daikin.h>
#include <ir_Delonghi.h>
#include <ir_Electra.h>
#include <ir_Fujitsu.h>
#include <ir_Goodweather.h>
#include <ir_Gree.h>
#include <ir_Haier.h>
#include <ir_Hitachi.h>
#include <ir_Kelvinator.h>
#include <ir_LG.h>
#include <ir_Magiquest.h>
#include <ir_Midea.h>
#include <ir_Mitsubishi.h>
#include <ir_MitsubishiHeavy.h>
#include <ir_NEC.h>
#include <ir_Neoclima.h>
#include <ir_Panasonic.h>
#include <ir_Samsung.h>
#include <ir_Sharp.h>
#include <ir_Tcl.h>
#include <ir_Teco.h>
#include <ir_Toshiba.h>
#include <ir_Trotec.h>
#include <ir_Vestel.h>
#include <ir_Whirlpool.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

#include <IRremoteESP8266.h>
#define IR_SEND_PIN D2  //Digital pin connected to IR_SEND

const char* ssid = "XXXXX"; //Type here your wifi SSID
const char* password = "XXXXX"; //Type here your wifi PASSWORD

IRsend irsend(IR_SEND_PIN);

WiFiClient espClient;
ESP8266WebServer webServer(80);

int requestIRCode() {
  return webServer.arg("code").toInt();
}

int requestIRLength() {
  return webServer.arg("length").toInt();
}

void setup() {
  Serial.begin(115200);
  WiFi.softAPdisconnect(true);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  webServer.on("/NEC", [](){
    irsend.sendNEC(requestIRCode(), requestIRLength());
    webServer.send(200, "text/plain", "Ok");
  });

  webServer.on("/RC5", [](){
    irsend.sendRC5(requestIRCode(), requestIRLength());
    webServer.send(200, "text/plain", "Ok");
  });

  webServer.on("/RC6", [](){
    irsend.sendRC6(requestIRCode(), requestIRLength());
    webServer.send(200, "text/plain", "Ok");
  });

  webServer.on("/PANASONIC", [](){
    int address = webServer.arg("address").toInt();
    irsend.sendPanasonic(address, requestIRCode(), requestIRLength());
    webServer.send(200, "text/plain", "Ok");
  });
  
  webServer.on("/LG", [](){
    irsend.sendLG(requestIRCode(), requestIRLength());
    webServer.send(200, "text/plain", "Ok");
  });
  webServer.begin();

  irsend.begin();
}


void loop() {
  webServer.handleClient();
}
