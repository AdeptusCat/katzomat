//char* wifi_ssid = "44386";
//char* wifi_password = "Ruebenwalder.Muehlenfest.Lebe.Hoch!";
char wifi_ssid[20];
char wifi_password[50];

//char* mqtt_server = "adeptuscat.ddns.net";
//char* mqtt_user = "katz";
//char* mqtt_password = "katz!";
char mqtt_server[30];
char mqtt_user[20];
char mqtt_password[20];

int utcOffset = 1;
//char ntpServerName[] = "europe.pool.ntp.org";
char ntpServerName[30];

char rootCertificateURL[50];
char AWS_PUBLIC_CERT[2000] = "-----BEGIN CERTIFICATE-----\n\
IIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\
-----END CERTIFICATE-----";

unsigned int last_unix_time = 0;

// WIFI #############################################################
#include <WiFiClientSecure.h>
#include <WiFi.h>

WiFiClientSecure wifiClient;
WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets
// HTTP #############################################################
#include <HTTPClient.h>

// CAMERA #############################################################
#include "esp_camera.h"
#define CAMERA_MODEL_AI_THINKER
//#define CAMERA_MODEL_M5STACK_NO_PSRAM
#include "camera_pins.h"
// BASE64 #############################################################
#include <base64.h>
// MQTT #############################################################
#include <PubSubClient.h>
#include <ArduinoJson.h>
// SD CARD #############################################################
#include "FS.h"
#include "SD_MMC.h"

PubSubClient client(wifiClient);

#define mqtt_port 8883

// use username and password for authorisation
//#define MQTT_USER "katz"
//#define MQTT_PASSWORD "katz!"

char mqtt_path[40];

char publish_status[70];
#define MQTT_STATUS_PUBLISH_CH "/status"
char publish_status_verbose[70];
#define MQTT_STATUS_VERBOSE_PUBLISH_CH "/status/verbose"
char publish_image0[70];
#define MQTT_IMAGE0_PUBLISH_CH "/images/0"
char publish_feed[70];
#define MQTT_FEED_PUBLISH_CH "/feed"

char subscribe_setting[70];
#define MQTT_SETTINGS_RECEIVER_CH "/settings"
char subscribe_status_verbose[70];
#define MQTT_STATUS_VERBOSE_RECEIVER_CH "/status/verbose/send"
char subscribe_timetable[70];
#define MQTT_TIMETABLE_RECEIVER_CH "/timetable"
char subscribe_take_image[70];
#define MQTT_TAKE_IMAGE_RECEIVER_CH "/images/take"
char subscribe_feed[70];
#define MQTT_FEED_RECEIVER_CH "/feed"

const int image_msg_size = 15000;
char image_msg[image_msg_size];
//char msg[1];

// when did the last 14 feeds happen
const int history_array_size = 14;
int history_array[history_array_size];

//const char rootCertificateURL[] = "https://letsencrypt.org/certs/isrgrootx1.pem";
//String AWS_PUBLIC_CERT = "";

// TIME #############################################################
#include <TimeLib.h>

time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);
time_t prevDisplay = 0; // when the digital clock was displayed
long lastMsg = 0;
long relayStart = 0;
long lastPicture = 0;
long lastReconnect = 0;
long lastCertRequest = 0;
// ALARMS #############################################################
#include <TimeAlarms.h>
AlarmId ids[28];
// SD CARD #############################################################


bool flash = true;
bool bootup = true;
bool loadedAlarmsFromFile = false;

long int counter = 0;
long int feed_counter = 0;

const int ledPin = 4;
const int relayPin = 13;
int rotation_time = 5900;

bool feed_command = false;
bool feeding = false;
bool mid_pic = false;

