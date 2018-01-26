# Copyright 2018 Pedro Cuadra - pjcuadra@gmail.com
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# import os
import yaml
import json
import logging
from collections import OrderedDict


media_type = ["application/json", "application/xml"]
success_return_codes = [200, 201]

resource_types = {}

def filter_media_type(raml):

    for key, value in raml.items():

        if isinstance(value, dict) or isinstance(value, list):
            for x in value.keys():
                if x in media_type:
                    value = value[x]
                    raml[key] = value

            value = filter_media_type(value)


def parse_root_raml(yraml, jraml):

    for key, value in yraml.items():
        if isinstance(value, dict) or isinstance(value, list):
            continue

        if key == 'resourceTypes':
            continue

        # Format the API Name
        if key == 'title':
            jraml['class_name'] = value.title().replace(" ", "")
            jraml['service_class_name'] = "Service"
            jraml['service_class_name'] += jraml['class_name']
            jraml['api_name'] = jraml['class_name'].lower()
            jraml[key] = value.title()
            continue

        # Force media type to be json
        if key == 'mediaType':
            jraml[key] = 'application/json'
            continue

        jraml[key] = value

    # If it doesn't have media type add JSON
    if any(x in media_type for x in jraml.keys()):
        jraml['mediaType'] = 'application/json'


def parse_resource_types(yaml, jraml):
    if 'resourceTypes' not in yaml:
        return

    jraml['resourceTypes']

    resource_types


def parse_type(yraml, jraml):
    if 'type' in yraml:
        jraml.append({'type': yraml['type'], 'name': 'default'})

        return

    tmp_yraml = OrderedDict(sorted(yraml.items()))

    for key, value in tmp_yraml.items():
        if 'type' not in value:
            continue

        jraml.append({'type': value['type'], 'name': key})


def parse_class_method_in_params(yraml, jraml):

    if 'body' not in yraml:
        return

    tmp_raml = yraml['body']

    if 'properties' in tmp_raml:
        tmp_raml = tmp_raml['properties']

    # If only param is defined
    parse_type(tmp_raml, jraml)


def parse_class_method_out_params(yraml, jraml):

    if not yraml:
        return

    if 'responses' not in yraml:
        return

    tmp_raml = yraml['responses']

    for key, value in tmp_raml.items():
        if key not in success_return_codes:
            continue

        # Check if it has body first
        if 'body' not in value:
            logging.warning('Response without body')
            continue

        params_raml = value['body']

        if 'properties' in params_raml:
            params_raml = params_raml['properties']

        # If only param is defined
        parse_type(params_raml, jraml)


def parse_class_method_params(yraml, jraml):

    jraml['in_params'] = []
    jraml['out_params'] = []

    # Parse input params first
    if 'post' in yraml:
        parse_class_method_in_params(yraml['post'], jraml['in_params'])

    # Parse output params
    if 'get' in yraml:
        parse_class_method_out_params(yraml['get'], jraml['out_params'])


def parse_class_methods(yraml, jraml):

    jraml['methods'] = {}

    methods = jraml['methods']

    for key, value in yraml.items():
        if key[:1] != '/':
            continue

        if not value:
            continue

        if all(x not in ['post', 'get'] for x in value.keys()):
            continue

        method_name = key[1:]

        methods[method_name] = {}

        if 'description' in value:
            methods[method_name]['description'] = value['description']

        parse_class_method_params(value, methods[method_name])


def parse_JSON(yraml):
    jraml = {}

    # Root parsing
    parse_root_raml(yraml, jraml)

    parse_class_methods(yraml, jraml)

    return jraml


def parse(raml_file_path):
    # Needed to resolve includes
    # model_loc = os.path.dirname(raml_file_path)

    # Read the model
    f = open(raml_file_path, "r")
    yraml = yaml.load(f.read())

    # Pre process the RAML file
    filter_media_type(yraml)

    jraml = parse_JSON(yraml)
    logging.info('JSON Model - ' + json.dumps(jraml, sort_keys=True, indent=4))

    return jraml
