

//List dir in SD card
void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

//Create a dir in SD card
void createDir(fs::FS &fs, const char * path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

//delete a dir in SD card
void removeDir(fs::FS &fs, const char * path) {
  Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path)) {
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

//Read a file in SD card
void readFile(fs::FS &fs, const char * path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    writeFile(SD_MMC, path, "");
    return;
  }

  Serial.print("Read from file: ");
  int i = 0;
  int line = 0;
  char cr;
  char last_unix_time_array[100];
  char json[500];
  char timezone_array[5];
  char time_array[20];
  Serial.println(path);
  while (file.available()) {
    cr = file.read();
    //    Serial.print(cr);
    if (strcmp("/device_settings.txt", path) == 0)
    {
      if (cr == '\n') {
        if (line == 0) {
          time_array[i] = '\0';
          rotation_time = atoi(time_array);
          line += 1;
          i = 0;
          continue;
        }
      }
      time_array[i] = cr;
    }
    if (strcmp("/feeding_history.txt", path) == 0)
    {
      if (line >= history_array_size)
      {
        break;
      }
      if (cr == '\n') {
        time_array[i] = '\0';
        history_array[line] = atoi(time_array);
        line += 1;
        i = 0;
        continue;
      }
      time_array[i] = cr;
    }
    if (strcmp("/wifi_settings.txt", path) == 0)
    {
      if (line == 0) {
        if (cr == '\n') {
          line += 1;
          i = 0;
          continue;
        }
      }
      if (line == 1) {
        if (cr == '\n') {
          wifi_ssid[i] = '\0';
          line += 1;
          i = 0;
          continue;
        }
        wifi_ssid[i] = cr;
      }
      if (line == 2) {
        if (cr == '\n') {
          line += 1;
          i = 0;
          continue;
        }
      }
      if (line == 3) {
        if (cr == '\n') {
          wifi_password[i] = '\0';
          line += 1;
          i = 0;
          continue;
        }
        wifi_password[i] = cr;
      }
    }
    if (strcmp("/account_settings.txt", path) == 0)
    {
      if (line == 0) {
        if (cr == '\n') {
          line += 1;
          i = 0;
          continue;
        }
      }
      if (line == 1) {
        if (cr == '\n') {
          mqtt_server[i] = '\0';
          line += 1;
          i = 0;
          continue;
        }
        mqtt_server[i] = cr;
      }
      if (line == 2) {
        if (cr == '\n') {
          line += 1;
          i = 0;
          continue;
        }
      }
      if (line == 3) {
        if (cr == '\n') {
          mqtt_user[i] = '\0';
          line += 1;
          i = 0;
          continue;
        }
        mqtt_user[i] = cr;
      }
      if (line == 4) {
        if (cr == '\n') {
          line += 1;
          i = 0;
          continue;
        }
      }
      if (line == 5) {
        if (cr == '\n') {
          mqtt_password[i] = '\0';
          line += 1;
          i = 0;
          continue;
        }
        mqtt_password[i] = cr;
      }
    }
    if (strcmp("/time.txt", path) == 0)
    {
      last_unix_time_array[i] = cr;
    }
    if (strcmp("/general_settings.txt", path) == 0)
    {
      if (line == 0) {
        if (cr == '\n') {
          line += 1;
          i = 0;
          continue;
        }
      }
      if (line == 1) {
        if (cr == '\n') {
          timezone_array[i] = '\0';
          utcOffset = atoi(timezone_array);
          line += 1;
          i = 0;
          continue;
        }
        timezone_array[i] = cr;
      }
      if (line == 2) {
        if (cr == '\n') {
          line += 1;
          i = 0;
          continue;
        }
      }
      if (line == 3) {
        if (cr == '\n') {
          ntpServerName[i] = '\0';
          line += 1;
          i = 0;
          continue;
        }
        ntpServerName[i] = cr;
      }
      if (line == 4) {
        if (cr == '\n') {
          line += 1;
          i = 0;
          continue;
        }
      }
      if (line == 5) {
        if (cr == '\n') {
          rootCertificateURL[i] = '\0';
          line += 1;
          i = 0;
          continue;
        }
        rootCertificateURL[i] = cr;
      }
    }
    if (strcmp("/certificate.txt", path) == 0)
    {
      AWS_PUBLIC_CERT[i] = cr;
    }
    if (strcmp("/alarms.txt", path) == 0)
    {
      json[i] = cr;
    }
    i += 1;
  }
  if (strcmp("/time.txt", path) == 0)
  {
    last_unix_time_array[i] = '\0';
    sscanf(last_unix_time_array, "%d", &last_unix_time);
    setTime(last_unix_time);
    digitalClockDisplay();
  }
  if (strcmp("/certificate.txt", path) == 0)
  {
    AWS_PUBLIC_CERT[i] = '\0';
  }
  if (strcmp("/alarms.txt", path) == 0)
  {
    json[i] = '\0';
    parse_json(json);
  }
}

