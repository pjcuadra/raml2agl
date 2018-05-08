{% macro list_fn_params(method, maps, indent=0) -%}
{% for param in method['in_params'] %}
{% if not loop.first %}{{ ' ' * indent }}{% endif %}
const {{ maps['type_to_cpp'][param['type']] }} in_{{ param['name']|lower }}{% if not loop.last or method['out_params']|length > 0 %},
{% endif %}
{% endfor %}
{% for param in method['out_params'] %}
{% if not loop.first or method['in_params']|length > 0 %}{{ ' ' * indent }}{% endif %}
{% if param['type'] == 'string' %}const {% endif %}
{{ maps['type_to_cpp'][param['type']] }} &out_{{ param['name']|lower }}{% if not loop.last %},
{% endif %}
{% endfor %}
{%- endmacro %}

{% macro list_fn_params_call_json(method, maps, indent=0) -%}
{% for param in method['in_params'] %}
{% if not loop.first %}{{ ' ' * indent }}{% endif %}
json_object_object_get_ex(args, "{{ param['name'] }}", &val) ? static_cast<{{ maps['type_to_cpp'][param['type']] }}>({{ maps['type_to_json_get_fn'][param['type']] }}(val)) : static_cast<{{ maps['type_to_cpp'][param['type']] }}>(0){% if not loop.last or method['out_params']|length > 0 %},
{% endif %}
{% endfor %}
{% if method['out_params']|length > 0 %}
{% for param in method['out_params'] %}
{% if not loop.first or method['in_params']|length > 0 %}{{ ' ' * indent }}{% endif %}
_var_{{ param['name'] }}{% if not loop.last %},
{% endif %}
{% endfor %}
{% endif %}
{%- endmacro %}

{% macro define_type(type_def) -%}
{% if type_def['type'] == 'enum' %}
enum {{ type_def['name'] }} {
{% for val in type_def['enum_vals'] %}
  {{ val }}{% if not loop.last %},
{% endif %}
{% endfor %}

};
{% endif %}
{%- endmacro %}
