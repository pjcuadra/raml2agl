{% include 'c-license.c' %}

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
    void {{ verb_name }}(
      {% if verb_type == 'post' %}
        {% if 'body' in verb_body %}
        {% if 'application/json' in verb_body['body'] %}
        {% if 'properties' in verb_body['body']['application/json'] %}
          {% for param_key, param in verb_body['body']['application/json']['properties'].items() %}
    const {{ param['type']|ramltype_to_cpp }} _{{ param_key|lower }}{% if not loop.last %}, {% endif %}
          {% endfor %}
        {% endif %}
        {% endif %}
        {% endif %});
      {% elif verb_type == 'get' %}
        {% if 'responses' in verb_body %}
        {% if '200' in verb_body['responses'] %}
        {% if 'body' in verb_body['responses']['200'] %}
        const {{ verb_body['responses']['200']['body']|ramltype_to_cpp }} &_{{ verb_body['responses']['200']['body']|lower|strip_ilegal_chars }}{% if not loop.last %}, {% endif %}
        {% endif %}
        {% endif %}
        {% endif %});
      {% endif %}
    {% endfor %}
  {% endfor %}
};

#endif //__RAML2AGL_SERVICE_CLASS_{{ class_name|upper }}_H_
