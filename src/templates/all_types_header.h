 {% include 'c-license.c' %}

#ifndef __RAML2AGL_ALL_TYPES_H_
#define __RAML2AGL_ALL_TYPES_H_

{% for type_def in all_types %}
// Class {{ type_def }}
#include "types/{{ type_def }}.h"
{% endfor %}

#endif //__RAML2AGL_ALL_TYPES_H_
