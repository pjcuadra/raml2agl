{% include 'c-license.c' %}

#ifndef __RAML2AGL_TYPE_{{ type_name|upper }}_H_
#define __RAML2AGL_TYPE_{{ type_name|upper }}_H_

#include <string.h>

// RAML Defined types
{% if this_type.type != 'object' and this_type.type in all_types %}
#include "types/{{ this_type.type }}.h"
{% endif %}

class {{ type_name }} {% if this_type.type != 'object' %}: Public {{ this_type.type }}{% endif %} {
public:

  {{ type_name }}(json_object *json_obj){% if this_type.type != 'object' %}: {{ this_type.type }}(json_obj){% endif %} {
    json_object *val = NULL;

    {% for prop, prop_type in this_type.properties.items() %}
    if (json_object_object_get_ex(json_obj, "{{ prop }}", &val)) {
      this.{{ prop }} = {{ prop_type|json_get_fn }}(val);
    }
    {% endfor %}
  };

  json_object toJson() {
    {% if this_type.type != 'object' %}
    json_object * newJson = {{ this_type.type }}::toJson();
    {% else %}
    json_object * newJson = json_object_new_object();
    {% endif %}
    json_object * newSubJson = NULL;

    {% for prop, prop_type in this_type.properties.items() %}
    // Adding {{ prop }}
    newSubJson = {{ prop_type|json_new_fn }}(this.{{ prop }});
    json_object_object_add(newJson, "{{ prop }}", newSubJson);
    {% endfor %}

    return *json_object;
  }

  {% for prop, prop_type in this_type.properties.items() %}
  /** Autogenrated doc for {{ prop }} */
  {{ prop_type|ramltype_to_cpp }} {{ prop }};
  {% endfor %}
};

#endif //__RAML2AGL_TYPE_{{ type_name|upper }}_H_
