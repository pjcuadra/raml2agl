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
  {{ maps['type_to_cpp'][param['type']] }} _var_{{ param['name'] }}{% if param['type'] != 'string' %} = static_cast<{{ maps['type_to_cpp'][param['type']] }}>(0){% endif %};
  {% endfor %}
  {% if verb_desc['out_params']|length > 0 %}
  json_object * new_json = json_object_new_object();
  json_object * new_sub_json = NULL;
  {% endif %}
  {% if verb_desc['in_params']|length > 0 %}
  json_object *val[{{ verb_desc['in_params']|length }}];
  {% endif %}
  {% for param in verb_desc['in_params'] %}
  {% if param['array'] %}
  json_object *val_{{ param['name'] }}_size_obj = NULL;
  int _var_{{ param['name'] }}_size = 0;
  {{ maps['type_to_cpp'][param['type']] }} * _var_{{ param['name'] }} = nullptr;
  {% endif %}
  {% endfor %}
  {% for param in verb_desc['out_params'] %}
  {% if param['array'] %}
  {{ maps['type_to_cpp'][param['type']] }} * _var_out_{{ param['name'] }} = nullptr;
  json_object *val_{{ param['name'] }}_size_obj = NULL;
  int _var_in_{{ param['name'] }}_size = 0;
  {% endif %}
  {% endfor %}
  int ret = 0;

  AFB_NOTICE("[{{ model['api_name'] }}] Calling {{ verb_name }}");


  if (args) {
    // Parse args if possible
  {% if verb_desc['in_params']|length > 0 %}
  {% for param in verb_desc['in_params'] %}
    if (!json_object_object_get_ex(args, "{{ param['name'] }}", &val[{{ loop.index - 1 }}])) {
      AFB_ERROR("[{{ model['api_name'] }}] No '{{ param['name'] }}' param provided");
      afb_req_fail(request, "bad-request", "No '{{ param['name'] }}' param provided");
      return;
    }
    {% if param['array'] %}
    if (!json_object_object_get_ex(args, "{{ param['name'] }}_size", &val_{{ param['name'] }}_size_obj)) {
      AFB_ERROR("[{{ model['api_name'] }}] No '{{ param['name'] }}_size' param provided");
      afb_req_fail(request, "bad-request", "No '{{ param['name'] }}_size' param provided");
      return;
    }

    _var_{{ param['name'] }}_size = json_object_get_int(val_{{ param['name'] }}_size_obj);

    _var_{{ param['name'] }} = new {{ maps['type_to_cpp'][param['type']] }}[_var_{{ param['name'] }}_size];

    for (int i = 0; i < _var_{{ param['name'] }}_size; i++) {
      _var_{{ param['name'] }}[i] = static_cast<{{ maps['type_to_cpp'][param['type']] }}>({{ maps['type_to_json_get_fn'][param['type']] }}(json_object_array_get_idx(val[{{ loop.index - 1 }}], i)));
    }

    {% endif %}
  {% endfor %}
  {% endif %}

  {% for param in verb_desc['out_params'] %}
  {% if param['array'] %}
    if (!json_object_object_get_ex(args, "{{ param['name'] }}_size", &val_{{ param['name'] }}_size_obj)) {
      AFB_ERROR("[{{ model['api_name'] }}] No '{{ param['name'] }}_size' param provided");
      afb_req_fail(request, "bad-request", "No '{{ param['name'] }}_size' param provided");
      return;
    }

    _var_in_{{ param['name'] }}_size = json_object_get_int(val_{{ param['name'] }}_size_obj);

    _var_out_{{ param['name'] }} = new {{ maps['type_to_cpp'][param['type']] }}[_var_in_{{ param['name'] }}_size];
  {% endif %}
  {% endfor %}

  }


  ret = obj.{{ verb_name }}({{ list_fn_params_call_json(verb_desc, maps, 6) }});
  if (ret) {
    AFB_ERROR("[{{ model['api_name'] }}] Verb '{{ verb_name }}' returning error");
    afb_req_fail_f(request, "bad-request", "Verb '{{ verb_name }}' returning error %d", ret);
    return;
  }

  {% for param in verb_desc['in_params'] %}
  {% if param['array'] %}
  delete [] _var_{{ param['name'] }};
  {% endif %}
  {% endfor %}

  {% for param in verb_desc['out_params'] %}
  {% if param['array'] %}
  new_sub_json = json_object_new_array();
  for (int i = 0; i < _var_in_{{ param['name'] }}_size; i++) {
    json_object_array_put_idx(new_sub_json, i, {{ maps['type_to_json_new_fn'][param['type']] }}(_var_out_{{ param['name'] }}[i]));
  }
  json_object_object_add(new_json, "{{ param['name'] }}", new_sub_json);

  delete [] _var_out_{{ param['name'] }};

  {% else %}
  new_sub_json = {{ maps['type_to_json_new_fn'][param['type']] }}(_var_{{ param['name'] }}{% if param['type'] == 'string' %}.c_str(){% endif %});
  json_object_object_add(new_json, "{{ param['name'] }}", new_sub_json);
  {% endif %}
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
