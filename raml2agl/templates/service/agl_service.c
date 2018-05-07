{% include 'c-license.c' %}
{% from 'service/macros.c' import list_fn_params_call_json %}

extern "C"
{
   #define AFB_BINDING_VERSION 2
   #include <afb/afb-binding.h>
};


#include <cstddef>
#include <cstdio>
#include <cstring>

#include <json-c/json.h>

#include <service/{{ model['service_class_name'] }}.h>
// #include <all_types.h>

static {{ model['service_class_name'] }} obj;

static int init()
{
  int rc = 0;
	AFB_NOTICE("Initializing... ");
  rc = obj.init();
	AFB_NOTICE("Initialization done!");
	return rc;
}

{% for verb_name, verb_desc in model['methods'].items() %}
static void {{ verb_name }}(struct afb_req request) {
  json_object *args = afb_req_json(request);
  {% for param in verb_desc['out_params'] %}
  {% if param['type'] == 'string' %}const {% endif %}
  {{ param['type']|ramltype_to_cpp }} _var_{{ param['name'] }} = static_cast<{{ param['type']|ramltype_to_cpp }}>(0);
  {% endfor %}
  {% if verb_desc['out_params']|length > 0 %}
  json_object * new_json = json_object_new_object();
  json_object * new_sub_json = NULL;
  {% endif %}
  {% if verb_desc['in_params']|length > 0 %}
  json_object *val = NULL;
  {% endif %}
  int ret = 0;

  AFB_NOTICE("[{{ model['api_name'] }}] Calling {{ verb_name }}");

  {% if verb_desc['in_params']|length > 0 %}
  if (args) {
  {% for param in verb_desc['in_params'] %}
      if (!json_object_object_get_ex(args, "{{ param['name'] }}", &val)) {
        AFB_ERROR("[{{ model['api_name'] }}] No '{{ param['name'] }}' param provided");
        afb_req_fail(request, "bad-request", "No '{{ param['name'] }}' param provided");
        return;
      }
  {% endfor %}
  }

  {% endif %}
  ret = obj.{{ verb_name }}({{ list_fn_params_call_json(verb_desc, 6) }});
  if (ret) {
    AFB_ERROR("[{{ model['api_name'] }}] Verb '{{ verb_name }}' returning error");
    afb_req_fail_f(request, "bad-request", "Verb '{{ verb_name }}' returning error %d", ret);
    return;
  }

  {% for param in verb_desc['out_params'] %}
  new_sub_json = {{ param['type']|json_new_fn }}(_var_{{ param['name'] }});
  json_object_object_add(new_json, "{{ param['name'] }}", new_sub_json);
  {% endfor %}

  {% if verb_desc['out_params']|length > 0 %}
  afb_req_success(request, new_json, NULL);
  // Release the request json object
  json_object_put(new_json);
  {% else %}
  afb_req_success(request, args, NULL);
  {% endif %}

}

{% endfor %}

// static const struct afb_auth _afb_auths_v2_monitor[] = {
// 	{type : afb_auth_Permission, text : "urn:AGL:permission:monitor:public:set"},
// 	{type : afb_auth_Permission, text : "urn:AGL:permission:monitor:public:get"},
// 	{type : afb_auth_Or, first : &_afb_auths_v2_monitor[1], next : &_afb_auths_v2_monitor[0]}
// };

static const struct afb_verb_v2 verbs[] = {
  /*Without security*/
  {% for verb_name, verb_desc in model['methods'].items() %}
  {% if 'description' in verb_desc %}
  {% set desc = verb_desc['description'] %}
  {% else %}
  {% set desc = "Auto Generated - " ~ verb_name %}
  {% endif %}
  {.verb = "{{ verb_name }}", .callback = {{ verb_name }}, .auth = NULL, .info = "{{ desc }}", .session = 0},
  {% endfor %}
  {.verb= NULL, .callback=NULL, .auth = NULL, .info = NULL, .session = 0 }
};

const struct afb_binding_v2 afbBindingV2 = {
  .api = "{{ model['api_name'] }}",
  .specification = "",
  .info = "Auto generated - {{ model['title'] }}",
  .verbs = verbs,
  .preinit = NULL,
  .init = init,
  .onevent = NULL,
  .noconcurrency = 1
};
