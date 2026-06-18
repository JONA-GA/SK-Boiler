#include "Triac_controller.h"




TriacController::TriacController(bool auto_initialize, String config_path, const char* sk_sync_paths[]) 
   : FileSystemSaveable(config_path), auto_initialize_{auto_initialize} {

  if (sk_sync_paths != NULL) {
      sync_paths_.clear();
      int i = 0;
      while (strlen(sk_sync_paths[i]) > 0) {
          SyncPath path(sk_sync_paths[i]);
          sync_paths_.insert(path);
          i++;
      } // while
  }

 load();
 // Emit the initial state once the event loop starts
  if (auto_initialize_) {
    event_loop()->onDelay(10000, [this]() { this->emit(is_on_); });
    ESP_LOGI(__FILENAME__,"skboiler-Triac controller new value: %d", is_on_);
  }
}


bool TriacController::to_json(JsonObject& root) {
  JsonArray jPaths = root["sync_paths"].to<JsonArray>();
  for (auto& path : sync_paths_) {
    jPaths.add(path.sk_sync_path_);
  }
  return true;
}

bool TriacController::from_json(const JsonObject& config) {
  JsonArray arr = config["sync_paths"];
  if (arr.size() > 0) {
    sync_paths_.clear();
    for (String sk_path : arr) {
      SyncPath path(sk_path);
      sync_paths_.insert(path);
    }
  }

  return true;
}

TriacController::SyncPath::SyncPath() {}

TriacController::SyncPath::SyncPath(String sk_sync_path)
    : sk_sync_path_{sk_sync_path} {
  ESP_LOGD(__FILENAME__, "DoubleClick will also sync %s", sk_sync_path.c_str());
  this->put_request_ = std::make_shared<IntSKPutRequest>(sk_sync_path, "", false);
}
