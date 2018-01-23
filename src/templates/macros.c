{% macro iterate_post_fn_params(verb_body) -%}
  {# Get the properties list #}
  {% set prop_list = {} %}
  {% set tmp = {} %}

  {% if 'body' in verb_body %}
  {% set tmp = verb_body['body'] %}
  {% endif %}

  {% if 'application/json' in verb_body['body'] %}
  {% set tmp = tmp['application/json'] %}
  {% endif %}

  {% if 'properties' in tmp %}
    {% set prop_list = tmp['properties'] %}
  {% elif 'type' in tmp %}
    {% set prop_list =  {'default': {'type': tmp['type'] }} %}
  {% endif %}

{% for param_key, param in prop_list.items() %}
{% if not param %}
{{ caller(param_key, {'type' : 'string'}, loop.last) }}
{% else %}
{{ caller(param_key, param, loop.last) }}
{% endif %}
{% endfor %}
{%- endmacro %}

{% macro iterate_get_fn_params(verb_body) -%}
  {# Get the properties list #}
  {% set prop_list = {} %}
  {% set tmp = {} %}

  {% if 'responses' in verb_body %}
  {% set tmp = verb_body['responses'] %}
  {% endif %}

  {% if 'body' in tmp.values()[0] %}
  {% set tmp = tmp.values()[0]['body'] %}
  {% endif %}

  {% if 'application/json' in tmp %}
  {% set tmp = tmp['application/json'] %}
  {% endif %}

  {% if 'properties' in tmp %}
    {% set prop_list = tmp['properties'] %}
  {% elif 'type' in tmp %}
    {% set prop_list =  {'default': {'type': tmp['type'] }} %}
  {% endif %}

{% for param_key, param in prop_list.items() %}
{% if not param %}
{{ caller(param_key, {'type' : 'string'}, loop.last) }}
{% else %}
{{ caller(param_key, param, loop.last) }}
{% endif %}
{% endfor %}
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
