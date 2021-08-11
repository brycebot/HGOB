#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Adafruit_NeoPixel.h>
#include <string.h>

// Ethernet

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192,168,0,102);

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged";        // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

// Neopixel
#define LED_PIN     6
#define LED_COUNT  12

int hue = 0;
int saturation = 0;
int value = 0;
int brightness = 250;
Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // Neopixel setup
  pixels.begin();           // INITIALIZE NeoPixel pixels object (REQUIred)
  pixels.show();            // Turn OFF all pixels ASAP
  pixels.setBrightness(brightness);

  // Ethernet setup
  
  // You can use Ethernet.init(pin) to configure the CS pin
  Ethernet.init(10);  // Feather 328p
  // start the Ethernet
  Ethernet.begin(mac, ip);
  
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start UDP
  Udp.begin(localPort);
  Serial.println("Start");
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());

  //startupSuccessFlash();
  
}

void loop() {
  checkEthernet();
  setLight();
}

void checkEthernet() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int packetLength = Udp.available();
    Udp.read(packetBuffer, packetLength);
    //Serial.println("Contents:");
    //Serial.println(packetBuffer);

    int dataIndex = 0;
    String data;
    for(int i=0;i<packetLength;i++) {
      if(packetBuffer[i] == 32 || packetBuffer[i] == 59){ // " " or ";"
        switch(dataIndex){
          case 0 : 
            hue = data.toInt();
            hue = constrain(hue, 0, 255);
          case 1 : 
            saturation = data.toInt();
            saturation = constrain(saturation, 0, 255);
          case 2 : 
            value = data.toInt();
            value = constrain(value, 0, 255);
          default: break;
        }
        dataIndex++;
        data = "";
        
      }
      else {
        data.concat(packetBuffer[i]);
      }      
    }
  printValues();
  }
}

void setLight() {
    pixels.fill(pixels.gamma32((pixels.ColorHSV(hue*255, saturation, value))));
    pixels.show();
}

void printValues()
{
  //For debug
    Serial.print(hue);
    Serial.print(" ");
    Serial.print(saturation);
    Serial.print(" ");
    Serial.print(value);
    Serial.println(" ");
}

void startupSuccessFlash()
{
  for(int i=0;i<3;i++){
    pixels.fill(pixels.Color(255, 0, 0));
    pixels.show();
    delay(200);
    pixels.fill(pixels.Color(0, 0, 0));
    pixels.show();
    delay(200);
  }
}

