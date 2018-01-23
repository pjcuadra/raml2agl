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
{% for verb_type, verb_body in verb_desc.items() %}
static void {{ verb_name }}(struct afb_req request) {
  json_object *args = afb_req_json(request), *val = NULL;

  AFB_NOTICE("[{{ api_name }}] Calling {{ verb_name }}");

  {% call(param_key, param, is_last = False) iterate_post_fn_params(verb_body) %}
      if (!json_object_object_get_ex(args, "{{ param_key }}", &val)) {
        AFB_ERROR("[{{ api_name }}] No {{ param_key }} param provided");
        afb_req_fail(request, "bad-request", "No {{ param_key }} param provided");
        return;
      }
  {% endcall %}

  obj.{{ verb_name }}({% call(param_key, param, is_last = False) iterate_post_fn_params(verb_body) %}
          json_object_object_get_ex(args, "{{ param_key }}", &val) ? {{ param['type']|json_get_fn }}(val) : static_cast<{{ param['type']|ramltype_to_cpp }}>(0){% if not is_last %}, {% endif %}{% endcall %});

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
