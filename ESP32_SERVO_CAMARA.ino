// -----------------------------------------------------------------
// PROYECTO JOJO - CONTROLADOR COMPLETO (ESP32)
// Módulos integrados:
//  - Control de Ruedas (L298N)
//  - Audio DFPlayer Mini (PINES 16/17)
//  
// -----------------------------------------------------------------

// --- 1. INCLUIR LIBRERÍAS ---
#include <WiFi.h>
#include <PubSubClient.h>
#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <ESP32Servo.h>

// --- 2. CONFIGURACIÓN DE RED ---
const char* ssid = "JOJO_NET";
const char* password = "jojo2025";
const char* mqtt_server = "192.168.0.102";  // IP de tu PC en JOJO_NET
const int mqtt_port = 1884;  // Puerto correcto del broker

// --- 3. DEFINICIÓN DE PINES ---
#define LED_INTEGRADO 2

// --- PINES DEL PUENTE H (L298N) ---
#define ENA 22
#define IN1 25
#define IN2 26
#define IN3 27
#define IN4 33
#define ENB 32

// --- SERVO CÁMARA ---
// #define SERVO_PIN 13  // Eliminado: no se usa servo

// --- PINES DFPLAYER (UART2) ---
// (ESP32 RX=16, TX=17)
HardwareSerial dfSerial(2);

// --- 4. OBJETOS GLOBALES ---
DFRobotDFPlayerMini dfPlayer;
WiFiClient espClient;
PubSubClient client(espClient);
// Servo servoCamara;  // Eliminado: no se usa servo

// -----------------------------------------------------------------
// FUNCIÓN setup()
// -----------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("\n[JOJO COMPLETO v3 - Ruedas + Audio + Servo] Iniciando...");

  // --- LED ---
  pinMode(LED_INTEGRADO, OUTPUT);
  digitalWrite(LED_INTEGRADO, LOW);

  // --- MOTORES ---
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  controlRuedas("stop");

  // --- SERVO CÁMARA ---
  // Eliminado: no se usa servo

  // --- DFPLAYER ---
  // (ESP32 RX=16, TX=17)
  dfSerial.begin(9600, SERIAL_8N1, 16, 17); 
  if (dfPlayer.begin(dfSerial)) {
    Serial.println("DFPlayer OK.");
    dfPlayer.volume(50);  // Volumen MÁXIMO (0-50)
    delay(200);
    dfPlayer.EQ(DFPLAYER_EQ_NORMAL);  // Ecualización normal
    delay(200);
  } else {
    Serial.println("ERROR DFPlayer — revisar pines 16/17.");
  }

  // --- WiFi + MQTT ---
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// -----------------------------------------------------------------
// FUNCIÓN setup_wifi()
// -----------------------------------------------------------------
void setup_wifi() {
  Serial.print("Conectando a WiFi ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED && WiFi.status() != WL_CONNECT_FAILED) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n¡WiFi conectado!");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nError WiFi. Reiniciando...");
    delay(3000);
    ESP.restart();
  }
}

// -----------------------------------------------------------------
// FUNCIÓN callback()  — OÍDO MQTT
// -----------------------------------------------------------------
void callback(char* topic, byte* payload, unsigned int length) {
  String mensaje = "";
  for (int i = 0; i < length; i++) mensaje += (char)payload[i];

  Serial.print("Tema: ");
  Serial.print(topic);
  Serial.print(" | Mensaje: ");
  Serial.println(mensaje);

  // -------------------------------
  // CONTROL LED
  // -------------------------------
  if (strcmp(topic, "jojo/luz") == 0) {
    if (mensaje == "ON") {
      digitalWrite(LED_INTEGRADO, HIGH);
      Serial.println("LED ON");
    } else if (mensaje == "OFF") {
      digitalWrite(LED_INTEGRADO, LOW);
      Serial.println("LED OFF");
    }
  }

  // -------------------------------
  // CONTROL RUEDAS
  // -------------------------------
  if (strcmp(topic, "jojo/ruedas") == 0) {
    Serial.print("Ruedas → ");
    Serial.println(mensaje);
    controlRuedas(mensaje);
  }

  // -------------------------------
  // CONTROL AUDIO (DFPlayer)
  // -------------------------------
  if (strcmp(topic, "jojo/audio/play") == 0) {
    int track = mensaje.toInt();
    dfPlayer.play(track);  // Reproduce 0001.mp3, 0002.mp3, 0003.mp3 desde carpeta 01
    Serial.print("Reproduciendo track: ");
    Serial.println(track);
    delay(500); // Pausa para procesar
  }

  // -------------------------------
  // CONTROL SERVO CÁMARA
  // Eliminado: no se usa servo
}

// -----------------------------------------------------------------
// FUNCIÓN reconnect()
// -----------------------------------------------------------------
void reconnect() {
  while (!client.connected()) {
    Serial.print("Reconectando MQTT... ");
    if (client.connect("ESP32-JOJO-Control")) {
      Serial.println("OK");

      client.subscribe("jojo/luz");
      client.subscribe("jojo/ruedas");
      client.subscribe("jojo/audio/play");
      client.subscribe("jojo/servo/camara");

      Serial.println("Suscrito a todos los temas.");
    } else {
      Serial.print("Falló, rc=");
      Serial.print(client.state());
      Serial.println(" -> Reintentando en 5s");
      delay(5000);
    }
  }
}

// -----------------------------------------------------------------
// loop()
// -----------------------------------------------------------------
void loop() {
  if (!client.connected()) reconnect();
  client.loop();
}

// -----------------------------------------------------------------
// controlRuedas()
// -----------------------------------------------------------------
void controlRuedas(String comando) {
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);

  if (comando == "adelante") {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  }
  else if (comando == "atras") {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  }
  else if (comando == "izquierda") {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  }
  else if (comando == "derecha") {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  }
  else if (comando == "stop") {
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  }
}
