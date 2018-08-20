{% include 'c-license.c' %}
{% from 'service/macros.c' import list_fn_params %}

#include <app/{{ model['class_name'] }}.h>
{% if 'baseUri' in model %}
{% set uri = model['baseUri'] %}
{% else %}
{% set uri = "localhost:8000/api?token=x" %}
{% endif %}

{{ model['class_name'] }}::{{ model['class_name'] }}(const char * uri) : WebSocketApi(uri, "{{ model['api_name'] }}") {

}

{{ model['class_name'] }}::{{ model['class_name'] }}() : WebSocketApi("{{ uri }}", "{{ model['api_name'] }}") {

}

{% for verb_name, verb_desc in model['methods'].items()|sort %}

/** Autogenrated doc for {{ verb_name }} */
int {{ model['class_name'] }}::{{ verb_name }}({{ list_fn_params(verb_desc, maps, 4) }}) {
  json_object * rec = NULL;
  json_object * req = NULL;
  {% if verb_desc['in_params']|length > 0 %}
  json_object * new_sub_json = NULL;
  {% else %}
  {% for param in verb_desc['out_params'] %}
  {% if param['array'] %}
  json_object * new_sub_json = NULL;
  {% break %}
  {% endif %}
  {% endfor %}
  {% endif %}
  {% if verb_desc['out_params']|length > 0 %}
  json_object *val[{{ verb_desc['out_params']|length }}];
  {% endif %}
  json_object * response = NULL;
  {% for param in verb_desc['out_params'] %}
  {% if param['array'] %}
  json_object *val_{{ param['name'] }}_size_obj = NULL;
  int _var_{{ param['name'] }}_size = 0;
  {{ maps['type_to_cpp'][param['type']] }} * _var_{{ param['name'] }} = nullptr;
  {% endif %}
  {% endfor %}
  int rc = 0;

  if (!connected)
    return -1;

  {% if verb_desc['in_params']|length > 0 %}
  req =  json_object_new_object();
  {% else %}
  {% for param in verb_desc['out_params'] %}
  {% if param['array'] %}
  req =  json_object_new_object();
  {% break %}
  {% endif %}
  {% endfor %}
  {% endif %}
  {% for param in verb_desc['in_params'] %}
  {% if param['array'] %}
  new_sub_json = json_object_new_array();
  for (int i = 0; i < in_{{ param['name'] }}_size; i++) {
    json_object_array_put_idx(new_sub_json, i, {{ maps['type_to_json_new_fn'][param['type']] }}(in_{{ param['name'] }}[i]));
  }
  json_object_object_add(req, "{{ param['name'] }}", new_sub_json);
  new_sub_json = json_object_new_int(in_{{ param['name'] }}_size);
  json_object_object_add(req, "{{ param['name'] }}_size", new_sub_json);
  {% else %}
  new_sub_json = {{ maps['type_to_json_new_fn'][param['type']] }}(in_{{ param['name'] }}{% if param['type'] == 'string' %}.c_str(){% endif %});
  json_object_object_add(req, "{{ param['name'] }}", new_sub_json);
  {% endif %}
  {% endfor %}

  {% for param in verb_desc['out_params'] %}
  {% if param['array'] %}
  new_sub_json = json_object_new_int(in_{{ param['name'] }}_size);
  json_object_object_add(req, "{{ param['name'] }}_size", new_sub_json);
  {% endif %}
  {% endfor %}

  {% if verb_desc['in_params']|length > 0 %}
  printf("INFO: Verb '{{ verb_name }}' sending %s\n", json_object_to_json_string(req));
  {% endif %}
  // Get the response
  rec = this->emit("{{ verb_name }}", req ? json_object_to_json_string(req) : NULL);

  // Release the request json object
  {% if verb_desc['in_params']|length > 0 %}
  json_object_put(req);
  {% endif %}

  if (!rec) {
    printf("ERROR: Posting error or timeout while waiting for reply\n");
    return -1;
  }

  rc = json_object_object_get_ex(rec, "response", &response);
  if (!rc) {
    printf("ERROR: Malformed response\n");
    json_object_put(rec);
    return -1;
  }

  {% for param in verb_desc['out_params'] %}
    if (!json_object_object_get_ex(response, "{{ param['name'] }}", &val[{{ loop.index - 1 }}])) {
      printf("ERROR: Malformed response\n");
      json_object_put(response);
      return -1;
    }
    {% if param['array'] %}

    for (int i = 0; i < in_{{ param['name'] }}_size; i++) {
      out_{{ param['name'] }}[i] = static_cast<{{ maps['type_to_cpp'][param['type']] }}>({{ maps['type_to_json_get_fn'][param['type']] }}(json_object_array_get_idx(val[{{ loop.index - 1 }}], i)));
    }
    {% else %}
    out_{{ param['name'] }} = static_cast<{{ maps['type_to_cpp'][param['type']] }}>({{ maps['type_to_json_get_fn'][param['type']] }}(val[{{ loop.index - 1 }}]));
    {% endif %}
  {% endfor %}

  printf("INFO: %s\n", json_object_to_json_string_ext(rec, JSON_C_TO_STRING_PRETTY));

  json_object_put(rec);

  return 0;
}
{% endfor %}
