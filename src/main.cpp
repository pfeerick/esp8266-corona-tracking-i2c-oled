#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <json_parser.h>
#include <WifiCredentials.h>

extern const char ssid[32];
extern const char password[64];

#define s2ms(second) (second * 1000)
unsigned long long prev_millis(0);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

unsigned long long interval = s2ms(60);
unsigned long long PreviousMillis = 0;
unsigned long long CurrentMillis = interval;
boolean firstUpdate = true;

void setup(void)
{
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) // Address 0x3D for 128x64
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  delay(2000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Covids Tracker by BwE");
  display.print("WiFi: ");
  display.println(ssid);
  display.println("Wait or Reset");
  display.display();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println(F(" CONNECTED!"));
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());
}

void loop()
{
  CurrentMillis = millis();

  if (firstUpdate || CurrentMillis - PreviousMillis >= interval)
  {
    firstUpdate = false;
    PreviousMillis = CurrentMillis;

    WiFiClient client;
    HTTPClient http;

    //GET directly from the URL (Dont use HTTPS) Modify the JSON Value as required!
    if (http.begin(client, "http://coronavirus-tracker-api.herokuapp.com/v2/locations?country_code=AU&province=Queensland"))
    {
      //http.begin("http://coronavirus-19-api.herokuapp.com/countries/Australia");

      int httpCode = http.GET();

      if (httpCode > 0)
      {
        String payload = http.getString();

        char *JsonArray = (char *)malloc(payload.length() + 1);
        if (!JsonArray)
          JSON_LOG("Uh Oh!");

        payload.toCharArray(JsonArray, payload.length() + 1);

        JSON_LOG(JsonArray);

        if (json_validate(JsonArray))
        {
          int confirmed = (int)get_json_value(JsonArray, "confirmed", INT);
          int deaths = (int)get_json_value(JsonArray, "deaths", INT);
          // int recovered = (int)get_json_value(JsonArray, "recovered", INT);

          JSON_LOG(confirmed);
          JSON_LOG(deaths);
          // JSON_LOG(recovered);

          display.clearDisplay();
          display.setTextSize(1);
          display.setTextColor(WHITE);
          display.setCursor(0, 0);
          display.println("Covids Tracker by BwE");
          display.print("Confirmed: ");
          display.println(confirmed);
          display.print("Deaths: ");
          display.println(deaths);
          display.display();
        }
        free(JsonArray);
      }
      else
      {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    }
    else
    {
      Serial.println(F("[HTTP} Unable to connect"));
    }
  }
}
