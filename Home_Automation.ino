/*Template ID using Blynk.Cloud*/
#define BLYNK_TEMPLATE_ID "BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Home Automation"
#define BLYNK_AUTH_TOKEN "BLYNK_AUTH_TOKEN"

// WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "WIFI NAME";
char pass[] = "WIFI PASSWORD";

bool fetch_blynk_state = true; // true or false

// Update the  code of IR Remote buttons
#define IR_Button_1 33444015
#define IR_Button_2 33478695
#define IR_Button_3 33486855
#define IR_Button_4 33435333
#define IR_All_Off 33435111

// Library Include
/* --- Firebase --- */
#include <FirebaseESP8266.h>

#define FIREBASE_HOST "FIREBASE_HOST"
#define FIREBASE_AUTH "FIREBASE_AUTH"

FirebaseData firebaseData;

// Google Sheet
// Your Domain name with URL path or IP address with path
String serverName = "https://script.google.com/macros/s/serverName/exec";
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
// unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

// #define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>
#include <IRremote.h>

#define DHTPIN D4      // D4  pin connected with DHT
#define IR_RECV_PIN 14 // D5 (IR receiver pin)

#define DHTTYPE DHT11 // DHT 11

IRrecv irrecv(IR_RECV_PIN);
decode_results results;

// define the GPIO connected with Relays and switches
#define RelayPin1 5  // D1
#define RelayPin2 4  // D2
#define RelayPin3 D8 // D8
#define RelayPin4 12 // D6

#define SwitchPin1 10 // SD3
#define SwitchPin2 D3 // D3
#define SwitchPin3 13 // D7
#define SwitchPin4 3  // RX

#define wifiLed 16 // D0

// Change the virtual pins according the rooms
#define VPIN_BUTTON_1 V1
#define VPIN_BUTTON_2 V2
#define VPIN_BUTTON_3 V3
#define VPIN_BUTTON_4 V4

#define VPIN_BUTTON_C V5
#define VPIN_TEMPERATURE V6
#define VPIN_HUMIDITY V7

// Relay State
bool toggleState_1 = LOW; // Define integer to remember the toggle state for relay 1
bool toggleState_2 = LOW; // Define integer to remember the toggle state for relay 2
bool toggleState_3 = LOW; // Define integer to remember the toggle state for relay 3
bool toggleState_4 = LOW; // Define integer to remember the toggle state for relay 4

// Switch State
bool SwitchState_1 = LOW;
bool SwitchState_2 = LOW;
bool SwitchState_3 = LOW;
bool SwitchState_4 = LOW;

int wifiFlag = 0;
float temperature1 = 0;
float humidity1 = 0;

char auth[] = BLYNK_AUTH_TOKEN;

BlynkTimer timer;
DHT dht(DHTPIN, DHTTYPE);

// When App button is pushed - switch the state
BLYNK_WRITE(VPIN_BUTTON_1)
{
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, !toggleState_1);
  if (Firebase.setString(firebaseData, "/relay1", String((toggleState_1 == LOW ? "off" : "on"))))
  {
  }
  else
  {
    Serial.println("relay1 Data Not Sent");
    Serial.println(firebaseData.errorReason());
  }
}

BLYNK_WRITE(VPIN_BUTTON_2)
{
  toggleState_2 = param.asInt();
  digitalWrite(RelayPin2, !toggleState_2);
  if (Firebase.setString(firebaseData, "/relay2", String((toggleState_2 == LOW ? "off" : "on"))))
  {
  }
  else
  {
    Serial.println("relay2 Data Not Sent");
    Serial.println(firebaseData.errorReason());
  }
}

BLYNK_WRITE(VPIN_BUTTON_3)
{
  toggleState_3 = param.asInt();
  digitalWrite(RelayPin3, !toggleState_3);
  if (Firebase.setString(firebaseData, "/relay3", String((toggleState_3 == LOW ? "off" : "on"))))
  {
  }
  else
  {
    Serial.println("relay3 Data Not Sent");
    Serial.println(firebaseData.errorReason());
  }
}

