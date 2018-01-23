/*
 * Copyright (C) 2016, 2017 "IoT.bzh"
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
#include "roverdriving-service-binding.hpp"

#include <cstddef>
#include <cstdio>
#include <cstring>

#include <json-c/json.h>

#include <service/{{ class_name }}.h>

static {{ class_name }} obj;

static int init()
{
	AFB_NOTICE("Initializing... ");
  // Do something
	AFB_NOTICE("Initialization done!");
	return 0;
}

{% for verb_name, verb_desc in verbs.items() %}
{% for verb_type, verb_body in verb_desc.items() %}
static void {{ verb_name }}(struct afb_req request) {
  json_object *args = afb_req_json(request), *val = NULL;

  AFB_NOTICE("[{{ api_name }}] Calling {{ verb_name }}");

  {% if verb_type == 'post' %}
  {% if 'body' in verb_body %}
  {% if 'application/json' in verb_body['body'] %}
  {% if 'properties' in verb_body['body']['application/json'] %}
    {% for param_key, param in verb_body['body']['application/json']['properties'].items() %}
      if (!json_object_object_get_ex(args, "{{ param_key }}", &val)) {
        AFB_ERROR("[{{ api_name }}] No {{ param_key }} param provided");
        afb_req_fail(request, "bad-request", "No {{ param_key }} param provided");
        return;
      }
    {% endfor %}
  {% endif %}
  {% endif %}
  {% endif %}
  {% endif %}

  obj.{{ verb_name }}(
    {% if verb_type == 'post' %}
      {% if 'body' in verb_body %}
      {% if 'application/json' in verb_body['body'] %}
      {% if 'properties' in verb_body['body']['application/json'] %}
        {% for param_key, param in verb_body['body']['application/json']['properties'].items() %}
          json_object_object_get_ex(args, "{{ param_key }}", &val) ? {{ param['type']|json_get_fn }}(val) : static_cast<{{ param['type']|ramltype_to_cpp }}>(0){% if not loop.last %}, {% endif %}
        {% endfor %}
      {% endif %}
      {% endif %}
      {% endif %}
    {% endif %}
  );

  afb_req_success(request, json_object_get(args), NULL);
}

{% endfor %}
{% endfor %}

// static const struct afb_auth _afb_auths_v2_monitor[] = {
// 	{type : afb_auth_Permission, text : "urn:AGL:permission:monitor:public:set"},
// 	{type : afb_auth_Permission, text : "urn:AGL:permission:monitor:public:get"},
// 	{type : afb_auth_Or, first : &_afb_auths_v2_monitor[1], next : &_afb_auths_v2_monitor[0]}
// };

static const struct afb_verb_v2 verbs[] = {
	/*Without security*/
  {% for verb_name, verb_desc in verbs.items() %}
	{.verb = "{{ verb_name }}", .callback = {{ verb_name }}, .auth = NULL, .info = "Auto Generated - {{ verb_name }}", .session = 0},
  {% endfor %}
	{.verb= NULL, .callback=NULL, .auth = NULL, .info = NULL, .session = 0 }
};

const struct afb_binding_v2 afbBindingV2 = {
	.api = "{{ api_name }}",
	.specification = "",
  .info = "Auto generated - {{ api_name }}",
	.verbs = verbs,
	.preinit = NULL,
	.init = init,
	.onevent = NULL,
	.noconcurrency = 1
};
