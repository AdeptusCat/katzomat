# KatzoApp - IoT pet feeding device
Control your pets feeding schedule while you are away/sleeping/working... just take your mind off of it.<br/>
This repository is for the mobile App. Check out the repository of the Katzomat, which is the hardware part of the project: [Katzomat](https://github.com/AdeptusCat/katzomat "Katzomat repository")

### How does the App work?
The App connects to a MQTT broker that holds all the relevant information about the Katzomat.
e.g. feeding schedule, status (online/offline), latest image of the feeding bowl, history of feeding.

### What User Data is saved, and where?
Locally stored data:    feeding schedule, last image, Katzomat settings, user credentials.<br/>
Online stored data:     feeding schedule, last image, Katzomat settings.

### Is my data save?
The network transmission is ssl encrypted. Local data on the mobile device is encrypted using the pyAesCrypt library.<br/>
Its certainly possible to setup your own MQTT broker, if you don't trust others with your data.

### Is this software cross-platform?
Yes. It is written in python and kivy/kivyMD. Its possible to compile with buildozer for Android, iOS and also possible to compile for desktop usage.

### How to use App?
The App tries to connect to the MQTT broker on startup. Access the main screens "Feeder", 'Timetable' and 'Log' from the Tab bar. Settings can be accessed via the the menu ribbon top left.<br/>
Request a still image from the Katzomat or feed the pet directly.<br/>
![Alt text](assets/feeder.png?raw=true "Feed Screen")
<br/>
Account Settings: If you use the app the first time or want to change user data you can change the MQTT domain, username and password here. <br/>
![Alt text](assets/acc_settings.png?raw=true "Account Settings")
<br/>
Timetable: Overview of the feeding schedule. To setup a feeding timetable, you can select either a recurring or once occuring feeding schedule.<br/>
![Alt text](assets/timetable.png?raw=true "Timetable")
<br/>
Device Settings: Here you can adjust things like turning time of the feeding motor.<br/>
![Alt text](assets/dev_settings.png?raw=true "Device Settings")
<br/>
Log: See the feeding history and current time on the device.<br/>
![Alt text](assets/log.png?raw=true "Log")
<br/>
### How do I know it actually works? 
The microcontroller has a camera so you can see if the feeding worked and if your pet did in fact eat anything. Request a still image at any time.

### Do I need a permanent internet connection for this to work?
No. The microcontroller saves the schedule on an SD-Card. The App sends its data to a MQTT broker and when the microcontroller comes online again, it receives all the latest information and vice versa.

### Can this be used for something else, like watering plants?
Yes. You would have to make some changes to how the actual mechanics work, but other than that it should be quit easy to adapt this to do something else.

### What kind of technologies are used?
This is an IoT (Internet of Things) project. So it uses the MQTT protocol to communicate between the clients (e.g. the feeding device and the App) to remote control a device via the Internet.<br/>
The App is written in Python and Kivy/KivyMD. The microcontroller is an ESP32 that is programmed c/c++.

## The Device: Katzomat
The device is a microcontroller, connected to the internet.<br/>
Check out the repository of the microcontroller: [KatzoApp](https://github.com/AdeptusCat/katzoapp "Katzoapp repository")



