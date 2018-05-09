 {% include 'c-license.c' %}
 {% from 'service/macros.c' import define_type %}

#ifndef __RAML2AGL_ALL_TYPES_H_
#define __RAML2AGL_ALL_TYPES_H_

{% if all_types %}

{% for type_name, type_def in all_types.items()|sort %}
// Class {{ type_def }}
{% if type_def['type'] == 'object' %}
#include "types/{{ type_def['name'] }}.h"
{% endif %}
{% endfor %}


{% for type_name, type_def in all_types.items()|sort %}

{% if type_def['type'] == 'enum' %}
{{ define_type(type_def) }}
{% endif %}

{% endfor %}

{% endif %}

#endif //__RAML2AGL_ALL_TYPES_H_
