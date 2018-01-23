 {% include 'c-license.c' %}

#ifndef __RAML2AGL_ALL_TYPES_H_
#define __RAML2AGL_ALL_TYPES_H_

{% if all_types %}

{% for type_def in all_types %}
// Class {{ type_def }}
#include "types/{{ type_def }}.h"
{% endfor %}

{% endif %}

#endif //__RAML2AGL_ALL_TYPES_H_
