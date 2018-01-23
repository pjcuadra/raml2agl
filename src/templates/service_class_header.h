{% include 'c-license.c' %}
{% from 'macros.c' import list_fn_params %}

#ifndef __RAML2AGL_SERVICE_CLASS_{{ class_name|upper }}_H_
#define __RAML2AGL_SERVICE_CLASS_{{ class_name|upper }}_H_

#pragma once
#include <cstddef>
#include <string.h>

extern "C"
{
    #define AFB_BINDING_VERSION 2
    #include <afb/afb-binding.h>
};

class {{ class_name }} {
public:
  {% for verb_name, verb_desc in verbs.items() %}
    {% for verb_type, verb_body in verb_desc.items() %}

    /** Autogenrated doc for {{ verb_name }} */
    void {{ verb_name }}({{ list_fn_params(verb_body, verb_type) }});
    {% endfor %}
  {% endfor %}
};

#endif //__RAML2AGL_SERVICE_CLASS_{{ class_name|upper }}_H_