BLYNK_WRITE(VPIN_BUTTON_4)
{
  toggleState_4 = param.asInt();
  digitalWrite(RelayPin4, !toggleState_4);
  if (Firebase.setString(firebaseData, "/relay4", String((toggleState_4 == LOW ? "off" : "on"))))
  {
  }
  else
  {
    Serial.println("relay4 Data Not Sent");
    Serial.println(firebaseData.errorReason());
  }
}

BLYNK_WRITE(VPIN_BUTTON_C)
{
  all_SwitchOff();
}

void all_SwitchOff()
{
  toggleState_1 = 0;
  digitalWrite(RelayPin1, HIGH);
  Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
  delay(100);
  if (Firebase.setString(firebaseData, "/relay1", String((toggleState_1 == LOW ? "off" : "on"))))
  {
  }
  else
  {
    Serial.println("relay1 Data Not Sent");
    Serial.println(firebaseData.errorReason());
  }
  toggleState_2 = 0;
  digitalWrite(RelayPin2, HIGH);
  Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
  delay(100);
  if (Firebase.setString(firebaseData, "/relay2", String((toggleState_2 == LOW ? "off" : "on"))))
  {
  }
  else
  {
    Serial.println("relay2 Data Not Sent");
    Serial.println(firebaseData.errorReason());
  }
  toggleState_3 = 0;
  digitalWrite(RelayPin3, HIGH);
  Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
  delay(100);
  if (Firebase.setString(firebaseData, "/relay3", String((toggleState_3 == LOW ? "off" : "on"))))
  {
  }
  else
  {
    Serial.println("relay3 Data Not Sent");
    Serial.println(firebaseData.errorReason());
  }
  toggleState_4 = 0;
  digitalWrite(RelayPin4, HIGH);
  Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
  if (Firebase.setString(firebaseData, "/relay4", String((toggleState_4 == LOW ? "off" : "on"))))
  {
  }
  else
  {
    Serial.println("relay4 Data Not Sent");
    Serial.println(firebaseData.errorReason());
  }
  delay(100);
  Blynk.virtualWrite(VPIN_HUMIDITY, humidity1);
  Blynk.virtualWrite(VPIN_TEMPERATURE, temperature1);
  if (Firebase.setString(firebaseData, "/temperature", temperature1))
  {
    // Serial.println ("Temperature Data Sent");
  }
  else
  {
    Serial.println("Temperature Data Not Sent");
    Serial.println(firebaseData.errorReason());
  }
  if (Firebase.setString(firebaseData, "/humidity", humidity1))
  {
    // Serial.println ("Humidity Data Sent");
  }
  else
  {
    Serial.println("Humidity Data Not Sent");
    Serial.println(firebaseData.errorReason());
  }
}

void checkBlynkStatus()
{ // called every 2 seconds by SimpleTimer
  bool isconnected = Blynk.connected();
  if (isconnected == false)
  {
    wifiFlag = 1;
    Serial.println("Blynk Not Connected");
    digitalWrite(wifiLed, LOW);
  }
  if (isconnected == true)
  {
    wifiFlag = 0;
    if (!fetch_blynk_state)
    {
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
      Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
      Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
      if (Firebase.setString(firebaseData, "/relay1", String((toggleState_1 == LOW ? "off" : "on"))))
      {
      }
      else
      {
        Serial.println("relay1 Data Not Sent");
        Serial.println(firebaseData.errorReason());
      }
      if (Firebase.setString(firebaseData, "/relay2", String((toggleState_2 == LOW ? "off" : "on"))))
      {
      }
      else
      {
        Serial.println("relay2 Data Not Sent");
        Serial.println(firebaseData.errorReason());
      }
      if (Firebase.setString(firebaseData, "/relay3", String((toggleState_3 == LOW ? "off" : "on"))))
      {
      }
      else
      {
        Serial.println("relay3 Data Not Sent");
        Serial.println(firebaseData.errorReason());
      }
      if (Firebase.setString(firebaseData, "/relay4", String((toggleState_4 == LOW ? "off" : "on"))))
      {
      }
      else
      {
        Serial.println("relay4 Data Not Sent");
        Serial.println(firebaseData.errorReason());
      }
    }
    digitalWrite(wifiLed, HIGH);
    Serial.println("Blynk Connected");
  }
}

