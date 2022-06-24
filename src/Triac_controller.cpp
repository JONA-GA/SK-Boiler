#include "Triac_controller.h"




TriacController::TriacController(bool auto_initialize, String config_path, const char* sk_sync_paths[]) :
   IntegerTransform(config_path), auto_initialize_{auto_initialize} {

  if (sk_sync_paths != NULL) {
      sync_paths.clear();
      int i = 0;
      while (strlen(sk_sync_paths[i]) > 0) {
          SyncPath path(sk_sync_paths[i]);
          sync_paths.insert(path);
          i++;
      } // while
  }

  load_configuration();
}


void TriacController::start() {
    if (auto_initialize_) {
       this->emit(is_on);
    }
    debugD("****** Triac  st : %d", is_on);
}

void TriacController::set_input(int new_value, uint8_t input_channel) {
   is_on = new_value;
   this->emit(is_on);
   debugD("****** Triac  set : %d", new_value);
   // Sync any specified sync paths...
    for (auto& path : sync_paths) {
      debugD("Sync status to %s", path.sk_sync_path.c_str());
      path.put_request->set_input(is_on);
    }
}

void TriacController::get_configuration(JsonObject& root) {
  JsonArray jPaths = root.createNestedArray("sync_paths");
  for (auto& path : sync_paths) {
    jPaths.add(path.sk_sync_path);
  }
}

static const char SCHEMA[] PROGMEM = R"({
    "type": "object",
    "properties": {
        "sync_paths": { "title": "Boiler Control ",
                        "type": "array",
                        "items": { "type": "string"}
        }
    }
  })";

String TriacController::get_config_schema() { return FPSTR(SCHEMA); }

bool TriacController::set_configuration(const JsonObject& config) {

  JsonArray arr = config["sync_paths"];
  if (arr.size() > 0) {
    sync_paths.clear();
    for (String sk_path : arr) {
      SyncPath path(sk_path);
      sync_paths.insert(path);
    }
  }

  return true;
}

TriacController::SyncPath::SyncPath(String sk_sync_path)
    : sk_sync_path{sk_sync_path} {
   debugD("Boiler will also sync %s", sk_sync_path.c_str());
   this->put_request = new IntSKPutRequest(sk_sync_path );
}
