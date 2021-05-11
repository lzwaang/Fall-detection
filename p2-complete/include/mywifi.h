#ifndef myWIFI_H
#define myWIFI_H

#define LED_BUILTIN 2



extern const char* ssid;
extern const char* password;

extern const char* host;
extern const int port;

extern const char* id;
extern int tick;
extern char a[10];

extern WiFiClient client;

void wifi_setup();
void wifi_loop();

#endif