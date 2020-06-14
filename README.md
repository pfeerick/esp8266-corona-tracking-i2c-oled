# ESP8266 Corona Tracking with a I2C OLED

Has been modified to:
* accomodate proper GET URL's 
* use a 128x64 I2C OLED (although is still compatiable with 128x32 display)
* work with PlatformIO
* use the native ESP8266Wifi library
* use a non-tracked `WifiCredentials.h` 

## To Use:

 1. Rename `include\WifiCredentials_template.h` to `include\WifiCredentials.h` and set your own WiFI SSID and password:

   * char ssid[32] = "yourssid";
   * char password[64] = "yourpass";

 2. Modify the HTTP (NOT HTTPS) GET URL to set your country / region:

    Examples:
    * `http.begin("http://coronavirus-tracker-api.herokuapp.com/v2/locations?country_code=AU&province=Queensland");`
    * `http.begin("http://coronavirus-19-api.herokuapp.com/countries/Australia");`

 3. Modify the JSON Values to match the output (Browse directly or look at serial monitor):

    Examples:
    * `int confirmed = (int)get_json_value(JsonArray, "confirmed", INT);`
    * `int deaths = (int)get_json_value(JsonArray, "deaths", INT);`

![Picture](https://i.imgur.com/uCq3Y1m.jpg)
