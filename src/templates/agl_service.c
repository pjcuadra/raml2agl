{% include 'c-license.c' %}
{% from 'macros.c' import iterate_post_fn_params %}

extern "C"
{
   #define AFB_BINDING_VERSION 2
   #include <afb/afb-binding.h>
};


#include <cstddef>
#include <cstdio>
#include <cstring>

#include <json-c/json.h>

#include <service/{{ class_name }}.h>
#include <all_types.h>

static {{ class_name }} obj;

static int init()
{
	AFB_NOTICE("Initializing... ");
  // Do something
	AFB_NOTICE("Initialization done!");
	return 0;
}

{% for verb_name, verb_desc in verbs.items() %}
static void {{ verb_name }}(struct afb_req request) {
  json_object *args = afb_req_json(request);
  {% if 'post' in verb_desc %}
  json_object *val = NULL;
  {% endif %}

  AFB_NOTICE("[{{ api_name }}] Calling {{ verb_name }}");

  {% if 'post' in verb_desc %}
  if (args) {


  {% call(param_key, param, is_last = False) iterate_post_fn_params(verb_desc['post']) %}
      if (!json_object_object_get_ex(args, "{{ param_key }}", &val)) {
        AFB_ERROR("[{{ api_name }}] No {{ param_key }} param provided");
        afb_req_fail(request, "bad-request", "No {{ param_key }} param provided");
        return;
      }
  {% endcall %}

  obj.{{ verb_name }}({% call(param_key, param, is_last = False) iterate_post_fn_params(verb_desc['post']) %}
          json_object_object_get_ex(args, "{{ param_key }}", &val) ? {{ param['type']|json_get_fn }}(val) : static_cast<{{ param['type']|ramltype_to_cpp }}>(0){% if not is_last %}, {% endif %}{% endcall %});

  }
  {% endif %}

  afb_req_success(request, json_object_get(args), NULL);
}

{% endfor %}

// static const struct afb_auth _afb_auths_v2_monitor[] = {
// 	{type : afb_auth_Permission, text : "urn:AGL:permission:monitor:public:set"},
// 	{type : afb_auth_Permission, text : "urn:AGL:permission:monitor:public:get"},
// 	{type : afb_auth_Or, first : &_afb_auths_v2_monitor[1], next : &_afb_auths_v2_monitor[0]}
// };

static const struct afb_verb_v2 verbs[] = {
	/*Without security*/
  {% for verb_name, verb_desc in verbs.items() %}
  {% if 'description' in verb_desc %}
  {% set desc = verb_desc['description'] %}
  {% else %}
  {% set desc = "Auto Generated - {{ verb_name }}" %}
  {% endif %}
	{.verb = "{{ verb_name }}", .callback = {{ verb_name }}, .auth = NULL, .info = "{{ desc }}", .session = 0},
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