void setupWifi() {
  Alarm.delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);
  long int start_millis = millis();
  while (WiFi.status() != WL_CONNECTED) {
    Alarm.delay(500);
    Serial.print(".");
    long int current_millis = millis();
    if (current_millis - start_millis > 5000)
    {
      Serial.println("Wifi Connection Failed");
      start_millis = current_millis;
      return;
    }
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void feed() {
  Serial.println("feed!");
  feed_command = true;
  feeding = true;

  for (int i = history_array_size; i > 0; i--)
  {
    history_array[i] = history_array[i - 1];
  }
  history_array[0] = now();

  setupSDCard();
  writeFile(SD_MMC, "/feeding_history.txt", "");
  char time_array[11];
  for (int i = 0; i < history_array_size - 1; i++)
  {
    sprintf(time_array, "%d", history_array[i]);
    appendFile(SD_MMC, "/feeding_history.txt", time_array);
    appendFile(SD_MMC, "/feeding_history.txt", "\n");
  }
  endSDCard();

  send_status_verbose();
}

void take_picture(int nr = 0)
{
  long now = millis();
  if (now - lastPicture < 500) {
    return;
  }
  lastPicture = now;

  camera_fb_t * frameBuffer = NULL;

  if (flash == true)
  {
    digitalWrite(ledPin, HIGH);
    Alarm.delay(100);
  }

  // Take Picture with Camera
  frameBuffer = esp_camera_fb_get();

  if (flash == true)
  {
    digitalWrite(ledPin, LOW);
  }

  //  flash = true;

  if (!frameBuffer) {
    Serial.println("Camera capture failed");
    return;
  }


  //  char msg[13000];
  size_t size = frameBuffer->len;
  String buffer1 = base64::encode((uint8_t *) frameBuffer->buf, frameBuffer->len);
  //  snprintf (msg, 13000, "0|data:image;base64,%s", buffer1.c_str());
  buffer1.toCharArray(image_msg, image_msg_size);

  if (nr == 0)
  {
    client.publish(publish_image0, image_msg);
    //    Serial.println("Camera 0");
  }
  //  if (nr == 1)
  //  {
  //    client.publish(MQTT_IMAGE1_PUBLISH_CH, image_msg);
  //    //    Serial.println("Camera 1");
  //  }
  //  if (nr == 2)
  //  {
  //    client.publish(MQTT_IMAGE2_PUBLISH_CH, image_msg);
  //    //    Serial.println("Camera 2");
  //  }
  //  if (nr == 3)
  //  {
  //    client.publish(MQTT_IMAGE3_PUBLISH_CH, image_msg);
  //    //    Serial.println("Camera 3");
  //}

}



void callback(char* topic, byte *payload, unsigned int length) {
  Serial.println("-------new message from broker-----");
  Serial.print("channel:");
  Serial.println(topic);
  Serial.print("data:");
  Serial.write(payload, length);
  Serial.println();

  if (strcmp(topic, subscribe_take_image) == 0)
  {
    if (!strncmp((char *)payload, "0", length))
    {
      flash = false;
    }
    if (!strncmp((char *)payload, "1", length))
    {
      flash = true;
    }
    Serial.println("Take flash pic");
    take_picture();
  }
  if (strcmp(topic, subscribe_feed) == 0)
  {
    if (!strncmp((char *)payload, "0", length))
    {
      flash = false;
      feed();
    }
    if (!strncmp((char *)payload, "1", length))
    {
      flash = true;
      feed();
    }
  }
  if (strcmp(topic, subscribe_timetable) == 0)
  {
    char json[500];
    for (int i = 0; i < length; i++) {
      json[i] = (char)payload[i];
    }
    json[length] = '\0';
    setupSDCard();
    writeFile(SD_MMC, "/alarms.txt", json);
    endSDCard();
    parse_json(json);
  }
  if (strcmp(topic, subscribe_status_verbose) == 0)
  {
    send_status_verbose();
  }
  if (strcmp(topic, subscribe_setting) == 0)
  {
    char message[20];
    for (int i = 0; i < length; i++) {
      message[i] = (char)payload[i];
    }
    message[length] = '\0';
    rotation_time = atoi(message);

    setupSDCard();
    writeFile(SD_MMC, "/device_settings.txt", message);
    appendFile(SD_MMC, "/device_settings.txt", "\n");
    endSDCard();
  }

}

void send_status_verbose()
{
  DynamicJsonDocument msg(500);
  msg["time"] = now();
  for (int i = 0; i < history_array_size; i++)
  {
    msg["history"][i] = history_array[i];
  }

  char jsonChar[500];
  serializeJson(msg, jsonChar);

  client.publish(publish_status_verbose, jsonChar, true);
}

void parse_json(char* json)
{
  StaticJsonDocument<500> root;
  DeserializationError err = deserializeJson(root, json);

  if (!root.isNull()) {
    resetAlarms();

    unsigned int message_timestamp = root["timestamp"];
    JsonArray values = root["values"].as<JsonArray>();
    for (int i = 0; i < values.size(); i++)
    {
      JsonArray weekdays = values[i]["weekdays"].as<JsonArray>();
      if (weekdays.size() == 0)
      {
        Serial.print("once ");
        Serial.println(values[i]["timestamp"].as<unsigned int>());
        Serial.print("now ");
        Serial.println(now());

        if (values[i]["timestamp"].as<unsigned int>() > now())
        {
          add_alarm_once(values[i]["timestamp"].as<unsigned int>());
        }
      }
      else
      {
        for (int ii = 0; ii < weekdays.size(); ii++)
        {
          add_recurring_alarm(weekdays[ii].as<byte>(), values[i]["hour"].as<byte>(), values[i]["minute"].as<byte>());
        }
      }
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  wifiClient.setCACert(AWS_PUBLIC_CERT);
  //  wifiClient.setCertificate(AWS_DEVICE_CERT);
  //  wifiClient.setPrivateKey(AWS_PRIVATE_KEY);
  if (WiFi.status() != WL_CONNECTED)
  {
    setupWifi();
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    return;
  }
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "katz1";
    //    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //    ,MQTT_USER,MQTT_PASSWORD

    // QOS 1, willRetain, willMessage, cleanSession
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password, publish_status, 1, true, "offline", false)) {
      Serial.println("connected");

      // publish device status RETAINED
      char device_status[] = "online";
      client.publish(publish_status, device_status, true);

      // subscribe with QOS 1
      client.subscribe(subscribe_status_verbose, 1);
      client.subscribe(subscribe_setting, 1);
      client.subscribe(subscribe_timetable, 1);
      client.subscribe(subscribe_take_image, 1);
      client.subscribe(subscribe_feed, 1);
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      // Request Certificate only if the last request was 24 hours ago.
      long int current_millis = millis();
      if (current_millis - lastCertRequest > 86400000) {
        lastCertRequest = current_millis;
        requestCert();
      }
    }
  }
}


