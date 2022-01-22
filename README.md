# Mailio
This is a simple arduino script that notifies if some new mail is delivered to the Mailbox. It works by detecting if the mailbox door/hatch is opened. Then a simple microcontroller turns on, connects to a WiFi and sends a message to a desired Telegram chat or channel. it's made to run really power efficient. Thats achived due to deep-sleep. 99% of the day the microcontroller is running in deep-sleep. And only about 60 seconds, during sending and connecting to a WiFi the microcontroller is running. Until it goes back to sleep, waiting for the next mail.

---

## Features
- Power efficient (99% of the day in deep-sleep)
- Can be run from a 1S lipo and/or off solar power for ages
- Sends Telegram messages to specifig chats/channels
- Sends Telegram battery information to admin chats/channels (optional)
- configurable GPIO or onboard LED for indicating WiFi status (optional)

## You need
### Hardware
- [Lolin D32 microcontroller](https://www.wemos.cc/en/latest/d32/d32.html)[^1] (Or use your own microcontroller. PIN mapping may differ.)[^2] (Would be happy if anyone attributes tested hardware.)
- NO-switch for detecting opening of the mailbox door/hatch. I used [this](https://www.farnell.com/datasheets/2791160.pdf) reed-switch.
- 1S lipo with about 300-1000mA
- 5V solar panel
- Breadboard, cables, enclosure and maybe some soldering stuff

### Software and additional libraries
- Arduino IDE or VSCode with PlatformIO <-- prefered methode
- [witnessmenow/UniversalTelegramBot](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot)

## Installation
- Clone repository 
```
git clone https://github.com/dimschlukas/mailio.git
```
- Check for missing libraries
  - [UniversalTelegramBot](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot) needs to be installed, if PlatformIO does not install it automatically
- Edit [include/configuration.h](include/configuration.h)
  - You need a Telegram bot API token. Follow [this guide](https://core.telegram.org/bots#3-how-do-i-create-a-bot) to acquire a token. And use an IDBot to get your chats/channels id.
  - Comment out unwanted optional features
- Flash microcontroller

[^1]: Why a ESP32 for this simple task? The Lolin D32 has a built in battery charger and a JST connector to connet to a battery. And it was the board that was laying around at home.
[^2]: You need RTC GPIO pins in order to use deep-sleep on your ESP32.
