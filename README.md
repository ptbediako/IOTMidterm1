![image](https://github.com/user-attachments/assets/879d1a2c-634a-487f-ab07-370a8b28d94d)# IOTMidterm1
# IoT Midterm 1 Smart Room Controller
### Authored by Phylicia Bediako
### Hackster: 
https://www.hackster.io/ptbediako/midterm-1-smart-room-controller-73c56e

## Snack Trax
Snack Trax is a combination environmental sensor and food scale designed to help you keep the foods in your pantry fresh and fully stocked. Using a temperature, humidity, and weight tracking system, Snack Trax displays live readings of storage conditions and communicates with wireless devices around the room to let you know if you're running low on snacks or if your snack shelf is too warm, cold, dry, or humid.

## Motivation
The IOT classroom has a great supply of snacks that we can request to have restocked, but sometimes things run out before anyone notices. Plus, storing certain types of snacks in the wrong environmental conditions can shorten their shelf life or make them taste less fresh. This device helps maximize the classroom snack stash by giving the whole room a heads up if we need to restock or check on the temperature or humidity conditions in the snack pantry.

## Main components
* Particle Photon2
* Food scale: 1kg Load Cell with HX711 Load Cell Amp to weigh snacks
* Environmental sensor: BME280 for measuring temperature and humidity
* Display: OLED for displaying temperature, humidity, and weight readings
* Controls: RGB Encoder for controlling brightness of hue lights
* Status lights: Neopixels for displaying whether readings are within bounds
* Smart room devices: Hue lights and Wemo outlets for warning when conditions are out of ideal bounds

## Parts
* Wire
* Acrylic sheets
* Wood (~1/8 in)
* 3D printed tray

## Equipment
* Wire cutters
* Epilog Helix Lazer
* Miter saw
* 1000g calibration weights
* Bambu Lab X1 Carbon 3D Printer

## Features
* Dynamic OLED display message for detailed readings of snack and pantry status
* Control panel lights to easily see if any pantry or stock conditions are out of bounds
* Smart lights and outlets for whole room warnings of pantry and stock status

## How it works and what it does
Snack Trax has two pieces: 1) a sensor and control panel and 2) a food scale. The sensor and control panel features an OLED that displays humidty, temperature, and weight readings as well as neopixels that show users whether readings are within an ideal range and a dial that can modify the brightness of the status hue lights to meet the user's preference. Hue and neopixel lights flash green for readings within ideal bounds or red for readings that are out of bounds. Different wemo outlets turn on when temperature and humidity readings are out of bounds to warn users to check on the pantry.