BLYNK_CONNECTED()
{
  // Request the latest state from the server
  if (fetch_blynk_state)
  {
    Blynk.syncVirtual(VPIN_BUTTON_1);
    Blynk.syncVirtual(VPIN_BUTTON_2);
    Blynk.syncVirtual(VPIN_BUTTON_3);
    Blynk.syncVirtual(VPIN_BUTTON_4);
  }
  Blynk.syncVirtual(VPIN_TEMPERATURE);
  Blynk.syncVirtual(VPIN_HUMIDITY);
}

void readSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  else
  {
    humidity1 = h;
    temperature1 = t;
    // Serial.println(temperature1);
    // Serial.println(humidity1);
    if (Firebase.setString(firebaseData, "/temperature", temperature1))
    {
      // Serial.println ("Temperature Data Sent");
    }
    else
    {
      Serial.println("Temperature Data Not Sent");
      Serial.println(firebaseData.errorReason());
    }
    if (Firebase.setString(firebaseData, "/humidity", humidity1))
    {
      // Serial.println ("Humidity Data Sent");
    }
    else
    {
      Serial.println("Humidity Data Not Sent");
      Serial.println(firebaseData.errorReason());
    }
  }
}

void sendSensor()
{
  readSensor();
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(VPIN_HUMIDITY, humidity1);
  Blynk.virtualWrite(VPIN_TEMPERATURE, temperature1);
}

void ir_remote()
{
  if (irrecv.decode(&results))
  {
    Serial.println(results.value);
    unsigned int ircode = results.value;
    switch (ircode)
    {
    case IR_Button_1:
      digitalWrite(RelayPin1, toggleState_1);
      toggleState_1 = !toggleState_1;
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
      if (Firebase.setString(firebaseData, "/relay1", String((toggleState_1 == LOW ? "off" : "on"))))
      {
      }
      else
      {
        Serial.println("relay1 Data Not Sent");
        Serial.println(firebaseData.errorReason());
      }
      delay(100);
      break;
    case IR_Button_2:
      digitalWrite(RelayPin2, toggleState_2);
      toggleState_2 = !toggleState_2;
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
      if (Firebase.setString(firebaseData, "/relay2", String((toggleState_2 == LOW ? "off" : "on"))))
      {
      }
      else
      {
        Serial.println("relay2 Data Not Sent");
        Serial.println(firebaseData.errorReason());
      }
      delay(100);
      break;
    case IR_Button_3:
      digitalWrite(RelayPin3, toggleState_3);
      toggleState_3 = !toggleState_3;
      Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
      if (Firebase.setString(firebaseData, "/relay3", String((toggleState_3 == LOW ? "off" : "on"))))
      {
      }
      else
      {
        Serial.println("relay3 Data Not Sent");
        Serial.println(firebaseData.errorReason());
      }
      delay(100);
      break;
    case IR_Button_4:
      digitalWrite(RelayPin4, toggleState_4);
      toggleState_4 = !toggleState_4;
      Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
      if (Firebase.setString(firebaseData, "/relay4", String((toggleState_4 == LOW ? "off" : "on"))))
      {
      }
      else
      {
        Serial.println("relay4 Data Not Sent");
        Serial.println(firebaseData.errorReason());
      }
      delay(100);
      break;
    case IR_All_Off:
      all_SwitchOff();
      break;
    default:
      break;
    }
    // Serial.println(results.value, HEX);
    irrecv.resume();
  }
}

