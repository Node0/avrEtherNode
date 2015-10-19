#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include "avrEtherNode_header.h"


// BEGIN setup of Ethernet connectivity parameters
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 0, 1);
IPAddress gateway(192, 168, 5, 2);
IPAddress subnet(255, 255, 255, 0);
// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);
// END setup of Ethernet connectivity parameters






// Delay times for various resolutions
// delay(100); // 100ms for 9bit resolution
// delay(190); // 190ms for 10bit resolution  
// delay(375); // 375ms for 11bit resolution
// delay(750); // 750ms for 12bit resolution
OneWire  busA(22);  // the pin for busA requires it's own 4.7K pullup
OneWire  busB(24);  // the pin for busB requires it's own 4.7K pullup
OneWire  busC(26);  // the pin for busA requires it's own 4.7K pullup
OneWire  busD(28);  // the pin for busA requires it's own 4.7K pullup
OneWire  busE(30);  // the pin for busA requires it's own 4.7K pullup

void setup() {
Serial.begin(115200);

// start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at "); Serial.println(Ethernet.localIP());
}

void loop() {

// BEGIN temperature collection
// Declare and init all the one wire bus instance config params
owbInstanceParam bsA;
owbInstanceParam bsB;
owbInstanceParam bsC;
owbInstanceParam bsD;
owbInstanceParam bsE;


// BEGIN Handling Setup for bus A -----------------------------------------------------
// Search bus A for a sensor
  if ( !busA.search(bsA.addr)) { busA.reset_search(); delay(25); return; }
// CRC for bus A
  if (OneWire::crc8(bsA.addr, 7) != bsA.addr[7]) { Serial.println("CRC on bus A is not valid!"); return; }
// Select sensor type DS18B20 for Bus A
  bsA.type_s = 0;
// Tell sensor on Bus A to fetch a reading
  busA.reset(); busA.select(bsA.addr); busA.write(0x44, 0); // start conversion, with parasite power on at the end
// END bus A --------------------------------------------------------------------------


// BEGIN Handling Setup for bus B -----------------------------------------------------
// Search bus B for a sensor
  if ( !busB.search(bsB.addr)) { busB.reset_search(); delay(25); return; }
// CRC for bus B
  if (OneWire::crc8(bsB.addr, 7) != bsB.addr[7]) { Serial.println("CRC on bus B is not valid!"); return; }
// Select sensor type DS18B20 for Bus B
  bsB.type_s = 0;
// Tell sensor on Bus B to fetch a reading
  busB.reset(); busB.select(bsB.addr); busB.write(0x44, 0); // start conversion, with parasite power on at the end
// END bus B --------------------------------------------------------------------------


// BEGIN Handling Setup for bus C -----------------------------------------------------
// Search bus C for a sensor
  if ( !busC.search(bsC.addr)) { busC.reset_search(); delay(25); return; }
// CRC for bus C
  if (OneWire::crc8(bsC.addr, 7) != bsC.addr[7]) { Serial.println("CRC on bus C is not valid!"); return; }
// Select sensor type DS18B20 for Bus C
  bsC.type_s = 0;
// Tell sensor on Bus C to fetch a reading
  busC.reset(); busC.select(bsC.addr); busC.write(0x44, 0); // start conversion, with parasite power on at the end
// END bus C --------------------------------------------------------------------------


// BEGIN Handling Setup for bus D -----------------------------------------------------
// Search bus D for a sensor
  if ( !busD.search(bsD.addr)) { busD.reset_search(); delay(25); return; }
// CRC for bus D
  if (OneWire::crc8(bsD.addr, 7) != bsD.addr[7]) { Serial.println("CRC on bus D is not valid!"); return; }
// Select sensor type DS18B20 for Bus D
  bsD.type_s = 0;
// Tell sensor on Bus D to fetch a reading
  busD.reset(); busD.select(bsD.addr); busD.write(0x44, 0); // start conversion, with parasite power on at the end
// END bus D --------------------------------------------------------------------------


// BEGIN Handling Setup for bus E -----------------------------------------------------
// Search bus E for a sensor
  if ( !busE.search(bsE.addr)) { busE.reset_search(); delay(25); return; }
// CRC for bus E
  if (OneWire::crc8(bsE.addr, 7) != bsE.addr[7]) { Serial.println("CRC on bus E is not valid!"); return; }
// Select sensor type DS18B20 for Bus E
  bsE.type_s = 0;
// Tell sensor on Bus E to fetch a reading
  busE.reset(); busE.select(bsE.addr); busE.write(0x44, 0); // start conversion, with parasite power on at the end
// END bus E --------------------------------------------------------------------------


// Delay for all buses
  delay(380); // 750ms for 12bit resolution


// BEGIN Finishing data collection from bus A -----------------------------------------
// Proceed for Bus A
  bsA.present = busA.reset(); busA.select(bsA.addr); busA.write(0xBE); // Read Scratchpad of sensor on Bus A
// Housekeeping for Bus A
  for ( bsA.i = 0; bsA.i < 9; bsA.i++) { /* <-- We need 9 bytes*/ bsA.data[bsA.i] = busA.read(); }

// Resolution configuration for bus A
  bsA.raw = (bsA.data[1] << 8) | bsA.data[0];
  if (bsA.type_s) {
    bsA.raw = bsA.raw << 3; // 9 bit resolution default
    if (bsA.data[7] == 0x10) {
      bsA.raw = (bsA.raw & 0xFFF0) + 12 - bsA.data[6];
    }
  } else {
    bsA.cfg = (bsA.data[4] & 0x40);
    if (bsA.cfg == 0x00) bsA.raw = bsA.raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (bsA.cfg == 0x20) bsA.raw = bsA.raw & ~3; // 10 bit res, 187.5 ms
    else if (bsA.cfg == 0x40) bsA.raw = bsA.raw & ~1; // 11 bit res, 375 ms
  }
// END Finishing data collection from bus A -----------------------------------------


// BEGIN Finishing data collection from bus B -----------------------------------------
// Proceed for Bus B
  bsB.present = busB.reset(); busB.select(bsB.addr); busB.write(0xBE); // Read Scratchpad of sensor on Bus B
// Housekeeping for Bus B
  for ( bsB.i = 0; bsB.i < 9; bsB.i++) { /* <-- We need 9 bytes*/ bsB.data[bsB.i] = busB.read(); }

// Resolution configuration for bus B
  bsB.raw = (bsB.data[1] << 8) | bsB.data[0];
  if (bsB.type_s) {
    bsB.raw = bsB.raw << 3; // 9 bit resolution default
    if (bsB.data[7] == 0x10) {
      bsB.raw = (bsB.raw & 0xFFF0) + 12 - bsB.data[6];
    }
  } else {
    bsB.cfg = (bsB.data[4] & 0x40);
    if (bsB.cfg == 0x00) bsB.raw = bsB.raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (bsB.cfg == 0x20) bsB.raw = bsB.raw & ~3; // 10 bit res, 187.5 ms
    else if (bsB.cfg == 0x40) bsB.raw = bsB.raw & ~1; // 11 bit res, 375 ms
  }
// END Finishing data collection from bus B -----------------------------------------


// BEGIN Finishing data collection from bus C -----------------------------------------
// Proceed for Bus C
  bsC.present = busC.reset(); busC.select(bsC.addr); busC.write(0xBE); // Read Scratchpad of sensor on Bus C
// Housekeeping for Bus C
  for ( bsC.i = 0; bsC.i < 9; bsC.i++) { /* <-- We need 9 bytes*/ bsC.data[bsC.i] = busC.read(); }

// Resolution configuration for bus C
  bsC.raw = (bsC.data[1] << 8) | bsC.data[0];
  if (bsC.type_s) {
    bsC.raw = bsC.raw << 3; // 9 bit resolution default
    if (bsC.data[7] == 0x10) {
      bsC.raw = (bsC.raw & 0xFFF0) + 12 - bsC.data[6];
    }
  } else {
    bsC.cfg = (bsC.data[4] & 0x40);
    if (bsC.cfg == 0x00) bsC.raw = bsC.raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (bsC.cfg == 0x20) bsC.raw = bsC.raw & ~3; // 10 bit res, 187.5 ms
    else if (bsC.cfg == 0x40) bsC.raw = bsC.raw & ~1; // 11 bit res, 375 ms
  }
// END Finishing data collection from bus C -----------------------------------------


// BEGIN Finishing data collection from bus D -----------------------------------------
// Proceed for Bus D
  bsD.present = busD.reset(); busD.select(bsD.addr); busD.write(0xBE); // Read Scratchpad of sensor on Bus D
// Housekeeping for Bus D
  for ( bsD.i = 0; bsD.i < 9; bsD.i++) { /* <-- We need 9 bytes*/ bsD.data[bsD.i] = busD.read(); }

// Resolution configuration for bus D
  bsD.raw = (bsD.data[1] << 8) | bsD.data[0];
  if (bsD.type_s) {
    bsD.raw = bsD.raw << 3; // 9 bit resolution default
    if (bsD.data[7] == 0x10) {
      bsD.raw = (bsD.raw & 0xFFF0) + 12 - bsD.data[6];
    }
  } else {
    bsD.cfg = (bsD.data[4] & 0x40);
    if (bsD.cfg == 0x00) bsD.raw = bsD.raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (bsD.cfg == 0x20) bsD.raw = bsD.raw & ~3; // 10 bit res, 187.5 ms
    else if (bsD.cfg == 0x40) bsD.raw = bsD.raw & ~1; // 11 bit res, 375 ms
  }
// END Finishing data collection from bus D -----------------------------------------


// BEGIN Finishing data collection from bus E -----------------------------------------
// Proceed for Bus E
  bsE.present = busE.reset(); busE.select(bsE.addr); busE.write(0xBE); // Read Scratchpad of sensor on Bus E
// Housekeeping for Bus E
  for ( bsE.i = 0; bsE.i < 9; bsE.i++) { /* <-- We need 9 bytes*/ bsE.data[bsE.i] = busE.read(); }

// Resolution configuration for bus E
  bsE.raw = (bsE.data[1] << 8) | bsE.data[0];
  if (bsE.type_s) {
    bsE.raw = bsE.raw << 3; // 9 bit resolution default
    if (bsE.data[7] == 0x10) {
      bsE.raw = (bsE.raw & 0xFFF0) + 12 - bsE.data[6];
    }
  } else {
    bsE.cfg = (bsE.data[4] & 0x40);
    if (bsE.cfg == 0x00) bsE.raw = bsE.raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (bsE.cfg == 0x20) bsE.raw = bsE.raw & ~3; // 10 bit res, 187.5 ms
    else if (bsE.cfg == 0x40) bsE.raw = bsE.raw & ~1; // 11 bit res, 375 ms
  }
// END Finishing data collection from bus E -----------------------------------------  
// END Temperature collection 

// BEGIN Temperature formatting and conversion prep for all busses
bsA.celsius = (float)bsA.raw / 16.0;
bsA.fahrenheit = bsA.celsius * 1.8 + 32.0;

bsB.celsius = (float)bsB.raw / 16.0;
bsB.fahrenheit = bsB.celsius * 1.8 + 32.0;

bsC.celsius = (float)bsC.raw / 16.0;
bsC.fahrenheit = bsC.celsius * 1.8 + 32.0;

bsD.celsius = (float)bsD.raw / 16.0;
bsD.fahrenheit = bsD.celsius * 1.8 + 32.0;

bsE.celsius = (float)bsE.raw / 16.0;
bsE.fahrenheit = bsE.celsius * 1.8 + 32.0;
// END Temperature formatting and conversion prep for all busses


// BEGIN Ethernet instantiation and temperature display.

  // Listen for incoming http requests and respond...
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;

    while (client.connected()) {

      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply

        // BEGIN Micro-server response
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          // IMPORTANT!!! There MUST be an empty line after the response and before the html document for this server to work.
          client.println();
          // IMPORTANT!!! Do NOT remove the client.println(); statement above this line.
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

          client.println("<div class=\"tempContainer\" style=\"font-family: helvetica; font-style: normal;\">");
          

client.println("<table style=\"width:30%\">");
  
  client.println("<tbody>");
    
    client.println("<tr>");
      client.println("<td colspan=\"3\" align=\"center\">AC Operating Details</td>");
    client.println("</tr>");
    
    client.println("<tr>");
      client.println("<th>Subsystem</th>");
      client.println("<th>Degrees Celsius</th>");
      client.println("<th>Degrees Fahrenheit</th>");
    client.println("</tr>");
  
    client.println("<tr>");
      client.println("<td>Air Inlet Temp</td>");
      
      client.print("<td>");
        client.print(bsA.celsius);
      client.println("</td>");
      
      client.print("<td>");
        client.print(bsA.fahrenheit);
      client.println("</td>");
    client.println("</tr>");
  
    client.println("<tr>");
      client.println("<td>Air Outlet Temp</td>");
      
      client.print("<td>");
        client.print(bsB.celsius);
      client.println("</td>");
      
      client.print("<td>");
        client.print(bsB.fahrenheit);
      client.println("</td>");
    client.println("</tr>");
  
    client.println("<tr>");
      client.println("<td>Condenser Coil Temp</td>");
      
      client.print("<td>");
        client.print(bsC.celsius);
      client.println("</td>");
      
      client.print("<td>");
        client.print(bsC.fahrenheit);
      client.println("</td>");
    client.println("</tr>");


    client.println("<tr>");
      client.println("<td>Water Inlet Temp</td>");
      
      client.print("<td>");
        client.print(bsD.celsius);
      client.println("</td>");
      
      client.print("<td>");
        client.print(bsD.fahrenheit);
      client.println("</td>");
    client.println("</tr>");


    client.println("<tr>");
      client.println("<td>Water Outlet Temp</td>");
      
      client.print("<td>");
        client.print(bsE.celsius);
      client.println("</td>");
      
      client.print("<td>");
        client.print(bsE.fahrenheit);
      client.println("</td>");
    client.println("</tr>");


  client.println("</tbody>");
client.println("</table>");


          client.println("</html>");
          break;
        }


        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      // END Micro-webserver response


      } //End of while client available
    } //End of while client connected

    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  } //End of ethernet client








}
