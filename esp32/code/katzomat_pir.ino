//char wifi_ssid[] = "44386";
//char wifi_password[] = "Ruebenwalder.Muehlenfest.Lebe.Hoch!";
char wifi_ssid[20];
char wifi_password[50];

//char mqtt_server[] = "adeptuscat.ddns.net";
//char mqtt_user[] = "katz";
//char mqtt_password[] = "katz!";
char mqtt_server[30];
char mqtt_user[20];
char mqtt_password[20];

int utcOffset = 1;
//char ntpServerName[] = "europe.pool.ntp.org";
char ntpServerName[30];

//char rootCertificateURL[] = "http://letsencrypt.org/certs/isrgrootx1.pem";
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

// PIR #############################################################

const int pirPin = 12;
bool pirState = 0;
bool pirStateBefore = 0;

// WIFI #############################################################
#include <WiFiClientSecure.h>
#include <WiFi.h>

WiFiClientSecure wifiClient;
WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets
// HTTP #############################################################
#include <HTTPClient.h>

// CAMERA #############################################################
int picturesTaken = 0;
#include "esp_camera.h"
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"
int jpegQuality = 10;
int currentJpegQuality = 10;

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

char mqtt_path[40];

char publish_status[70];
#define MQTT_STATUS_PUBLISH_CH "/status"
char publish_status_verbose[70];
#define MQTT_STATUS_VERBOSE_PUBLISH_CH "/status/verbose"
char publish_image0[70];
#define MQTT_IMAGE0_PUBLISH_CH "/images/0"
#define MQTT_IMAGE1_PUBLISH_CH "/images/1"
#define MQTT_IMAGE2_PUBLISH_CH "/images/2"
#define MQTT_IMAGE3_PUBLISH_CH "/images/3"
#define MQTT_IMAGE4_PUBLISH_CH "/images/4"
#define MQTT_IMAGE5_PUBLISH_CH "/images/5"
#define MQTT_IMAGE6_PUBLISH_CH "/images/6"
#define MQTT_IMAGE7_PUBLISH_CH "/images/7"
#define MQTT_IMAGE8_PUBLISH_CH "/images/8"
#define MQTT_IMAGE9_PUBLISH_CH "/images/9"
#define MQTT_IMAGE10_PUBLISH_CH "/images/10"
#define MQTT_IMAGE11_PUBLISH_CH "/images/11"
#define MQTT_IMAGE12_PUBLISH_CH "/images/12"
#define MQTT_IMAGE13_PUBLISH_CH "/images/13"
#define MQTT_IMAGE14_PUBLISH_CH "/images/14"
#define MQTT_IMAGE15_PUBLISH_CH "/images/15"

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

//const int image_msg_size = 20000;
//char image_msg[image_msg_size];
//char msg[1];

