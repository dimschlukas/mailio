#include <Arduino.h>
#include <wifi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <configuration.h>

WiFiClientSecure secured_client;
UniversalTelegramBot bot(TELEGRAM_BOT_TOKEN, secured_client);

void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    Serial.println("Wakeup caused by external signal using RTC_IO");
    break;
  case ESP_SLEEP_WAKEUP_EXT1:
    Serial.println("Wakeup caused by external signal using RTC_CNTL");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    Serial.println("Wakeup caused by timer");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    Serial.println("Wakeup caused by touchpad");
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    Serial.println("Wakeup caused by ULP program");
    break;
  default:
    Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
    break;
  }
}

void go_to_sleep()
{
  Serial.print("Going to sleep");
  gpio_pulldown_en(EXT_WAKEUP_PIN);
  esp_sleep_enable_ext0_wakeup(EXT_WAKEUP_PIN, 1);
  esp_deep_sleep_start();
  delay(100);
}

void setup()
{
  pinMode(5, OUTPUT);
  int ADC_BAT_VALUE;
  float voltage_value = 0;
  Serial.begin(115200); // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Start WiFi connecting process
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  { // Wait for the WiFi to connect.
#ifdef LED_PIN
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
#endif
    Serial.print(++i);
    Serial.print(' ');
    if (i > 30)
    {
#ifdef LED_PIN
      for (int i = 0; i <= 3; i++)
      { // Slowly blink onboard LED (could not connect)
        digitalWrite(LED_PIN, HIGH);
        delay(200);
        digitalWrite(LED_PIN, LOW);
        delay(1000);
      }
#endif
      go_to_sleep();
    }
  }
#ifdef LED_PIN
  for (int i = 0; i <= 3; i++)
  { // blink blue onboard LED fast (WiFi connected)
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(300);
  }
#endif

  Serial.println('\n');
  Serial.println("Connection established!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP()); // Send the IP address of the ESP8266 to the computer

  delay(1000);
  print_wakeup_reason();

  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0)
  {
    bot.sendMessage(TELEGRAM_CHAT_ID, TELEGRAM_NEW_MAIL_MESSAGE); // send Telegram message to channel "Chat_id"
  }

  String voltage_string;
  ulong prev_millis = millis();
  while (millis() < prev_millis + 60000)
  { // Loops for 60 seconds and checks battery level
#ifdef ADC_BAT_PIN
    ADC_BAT_VALUE = analogRead(ADC_BAT_PIN);
    voltage_value = (ADC_BAT_VALUE * 3.3 * 2 * 1.08) / (4095);
    if (voltage_value < 3.6)
    {
      voltage_string = ("Battery charge is low: " + String(voltage_value) + " V");
#ifdef TELEGRAM_ADMIN_CHAT_ID
      bot.sendMessage(TELEGRAM_ADMIN_CHAT_ID, voltage_string);
#else
      bot.sendMessage(TELEGRAM_CHAT_ID, voltage_string);
#endif
      break;
    }
#endif
  }

#if defined(SEND_BAT_READINGS_EVERYTIME) && defined(ADC_BAT_PIN)
#ifdef TELEGRAM_ADMIN_CHAT_ID
  bot.sendMessage(TELEGRAM_ADMIN_CHAT_ID, voltage_string);
#else
  bot.sendMessage(TELEGRAM_CHAT_ID, voltage_string);
#endif
#endif

  go_to_sleep();
}

void loop()
{
}