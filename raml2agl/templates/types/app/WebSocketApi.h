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
#ifndef __RAML2AGL_WEB_SOCKET_API_H_
#define __RAML2AGL_WEB_SOCKET_API_H_

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

class WebSocketApi {
public:
  WebSocketApi(const char * uri, const char * api_name);

  ~WebSocketApi();

protected:
  json_object * emit(const char *verb, const char *object);
  bool connected;

private:

  /* the callback interface for wsj1 */
  static struct afb_wsj1_itf wsj1_itf;

  static struct afb_wsj1 *wsj1;

  static int exonrep;
  int callcount;
  static sd_event *loop;
  bool reply;
  const char * uri;
  const char * api_name;
  json_object * curr_reply;

  /* called when wsj1 hangsup */
  static void on_wsj1_hangup(void *closure, struct afb_wsj1 *wsj1);

  /* called when wsj1 receives a method invocation */
  static void on_wsj1_call(void *closure, const char *api, const char *verb, struct afb_wsj1_msg *msg);

  /* called when wsj1 receives an event */
  static void on_wsj1_event(void *closure, const char *event, struct afb_wsj1_msg *msg);

  /* called when wsj1 receives a reply */
  static void on_wsj1_reply(void *closure, struct afb_wsj1_msg *msg);

  /* makes a call */
  void wsj1_call(const char *api, const char *verb, const char *object);

  /* decrement the count of calls */
  void dec_callcount();


};

#endif //__RAML2AGL_WEB_SOCKET_API_H_