void manual_control()
{
  if (digitalRead(SwitchPin1) == LOW && SwitchState_1 == LOW)
  {
    digitalWrite(RelayPin1, LOW);
    toggleState_1 = HIGH;
    SwitchState_1 = HIGH;
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    if (Firebase.setString(firebaseData, "/relay1", String((toggleState_1 == LOW ? "off" : "on"))))
    {
    }
    else
    {
      Serial.println("relay1 Data Not Sent");
      Serial.println(firebaseData.errorReason());
    }
    Serial.println("Switch-1 on");
  }
  if (digitalRead(SwitchPin1) == HIGH && SwitchState_1 == HIGH)
  {
    digitalWrite(RelayPin1, HIGH);
    toggleState_1 = LOW;
    SwitchState_1 = LOW;
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    if (Firebase.setString(firebaseData, "/relay1", String((toggleState_1 == LOW ? "off" : "on"))))
    {
    }
    else
    {
      Serial.println("relay1 Data Not Sent");
      Serial.println(firebaseData.errorReason());
    }
    Serial.println("Switch-1 off");
  }
  if (digitalRead(SwitchPin2) == LOW && SwitchState_2 == LOW)
  {
    digitalWrite(RelayPin2, LOW);
    toggleState_2 = HIGH;
    SwitchState_2 = HIGH;
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    if (Firebase.setString(firebaseData, "/relay2", String((toggleState_2 == LOW ? "off" : "on"))))
    {
    }
    else
    {
      Serial.println("relay2 Data Not Sent");
      Serial.println(firebaseData.errorReason());
    }
    Serial.println("Switch-2 on");
  }
  if (digitalRead(SwitchPin2) == HIGH && SwitchState_2 == HIGH)
  {
    digitalWrite(RelayPin2, HIGH);
    toggleState_2 = LOW;
    SwitchState_2 = LOW;
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    if (Firebase.setString(firebaseData, "/relay2", String((toggleState_2 == LOW ? "off" : "on"))))
    {
    }
    else
    {
      Serial.println("relay2 Data Not Sent");
      Serial.println(firebaseData.errorReason());
    }
    Serial.println("Switch-2 off");
  }
  if (digitalRead(SwitchPin3) == LOW && SwitchState_3 == LOW)
  {
    digitalWrite(RelayPin3, LOW);
    toggleState_3 = HIGH;
    SwitchState_3 = HIGH;
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
    if (Firebase.setString(firebaseData, "/relay3", String((toggleState_3 == LOW ? "off" : "on"))))
    {
    }
    else
    {
      Serial.println("relay3 Data Not Sent");
      Serial.println(firebaseData.errorReason());
    }
    Serial.println("Switch-3 on");
  }
  if (digitalRead(SwitchPin3) == HIGH && SwitchState_3 == HIGH)
  {
    digitalWrite(RelayPin3, HIGH);
    toggleState_3 = LOW;
    SwitchState_3 = LOW;
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
    if (Firebase.setString(firebaseData, "/relay3", String((toggleState_3 == LOW ? "off" : "on"))))
    {
    }
    else
    {
      Serial.println("relay3 Data Not Sent");
      Serial.println(firebaseData.errorReason());
    }
    Serial.println("Switch-3 off");
  }
  if (digitalRead(SwitchPin4) == LOW && SwitchState_4 == LOW)
  {
    digitalWrite(RelayPin4, LOW);
    toggleState_4 = HIGH;
    SwitchState_4 = HIGH;
    Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
    if (Firebase.setString(firebaseData, "/relay4", String((toggleState_4 == LOW ? "off" : "on"))))
    {
    }
    else
    {
      Serial.println("relay4 Data Not Sent");
      Serial.println(firebaseData.errorReason());
    }
    Serial.println("Switch-4 on");
  }
  if (digitalRead(SwitchPin4) == HIGH && SwitchState_4 == HIGH)
  {
    digitalWrite(RelayPin4, HIGH);
    toggleState_4 = LOW;
    SwitchState_4 = LOW;
    Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
    if (Firebase.setString(firebaseData, "/relay4", String((toggleState_4 == LOW ? "off" : "on"))))
    {
    }
    else
    {
      Serial.println("relay4 Data Not Sent");
      Serial.println(firebaseData.errorReason());
    }
    Serial.println("Switch-4 off");
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(wifiLed, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);

  // During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, !toggleState_1);
  digitalWrite(RelayPin2, !toggleState_2);
  digitalWrite(RelayPin3, !toggleState_3);
  digitalWrite(RelayPin4, !toggleState_4);
  if (Firebase.setString(firebaseData, "/relay1", String((toggleState_1 == LOW ? "off" : "on"))))
  {
  }
  else
  {
    Serial.println("relay1 Data Not Sent");
    Serial.println(firebaseData.errorReason());
  }
  if (Firebase.setString(firebaseData, "/relay2", String((toggleState_2 == LOW ? "off" : "on"))))
  {
  }
  else
  {
    Serial.println("relay2 Data Not Sent");
    Serial.println(firebaseData.errorReason());
  }
  if (Firebase.setString(firebaseData, "/relay3", String((toggleState_3 == LOW ? "off" : "on"))))
  {
  }
  else
  {
    Serial.println("relay3 Data Not Sent");
    Serial.println(firebaseData.errorReason());
  }
  if (Firebase.setString(firebaseData, "/relay4", String((toggleState_4 == LOW ? "off" : "on"))))
  {
  }
  else
  {
    Serial.println("relay4 Data Not Sent");
    Serial.println(firebaseData.errorReason());
  }
  irrecv.enableIRIn(); // Enabling IR sensor
  dht.begin();         // Enabling DHT sensor

  digitalWrite(wifiLed, HIGH);

  // Blynk.begin(auth, ssid, pass);
  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus); // check if Blynk server is connected every 2 seconds
  timer.setInterval(1000L, sendSensor);       // Sending Sensor Data to Blynk Cloud every 1 second
  Blynk.config(auth);
  delay(1000);

  if (!fetch_blynk_state)
  {
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
    Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4);
    if (Firebase.setString(firebaseData, "/relay1", String((toggleState_1 == LOW ? "off" : "on"))))
    {
    }
    else
    {
      Serial.println("relay1 Data Not Sent");
      Serial.println(firebaseData.errorReason());
    }
    if (Firebase.setString(firebaseData, "/relay2", String((toggleState_2 == LOW ? "off" : "on"))))
    {
    }
    else
    {
      Serial.println("relay2 Data Not Sent");
      Serial.println(firebaseData.errorReason());
    }
    if (Firebase.setString(firebaseData, "/relay3", String((toggleState_3 == LOW ? "off" : "on"))))
    {
    }
    else
    {
      Serial.println("relay3 Data Not Sent");
      Serial.println(firebaseData.errorReason());
    }
    if (Firebase.setString(firebaseData, "/relay4", String((toggleState_4 == LOW ? "off" : "on"))))
    {
    }
    else
    {
      Serial.println("relay4 Data Not Sent");
      Serial.println(firebaseData.errorReason());
    }
  }

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop()
{
  manual_control();
  ir_remote(); // IR remote Control
  Blynk.run();
  timer.run();
  /*
  // Call Google script
  String serverPath = serverName + "?relay1=" + String((toggleState_1 == LOW ? "off" : "on" )) + "&relay2=" + String((toggleState_2 == LOW ? "off" : "on")) + "&relay3="+ String((toggleState_3 == LOW ? "off" : "on")) + "&relay4=" + String((toggleState_4 == LOW ? "off" : "on")) + "&temperature=" + String(temperature1) + "&humidity=" + String(humidity1);
  Serial.println(serverPath);

  int httpCode = -5; // Initialize to a non-successful code
  HTTPClient http;
  while (httpCode != HTTP_CODE_OK) {
    WiFiClient client;
    http.begin(client,serverPath);
    httpCode = http.GET();
    if (httpCode != HTTP_CODE_OK) {
      Serial.println("Error calling Google script");
      delay(1000);
    }
  }
  String payload = http.getString();
  http.end();
  // Print response from Google script
  Serial.println(httpCode);
  Serial.println(payload);
  */
}
