{% include 'c-license.c' %}
{% from 'service/macros.c' import list_fn_params %}

#ifndef __RAML2AGL_CLASS_{{ model['class_name']|upper }}_H_
#define __RAML2AGL_CLASS_{{ model['class_name']|upper }}_H_

#pragma once
#include <cstddef>
#include <string.h>
#include <string>
#include <app/types.h>
#include <app/WebSocketApi.h>

extern "C"
{
    #define AFB_BINDING_VERSION 2
    #include <afb/afb-binding.h>
};

class {{ model['class_name'] }} : public WebSocketApi {
public:

  {{ model['class_name'] }}(const char * uri);

  {{ model['class_name'] }}();

  {% for verb_name, verb_desc in model['methods'].items()|sort %}
  /** Autogenrated doc for {{ verb_name }} */
  int {{ verb_name }}({{ list_fn_params(verb_desc, maps, 8) }});

  {% endfor %}
};

#endif //__RAML2AGL_CLASS_{{ model['class_name']|upper }}_H_
