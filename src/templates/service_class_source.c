{% include 'c-license.c' %}

#include <service/{{ class_name }}.h>

{% for verb_name, verb_desc in verbs.items() %}
  {% for verb_type, verb_body in verb_desc.items() %}

/** Autogenrated doc for {{ verb_name }} */
void {{ class_name }}::{{ verb_name }}(
    {% if verb_type == 'post' %}
      {% if 'body' in verb_body %}
      {% if 'application/json' in verb_body['body'] %}
      {% if 'properties' in verb_body['body']['application/json'] %}
        {% for param_key, param in verb_body['body']['application/json']['properties'].items() %}
  const {{ param['type']|ramltype_to_cpp }} _{{ param_key|lower }}{% if not loop.last %}, {% endif %}
        {% endfor %}
      {% endif %}
      {% endif %}
      {% endif %}) {
  AFB_NOTICE("[{{ class_name }}] {{ verb_name|capitalize }}");

}
    {% elif verb_type == 'get' %}
      {% if 'responses' in verb_body %}
      {% if '200' in verb_body['responses'] %}
      {% if 'body' in verb_body['responses']['200'] %}
  const {{ verb_body['responses']['200']['body']|ramltype_to_cpp }} &_{{ verb_body['responses']['200']['body']|lower|strip_ilegal_chars }}{% if not loop.last %}, {% endif %}
      {% endif %}
      {% endif %}
      {% endif %}) {
  AFB_NOTICE("[{{ class_name }}] {{ verb_name|capitalize }}");
}
    {% endif %}
  {% endfor %}
{% endfor %}
