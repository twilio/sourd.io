# Twilio-Powered Narrowband IoT Sourdough Starter Monitor

### Like a Fitbit, but for your sourdough starter

A sourdough starter is a little yeast-driven pet that eats flour and water and makes sourdough. More and more people are keeping these delightful critters in their houses, feeding them when they start to look sluggish and baking bread with them every few days.

With Twilio's Narrowband IoT Developer Kit, we can build a little monitor that helps us keep track of our starter's temperature, humidity, and rise.

### Why Narrowband?

Twilio's Narrowband IoT dev kit happens to have some great sensors we can use to get started, and it's also designed for constant, low-power connectivity, which is what I want for this particular product. Cellular connectivity also means we don't have to do any setup or fight with our Wi-Fi routers.

The downside to Twilio Narrowband is that it only works in the US. If you're hoping to monitor your bread from outside the US, stay tuned! I'm planning another bread monitor in the future that connects with Super SIM, which is available everywhere.

### What you'll need

_The Basics: What you need to get a proof-of-concept up and running_
- 1 x [Twilio Developer Kit for Narrowband IoT](https://www.twilio.com/docs/wireless/quickstart/alfa-developer-kit-mqtt?campaign=nbsdm01). Dev kits are currently ordered through the console. To order, first add your payment info to your Twilio account, then go to [the Wireless order page](https://www.twilio.com/console/wireless/orders) and fill out the form.
- The United States. Twilio Narrowband currently works in the US.
- The (Arduino IDE)[https://www.arduino.cc/en/main/software]
- An MQTT broker of choice. This build has been tested with Adafruit IO and Thingspeak, and has info on how to set that up in the `/nbiot-sourdough-monitor` folder of this repo.

_Enclosures: Making it pretty_
- A way to print the .stl files found in the `/enclosure` folder of this repo
- [# of screws and type]
- A ball jar with canning lid (the one where the center is removeable)

### Putting it together

_Bringing your Narrowband Kit online_

Rather than replicating the tutorial here, I'm going to suggest that you follow the steps in the [Narrowband Kit Quickstart](https://www.twilio.com/docs/wireless/quickstart/alfa-developer-kit-mqtt?campaign=nbsdm01) to get your Twilio SIM registered and your libraries installed. Check that the code compiles. You can even try uploading it and resetting the board to see if you can get it to come online. Once your board is making happy blue lights, then come back to this tutorial.

_Hooking up your hardware_

This kit comes with several sensors. We're going to use the Ultrasonic Distance sensor and the Temperature/Humidity sensor. Plug the distance sensor into [PORT] and the temp/humidity sensor into [PORT].

_Adding custom bread firmware_

Download or clone this repo and open the folder `nbiot-sourdough-monitor` in Arduino. Add your MQTT info to the file `mqttcreds-edit-and-rename.h` and then-- you guessed it-- rename it to `mqttcreds.h`

To fill out this file, you'll probably need to actually set up a broker. Instructions in the file tell you what you need to set up Adafruit IO or Thingspeak to work with this monitor, since those have an easy way to create public visualizations so you can share your bread's health with friends. Feel free to use a different broker of your choice as well!

Put your board in bootloader mode by holding the bootloader button and tapping the reset button. Compile and upload `nbiot-sourdough-monitor.ino`.

If all is working, then your board should come online with a happy blue light. Open the Serial Monitor to see if the board is sending commands.

_See your data_

Navigate to your chosen MQTT broker and check out the commands coming into your channel or dashboard!

_Assembling the enclosure_

Print the .stl files in the `/enclosure` folder. Assemble like this:

[Image]

Screw the bread monitor onto your jar and put your sourdough starter in the jar!

_If you don't have a sourdough starter_

There are plenty of excellent ways for you to get a sourdough starter. Here are some favorite tutorials of people at Twilio who are now baking bread regularly:
- LINKS