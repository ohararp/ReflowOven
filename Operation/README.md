This is the initial starup screen after power up.

![Logo Title] (https://raw.github.com/ohararp/ReflowOven/master/Operation/Images/0_Logo.JPG "Initial Logo")

#Start a Reflow Profile:
By default this reflow oven is set to run out of the box.  A reflow procedure can be started by simply pressing and holding the "Start" button for 6 seconds.  During this time a succession of 6 "beeps" from the piezo buzzer will be heard. The oven will now cycle through the Preheat, Soak, and Reflow profiles to properly reflow your pcbs.  The buzzer will sound when the cool cycle begins. It is suggested that the oven door be opened to allow the reflowed assembly to cool more quickly.  A fan can also be used to increase airflow and the cooling of the reflowed pcb.


#Changing Reflow Profile and PID settings:
By default the reflow controller is set with PID parameters that we have found to be nearly optimal for a leaded profile. There should not need to be any reason that you should need to change the default parameters.  However, if you need to change the profile or the settings you can do this following the steps below.  These settings can be changed without modifying the firmware on the Arduino compatible controller (Atmega 32U4).

By pressing the menu button it is possible to cycle through the oven settings.  Unlike previous code efforts these menu options can be changed to allow for different reflow profiles for leaded and non-leaded solder. **Settings are stored in EEPROM after completely cycling through all of the settings**.  The default settings are used for the Black and Decker Infrawave ovens.  You may need to adjust these settings based on your particular application.  The arrow indicates the setting that is to be changed.  The setting can be changed by pressing the plus and minus buttons on the control pad.

![Logo Title] (https://raw.github.com/ohararp/ReflowOven/master/Operation/Images/1_TempMenu.JPG "Temperature Menu")

![Logo Title] (https://raw.github.com/ohararp/ReflowOven/master/Operation/Images/2_PreheatMenu.JPG "Preheat Menu")

![Logo Title] (https://raw.github.com/ohararp/ReflowOven/master/Operation/Images/3_SoakMenu.JPG "Soak Menu")

![Logo Title] (https://raw.github.com/ohararp/ReflowOven/master/Operation/Images/4_ReflowMenu.JPG "Reflow Menu")

![Logo Title] (https://raw.github.com/ohararp/ReflowOven/master/Operation/Images/5_SettingsStored.JPG "Settings Stored")


