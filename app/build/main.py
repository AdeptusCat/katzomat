__version__ = "0.1.1"

import cProfile

from kivy.lang import Builder
from kivy.properties import StringProperty, NumericProperty, ObjectProperty
from kivymd.app import MDApp

from kivymd.color_definitions import colors
from kivy.utils import get_color_from_hex
from kivy.metrics import dp
from kivymd.uix.menu import MDDropdownMenu
from kivymd.uix.floatlayout import MDFloatLayout
from kivymd.uix.tab import MDTabsBase
from kivymd.uix.behaviors import RoundedRectangularElevationBehavior
from kivymd.uix.boxlayout import MDBoxLayout
from kivymd.uix.card import MDCard
from kivymd.uix.snackbar import BaseSnackbar
from kivymd.uix.button import MDFlatButton
from kivy.core.window import Window
from kivy.animation import Animation
from kivy.clock import Clock
from kivy.clock import mainthread
from kivy.utils import platform
from kivymd.uix.dialog import MDDialog
#from kivymd.uix.picker import MDTimePicker
#from kivymd.uix.picker import MDDatePicker
from picker import MDTimePicker
from picker import MDDatePicker
from kivymd.uix.list import IRightBodyTouch, OneLineListItem, OneLineAvatarIconListItem
from kivymd.icon_definitions import md_icons
from kivymd.uix.selectioncontrol import MDCheckbox
from kivymd.uix.button import MDFloatingActionButtonSpeedDial

import pyAesCrypt
import io
from os import stat, remove

import sys
from PIL import Image as PILImage  # Name conflict with kivy.uix.image

import paho.mqtt.client as mqtt
import ssl

import base64
from io import BytesIO
import glob

from _thread import start_new_thread
import time, threading
import datetime

import json 
import socket

import locale

import os
from os.path import exists

client = None
flag_connected = 0
image_array = []


if platform == "android":
     from android.permissions import request_permissions, Permission
     request_permissions([Permission.INTERNET])

class MD3Card(MDCard, RoundedRectangularElevationBehavior):
    '''Implements a material design v3 card.'''
    text = StringProperty()

class MD4Card(MDCard, RoundedRectangularElevationBehavior):
    '''Implements a material design v3 card.'''
    text = StringProperty()

class ContentNavigationDrawer(MDBoxLayout):
    pass

class Tab(MDFloatLayout, MDTabsBase):
    pass

class CustomSnackbar(BaseSnackbar):
    text = StringProperty(None)
    icon = StringProperty(None)
    font_size = NumericProperty("15sp")

class MyCheckbox(IRightBodyTouch, MDCheckbox):
    pass

class DrawerList():
    def set_color_item(self, instance_item):
        '''Called when tap on a menu item.'''

        # Set the color of the icon and text for the menu item.
        for item in self.children:
            if item.text_color == self.theme_cls.primary_color:
                item.text_color = self.theme_cls.text_color
                break
        instance_item.text_color = self.theme_cls.primary_color


class ListItemWithButton(OneLineListItem):
    timetable_icon = StringProperty("android")



class ItemConfirm(OneLineAvatarIconListItem):
    divider = None
    is_checked = False
    def set_icon(self, instance_check):
        if instance_check.active == True:
            instance_check.active = False
            self.is_checked = False
        else:
            instance_check.active = True
            self.is_checked = True
        #instance_check.active = True
        #check_list = instance_check.get_widgets(instance_check.group)
        #for check in check_list:
        #    if check != instance_check:
        #        check.active = False

