{% include 'c-license.c' %}
{% from 'service/macros.c' import list_fn_params %}

#include <app/{{ model['class_name'] }}.h>
{% if 'baseUri' in model %}
{% set uri = model['baseUri'] %}
{% else %}
{% set uri = "localhost:8000/api?token=x" %}
{% endif %}

{{ model['class_name'] }}::{{ model['class_name'] }}() : WebSocketApi("{{ uri }}", "{{ model['api_name'] }}") {

}
{% for verb_name, verb_desc in model['methods'].items()|sort %}

/** Autogenrated doc for {{ verb_name }} */
int {{ model['class_name'] }}::{{ verb_name }}({{ list_fn_params(verb_desc, 4) }}) {
  json_object * rec = NULL;
  json_object * req = NULL;
  {% if verb_desc['in_params']|length > 0 %}
  json_object * new_sub_json = NULL;
  {% endif %}
  {% if verb_desc['out_params']|length > 0 %}
  json_object * val = NULL;
  {% endif %}
  json_object * response = NULL;
  int rc = 0;

  {% if verb_desc['in_params']|length > 0 %}
  req =  json_object_new_object();
  {% endif %}

  {% for param in verb_desc['in_params'] %}
  new_sub_json = {{ param['type']|json_new_fn }}(in_{{ param['name'] }});
  json_object_object_add(req, "{{ param['name'] }}", new_sub_json);
  {% endfor %}

  {% if verb_desc['in_params']|length > 0 %}
  printf("INFO: Verb '{{ verb_name }}' sending %s\n", json_object_to_json_string(req));
  {% endif %}

  // Get the response
  rec = this->emit("{{ verb_name }}", req ? json_object_to_json_string(req) : NULL);
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

  {% for param in verb_desc['out_params'] %}
  rc = json_object_object_get_ex(response, "{{ param['name'] }}", &val);
  out_{{ param['name'] }} = rc ? {{ param['type']|json_get_fn }}(val) : static_cast<{{ param['type']|ramltype_to_cpp }}>(0);
  {% if param['type'] == 'string' %}
  out_{{ param['name'] }} = strdup(out_{{ param['name'] }});
  {% endif %}
  if (!rc) {
    printf("ERROR: Malformed response\n");
    json_object_put(rec);
    json_object_put(req);
    return -1;
  }
  {% endfor %}

  printf("INFO: %s\n", json_object_to_json_string_ext(rec, JSON_C_TO_STRING_PRETTY));

  json_object_put(req);
  json_object_put(rec);

  return 0;
}
{% endfor %}
