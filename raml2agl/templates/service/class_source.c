{% include 'c-license.c' %}
{% from 'service/macros.c' import list_fn_params %}

#include <service/{{ model['service_class_name'] }}.h>

{{ model['service_class_name'] }}::{{ model['service_class_name'] }}() {
  AFB_NOTICE("[{{ model['service_class_name'] }}] Constructor ");
}

int {{ model['service_class_name'] }}::init() {
  AFB_NOTICE("[{{ model['service_class_name'] }}] Init ");
  return -1;
}

{% for verb_name, verb_desc in model['methods'].items()|sort %}

/** Autogenrated doc for {{ verb_name }} */
int {{ model['service_class_name'] }}::{{ verb_name }}({{ list_fn_params(verb_desc, maps, 4) }}) {

  AFB_NOTICE("[{{ model['service_class_name'] }}] {{ verb_name|capitalize }}");
  AFB_ERROR("Method {{ verb_name|capitalize }} not implemented");

  return -1;
}
{% endfor %}
