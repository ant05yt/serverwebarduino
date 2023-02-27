#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

const char* ssid = "onor";
const char* password = "12345678";

WebServer server(80);
const char* host = "example.com";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  server.on("/", []() {
    WiFiClient client;
    if (!client.connect(host, 80)) {
      Serial.println("Connection to host failed");
      return;
    }
    client.print(String("GET ") + server.uri() + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    while(client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        break;
      }
    }
    while(client.available()) {
      String line = client.readStringUntil('\n');
      server.send(200, "text/plain", line);
    }
    client.stop();
  });
  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
}
