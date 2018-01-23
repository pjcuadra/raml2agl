{% include 'c-license.c' %}
{% from 'macros.c' import list_fn_params %}

#include <service/{{ class_name }}.h>

{% for verb_name, verb_desc in verbs.items()|sort %}
{% if 'post' in verb_desc or 'get' in verb_desc %}
  {% for verb_type, verb_body in verb_desc.items()|sort %}

{% if 'post' == verb_type or 'get' == verb_type %}
/** Autogenrated doc for {{ verb_name }} */
void {{ class_name }}::{{ verb_name }}({{ list_fn_params(verb_body, verb_type) }}) {
  AFB_NOTICE("[{{ class_name }}] {{ verb_name|capitalize }}");
}
{% endif %}

  {% endfor %}
  {% endif %}
{% endfor %}