//Write a file in SD card
void writeFile(fs::FS & fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  
if (strcmp("/device_settings.txt", path) == 0)
  {
    file.print("4450\n");
  }
  if (strcmp("/general_settings.txt", path) == 0)
  {
    file.print("### timezone ###\n");
    file.print("1\n");
    file.print("### ntp server ###\n");
    file.print("europe.pool.ntp.org\n");
    file.print("### certificate URL ###\n");
    file.print("https://letsencrypt.org/certs/isrgrootx1.pem\n");
  }
  if (strcmp("/wifi_settings.txt", path) == 0)
  {
    file.print("### WIFI SSID ###\n");
    file.print("YOUR SSID HERE\n");
    file.print("### WIFI Password ###\n");
    file.print("YOUR PASSWORD HERE\n");
  }
  if (strcmp("/account_settings.txt", path) == 0)
  {
    file.print("### MQTT Server Domain ###\n");
    file.print("MQTT SERVER DOMAIN HERE\n");
    file.print("### User Name ###\n");
    file.print("YOUR USER NAME HERE\n");
    file.print("### Password ###\n");
    file.print("YOUR PASSWORD HERE\n");
  }
  //fwrite(fb->buf, 1, fb->len, file);
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
}

//Append to the end of file in SD card
void appendFile(fs::FS & fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    writeFile(SD_MMC, path, message);
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
}

//Rename a file in SD card
void renameFile(fs::FS & fs, const char * path1, const char * path2) {
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

//Delete a file in SD card
void deleteFile(fs::FS & fs, const char * path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}


//Test read and write speed using test.txt file
void testFileIO(fs::FS & fs, const char * path) {
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if (file) {
    len = file.size();
    size_t flen = len;
    start = millis();
    while (len) {
      size_t toRead = len;
      if (toRead > 512) {
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  } else {
    Serial.println("Failed to open file for reading");
  }


  file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for (i = 0; i < 2048; i++) {
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}

void endSDCard()
{
  SD_MMC.end();
}

void setupSDCard() {
  //  By setting the parameter mode1bit to true,
  // the ESP32-CAM just uses the GPIO02 to read and
  // write data to the SD card, which frees up the GPIO pins
  // GPIO04 (HS_DATA1), GPIO12 (HS_DATA2), and GPIO13 (HS_DATA3)
  // as further GPIO pins for input or output.,
  if (!SD_MMC.begin("/sdcard", true)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD_MMC card attached");
    return;
  }

  //    Serial.print("SD_MMC Card Type: ");
  //    if(cardType == CARD_MMC){
  //        Serial.println("MMC");
  //    } else if(cardType == CARD_SD){
  //        Serial.println("SDSC");
  //    } else if(cardType == CARD_SDHC){
  //        Serial.println("SDHC");
  //    } else {
  //        Serial.println("UNKNOWN");
  //    }
  //
  //    uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
  //    Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);
  //
  //    listDir(SD_MMC, "/", 0);
  //    createDir(SD_MMC, "/mydir");
  //    listDir(SD_MMC, "/", 0);
  //    removeDir(SD_MMC, "/mydir");
  //    listDir(SD_MMC, "/", 2);
  //    writeFile(SD_MMC, "/hello.txt", "Hello ");
  //    appendFile(SD_MMC, "/hello.txt", "World!\n");
  //    readFile(SD_MMC, "/hello.txt");
  //    deleteFile(SD_MMC, "/foo.txt");
  //    renameFile(SD_MMC, "/hello.txt", "/foo.txt");
  //    readFile(SD_MMC, "/foo.txt");
  //    testFileIO(SD_MMC, "/test.txt");
  //    Serial.printf("Total space: %lluMB\n", SD_MMC.totalBytes() / (1024 * 1024));
  //    Serial.printf("Used space: %lluMB\n", SD_MMC.usedBytes() / (1024 * 1024));

}
