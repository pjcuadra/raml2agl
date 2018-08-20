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
supported_types = ["enum"]
resource_types = {}


def ordered_load(stream, Loader=yaml.Loader, object_pairs_hook=OrderedDict):
    class OrderedLoader(Loader):
        pass

    def construct_mapping(loader, node):
        loader.flatten_mapping(node)
        return object_pairs_hook(loader.construct_pairs(node))

    OrderedLoader.add_constructor(
        yaml.resolver.BaseResolver.DEFAULT_MAPPING_TAG,
        construct_mapping)

    return yaml.load(stream, OrderedLoader)


class RamlParser:

    def filter_media_type(self, raml):

        for key, value in raml.items():
            if isinstance(value, dict):
                for x in value.keys():
                    if x in media_type:
                        value = value[x]
                        raml[key] = value

                value = self.filter_media_type(value)

    def parse_single_custom_type(self, yaml, json):
        json['def'] = yaml

        if 'enum' in yaml:
            json['type'] = 'enum'
            json['enum_vals'] = yaml['enum']

            return

        if 'type' not in yaml:
            json['type'] = 'unknown'
            return

        if yaml['type'] not in supported_types:
            json['type'] = 'unsupported_type'
            return

        json['type'] = yaml['type']

    def parse_custom_types(self):
        self.jraml["types"] = dict()

        if 'types' not in self.yraml:
            return

        logging.info(self.yraml["types"])

        for key, value in self.yraml["types"].items():
            tmp = {'name': key}
            self.parse_single_custom_type(value, tmp)
            self.jraml["types"][key] = tmp

    def parse_root_raml(self):

        logging.info(self.yraml)

        for key, value in self.yraml.items():
            if isinstance(value, dict) or isinstance(value, list):
                continue

            if key == 'resourceTypes':
                continue

            # Format the API Name
            if key == 'title':
                self.jraml['class_name'] = value.title().replace(" ", "")
                self.jraml['service_class_name'] = "Service"
                self.jraml['service_class_name'] += self.jraml['class_name']
                self.jraml['api_name'] = self.jraml['class_name'].lower()
                self.jraml[key] = value.title()
                continue

            # Force media type to be json
            if key == 'mediaType':
                self.jraml[key] = 'application/json'
                continue

            self.jraml[key] = value

        # If it doesn't have media type add JSON
        if any(x in media_type for x in self.jraml.keys()):
            self.jraml['mediaType'] = 'application/json'

    def parse_resource_types(self):
        if 'resourceTypes' not in self.yraml:
            return

        self.jraml['resourceTypes']

        resource_types

    def check_type_is_array(self, type):
        if '[]' == type[-2:]:
            return True

        return False

    def get_actual_type_name(self, type_name):
        if self.check_type_is_array(type_name):
            return type_name[:-2]

        return type_name

    def parse_type(self, yaml, json):

        if 'type' in yaml:
            json.append({'type': self.get_actual_type_name(yaml['type']),
                         'name': 'default',
                         'array': self.check_type_is_array(yaml['type'])})
            return

        tmp_yraml = yaml.items()

        for key, value in tmp_yraml:
            if 'type' not in value:
                continue
            json.append({'type': self.get_actual_type_name(value['type']),
                         'name': key,
                         'array': self.check_type_is_array(value['type'])})

    def parse_class_method_in_params(self, yaml, json):

        if 'body' not in yaml:
            return

        tmp_raml = yaml['body']

        if 'properties' in tmp_raml:
            tmp_raml = tmp_raml['properties']

        # If only param is defined
        self.parse_type(tmp_raml, json)

    def parse_class_method_out_params(self, yaml, json):

        if not yaml:
            return

        if 'responses' not in yaml:
            return

        tmp_raml = yaml['responses']

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
            self.parse_type(params_raml, json)

    def parse_class_method_params(self, yaml, json):

        json['in_params'] = []
        json['out_params'] = []

        # Parse input params first
        if 'post' in yaml:
            self.parse_class_method_in_params(yaml['post'],
                                              json['in_params'])

        # Parse output params
        if 'get' in yaml:
            self.parse_class_method_out_params(yaml['get'],
                                               json['out_params'])

    def parse_class_methods(self):

        self.jraml['methods'] = {}

        methods = self.jraml['methods']

        for key, value in self.yraml.items():
            if key[:1] != '/':
                continue

            if not value:
                continue

            # if all(x not in ['post', 'get'] for x in value.keys()):
            #     continue

            method_name = key[1:]

            methods[method_name] = {}

            if 'description' in value:
                methods[method_name]['description'] = value['description']

            self.parse_class_method_params(value, methods[method_name])

    def parse(self, raml_file_path):
        # Needed to resolve includes
        # model_loc = os.path.dirname(raml_file_path)

        # Read the model
        f = open(raml_file_path, "r")
        self.yraml = ordered_load(f.read(), yaml.SafeLoader)

        # Pre process the RAML file
        self.filter_media_type(self.yraml)

        self.jraml = {}

        # Root parsing
        self.parse_root_raml()

        self.parse_class_methods()

        self.parse_custom_types()

        logging.info('JSON Model - ' + json.dumps(self.jraml,
                                                  sort_keys=True, indent=4))

        return self.jraml
