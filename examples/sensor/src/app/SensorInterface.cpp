/* Copyright 2018 Pedro Cuadra - pjcuadra@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* WARNING: This file is automatically generated by raml2agl */
#include <app/SensorInterface.h>

SensorInterface::SensorInterface() : WebSocketApi("localhost:8000/api?token=x", "sensorinterface") {

}

/** Autogenrated doc for getdata */
int SensorInterface::getdata(double &out_sensor_data,
    int &out_sensor_id) {
  json_object * rec = NULL;
  json_object * req = NULL;
  json_object * val = NULL;
  json_object * response = NULL;
  int rc = 0;

  // Get the response
  rec = this->emit("getdata", req ? json_object_to_json_string(req) : NULL);
  if (!rec) {
    printf("ERROR: Posting error or timeout while waiting for reply\n");
    return -1;
  }

  rc = json_object_object_get_ex(rec, "response", &response);
  if (!rc) {
    printf("ERROR: Malformed response\n");
    json_object_put(rec);
    json_object_put(req);
    return -1;
  }

  rc = json_object_object_get_ex(response, "sensor_data", &val);
  out_sensor_data = rc ? json_object_get_double(val) : static_cast<double>(0);
  if (!rc) {
    printf("ERROR: Malformed response\n");
    json_object_put(rec);
    json_object_put(req);
    return -1;
  }
  rc = json_object_object_get_ex(response, "sensor_id", &val);
  out_sensor_id = rc ? json_object_get_int(val) : static_cast<int>(0);
  if (!rc) {
    printf("ERROR: Malformed response\n");
    json_object_put(rec);
    json_object_put(req);
    return -1;
  }

  printf("INFO: %s\n", json_object_to_json_string_ext(rec, JSON_C_TO_STRING_PRETTY));

  json_object_put(req);
  json_object_put(rec);

  return 0;
}

/** Autogenrated doc for setupsensor */
int SensorInterface::setupsensor(const int in_param1,
    const char * in_param2) {
  json_object * rec = NULL;
  json_object * req = NULL;
  json_object * new_sub_json = NULL;
  json_object * response = NULL;
  int rc = 0;

  req =  json_object_new_object();
  new_sub_json = json_object_new_int(in_param1);
  json_object_object_add(req, "param1", new_sub_json);
  new_sub_json = json_object_new_string(in_param2);
  json_object_object_add(req, "param2", new_sub_json);
  printf("INFO: Verb 'setupsensor' sending %s\n", json_object_to_json_string(req));
  // Get the response
  rec = this->emit("setupsensor", req ? json_object_to_json_string(req) : NULL);
  if (!rec) {
    printf("ERROR: Posting error or timeout while waiting for reply\n");
    return -1;
  }

  rc = json_object_object_get_ex(rec, "response", &response);
  if (!rc) {
    printf("ERROR: Malformed response\n");
    json_object_put(rec);
    json_object_put(req);
    return -1;
  }


  printf("INFO: %s\n", json_object_to_json_string_ext(rec, JSON_C_TO_STRING_PRETTY));

  json_object_put(req);
  json_object_put(rec);

  return 0;
}
