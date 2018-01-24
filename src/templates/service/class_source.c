{% include 'c-license.c' %}
{% from 'service/macros.c' import list_fn_params %}

#include <service/{{ model['class_name'] }}.h>

{% for verb_name, verb_desc in model['methods'].items()|sort %}

/** Autogenrated doc for {{ verb_name }} */
void {{ model['class_name'] }}::{{ verb_name }}({{ list_fn_params(verb_desc, 4) }}) {
  
  AFB_NOTICE("[{{ model['class_name'] }}] {{ verb_name|capitalize }}");
  AFB_ERROR("Method {{ verb_name|capitalize }} not implemented");

}
{% endfor %}