class Thread(threading.Thread):
    encryption_key = "123kjlaofs9ypöokmnafbvk"
    bufferSize = 64 * 1024
    service_id = "KatzomApp"
    user_key = "user_key"
    password_key = "password_key"
    domain_key = "domain_key"

    app = None

    def __init__(self,app):
        self.app = app # kivy object
        threading.Thread.__init__(self)

    def connect_to_broker(self):
        #self.app.root.ids.spinner.active = True
        if self.check_internet_connection():
            #self.login() 
            if self.app.settings_dict["domain"] == "" or self.app.settings_dict["user_name"] == "" or self.app.settings_dict["password"] == "":
                self.app.login_failed()             
            else:
                print("trying to log in")
                self.login() 
        else:
            self.app.show_no_internet_dialog()
            if not self.app.timetable_loaded:
                self.load_timetable()
        #self.app.root.ids.spinner.active = False

    def load_settings(self):
        try:
            # get encrypted file size
            encFileSize = stat("acc.aes").st_size
            # initialize decrypted binary stream
            fDec = io.BytesIO()
            # decrypt
            with open("acc.aes", "rb") as fIn:
                pyAesCrypt.decryptStream(fIn, fDec, self.encryption_key, self.bufferSize, encFileSize)
            text = fDec.getvalue().decode()
            arr = text.split("\n")
            domain = arr[0]
            user_name = arr[1]
            password = arr[2]
            # get encrypted file size
            #encFileSize = stat("u.aes").st_size
            # initialize decrypted binary stream
            #fDec = io.BytesIO()
            # decrypt
            #with open("u.aes", "rb") as fIn:
            #    pyAesCrypt.decryptStream(fIn, fDec, self.encryption_key, self.bufferSize, encFileSize)
            #user_name = fDec.getvalue().decode()

            # get encrypted file size
            #encFileSize = stat("p.aes").st_size
            # initialize decrypted binary stream
            #fDec = io.BytesIO()
            # decrypt
            #with open("p.aes", "rb") as fIn:
            #    pyAesCrypt.decryptStream(fIn, fDec, self.encryption_key, self.bufferSize, encFileSize)
            #password = fDec.getvalue().decode()

            self.app.settings_dict["domain"] = domain
            self.app.settings_dict["user_name"] = user_name
            self.app.settings_dict["password"] = password
            self.app.root.ids.domain.text = self.app.settings_dict["domain"]
            self.app.root.ids.user_name.text = self.app.settings_dict["user_name"]
            self.app.root.ids.password.text = self.app.settings_dict["password"]

        except Exception as e:
            print(e)
            print("Failed retriving user data.")   
            #self.app.show_snackbar("keyfile_error")  
  
        try:
            # get encrypted file size
            encFileSize = stat("s.aes").st_size
            # initialize decrypted binary stream
            fDec = io.BytesIO()
            # decrypt
            with open("s.aes", "rb") as fIn:
                pyAesCrypt.decryptStream(fIn, fDec, self.encryption_key, self.bufferSize, encFileSize)
            rotation_time = fDec.getvalue().decode()

            self.app.settings_dict["rotation_time"] = rotation_time
            self.app.root.ids.rotation_time.text = self.app.settings_dict["rotation_time"]

        except Exception as e:
            print(e)
            print("Failed retriving device settings.")   
            #self.app.show_snackbar("keyfile_error")  
             
        


    def get_locale_language(self):
        # use user's default settings
        locale.setlocale(locale.LC_ALL, '')
        # use current setting
        locale.setlocale(locale.LC_ALL, None)
        # get current locale
        loc = locale.getlocale()
        self.app.locale_language = loc
        

    def get_timezone_offset(self):
        timezone_offset = time.timezone if (time.localtime().tm_isdst == 0) else time.altzone
        timezone_offset = timezone_offset / 60 / 60 * -1
        self.app.timezone_offset = timezone_offset

    def check_internet_connection(self):
            """ Returns True if there's a connection """

            IP_ADDRESS_LIST = [
                "1.1.1.1",  # Cloudflare
                "1.0.0.1",
                "8.8.8.8",  # Google DNS
                "8.8.4.4",
                "208.67.222.222",  # Open DNS
                "208.67.220.220"
            ]

            port = 53
            timeout = 3

            for host in IP_ADDRESS_LIST:
                try:
                    socket.setdefaulttimeout(timeout)
                    socket.socket(socket.AF_INET, socket.SOCK_STREAM).connect((host, port))
                    return True
                except socket.error:
                    pass
            else:
                #logger.exception("No internet connection")
                return False 

    def login(self):
        try:
            global client
            client_id = "Katzomat:" + self.app.settings_dict["user_name"] + "_" + str(int(datetime.datetime.timestamp(datetime.datetime.now())))
            client = mqtt.Client(client_id=client_id, clean_session=True, userdata=None, protocol=mqtt.MQTTv311, transport="tcp")
            client.tls_set(ca_certs="certs/isrgrootx1.pem", certfile=None, keyfile=None, cert_reqs=ssl.CERT_REQUIRED,
                tls_version=ssl.PROTOCOL_TLS, ciphers=None)
            client.username_pw_set(self.app.settings_dict["user_name"], self.app.settings_dict["password"])
            client.on_connect = self.on_connect
            client.on_disconnect = self.on_disconnect
            client.on_message = self.on_message
            client.on_publish = self.on_publish
            client.message_callback_add("katzomat/"+ self.app.settings_dict["user_name"] +"/status", self.status_callback)
            client.message_callback_add("katzomat/"+ self.app.settings_dict["user_name"] +"/status/verbose", self.status_verbose_callback)
            client.message_callback_add("katzomat/"+ self.app.settings_dict["user_name"] +"/images/0", self.image0_callback)
            client.message_callback_add("katzomat/"+ self.app.settings_dict["user_name"] +"/images/1", self.image1_callback)
            client.message_callback_add("katzomat/"+ self.app.settings_dict["user_name"] +"/images/2", self.image2_callback)
            client.message_callback_add("katzomat/"+ self.app.settings_dict["user_name"] +"/images/3", self.image3_callback)
            client.message_callback_add("katzomat/"+ self.app.settings_dict["user_name"] +"/feed", self.feed_callback)
            client.message_callback_add("katzomat/"+ self.app.settings_dict["user_name"] +"/timetable", self.timetable_callback)
            client.message_callback_add("katzomat/"+ self.app.settings_dict["user_name"] +"/settings", self.device_settings_callback)
            #client.reconnect_delay_set(min_delay=1, max_delay=5)
            client.connect_async(self.app.settings_dict["domain"], 8883, keepalive=5)

            client.loop_start()

            threading.Timer(5, self.app.login_failed).start()
        except Exception as e:
            print(e)
            print("Did you enter the proper domain? Are you connected to the internet?")
            self.app.show_snackbar("error")


    def save_account_settings(self):
        #self.app.root.ids.spinner.active = True
        #if self.app.root.ids.screen_manager.current == "screen_account_settings":
        #    self.app.root.ids.screen_manager.current = "screen_feeder"

        self.connect_to_broker()
        #self.app.root.ids.spinner.active = False
        settings_string = self.app.root.ids.domain.text + "\n" + self.app.root.ids.user_name.text + "\n" + self.app.root.ids.password.text
        fIn = io.BytesIO(settings_string.encode())
        with open("acc.aes", "wb") as fOut:
            pyAesCrypt.encryptStream(fIn, fOut, self.encryption_key, self.bufferSize)   
        #fIn = io.BytesIO(self.app.root.ids.user_name.text.encode())
        #with open("u.aes", "wb") as fOut:
        #    pyAesCrypt.encryptStream(fIn, fOut, self.encryption_key, self.bufferSize)
        #fIn = io.BytesIO(self.app.root.ids.password.text.encode())
        #with open("p.aes", "wb") as fOut:
        #    pyAesCrypt.encryptStream(fIn, fOut, self.encryption_key, self.bufferSize)   



    def save_device_settings(self):
        global client
        global flag_connected
        if flag_connected == 0:
            self.app.show_snackbar("disconnected")
            print("cannot publish, not connected.")
            return
        fIn = io.BytesIO(self.app.root.ids.rotation_time.text.encode())
        with open("s.aes", "wb") as fOut:
            pyAesCrypt.encryptStream(fIn, fOut, self.encryption_key, self.bufferSize)   
        rotation_time = self.app.root.ids.rotation_time.text
        self.app.settings_dict["roation_time"] = rotation_time
        client.publish("katzomat/"+ self.app.settings_dict["user_name"] +"/settings",rotation_time,qos=1,retain=True)


    def save_timetable(self):
        current_time = int(time.time()) + 60*60*self.app.timezone_offset
        dic = {
            "timestamp" : current_time,
            "values" : []        
        }
        for child in self.app.root.ids.scroll.children:
            arr = child.text.split(", ")
            item_dic = {}
            item_dic["weekdays"] = []
            index = 0
            for day in arr:
                if day == "Everyday":
                    item_dic["weekdays"].append(0)
                elif day == "Sun":
                    item_dic["weekdays"].append(1)
                elif day == "Mon":
                    item_dic["weekdays"].append(2)
                elif day == "Tue":
                    item_dic["weekdays"].append(3)
                elif day == "Wed":
                    item_dic["weekdays"].append(4)
                elif day == "Thu":
                    item_dic["weekdays"].append(5)
                elif day == "Fri":
                    item_dic["weekdays"].append(6)
                elif day == "Sat":
                    item_dic["weekdays"].append(7)
                else:
                    break
                index += 1

            if len(item_dic["weekdays"]) == 0:
                date_arr = [int(x) for x in arr[index].split("-")]
                index += 1
                time_arr = [int(x) for x in arr[index].split(":")]
                timestamp_of_once = datetime.datetime.timestamp(datetime.datetime(date_arr[0], date_arr[1], date_arr[2], time_arr[0], time_arr[1], 0))
                #timezone correction
                timestamp_of_once += 60*60*self.app.timezone_offset
                item_dic["timestamp"] = timestamp_of_once
            else:
                time_arr = arr[index].split(":")
                item_dic["hour"] = int(time_arr.pop(0))
                item_dic["minute"] = int(time_arr.pop(0))
            dic["values"].append(item_dic)
        json_result = json.dumps(dic)

        fIn = io.BytesIO(json_result.encode())
        with open("t.aes", "wb") as fOut:
            pyAesCrypt.encryptStream(fIn, fOut, self.encryption_key, self.bufferSize)

        global client
        global flag_connected
        if flag_connected == 0:
            self.app.show_snackbar("disconnected")
            print("cannot publish, not connected.")
            return
        client.publish("katzomat/"+ self.app.settings_dict["user_name"] +"/timetable",json_result,qos=1,retain=True)


    def load_timetable(self):
        if self.app.timetable_received:
            return
        try:
            self.app.clear_listview()
            #self.show_connection_error_dialog()       
            # get encrypted file size
            encFileSize = stat("t.aes").st_size
            # initialize decrypted binary stream
            fDec = io.BytesIO()
            # decrypt
            with open("t.aes", "rb") as fIn:
                pyAesCrypt.decryptStream(fIn, fDec, self.encryption_key, self.bufferSize, encFileSize)
            data = json.loads(fDec.getvalue().decode())
            self.fill_timetable(data)
            self.app.timetable_loaded = True
        except Exception as e:
            print(e)
        


    def fill_timetable(self, data):
        for value in data["values"]:
            recurring = True
            days_count = 0
            self.app.listviewitems += 1
            item_text = ""
            for day in value["weekdays"]:
                days_count += 1
                if day == 0:
                    item_text += "Everyday, "
                elif day == 1:
                    item_text += "Sun, "
                elif day == 2:
                    item_text += "Mon, "
                elif day == 3:
                    item_text += "Tue, "
                elif day == 4:
                    item_text += "Wed, "
                elif day == 5:
                    item_text += "Thu, "
                elif day == 6:
                    item_text += "Fri, "
                elif day == 7:
                    item_text += "Sat, "
            if days_count == 0:
                recurring = False
                date = datetime.datetime.fromtimestamp(value["timestamp"] - 60*60*self.app.timezone_offset)
                hour = date.hour
                minute = date.minute
                year = date.year
                month = date.month
                day = date.day

                item_text += str(year)
                item_text += "-"
                item_text += f"{month:02d}" 
                item_text += "-"
                item_text += f"{day:02d}" 
                item_text += ", "
                item_text += f"{hour:02d}" 
                item_text += ":"
                item_text += f"{minute:02d}" 
                if value["timestamp"] < int(time.time()) + self.app.timezone_offset * 60 * 60:
                    continue
            else:
                hour = value["hour"]
                minute = value["minute"]
                item_text += f"{hour:02d}" + ":" + f"{minute:02d}" 
            self.app.add_listviewitem(item_text, recurring)
            
    def on_connect(self, client, userdata, flags, rc):
        print("Connected with result code "+str(rc))
        global flag_connected
        flag_connected = 1

        # Subscribing in on_connect() means that if we lose the connection and
        # reconnect then subscriptions will be renewed.
        client.subscribe("home/kitchen/lights")
        client.subscribe("katzomat/"+ self.app.settings_dict["user_name"] +"/images/0")
        client.subscribe("katzomat/"+ self.app.settings_dict["user_name"] +"/images/1")
        client.subscribe("katzomat/"+ self.app.settings_dict["user_name"] +"/images/2")
        client.subscribe("katzomat/"+ self.app.settings_dict["user_name"] +"/images/3")
        client.subscribe("katzomat/"+ self.app.settings_dict["user_name"] +"/status/verbose")
        client.subscribe("katzomat/"+ self.app.settings_dict["user_name"] +"/status")
        client.subscribe("katzomat/"+ self.app.settings_dict["user_name"] +"/feed")
        client.subscribe("katzomat/"+ self.app.settings_dict["user_name"] +"/timetable")
        client.subscribe("katzomat/"+ self.app.settings_dict["user_name"] +"/settings")


        if rc == 0:
            #self.show_snackbar("connected")
            self.app.root.ids.login_button.md_bg_color=self.app.theme_cls.primary_dark
            self.app.root.ids.login_button.text="connected"
            if self.app.root.ids.screen_manager.current == "screen_account_settings":
                self.app.switch_screen("screen_overview")
        
        # test
        #alarm = int(time.time()) + 20 #+ 7200
        #client.publish("katzomat/"+ self.app.settings_dict["user_name"] +"/timetable","""{"values":[{"timestamp":""" + str(alarm) + "}]}",qos=1,retain=True)
    

    def on_disconnect(self, client, userdata, rc):
        print("disconnected")
        global flag_connected
        flag_connected = 0
        #client.loop_stop()
        self.app.root.ids.login_button.md_bg_color=get_color_from_hex(colors["Red"]["900"])
        self.app.root.ids.login_button.text="Log in"
        self.app.root.ids.status_label.text="unkown"
        #self.root.ids.status_label.color = get_color_from_hex("#FFFF00")
        #self.show_snackbar("error")
        if self.check_internet_connection():
            pass
            #self.login() 
        else:
            self.app.show_no_internet_dialog()

    def on_publish(self, client, userdata, mid):
        #self.show_snackbar("published")
        print("successully published")



    # The callback for when a PUBLISH message is received from the server.
    def on_message(self, client, userdata, msg):
        #self.show_snackbar("received")
        message = msg.payload.decode("utf-8")
        topic = msg.topic
        print("message received " ,str(msg.payload.decode("utf-8")))
        print("message topic=",msg.topic)
        print("message qos=",msg.qos)
        print("message retain flag=",msg.retain)


    def timetable_callback(self, client, userdata, msg):
        print("timetable callback")
        if self.app.timetable_received:
            return
        self.app.clear_listview()
        message = msg.payload.decode("utf-8")
        topic = msg.topic
        data = json.loads(message)
        self.fill_timetable(data)
        self.app.timetable_received = True
        
        fIn = io.BytesIO(message.encode())
        with open("t.aes", "wb") as fOut:
            pyAesCrypt.encryptStream(fIn, fOut, self.encryption_key, self.bufferSize)

    def status_callback(self, client, userdata, msg):
        message = msg.payload.decode("utf-8")
        print(message)
        self.app.device_status = message
        if message == "online":
            self.app.root.ids.status_label.text = message
            self.app.refresh_status_verbose()
            #self.root.ids.status_label.color = get_color_from_hex("#00FF00")
        elif message == "offline":
            self.app.root.ids.status_label.text = message
            #self.root.ids.status_label.color = get_color_from_hex("#FF0000")

    def status_verbose_callback(self, client, userdata, msg):
        print("status verbose callback")
        message = msg.payload.decode("utf-8")
        data = json.loads(message)
        device_time = dt_object = datetime.datetime.fromtimestamp(data["time"] - self.app.timezone_offset * 60 * 60)

        self.app.clear_feeding_history()
        self.app.add_feeding_history_entry(f"Katzomat time: {device_time}")
        self.app.add_feeding_history_entry(f"The cat was fed at the following dates:")

        i = 1
        for timestamp in data["history"]:
            if timestamp == 0:
                continue
            dt_object = datetime.datetime.fromtimestamp(timestamp - self.app.timezone_offset * 60 * 60) 
            self.app.add_feeding_history_entry(f"{i}:  {dt_object}")
            i += 1



    def image0_callback(self, client, userdata, msg):
        print("got image 0")
        message = msg.payload.decode("utf-8")

        try:
            data = json.loads(message)     
            timestamp = data["timestamp"]
            dt_object = datetime.datetime.fromtimestamp(int(timestamp) - self.app.timezone_offset * 60 * 60)
            self.app.root.ids.image_timestamp.text = str(dt_object)
            imge_base64 = data["value"]

            #with open('image.txt', 'w') as f:
            #    f.write(message)
            img = PILImage.open(BytesIO(base64.b64decode(imge_base64)))
            rgba = img.convert("RGBA")
            rgba.save('image_0.png', 'PNG')
            self.app.root.ids.image.remove_from_cache()
            self.app.root.ids.image.source = 'image_0.png'
            self.app.root.ids.image.reload()
        except Exception as e:
            print(e)


    def image1_callback(self, client, userdata, msg):
        print("got image 1")
        message = msg.payload.decode("utf-8")
        try:
            img = PILImage.open(BytesIO(base64.b64decode(message)))
            #rgba = img.convert("RGBA")
            rgba = img.convert('RGBA', dither=None, palette='WEB')
            rgba.save('gif_image_1.png', 'PNG')
        except Exception as e:
            print(e)

    def image2_callback(self, client, userdata, msg):
        print("got image 2")
        message = msg.payload.decode("utf-8")
        try:
            img = PILImage.open(BytesIO(base64.b64decode(message)))
            #   rgba = img.convert("RGBA")
            rgba = img.convert('RGBA', dither=None, palette='WEB')
            rgba.save('gif_image_2.png', 'PNG')
        except Exception as e:
            print(e)

    def image3_callback(self, client, userdata, msg):
        print("got image 3")
        message = msg.payload.decode("utf-8")
        try:
            #with open('image.txt', 'w') as f:
            #    f.write(message)
            #img = PILImage.open(BytesIO(base64.b64decode(message)))
            #rgba = img.convert("RGBA")
            #rgba.save('image_3.png', 'PNG')
            #self.root.ids.image.remove_from_cache()
            #self.root.ids.image.source = 'image_3.png'
            #self.root.ids.image.reload()
            img = PILImage.open(BytesIO(base64.b64decode(message)))
            rgba = img.convert("RGBA")
            rgba.save('image_0.png', 'PNG')
            self.app.root.ids.image.remove_from_cache()
            self.app.root.ids.image.source = 'image_0.png'
            self.app.root.ids.image.reload()
        except Exception as e:
            print(e)



    def feed_callback(self, client, userdata, msg):
        #self.show_snackbar("received")
        message = msg.payload.decode("utf-8")
        print(message)
        if message == "done":
            print("feeding done")

            #fp_in = "gif_image_*.png"
            #fp_out = "image.gif"
            #frames = []
            #imgs = glob.glob("gif_image_*.png")
            #for i in imgs:
            #    new_frame = PILImage.open(i)
            #    frames.append(new_frame)
            #frames[0].save("image.gif", format="GIF", append_images=frames[1:], save_all=True, duration=200, loop=0)
            #self.root.ids.image.remove_from_cache()
            #self.root.ids.image.source = 'image.gif'
            #self.root.ids.image.reload()
            #self.root.ids.image.anim_delay = 0.25

    
    def device_settings_callback(self, client, userdata, msg):
        message = msg.payload.decode("utf-8")
        self.app.set_device_settings(message)



