{% macro list_fn_params(method, indent=0) -%}
{% for param in method['in_params'] %}
{% if not loop.first %}{{ ' ' * indent }}{% endif %}
const {{ param['type']|ramltype_to_cpp }} in_{{ param['name']|lower }}{% if not loop.last or method['out_params']|length > 0 %},
{% endif %}
{% endfor %}
{% for param in method['out_params'] %}
{% if not loop.first or method['in_params']|length > 0 %}{{ ' ' * indent }}{% endif %}
{% if param['type'] == 'string' %}const {% endif %}
{{ param['type']|ramltype_to_cpp }} &out_{{ param['name']|lower }}{% if not loop.last %},
{% endif %}
{% endfor %}
{%- endmacro %}

{% macro list_fn_params_call_json(method, indent=0) -%}
{% for param in method['in_params'] %}
{% if not loop.first %}{{ ' ' * indent }}{% endif %}
json_object_object_get_ex(args, "{{ param['name'] }}", &val) ? {{ param['type']|json_get_fn }}(val) : static_cast<{{ param['type']|ramltype_to_cpp }}>(0){% if not loop.last or method['out_params']|length > 0 %},
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