void requestCert()
{
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Requesting root certificate.");
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(rootCertificateURL);

    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      //      Serial.println(payload);
      int str_len = payload.length() + 1;
      payload.toCharArray(AWS_PUBLIC_CERT, str_len);
      setupSDCard();
      writeFile(SD_MMC, "/certificate.txt", AWS_PUBLIC_CERT);
      endSDCard();
      Serial.print("Requesting root certificate successful.");
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}

void setupCamera()
{
  // CAMERA #############################################################
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 5000000;
  //                        10000000
  config.pixel_format = PIXFORMAT_JPEG;

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  //10-63 lower number means higher quality
  if (psramFound()) {
    config.frame_size = FRAMESIZE_VGA; //FRAMESIZE_SVGA
    config.jpeg_quality = 10;
    config.fb_count = 1;

    //    config.frame_size = FRAMESIZE_UXGA;
    //    config.jpeg_quality = 10;
    //    config.fb_count = 2;
  } else {
    //    FRAMESIZE_UXGA (1600 x 1200)
    //    FRAMESIZE_QVGA (320 x 240)
    //    FRAMESIZE_CIF (352 x 288)
    //    FRAMESIZE_VGA (640 x 480)
    //    FRAMESIZE_SVGA (800 x 600)
    //    FRAMESIZE_XGA (1024 x 768)
    //    FRAMESIZE_SXGA (1280 x 1024)

    config.frame_size = FRAMESIZE_XGA; //FRAMESIZE_SVGA
    config.jpeg_quality = 20;
    config.fb_count = 1;
    Serial.println(1);
    Serial.println(ESP.getFreeHeap());
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    //    does not work as intended
    //    ESP.restart();
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif
}

void setup() {
  digitalWrite(ledPin, LOW);
  pinMode(ledPin, OUTPUT);

  digitalWrite(relayPin, HIGH);
  pinMode(relayPin, OUTPUT);

  client.setBufferSize(16000);
  Serial.begin(9600);
  Serial.setTimeout(500);// Set time out for
  // SD CARD #############################################################
  setupSDCard();
  readFile(SD_MMC, "/wifi_settings.txt");
  readFile(SD_MMC, "/general_settings.txt");
  readFile(SD_MMC, "/account_settings.txt");
  readFile(SD_MMC, "/certificate.txt");
  readFile(SD_MMC, "/feeding_history.txt");
  readFile(SD_MMC, "/device_settings.txt");
  endSDCard();
  
  char mqtt_root_path[] = "katzomat/";
  sprintf(mqtt_path, "%s%s", mqtt_root_path, mqtt_user);

  sprintf(publish_status, "%s%s", mqtt_path, MQTT_STATUS_PUBLISH_CH);
  sprintf(publish_status_verbose, "%s%s", mqtt_path, MQTT_STATUS_VERBOSE_PUBLISH_CH);
  sprintf(publish_image0, "%s%s", mqtt_path, MQTT_IMAGE0_PUBLISH_CH);
  sprintf(publish_feed, "%s%s", mqtt_path, MQTT_FEED_PUBLISH_CH);

  sprintf(subscribe_setting, "%s%s", mqtt_path, MQTT_SETTINGS_RECEIVER_CH);
  sprintf(subscribe_status_verbose, "%s%s", mqtt_path, MQTT_STATUS_VERBOSE_RECEIVER_CH);
  sprintf(subscribe_timetable, "%s%s", mqtt_path, MQTT_TIMETABLE_RECEIVER_CH);
  sprintf(subscribe_take_image, "%s%s", mqtt_path, MQTT_TAKE_IMAGE_RECEIVER_CH);
  sprintf(subscribe_feed, "%s%s", mqtt_path, MQTT_FEED_RECEIVER_CH);



  // CAMERA #############################################################
  setupCamera();
  // WIFI #############################################################
  setupWifi();
  // TIME #############################################################
  setupTime();

  // MQTT #############################################################
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
  if (!client.connected())
  {
    setupSDCard();
    readFile(SD_MMC, "/alarms.txt");
    endSDCard();
  }
}

void loop() {
  long int current_millis = millis();
  if (feeding == false)
  {
    if (!client.connected()) {
      if (current_millis - lastReconnect > 120000)
      {
        lastReconnect = current_millis;
        reconnect();
      }
    }
    else
    {
      client.loop();
    }
  }

  //  int buttonState = digitalRead(buttonPin);
  //  if (feeding == false)
  //  {
  //    if (buttonState == HIGH) {
  //      digitalWrite(relayPin, HIGH);
  //    } else {
  //      digitalWrite(relayPin, LOW);
  //    }
  //  }

  current_millis = millis();
  if (feed_command == true)
  {
    digitalWrite(relayPin, LOW);
    relayStart = current_millis;
    feed_command = false;
    mid_pic = true;
    //    take_picture(1);
  }
//  if (feeding == true)
//  {
//    Serial.println(current_millis - relayStart);
//  }
  if (current_millis - relayStart > rotation_time / 2)
  {
    if (mid_pic == true)
    {
      //      take_picture(2);
      mid_pic = false;
    }
  }
  if (current_millis - relayStart > rotation_time)
  {
    if (feeding == true)
    {
      digitalWrite(relayPin, HIGH);
      Alarm.delay(500);
      take_picture();
      feeding = false;
      client.publish(publish_feed, "done");
    }
  }

//  if (current_millis - lastMsg > 1000) {
//    lastMsg = current_millis;

    //    Serial.print("freeMemory()=");
    //    Serial.println(ESP.getFreeHeap());
//    digitalClockDisplay();
    //Serial.println(now());
//  }

  //  if (now - lastMsg > 60000) {
  //    lastMsg = now;
  //    ++counter;
  //
  //    String value;
  //    value = String(counter);
  //    snprintf (msg, 75, "0|%s", value.c_str());
  //    client.publish(outTopicCounter, msg);
  //        take_picture();
  //  }

  //  if (Serial.available() > 0) {
  //    char mun[501];
  //    memset(mun, 0, 501);
  //    Serial.readBytesUntil( '\n', mun, 500);
  //    if (!client.connected()) {
  //      reconnect();
  //    }
  //    client.publish(MQTT_SERIAL_PUBLISH_CH, mun);
  //  }
  Alarm.delay(0);
}
