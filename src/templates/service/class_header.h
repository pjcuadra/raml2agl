{% include 'c-license.c' %}
{% from 'service/macros.c' import list_fn_params %}

#ifndef __RAML2AGL_SERVICE_CLASS_{{ model['class_name']|upper }}_H_
#define __RAML2AGL_SERVICE_CLASS_{{ model['class_name']|upper }}_H_

#pragma once
#include <cstddef>
#include <string.h>
#include <string>

extern "C"
{
    #define AFB_BINDING_VERSION 2
    #include <afb/afb-binding.h>
};

class {{ model['class_name'] }} {
public:
  {% for verb_name, verb_desc in model['methods'].items()|sort %}
    /** Autogenrated doc for {{ verb_name }} */
    void {{ verb_name }}({{ list_fn_params(verb_desc, 8) }});

  {% endfor %}
};

#endif //__RAML2AGL_SERVICE_CLASS_{{ model['class_name']|upper }}_H_
