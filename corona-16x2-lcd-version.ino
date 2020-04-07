#include <ESP8266HTTPClient.h> 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "json_parser.h"
#include "WifiConnect.h"

#define s2ms(second) (second*1000)
unsigned long long prev_millis(0);

//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

int interval = s2ms(60);
unsigned long long PreviousMillis = 0;
unsigned long long CurrentMillis = interval;
bool bFirstKickMillis = false;

extern bool bGotIpFlag;

void setup(void)
{ 
  Serial.begin(115200);

  lcd.begin(16,2); //Defining 16 columns and 2 rows of lcd display
  lcd.backlight(); //To Power ON the back light
  
  lcd.setCursor(0,0);//Line 0, Character 0
  lcd.print("Covids Tracker");
  
  lcd.setCursor(0,1); //Line 1, Character 0
  lcd.print("Wait or Reset");
  
  lcd.display(); 
  
  JSON_LOG("Connecting...");
  
  vConnWifiNetworkViaSdk();
}

void loop()
{
  if(bGotIpFlag) bGotIp();
  
  if(bFirstKickMillis) CurrentMillis = millis();
  
  if (!bGotIpFlag && CurrentMillis - PreviousMillis >= interval) 
  {
    if(!bFirstKickMillis) CurrentMillis = 0;
    
    bFirstKickMillis = true;
    
    PreviousMillis = CurrentMillis;
    
    HTTPClient http; 
    
    //GET directly from the URL (Dont use HTTPS) Modify the JSON Value as required!
    http.begin("http://coronavirus-tracker-api.herokuapp.com/v2/locations?country_code=AU&province=Queensland");
    //http.begin("http://coronavirus-19-api.herokuapp.com/countries/Australia");
    
    int httpCode = http.GET(); 
  
    if(httpCode > 0) 
    {
      String payload = http.getString();
       
      char* JsonArray = (char *)malloc(payload.length() + 1);
      if (!JsonArray) JSON_LOG("upssss fuck");
      
      payload.toCharArray(JsonArray, payload.length() + 1);
      
      JSON_LOG(JsonArray);
      
      if (json_validate(JsonArray))
      {
        int confirmed = (int)get_json_value(JsonArray, "confirmed", INT);
        int deaths = (int)get_json_value(JsonArray, "deaths", INT);
        int recovered = (int)get_json_value(JsonArray, "recovered", INT);
      
        JSON_LOG(confirmed);
        JSON_LOG(deaths);
        JSON_LOG(recovered);
        
        clearLCDLine(1);
        clearLCDLine(2);
        
        lcd.setCursor(0, 0);
        lcd.print("Confirmed: ");
        lcd.setCursor(11, 0);
        lcd.print(confirmed);
        lcd.setCursor(0, 1);
        lcd.print("Deaths: ");
        lcd.setCursor(8, 1);
        lcd.print(deaths);
        lcd.display(); 
      }
      
      free(JsonArray);
    }
    
    http.end(); 
  }
}

void clearLCDLine(int line)
{               
        lcd.setCursor(0,line);
        for(int n = 0; n < 16; n++) // 20 indicates symbols in line. For 2x16 LCD write - 16
        {
                lcd.print(" ");
        }
}
