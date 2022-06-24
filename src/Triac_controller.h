#ifndef _Triac_controller_h
#define _Triac_controller_h



#include "sensesp/signalk/signalk_put_request.h"
#include "sensesp/system/valueconsumer.h"
#include "sensesp/transforms/transform.h"

using namespace sensesp;


/**
 * @brief A high level transform designed to control a digital output
 * (such as a relay) via manual button presses or programatic commands. 
 * 
 * To accomplish this, the controller accepts inputs from a generic int
 * producer (usually a SignalK listener).
 * 
 * <p>A TriacController object behaves differently depending on the type of 
 * input it receives. If the input is a boolean or a "valid truth type" 
 * (which is a specialized type of boolean - see below), TriacController's 
 * output will be "on" if the input is "true", or "off" if the input is "false". 
 * 
 *
 * 
 */
class TriacController : public IntegerTransform {

     public:
       /**
        * The constructor
        * @param auto_initialize If TRUE, the controller will emit an
        *  initial "off" status when enabled. This is generally the
        *  desired case unless this controller is mirroring the state
        *  of a remote load.
        * @param config_path The path to save configuration data (blank for
        *   no saving)
        * @param sk_sync_paths An optional array of Signal K paths that should
        *   synchronize their status whenever a double click ClickType is received.
        *   Each path listed will have a PUT request issued to set the status to
        *   be the same as this TriacController each time a double click occurs.
        *   This list, if specified, should have a zero length string as its last entry.
        */
       TriacController(bool auto_initialize = true, String config_path = "", const char* sk_sync_paths[] = NULL);
       void start() override;
       void set_input(int new_value, uint8_t input_channel = 0) override;
       //void set_input(String new_value, uint8_t input_channel = 0) override;
       //void set_input(ClickTypes new_value, uint8_t input_channel = 0) override;


       // For reading and writing the configuration of this transformation
       virtual void get_configuration(JsonObject& doc) override;
       virtual bool set_configuration(const JsonObject& config) override;
       virtual String get_config_schema() override;

     public:
       /// Used to store configuration internally.
       class SyncPath {
          public:
            String sk_sync_path;
            IntSKPutRequest* put_request;

            SyncPath(String sk_sync_path);

            friend bool operator<(const SyncPath& lhs, const SyncPath& rhs) {
             return lhs.sk_sync_path < rhs.sk_sync_path;
           }
       };

     protected:
       int is_on = 0;
       bool auto_initialize_;
       std::set<SyncPath> sync_paths;
};
#endif