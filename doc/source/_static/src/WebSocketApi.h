/*
 * Copyright (C) 2015, 2016, 2017 "IoT.bzh"
 * Author "Fulup Ar Foll"
 * Author José Bollo <jose.bollo@iot.bzh>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include <systemd/sd-event.h>
#include <json-c/json.h>

extern "C" {
  #include <afb/afb-wsj1.h>
	#include <afb/afb-ws-client.h>
}

/**
 * Handle Unix Web Socket connection and transmission
 */
class WebSocketApi {
public:
  /**
   * Constructor
   *
   * Creates Unix Web Socket connection and initialize the wait loop
   *
   * @param uri Base uri to the web socket
   * @param api_name API name
   */
  WebSocketApi(const char * uri, const char * api_name);

  /**
   * Destructor
   *
   * Releases the resources and disconnect from the Unix Web Socket
   *
   */
  ~WebSocketApi();

protected:

  /**
   * Send string to the specified API's verb
   *
   * @param verb API's verb
   * @param object Marshaled JSON object
   * @return Reply JSON object
   */
  json_object * emit(const char *verb, const char *object);

  /**
   * Flags connection status
   */
  bool connected;

private:

  /**
   * The Web Socket callback interface for wsj1
   */
  static struct afb_wsj1_itf wsj1_itf;

  /**
   * The Web Socket connection object
   */
  static struct afb_wsj1 *wsj1;

  /**
   * The Web Socket connection object
   */
  static int exonrep;

  /**
   * Calls Reference counter
   */
  static int callcount;

  /**
   * Wait loop event
   */
  static sd_event *loop;

  /**
   * Flags the presens of a reply
   */
  static bool reply;

  /**
   * Base URI of the API
   */
  const char * uri;

  /**
   * API name
   */
  const char * api_name;

  /**
   * Last received JSON object
   */
  static json_object * curr_reply;


  /**
   * Decrement the reference count of calls
   */
  static void dec_callcount();

  /**
   * Hang up callback
   * @param closure Hangup's closure
   * @param wsj1 Connection object
   */
  static void on_wsj1_hangup(void *closure, struct afb_wsj1 *wsj1);

  /**
   * Receives a method invocation callback
   * @param closure Call's closure
   * @param api API Name
   * @param verb API's verb
   * @param msg Message to be sent
   */
  static void on_wsj1_call(void *closure, const char *api, const char *verb, struct afb_wsj1_msg *msg);

  /**
   * Receive an event callback
   * @param closure Event's closure
   * @param event Issued event
   * @param msg Received message
   */
  static void on_wsj1_event(void *closure, const char *event, struct afb_wsj1_msg *msg);

  /**
   * Receive a reply callback
   * @param closure Reply's closure
   * @param msg Replied message
   */
  static void on_wsj1_reply(void *closure, struct afb_wsj1_msg *msg);

  /**
   * Send a marshaled object to the specified API and API's verb
   * @param api API name
   * @param verb API's verb
   * @param object Marshalled JSON object
   * @return Return POSIX error codes
   */
  static int wsj1_call(const char *api, const char *verb, const char *object);

};
