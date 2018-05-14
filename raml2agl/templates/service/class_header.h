{% include 'c-license.c' %}
{% from 'service/macros.c' import list_fn_params %}
{% from 'service/macros.c' import define_type %}

#ifndef __RAML2AGL_SERVICE_CLASS_{{ model['service_class_name']|upper }}_H_
#define __RAML2AGL_SERVICE_CLASS_{{ model['service_class_name']|upper }}_H_

#pragma once
#include <cstddef>
#include <string.h>
#include <string>

extern "C"
{
    #define AFB_BINDING_VERSION 2
    #include <afb/afb-binding.h>
};

{% for key, type in model['types'].items()|sort %}{{ define_type(type) }}{% endfor %}

class {{ model['service_class_name'] }} {
public:
  {{ model['service_class_name'] }}();

  int init();

  {% for verb_name, verb_desc in model['methods'].items()|sort %}
    /** Autogenrated doc for {{ verb_name }} */
    int {{ verb_name }}({{ list_fn_params(verb_desc, maps, 8) }});

  {% endfor %}
};

#endif //__RAML2AGL_SERVICE_CLASS_{{ model['service_class_name']|upper }}_H_