class TestNavigationDrawer(MDApp):
    #client = mqtt.Client(client_id="app", clean_session=True, userdata=None, protocol=mqtt.MQTTv311, transport="tcp")

    encryption_key = "123kjlaofs9ypöokmnafbvk"
    bufferSize = 64 * 1024
    service_id = "KatzomApp"
    user_key = "user_key"
    password_key = "password_key"
    domain_key = "domain_key"

    locale_language = []

    date = datetime.date(2011, 1, 1)
    time = datetime.time(10, 23)
    datetime = datetime.datetime.combine(date, time)   
    timestamp = 0

    listviewitem_instance = ""
    listviewitem_once = False
    listviewitems = 0
    listviewitem_text = ""

    timezone_offset = 0.0

    device_status = "offline"

    timetable_received = False
    timetable_loaded = False
    timetable_json = "{}"

    image_screen = "Image"
    overview_screen = "Log"
    feed_screen = "Feeder"
    timetable_screen = "Timetable"

    settings_dict = {"domain": "", "user_name": "", "password": ""}

    thread_node = None

    item_time = ""
    item_date = ""
    item_weekday = "Everyday"


    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.snackbar = None
        self.dialog = None



    def build(self):
        self.theme_cls.primary_palette = "Gray"
        #self.theme_cls.primary_palette = "Green"
        #self.theme_cls.primary_palette = "LightBlue"
        #self.theme_cls.primary_palette = "Lime"
        self.theme_cls.primary_hue = "900" 
        self.theme_cls.theme_style = "Dark"  # "Dark" "Light"
        self.screen = Builder.load_file("main.kv")

        menu_items = [
            {
                "text": f"Item {i}",
                "viewclass": "OneLineListItem",
                "on_release": lambda x=f"Item {i}": self.menu_callback(x),
                "height":dp(54),
            } for i in range(3)
        ]
        #self.menu = MDDropdownMenu(
        #    caller=self.screen.ids.menu_button,
        #    items=menu_items,
        #    width_mult=2,
        #    #max_height=menu_items[0]["height"] * 5,
        #    #max_height=10,
        #)
        #print(menu_items[0])
        return self.screen

    @mainthread
    def save_account_settings(self):
        self.settings_dict = {"domain": self.root.ids.domain.text, "user_name": self.root.ids.user_name.text, "password": self.root.ids.password.text}
        left_status_label.text = self.root.ids.user_name.text
        if self.root.ids.screen_manager.current == "screen_account_settings":
            self.root.ids.screen_manager.current = "screen_feeder"
            self.root.ids.tabs.carousel.index = 0
        threading.Timer(0.5, self.delayed_save_account_settings).start()
        #start_new_thread(self.save_account_settings,())
        #thread_node = Thread(self) # Create a thread
        #thread_node.start() # Start the thread 
        #self.thread_node.save_account_settings()

    def delayed_save_account_settings(self):
        #thread_node = Thread(self) # Create a thread
        #thread_node.start() # Start the thread 
        self.thread_node.save_account_settings()


    def save_device_settings(self):
        self.thread_node.save_device_settings()



    @mainthread
    def set_device_settings(self, time_value):
        self.root.ids.rotation_time.text = time_value
        
    def clear(self):
        self.root.ids.domain.text = ""
        self.root.ids.user_name.text = ""
        self.root.ids.password.text = ""
    

    def on_stop(self):
        pass
        #self.profile.disable()
        #self.profile.dump_stats('myapp.profile')
        #self.profile.print_stats()


    def on_start(self):
        #self.profile = cProfile.Profile()
        #self.profile.enable()

        self.thread_node = Thread(self) # Create a thread
        self.thread_node.daemon = True
        self.thread_node.start() # Start the thread 

        self.thread_node.load_settings()

        self.thread_node.get_timezone_offset()
        self.thread_node.get_locale_language()
        self.change_languange(self.locale_language[0])
        
        self.thread_node.connect_to_broker()

        if exists('image_0.png'):
            self.root.ids.image.remove_from_cache()
            self.root.ids.image.source = 'image_0.png'
            self.root.ids.image.reload() 
        else:
            self.root.ids.image.remove_from_cache()
            self.root.ids.image.source = 'images/feeder_logo.jpg'
            self.root.ids.image.reload() 

        self.root.ids.tabs.add_widget(Tab(title=self.feed_screen))       
        self.root.ids.tabs.add_widget(Tab(title=self.timetable_screen))
        self.root.ids.tabs.add_widget(Tab(title=self.overview_screen))
        self.root.ids.login_button.md_bg_color=self.theme_cls.primary_dark


    def on_pause(self):
        print("pause")
        global client
        if client != None:
            client.loop_stop()
            client.disconnect()
        return True


    def on_resume(self):
        self.root.ids.login_button.md_bg_color=get_color_from_hex(colors["Red"]["900"])
        self.root.ids.login_button.text="Log in"
        self.root.ids.status_label.text="unkown"
        print("resume")
        #global client
        #if client != None:
        #    client.loop_start()


    def change_languange(self, language):
        lang = language[:2]
        if lang == "en":
            pass
            

    def callback(self, instance):
        print(instance.icon)


    def on_tab_switch(self, instance_tabs, instance_tab, instance_tab_label, tab_text):
        '''Called when switching tabs.
        :type instance_tabs: <kivymd.uix.tab.MDTabs object>;
        :param instance_tab: <__main__.Tab object>;
        :param instance_tab_label: <kivymd.uix.tab.MDTabsLabel object>;
        :param tab_text: text or name icon of tab;
        '''
        print(tab_text)
        if tab_text == self.overview_screen:
            self.switch_screen("screen_overview")
        if tab_text == self.image_screen:
            self.switch_screen("screen_image")
        elif tab_text == self.feed_screen:
            self.switch_screen("screen_feeder")
        elif tab_text == self.timetable_screen:
            self.switch_screen("screen_timetable")
            
        #instance_tab.ids.label.text = tab_text
        #self.root.ids.screen_manager.current = "screen_feeder"

    def on_checkbox_active(checkbox, value):
        if value:
            print('The checkbox', checkbox, 'is active')
        else:
            print('The checkbox', checkbox, 'is inactive')


    def show_weekday_dialog(self):
        if self.dialog:
            self.dialog.dismiss()
        self.dialog = MDDialog(
            title="Pick the weeksdays of feeding",
            type="confirmation",
            items=[
                ItemConfirm(text="Sunday"),
                ItemConfirm(text="Monday"),
                ItemConfirm(text="Tuesday"),
                ItemConfirm(text="Wednesday"),
                ItemConfirm(text="Thursday"),
                ItemConfirm(text="Friday"),
                ItemConfirm(text="Saturday")
            ],
            buttons=[
                #MDFlatButton(
                #    text="CANCEL",
                #    text_color=self.theme_cls.primary_color,
                #    on_release=lambda x: self.close_dialog()
                #),
                MDFlatButton(
                    text="OK",
                    text_color=self.theme_cls.primary_color,
                    on_release=lambda x: self.confirm_weekday_dialog(self.dialog)
                ),
            ],
        )
        self.dialog.open()

    def confirm_weekday_dialog(self, instance):
        text = ""
        is_checked_count = 0
        for item in instance.items:
            if item.is_checked:
                text += item.text[:3]
                text += ", "
                is_checked_count += 1
        if is_checked_count == 7:
            text = "Everyday, "
        if is_checked_count == 0:
            text = "Everyday, "
        text += self.item_time
        #if is_checked_count == 0:
        #    text = self.listviewitem_instance.text + ", Once"
        self.listviewitem_instance.text = text
        self.thread_node.save_timetable()
        if self.dialog:
            self.dialog.dismiss()



    def show_alert_dialog(self):
        if self.dialog:
            self.dialog.dismiss()
        self.dialog = MDDialog(
            text="Feed the Cat?",
            buttons=[
                MDFlatButton(
                    text="CANCEL",
                    text_color=self.theme_cls.primary_color,
                    on_release=lambda x: self.close_dialog()
                ),
                MDFlatButton(
                    text="OK",
                    text_color=self.theme_cls.primary_color,
                    on_release=lambda x: self.feed()
                ),
            ],
        )
        self.dialog.open()

    def show_help_dialog(self):
        if self.dialog:
            self.dialog.dismiss()
        screen_name = self.root.ids.screen_manager.current
        dialog_text = ""
        if screen_name == "screen_overview":
            dialog_text = """Here you can see if the time of the Katzomat is properly set. \nYou can also see a history of feeding times."""
        elif screen_name == "screen_timetable":
            dialog_text = """Here you can enter two kinds feeding schedules: \n1) One time feeding, based on date and time. \n2) Recurrent feeding, based on time and weekday. \nThe current schedule of the Katzomat gets updated if you send the timetable to the device. \nAlso, the timetable is stored in the cloud, so everytime you login, the timetable gets downloaded to your phone."""
        elif screen_name == "screen_image":
            dialog_text = """Here you can request a current picture of the Katzomat, so you can see if your can ate something. Select if you want the Flash to be on or off."""
        elif screen_name == "screen_feeder":
            dialog_text = """Here you can directly feed you cat. You get a picture after the process is finished. Or take a picture yourself. Select if you want the Flash to be on or off."""

        self.dialog = MDDialog(
            text=dialog_text,
            buttons=[
                MDFlatButton(
                    text="OK",
                    text_color=self.theme_cls.primary_color,
                    on_release=lambda x: self.close_dialog()
                ),
            ],
        )
        self.dialog.open()

    @mainthread
    def show_connection_error_dialog(self):
        if self.dialog:
            self.dialog.dismiss()
        self.dialog = MDDialog(
            text="""Something went wrong while connecting to the Broker. \nIf you are using the App for the first time, enter your credentials in the Settings.""",
            buttons=[
                MDFlatButton(
                    text="Go to Account Settings",
                    text_color=self.theme_cls.primary_color,
                    on_release=lambda x: self.switch_screen("screen_account_settings")
                ),
                MDFlatButton(
                    text="OK",
                    text_color=self.theme_cls.primary_color,
                    on_release=lambda x: self.close_dialog()
                ),
            ],
        )
        self.dialog.open()

    @mainthread
    def show_no_internet_dialog(self):
        if self.dialog:
            self.dialog.dismiss()
        self.dialog = MDDialog(
            text="""Connect to the Internet to use this App properly. \nThen click the 'Log In' Button""",
            buttons=[
                MDFlatButton(
                    text="OK",
                    text_color=self.theme_cls.primary_color,
                    on_release=lambda x: self.close_dialog()
                ),
            ],
        )
        self.dialog.open()

    def close_dialog(self):
        if self.dialog:
            self.dialog.dismiss()


    def menu_callback(self, text_item):
        print(text_item)


    def take_picture(self):
        print("take picture")
        global client
        global flag_connected
        if flag_connected == 0:
            self.show_snackbar("disconnected")
            print("cannot publish, not connected.")
            return
        flash = "0"
        if self.root.ids.flash_checkbox.state == "down":
            flash = "1"
        client.publish("katzomat/"+ self.settings_dict["user_name"] +"/images/take",str(flash),qos=1,retain=False)

    def refresh_status_verbose(self):
        if self.device_status == "online":
            print("request status verbose")
            client.publish("katzomat/"+ self.settings_dict["user_name"] +"/status/verbose/send","refresh",qos=1,retain=False)
        else:
            self.show_snackbar(self.device_status)

    def feed(self):
        print("feed")
        global client
        global flag_connected
        if flag_connected == 0:
            self.show_snackbar("disconnected")
            print("cannot publish, not connected.")
            return
        flash = "0"
        if self.root.ids.flash_checkbox.state == "down":
            flash = "1"
        client.publish("katzomat/"+ self.settings_dict["user_name"] +"/feed",flash,qos=1,retain=False)
        if self.dialog:
            self.dialog.dismiss()


    def remove_timetable_item(self, instance):
        self.root.ids.scroll.remove_widget(instance)
        self.thread_node.save_timetable()

    def add_timetable(self):
        global flag_connected
        if flag_connected == 0:
            self.show_no_internet_dialog()
            return
        item_text = str(datetime.date.today()) + ", " + str(datetime.datetime.now().hour) + ":" + str(datetime.datetime.now().minute)
        item = ListItemWithButton(text=item_text) #, timetable_icon=icons[i]
        self.root.ids["listitem{self.listviewitems}"] = item
        item.bind(on_release=lambda x, y=item: self.show_date_picker(y))
        self.root.ids.scroll.add_widget(item)
        self.listviewitems += 1
        self.show_date_picker(item)


    def add_recurring_timetable(self):
        #global flag_connected
        #if flag_connected == 0:
        #    self.show_no_internet_dialog()
        #    return
        item_text = "Everyday, " + str(datetime.datetime.now().hour) + ":" + str(datetime.datetime.now().minute)
        item = ListItemWithButton(text="Everyday, 7:00") #, timetable_icon=icons[i]
        self.root.ids["listitem{self.listviewitems}"] = item
        item.bind(on_release=lambda x, y=item: self.show_time_picker(y))
        self.root.ids.scroll.add_widget(item)
        self.listviewitems += 1
        self.show_time_picker(item)


    def save_timetable(self):
        self.thread_node.save_timetable()

    @mainthread
    def show_snackbar(self, msg):
        #self.root.ids.spinner.active = False
        snacktext = ""
        if msg == "connected":
            snacktext = "Connected.."
        elif msg == "disconnected":
            snacktext = "Disconnected."
        elif msg == "published":
            snacktext = "Message sent."
        elif msg == "received":
            snacktext = "Message received." 
        elif msg == "error":
            snacktext = "Settings? Internet? Coffee?"
        elif msg == "keyfile_error":
            snacktext = "Check Your settings."
        elif msg == "offline":
            snacktext = "Katzomat is offline..."

        if self.snackbar:
            self.snackbar.text=snacktext

        if msg == "error":
            self.snackbar = CustomSnackbar(
                text=snacktext,
                icon="information",
                snackbar_x="10dp",
                snackbar_y="10dp",
                buttons=[MDFlatButton(text="Settings", text_color=(1, 1, 1, 1), on_press= lambda x:self.switch_to_settings())]
            )
            self.snackbar.size_hint_x = (
                Window.width - (self.snackbar.snackbar_x * 2)
            ) / Window.width
            self.snackbar.open()
        else:
            self.snackbar = CustomSnackbar(
                text=snacktext,
                icon="information",
                snackbar_x="10dp",
                snackbar_y="10dp",
            )
            self.snackbar.size_hint_x = (
                Window.width - (self.snackbar.snackbar_x * 2)
            ) / Window.width
            self.snackbar.open()


    def show_time_picker(self, instance):
        #global flag_connected
        #if flag_connected == 0:
        #    self.show_no_internet_dialog()
        #    return
        self.listviewitem_instance = instance

        '''Open time picker dialog.'''
        time_dialog = MDTimePicker(accent_color=self.theme_cls.primary_dark,  text_button_color=self.theme_cls.primary_light)
        time_dialog.bind(on_save=self.on_time_save, on_cancel=self.on_time_cancel) #, on_cancel=self.on_time_cancel
        time_dialog.set_time(datetime.datetime.now())
        #time_dialog = MDTimePicker()
        time_dialog.open()
        


    def on_time_save(self, instance, time):
        '''
        The method returns the set time.

        :type instance: <kivymd.uix.picker.MDTimePicker object>
        :type time: <class 'datetime.time'>
        '''

        x = str(time).split(":")
        self.item_time = str(x[0]) + ":" + str(x[1])
        if self.listviewitem_once:
            self.listviewitem_instance.text = self.item_date + ", " + self.item_time
            self.thread_node.save_timetable()
        else:
            self.listviewitem_instance.text = self.item_weekday + ", " + self.item_time
            self.show_weekday_dialog()
        self.listviewitem_once = False
        #self.root.ids[self.current_listitem_id].text = "lul"
        #self.root.ids.time_label.text = str(time)
        #self.time = time
        #self.datetime = datetime.datetime.combine(self.date, self.time)
        #self.root.ids.datetime_label.text = str(self.datetime)
        #self.root.ids.timestamp_label.text = str(datetime.datetime.timestamp(self.datetime))


    def on_time_cancel(self, instance, value):
        self.clear_listview()
        self.thread_node.load_timetable()
        '''Events called when the "CANCEL" dialog box button is clicked.'''


    def on_date_save(self, instance, value, date_range):
        '''
        Events called when the "OK" dialog box button is clicked.

        :type instance: <kivymd.uix.picker.MDDatePicker object>;

        :param value: selected date;
        :type value: <class 'datetime.date'>;

        :param date_range: list of 'datetime.date' objects in the selected range;
        :type date_range: <class 'list'>;
        '''

        date = datetime.datetime.now()
        hour = date.hour
        minute = date.minute
        self.item_time = str(hour) + ":" + str(minute)
        self.item_date = str(value)
        #x = str(time).split(":")
        self.listviewitem_instance.text = self.item_date + ", " + self.item_time
        
        self.show_time_picker(self.listviewitem_instance)
        #self.show_weekday_dialog()

        #self.root.ids.date_label.text = str(value)
        #self.date = value
        #self.datetime = datetime.datetime.combine(self.date, self.time)
        #self.root.ids.datetime_label.text = str(self.datetime)
        #self.root.ids.timestamp_label.text = str(datetime.datetime.timestamp(self.datetime))

    def on_date_cancel(self, instance, value):
        self.clear_listview()
        self.thread_node.load_timetable()
        '''Events called when the "CANCEL" dialog box button is clicked.'''

    def show_date_picker(self, instance):
        global flag_connected
        if flag_connected == 0:
            self.show_no_internet_dialog()
            return
        self.listviewitem_once = True
        self.listviewitem_instance = instance
        date_dialog = MDDatePicker()
        date_dialog.bind(on_save=self.on_date_save, on_cancel=self.on_date_cancel) #, on_cancel=self.on_date_cancel
        date_dialog.open()
    

    def switch_to_settings(self):
        if self.dialog:
            self.dialog.dismiss()
        self.root.ids.screen_manager.current = "screen_account_settings"


    @mainthread
    def switch_screen(self, screen_name):
        if self.dialog:
            self.dialog.dismiss()
        self.root.ids.screen_manager.current = screen_name


    @mainthread
    def clear_listview(self):
        self.root.ids.scroll.clear_widgets()


    @mainthread
    def add_listviewitem(self, item_text, recurring):
        item = ListItemWithButton(text=item_text, theme_text_color= "Custom", text_color= self.theme_cls.primary_light)
        self.root.ids["listitem{self.listviewitems}"] = item
        if recurring:
            item.bind(on_release=lambda x, y=item: self.show_time_picker(y))
        else:
            item.bind(on_release=lambda x, y=item: self.show_date_picker(y))
        self.root.ids.scroll.add_widget(item)


    @mainthread
    def clear_feeding_history(self):
        self.root.ids.feeding_history.clear_widgets()

    @mainthread
    def add_feeding_history_entry(self, item_text):
        self.root.ids.feeding_history.add_widget(
                OneLineListItem(text=str(item_text), theme_text_color= "Custom", text_color= self.theme_cls.primary_light))


    def login(self):
        self.thread_node.connect_to_broker()


    def login_failed(self):
        #self.root.ids.spinner.active = False
        global flag_connected
        if flag_connected == 0:
            print("DISCONNECTED")
            #self.show_snackbar("error")
            self.show_connection_error_dialog()
            if not self.timetable_loaded:
                self.thread_node.load_timetable()

TestNavigationDrawer().run()

