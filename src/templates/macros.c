{% macro iterate_post_fn_params(verb_body) -%}
  {# Get the properties list #}
  {% set prop_list = {} %}
  {% if 'body' in verb_body %}
    {% if 'application/json' in verb_body['body'] %}
      {% if 'properties' in verb_body['body']['application/json'] %}
        {% set prop_list = verb_body['body']['application/json']['properties'] %}
      {% endif %}
    {% elif 'properties' in verb_body['body'] %}
      {% set prop_list = verb_body['body']['properties'] %}
    {% endif %}
  {% endif %}
{% for param_key, param in prop_list.items() %}{{ caller(param_key, param, loop.last) }}{% endfor %}
{%- endmacro %}

{% macro list_post_fn_params(verb_body) -%}
{% call(param_key, param, is_last = False) iterate_post_fn_params(verb_body) %}const {{ param['type']|ramltype_to_cpp }} _{{ param_key|lower }}{% if not is_last %}, {% endif %}{% endcall %}
{%- endmacro %}

{% macro list_fn_params(verb_body, verb_type) -%}
{% if verb_type == 'post' %}{{ list_post_fn_params(verb_body) }}{% elif verb_type == 'get' %}
  {% if 'responses' in verb_body %}
  {% if '200' in verb_body['responses'] %}
  {% if 'body' in verb_body['responses']['200'] %}
  const {{ verb_body['responses']['200']['body']|ramltype_to_cpp }} &_{{ verb_body['responses']['200']['body']|lower|strip_ilegal_chars }}{% if not loop.last %}, {% endif %}
  {% endif %}
  {% endif %}
  {% endif %}
{% endif %}
{%- endmacro %}