// when did the last 14 feeds happen
const int history_array_size = 14;
int unsigned history_array[history_array_size];

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
//long lastPicture = 0;
long lastReconnect = 0;
long lastCertRequest = 0;
bool firstRequest = true;
long savedTime = 0;
bool got_time_after_start = false;
long onlineTime = 0;
// ALARMS #############################################################
#include <TimeAlarms.h>
AlarmId ids[28];
// SD CARD #############################################################
bool online = false;
long went_online = 0;

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
  sensor_t *s = esp_camera_sensor_get();
  long time_now = millis();

  if (currentJpegQuality > 60)
  {
    Serial.println("Image could not be sent.");
    s->set_quality(s, jpegQuality);
    currentJpegQuality = jpegQuality;
    return;
  }

  //  if (time_now - lastPicture < 500) {
  //    return;
  //  }
  //  lastPicture = time_now;

  camera_fb_t * fb = NULL;

  if (flash == true)
  {
    digitalWrite(ledPin, HIGH);
    Alarm.delay(100);
  }

  // Take Picture with Camera
  fb = esp_camera_fb_get();

  if (flash == true)
  {
    digitalWrite(ledPin, LOW);
  }

  //  flash = true;

  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }


  //  //  char msg[13000];
  //  size_t size = frameBuffer->len;
  //  String buffer1 = base64::encode((uint8_t *) frameBuffer->buf, frameBuffer->len);
  //  //  snprintf (msg, 13000, "0|data:image;base64,%s", buffer1.c_str());
  //  buffer1.toCharArray(image_msg, image_msg_size);

  //  String buffer1 = base64::encode((uint8_t *) frameBuffer->buf, frameBuffer->len);
  //  char *image_msg = (char*)heap_caps_malloc(buffer1.length() + 100, MALLOC_CAP_SPIRAM);
  //  buffer1.toCharArray(image_msg, buffer1.length() + 100);
  //  if (nr == 0)
  //  {
  //    client.publish(publish_image0, image_msg, true);
  //    //    Serial.println("Camera 0");
  //  }
  //  free(image_msg);

  esp_err_t res = ESP_OK;
  int64_t fr_start = esp_timer_get_time();

  size_t fb_len = 0;
  fb_len = fb->len;

  //  esp_camera_fb_return(fb);

  String buffer1 = base64::encode((uint8_t *) fb->buf, fb->len);
  char *image_msg = (char*)heap_caps_malloc(buffer1.length() + 100, MALLOC_CAP_SPIRAM);
  buffer1.toCharArray(image_msg, buffer1.length() + 100);
  //  Serial.println(image_msg);

  int timestamp = now();
  char timestamp_cstr[16];
  itoa(timestamp, timestamp_cstr, 10);
  char *jsonChar = (char*)heap_caps_malloc(buffer1.length() + 200, MALLOC_CAP_SPIRAM);
  sprintf(jsonChar, "%s%s%s%s%s", "{\"timestamp\":", timestamp_cstr, ",\"value\":\"", image_msg, "\"}");

  esp_camera_fb_return(fb);
  int64_t fr_end = esp_timer_get_time();

  Serial.print(buffer1.length());
  Serial.println(" MEssage len");

  if (buffer1.length() < 16000)
  {
    if (picturesTaken >= 16) {
      picturesTaken = 0;
    }
    if (picturesTaken == 0) {
      sprintf(publish_image0, "%s%s", mqtt_path, MQTT_IMAGE0_PUBLISH_CH);
    }
    if (picturesTaken == 1) {
      sprintf(publish_image0, "%s%s", mqtt_path, MQTT_IMAGE1_PUBLISH_CH);
    }
    if (picturesTaken == 2) {
      sprintf(publish_image0, "%s%s", mqtt_path, MQTT_IMAGE2_PUBLISH_CH);
    }
    if (picturesTaken == 3) {
      sprintf(publish_image0, "%s%s", mqtt_path, MQTT_IMAGE3_PUBLISH_CH);
    }
    if (picturesTaken == 4) {
      sprintf(publish_image0, "%s%s", mqtt_path, MQTT_IMAGE4_PUBLISH_CH);
    }
    if (picturesTaken == 5) {
      sprintf(publish_image0, "%s%s", mqtt_path, MQTT_IMAGE5_PUBLISH_CH);
    }
    if (picturesTaken == 6) {
      sprintf(publish_image0, "%s%s", mqtt_path, MQTT_IMAGE6_PUBLISH_CH);
    }
    if (picturesTaken == 7) {
      sprintf(publish_image0, "%s%s", mqtt_path, MQTT_IMAGE7_PUBLISH_CH);
    }
    if (picturesTaken == 8) {
      sprintf(publish_image0, "%s%s", mqtt_path, MQTT_IMAGE8_PUBLISH_CH);
    }
    if (picturesTaken == 9) {
      sprintf(publish_image0, "%s%s", mqtt_path, MQTT_IMAGE9_PUBLISH_CH);
    }
    if (picturesTaken == 10) {
      sprintf(publish_image0, "%s%s", mqtt_path, MQTT_IMAGE10_PUBLISH_CH);
    }
    if (picturesTaken == 11) {
      sprintf(publish_image0, "%s%s", mqtt_path, MQTT_IMAGE11_PUBLISH_CH);
    }
    if (picturesTaken == 12) {
      sprintf(publish_image0, "%s%s", mqtt_path, MQTT_IMAGE12_PUBLISH_CH);
    }
    if (picturesTaken == 13) {
      sprintf(publish_image0, "%s%s", mqtt_path, MQTT_IMAGE13_PUBLISH_CH);
    }
    if (picturesTaken == 14) {
      sprintf(publish_image0, "%s%s", mqtt_path, MQTT_IMAGE14_PUBLISH_CH);
    }
    if (picturesTaken == 15) {
      sprintf(publish_image0, "%s%s", mqtt_path, MQTT_IMAGE15_PUBLISH_CH);
    }
    picturesTaken += 1;

    //    client.publish(publish_image0, "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", true);
    bool i = client.publish(publish_image0, jsonChar, true);
    //    bool i = client.publish(publish_image0, (const uint8_t*)image_msg, buffer1.length() + 100, true);

    Serial.print(i);
    Serial.println(" Message sent!");
    Serial.println(publish_image0);
    s->set_quality(s, jpegQuality);
    currentJpegQuality = jpegQuality;
    free(image_msg);
    free(jsonChar);
  }
  else
  {
    Serial.println("Try again sending picture.");
    free(image_msg);
    free(jsonChar);
    currentJpegQuality += 4;
    s->set_quality(s, currentJpegQuality);

    delay(200);
    take_picture();
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

  // reason here is that the char array gets overwritten for some reason after the feed()
  sprintf(subscribe_setting, "%s%s", mqtt_path, MQTT_SETTINGS_RECEIVER_CH);
  sprintf(subscribe_status_verbose, "%s%s", mqtt_path, MQTT_STATUS_VERBOSE_RECEIVER_CH);
  sprintf(subscribe_timetable, "%s%s", mqtt_path, MQTT_TIMETABLE_RECEIVER_CH);
  sprintf(subscribe_take_image, "%s%s", mqtt_path, MQTT_TAKE_IMAGE_RECEIVER_CH);
  sprintf(subscribe_feed, "%s%s", mqtt_path, MQTT_FEED_RECEIVER_CH);
  Serial.println(subscribe_feed);

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
    Serial.println(subscribe_feed);
    if (!strncmp((char *)payload, "0", length))
    {
      Serial.println("no flash");
      flash = false;
      feed();
    }
    if (!strncmp((char *)payload, "1", length))
    {
      Serial.println("flash");
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

  sprintf(publish_status_verbose, "%s%s", mqtt_path, MQTT_STATUS_VERBOSE_PUBLISH_CH);
  client.publish(publish_status_verbose, jsonChar, true);
  //
  char device_status[] = "online";
  sprintf(publish_status, "%s%s", mqtt_path, MQTT_STATUS_PUBLISH_CH);
  client.publish(publish_status, device_status, true);
}

void parse_json(char* json)
{
  StaticJsonDocument<500> root;
  DeserializationError err = deserializeJson(root, json);

  if (!root.isNull()) {
    resetAlarms();
    unsigned int message_timestamp = root["timestamp"].as<unsigned int>();
    JsonArray values = root["values"].as<JsonArray>();
    for (int i = 0; i < values.size(); i++)
    {
      JsonArray weekdays = values[i]["weekdays"].as<JsonArray>();
      if (weekdays.size() == 0)
      {
        unsigned int time_once = values[i]["timestamp"].as<unsigned int>();
        Serial.print("once ");
        Serial.println(time_once);
        Serial.print("now ");
        Serial.println(now());

        if (time_once > now())
        {
          Serial.println("adding onetime feed");
          add_alarm_once(time_once);
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
    char clientId[40];
    char device[] = "-device-";
    char randomNumberCstr[40];
    int randomNumber = random(0xffff);
    itoa(randomNumber, randomNumberCstr, 10);
    sprintf(clientId, "%s%s%s", mqtt_user, device, randomNumberCstr);

    // QOS 1, willRetain, willMessage, cleanSession
    if (client.connect(clientId, mqtt_user, mqtt_password, publish_status, 1, true, "offline", false)) {
      Serial.println("connected");
      went_online = millis();
      // publish device status RETAINED
      char device_status[] = "online";
      sprintf(publish_status, "%s%s", mqtt_path, MQTT_STATUS_PUBLISH_CH);
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
      if (current_millis - lastCertRequest > 86400000 || firstRequest == true) {
        lastCertRequest = current_millis;
        requestCert();
        firstRequest = false;
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
      reconnect();
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
  config.xclk_freq_hz = 20000000;
  //  config.frame_size = FRAMESIZE_UXGA;
  config.frame_size = FRAMESIZE_VGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  //    FRAMESIZE_UXGA (1600 x 1200)
  //    FRAMESIZE_QVGA (320 x 240)
  //    FRAMESIZE_CIF (352 x 288)
  //    FRAMESIZE_VGA (640 x 480)
  //    FRAMESIZE_SVGA (800 x 600)
  //    FRAMESIZE_XGA (1024 x 768)
  //    FRAMESIZE_SXGA (1280 x 1024)

  config.frame_size = FRAMESIZE_CIF;
  config.jpeg_quality = jpegQuality;

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
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
  //  if(config.pixel_format == PIXFORMAT_JPEG){
  //    s->set_framesize(s, FRAMESIZE_QVGA);
  //  }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif
}

void setup() {

  digitalWrite(ledPin, LOW);
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  digitalWrite(relayPin, HIGH);
  pinMode(relayPin, OUTPUT);


  Serial.begin(115200);

  bool lol = client.setBufferSize(60000); //60000
  //  Serial.print(client.getBufferSize());
  //  Serial.println(" Buffer size");

  Serial.setTimeout(500);// Set time out for
  // SD CARD #############################################################
  setupSDCard();
  readFile(SD_MMC, "/wifi_settings.txt");
  readFile(SD_MMC, "/general_settings.txt");
  readFile(SD_MMC, "/account_settings.txt");
  readFile(SD_MMC, "/certificate.txt");
  readFile(SD_MMC, "/feeding_history.txt");
  readFile(SD_MMC, "/device_settings.txt");


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

  endSDCard();
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

  randomSeed(micros());
}


void loop() {
  long int current_millis = millis();
  if (feeding == false)
  {
    if (!client.connected()) {
      online = false;
      if (current_millis - lastReconnect > 120000)
      {
        lastReconnect = current_millis;
        reconnect();
      }
    }
    else
    {
      client.loop();
      if (online == false)
      {
        if (current_millis - went_online > 5000)
        {
          lastReconnect = current_millis;
          // publish device status RETAINED
          char device_status[] = "online";
          sprintf(publish_status, "%s%s", mqtt_path, MQTT_STATUS_PUBLISH_CH);
          client.publish(publish_status, device_status, true);

          online = true;
        }
      }

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
  // save time every hour and after first time sync
  if (current_millis - savedTime > 3600000)
  {
    if (got_time_after_start == true)
    {
      savedTime = current_millis;
      saveTime();
    }
    send_status_verbose();
  }
  if (current_millis - onlineTime > 10000)
  {
    onlineTime = current_millis;
    char device_status[] = "online";
    sprintf(publish_status, "%s%s", mqtt_path, MQTT_STATUS_PUBLISH_CH);
    client.publish(publish_status, device_status, true);
  }


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
      flash = false;
      take_picture();
      feeding = false;
      sprintf(publish_feed, "%s%s", mqtt_path, MQTT_FEED_PUBLISH_CH);
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

  pirState = digitalRead(pirPin);
  if (pirState == HIGH) {
    if (pirState != pirStateBefore) {
      take_picture();
    }
  }
  pirStateBefore = pirState;

  Alarm.delay(0);
}
