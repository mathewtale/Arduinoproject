#include <WiFiNINA.h>
#include "Secrets.h"
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>
#include <Arduino_MKRIoTCarrier.h>
#include "Display.h"

MKRIoTCarrier carrier;
bool hasData = false;
unsigned long lastRequestAt = 0;
unsigned long lastPressed = 0;
bool requestMade = false;

byte selectedCategory = 9;
byte categoryIndex = 0;
String data;

WiFiSSLClient secureClient;
WiFiClient wifiClient;

bool connectWifi() {
 if (WiFi.status() == WL_CONNECTED) return true;
 Serial.println("Connecting to WiFi...");
 WiFi.begin(SECRET_SSID, SECRET_PASS);
 unsigned long started = millis();
 while (WiFi.status() != WL_CONNECTED &&
 millis() - started < 20000UL) {
 delay(250);
 Serial.print(".");
 }
 if (WiFi.status() != WL_CONNECTED) {
 Serial.println("\nWiFi failed. Check name and code.");
 return false;
 }
 Serial.print("\nIP: ");
 Serial.println(WiFi.localIP());
 return true;
}

bool fetchTrivaBody(byte category, String &body) {
  String path = "/api.php?amount=1&category=";
  path += String(category); //9-32
  path += "&difficulty=easy&type=multiple";

  HttpClient http(secureClient, "opentdb.com", 443);

  http.get(path);

  int httpCode = http.responseStatusCode();
  body = http.responseBody();

  Serial.print("HTTP status: ");
  Serial.println(httpCode);

  if (httpCode == 200) {
    return true;
  }

  Serial.println("Trivia request failed");
  Serial.println(body);
  return false;
}

bool fetchApiBody(String &body) {
  String path = "/api/quiz/test";

  HttpClient http(wifiClient, API_IP_ADDRESS, 5166);

  http.get(path);

  int httpCode = http.responseStatusCode();
  body = http.responseBody();

  Serial.println(httpCode);

  if (httpCode == 200) {
    return true;
  }

  Serial.println("Trivia request failed");
  Serial.println(body);
  return false;
}

void saveToDatabase(unsigned long now) {
  if (data.isEmpty()) {
    showScreen("No data", "to save", "");
    return;
  }

  if (requestMade && now - lastRequestAt < 5000UL) {
    showScreen("Wait at", "least 5 sec.", "between calls");
    return;
  }

  requestMade = true;
  lastRequestAt = now;

  showScreen("Saving data", "Wait...", "");

  if (WiFi.status() != WL_CONNECTED) {
    showScreen("WiFi error", "No connection", "");
    return;
  }

  String path = "/api/quiz";

  HttpClient http(wifiClient, API_IP_ADDRESS, 5166);

  int err = http.post(path, "application/json", data);

  if (err != 0) {
    Serial.print("POST error: ");
    Serial.println(err);
    http.stop();
    return;
  }


  int httpCode = http.responseStatusCode();
  String body = http.responseBody();

  if (httpCode == 200) {
    data = "";
    showScreen("Data saved", "to database", "");
  }

  Serial.print("HTTP status: ");
  Serial.println(httpCode);
  Serial.println("Response:");
  Serial.println(body);

  http.stop();
  requestMade = false;
}

bool parseTriviaData(String &body) {
  JSONVar data = JSON.parse(body);

  if (JSON.typeof(data) == "undefined") {
    Serial.println("Failed to parse JSON");
    return false;
  }

  int responseCode = (int)data["response_code"];

  if (responseCode != 0) {
    Serial.print("Trivia API error: ");
    Serial.println(responseCode);
    return false;
  }

  JSONVar results = data["results"];

  if (results.length() == 0) {
    Serial.println("No trivia questions found");
    return false;
  }

  JSONVar questionData = results[0];

  String category = (const char*)questionData["category"];
  String question = (const char*)questionData["question"];
  String correctAnswer = (const char*)questionData["correct_answer"];

  JSONVar incorrectAnswers = questionData["incorrect_answers"];

  JSONVar newJson;
  newJson["category"] = category;
  newJson["question"] = question;
  newJson["correct_answer"] = correctAnswer;
  newJson["incorrect_answers"] = incorrectAnswers;
  
  body = JSON.stringify(newJson);

  return true;
}

void loadObject() {
  if (requestMade && millis() - lastRequestAt < 5000UL) {
    showScreen("Wait at", "least 5 sec.", "between calls");
    return;
  }

  lastRequestAt = millis();
  requestMade = true;
  hasData = false;

  showScreen("Loading trivia", "Wait...", "");

  if (WiFi.status() != WL_CONNECTED) {
    showScreen("WiFi error", "No connection", "");
    return;
  }

  String body;
  if (fetchTrivaBody(selectedCategory, body) & parseTriviaData(body)) {
  //if (postApiBody(body)) {
    hasData = true;
    showScreen("Trivia Loaded", "Look at the", "Serial");

    Serial.println(body);
    data = body;
    requestMade = false;

  } else {
    showScreen("Fetch error", "Check Serial", "");
  }
}

void changeCategory(unsigned long now) {
  if (now - lastPressed < 500) return;
  lastPressed = now;

  categoryIndex = (categoryIndex + 1) % 24;
  selectedCategory = categoryIndex + 9;

  showScreen("Category", String(selectedCategory), "");
}




void setup() {
  Serial.begin(9600);
  unsigned long started = millis();
  while (!Serial && millis() - started < 3000UL) {}
  carrier.noCase();
  if (!carrier.begin()) {
    Serial.println("Carrier could not start.");
    while (true) { delay(100); }
  }
  
  beginDisplay(carrier);
  carrier.display.setRotation(0);
  carrier.display.setTextWrap(false);
  Serial.print("Carrier revision: ");
  Serial.println(carrier.getBoardRevision());
  showScreen("Trivia API", "Connecting...", "");
  bool connected = connectWifi();
  randomSeed(micros());
  showScreen("Trivia API",
  connected ? "WiFi ready" : "WiFi failed", "Press TOUCH0");
}

void loop() {
  carrier.Buttons.update();
  unsigned long now = millis();
  if (carrier.Buttons.onTouchDown(TOUCH0)) loadObject();
  if (carrier.Buttons.onTouchDown(TOUCH1)) changeCategory(now);
  if (carrier.Buttons.onTouchDown(TOUCH4)) saveToDatabase(now);
}