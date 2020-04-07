# ESP8266 Corona Tracking with a I2C OLED
<p>

Modified to accomidate proper GET URL's and the 0.9" 128x32 I2C OLED<br>
Though I have also added my own take on a 16x2 I2C LCD version!


 1. Change your wifi information from **WifiConnect.h** file

   * char ssid[32] = "yourssid";
   * char password[64] = "yourpass";

 2. Modify the HTTP (NOT HTTPS) GET URL
 
    Examples:
    * http.begin("http://coronavirus-tracker-api.herokuapp.com/v2/locations?country_code=AU&province=Queensland");
    * http.begin("http://coronavirus-19-api.herokuapp.com/countries/Australia");
    
 3. Modify the JSON Values to match the output (Browse directly or look at serial monitor)
 
    Examples:
    * int confirmed = (int)get_json_value(JsonArray, "confirmed", INT);
    * int deaths = (int)get_json_value(JsonArray, "deaths", INT);
    * int recovered = (int)get_json_value(JsonArray, "recovered", INT);
   


![GitHub Logo](https://i.imgur.com/uCq3Y1m.jpg)  
   
  
  


