// Signal K application template file.
//
// This application demonstrates core SensESP concepts in a very
// concise manner. You can build and upload the application as is
// and observe the value changes on the serial port monitor.
//
// You can use this source file as a basis for your own projects.
// Remove the parts that are not relevant to you, and add your own code
// for external hardware libraries.


#include "sensesp.h"

#include <string>
#include "DEV_Config.h"
#include "Triac_controller.h"
#include "SCR_Drive.h"
#include "sensesp/signalk/signalk_listener.h"
#include "sensesp/signalk/signalk_value_listener.h"
#include "sensesp/signalk/signalk_output.h"
#include "sensesp/signalk/signalk_put_request_listener.h"
#include "sensesp/transforms/repeat.h"
#include "sensesp_app.h"
#include "sensesp_app_builder.h"







using namespace sensesp;





// The setup function performs one-time application initialization. 

void setup() {
  SetupLogging(ESP_LOG_DEBUG);

  // Construct the global SensESPApp() object
  SensESPAppBuilder builder;
  sensesp_app = builder.set_hostname("SK-Boiler-Controller")
                    // Optionally, hard-code the WiFi and Signal K server
                    // settings. This is normally not needed.
                    ->set_wifi_client("Blunova1", "Rollotommasi12062022")
                    //->set_wifi_access_point("My AP SSID", "my_ap_password")
                    //->set_sk_server("192.168.0.108", 3010)
                    ->get_app();
  
// Define the SK Path that represents the load this device controls.
  // This device will report its status on this path, as well as
  // respond to PUT requests to change its status.
  // To find valid Signal K Paths that fits your need you look at this link:
  // https://signalk.org/specification/1.4.0/doc/vesselsBranch.html  
  const char* sk_path = "electrical.boiler.state";


  // "Configuration paths" are combined with "/config" to formulate a URL
  // used by the RESTful API for retrieving or setting configuration data.
  // It is ALSO used to specify a path to the SPIFFS file system
  // where configuration data is saved on the MCU board.  It should
  // ALWAYS start with a forward slash if specified.  If left blank,
  // that indicates a sensor or transform does not have any
  // configuration to save.
  //const char* config_path_button_c = "/button/clicktime";
  //const char* config_path_status_light = "/button/statusLight";
  //const char* config_path = "/threshold/lights";
  const char* config_path_sk_sync_t = "/Triac/sync";
  const char* config_path_sk_output = "/signalk/path";
  //const char* config_path_repeat = "/signalk/repeat";



  // Create a digital output that is assumed to be connected to the
  // control channel of a relay or a MOSFET that will control the
  // electric light.  Also connect this pin's state to an LED to get
  // a visual indicator of load's state. 
  auto* triac_switch = new ScrDrive(1);
  

const char* sk_sync_paths[] = { sk_path ,  "" };
const bool auto_init_controller = true;
  // Create a switch controller to handle the user press logic and 
  // connect it to the load switch...
  TriacController* controller = new TriacController();
  controller->connect_to(triac_switch);
  

  // TriacController accepts explicit state settings via 
  // any int producer .
  // Here, we set up a SignalK PUT request listener to handle
  // requests made to the Signal K server to set the switch state.
  // This allows any device on the SignalK network that can make
  // such a request to also control the state of our switch.
  auto* sk_listener = new IntSKPutRequestListener(sk_path);
  sk_listener->connect_to(controller);


  // Finally, connect the load switch to an SKOutput so it reports its state 
  // to the Signal K server.  Since the load switch only reports its state 
  // whenever it changes (and switches like light switches change infrequently), 
  // send it through a `RepeatReport` transform, which will cause the state 
  // to be reported to the server every 10 seconds, regardless of whether 
  // or not it has changed.  That keeps the value on the server fresh and 
  // lets the server know the switch is still alive.
  auto* skMetadata = new SKMetadata("%", 
                                "Boiler control", 
                                "Boiler Power Control", 
                                "Boiler", 
                                -1.0f,
                                true);
  triac_switch ->connect_to(new Repeat<int16_t,int16_t>(10000))
             ->connect_to(new SKOutputInt(sk_path, config_path_sk_output,skMetadata));





  // Start networking, SK server connections and other SensESP internals
  //sensesp_app->start();

   // To avoid garbage collecting all shared pointers created in setup(),
  // loop from here.
  //while (true) {
  //  loop();
  //}
}

void loop() { event_loop()->tick(); }
