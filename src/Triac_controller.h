#ifndef _Triac_controller_h
#define _Triac_controller_h



#include "sensesp/signalk/signalk_put_request.h"
#include "sensesp/system/valueconsumer.h"
#include "sensesp/system/lambda_consumer.h"
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
class TriacController : public ValueProducer<int>, FileSystemSaveable {

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
       
        // For reading and writing the configuration of this transformation
  virtual bool to_json(JsonObject& doc) override;
  virtual bool from_json(const JsonObject& config) override;

LambdaConsumer<int> Triac_consumer_{[this](int value) {
    this->is_on_ = value;
    this->emit(is_on_);
  }};
     
       /// Used to store configuration internally.
  class SyncPath {
   public:
    String sk_sync_path_;
    std::shared_ptr<IntSKPutRequest> put_request_;

    SyncPath();
    SyncPath(String sk_sync_path);

    friend bool operator<(const SyncPath& lhs, const SyncPath& rhs) {
      return lhs.sk_sync_path_ < rhs.sk_sync_path_;
    }
 };
     protected:
  bool is_on_ = false;
  bool auto_initialize_;
  std::set<SyncPath> sync_paths_;
};
inline const String ConfigSchema(const TriacController& obj) {
  return R"({"type":"object","properties":{"sync_paths":{"title":"Sync on double click","type":"array","items":{"type":"string"}}}  })";
}
       
#endif