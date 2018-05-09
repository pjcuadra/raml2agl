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

#include <app/WebSocketApi.h>

struct afb_wsj1_itf WebSocketApi::wsj1_itf = {
   .on_hangup = WebSocketApi::on_wsj1_hangup,
   .on_call = WebSocketApi::on_wsj1_call,
   .on_event = WebSocketApi::on_wsj1_event
};

int WebSocketApi::exonrep = 0;
sd_event * WebSocketApi::loop = NULL;
struct afb_wsj1 * WebSocketApi::wsj1 = NULL;

WebSocketApi::WebSocketApi(const char * uri, const char * api_name) : uri(uri), api_name(api_name) {
  int rc = 0;

  callcount = 0;
  curr_reply = NULL;
  reply = false;

	/* get the default event loop */
	rc = sd_event_default(&loop);
	if (rc < 0) {
		fprintf(stderr, "connection to default event loop failed: %s\n", strerror(-rc));
		return;
	}

	wsj1 = afb_ws_client_connect_wsj1(loop, uri, &wsj1_itf, this);
	if (wsj1 == NULL) {
		fprintf(stderr, "connection to %s failed: %m\n", uri);
    connected = false;
		return;
	}

  connected = true;
}

WebSocketApi::~WebSocketApi() {
  afb_wsj1_unref(wsj1);
  sd_event_unref(loop);
}


/* decrement the count of calls */
void WebSocketApi::dec_callcount()
{
	callcount--;
	if (exonrep && !callcount)
		exit(0);
}

/* called when wsj1 hangsup */
void WebSocketApi::on_wsj1_hangup(void *closure, struct afb_wsj1 *wsj1)
{
  WebSocketApi * obj = static_cast<WebSocketApi *>(closure);
	printf("ON-HANGUP [%s]\n", obj->api_name);
	fflush(stdout);
	exit(0);
}

/* called when wsj1 receives a method invocation */
void WebSocketApi::on_wsj1_call(void *closure, const char *api, const char *verb, struct afb_wsj1_msg *msg)
{
  WebSocketApi * obj = static_cast<WebSocketApi *>(closure);
  // Do nothing
}

/* called when wsj1 receives an event */
void WebSocketApi::on_wsj1_event(void *closure, const char *event, struct afb_wsj1_msg *msg)
{
  WebSocketApi * obj = static_cast<WebSocketApi *>(closure);
	printf("ON-EVENT [%s] %s:\n%s\n", obj->api_name, event,
				json_object_to_json_string_ext(afb_wsj1_msg_object_j(msg),
							JSON_C_TO_STRING_PRETTY));
	fflush(stdout);
}

/* called when wsj1 receives a reply */
void WebSocketApi::on_wsj1_reply(void *closure, struct afb_wsj1_msg *msg)
{
  WebSocketApi * obj = static_cast<WebSocketApi *>(closure);
  obj->reply = afb_wsj1_msg_is_reply_ok(msg);

  if (obj->reply) {
    obj->curr_reply = afb_wsj1_msg_object_j(msg);
    json_object_get(obj->curr_reply);
  }

	obj->dec_callcount();
}

/* makes a call */
void WebSocketApi::wsj1_call(const char *api, const char *verb, const char *object)
{
	static int num = 0;
	char *key;
	int rc;

	/* allocates an id for the request */
	rc = asprintf(&key, "%d:%s/%s", ++num, api, verb);

	/* send the request */
	callcount++;
	reply = false;
	rc = afb_wsj1_call_s(wsj1, api, verb, object, on_wsj1_reply, this);
	if (rc < 0) {
		fprintf(stderr, "calling %s/%s(%s) failed: %m\n", api, verb, object);
		dec_callcount();
	}
}

/* emits either a call (when api!='!') or an event */
json_object * WebSocketApi::emit(const char *verb, const char *object)
{
	if (object == NULL || object[0] == 0)
		object = "null";

  if (!connected)
    return NULL;

	wsj1_call(api_name, verb, object);

	sd_event_run(loop, 3000000000);

  if (!reply)
    return NULL;

  return this->curr_reply;
}
