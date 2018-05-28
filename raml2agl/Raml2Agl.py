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
from raml2agl import RamlParser as rp
from jinja2 import Environment, FileSystemLoader
import os
import codecs
import logging

# FIXME: Add all RAML supported types
type_to_cpp_map = {"integer": "int",
                   "string": "std::string",
                   "number": "double",
                   "boolean": "bool",
                   "any": "void *"}

type_to_json_new_fn_map = {"integer": "json_object_new_int",
                           "string": "json_object_new_string",
                           "number": "json_object_new_double",
                           "boolean": "json_object_new_boolean"}

type_to_json_get_fn_map = {"integer": "json_object_get_int",
                           "string": "json_object_get_string",
                           "number": "json_object_get_double",
                           "boolean": "json_object_get_boolean"}


def strip_ilegal_chars(value):
    value = value.replace(" ", "").replace("[]", "").replace("*", "")
    return value


filters = {"strip_ilegal_chars": strip_ilegal_chars}


class Raml2Agl:
    """A simple example class"""

    def __init__(self):
        self.out_path = "./out"
        self.headers_out_path = self.out_path + "/include"
        self.source_out_path = self.out_path + "/src"

        # Load templates
        path = os.path.dirname(os.path.realpath(__file__)) + "/templates"
        self.env = Environment(
            loader=FileSystemLoader(path),
            trim_blocks=True,
            lstrip_blocks=True,
            keep_trailing_newline=False,
            extensions=['jinja2.ext.loopcontrols']
        )

        self.maps = dict()
        self.maps['type_to_cpp'] = type_to_cpp_map
        self.maps['type_to_json_new_fn'] = type_to_json_new_fn_map
        self.maps['type_to_json_get_fn'] = type_to_json_get_fn_map

        self.env.filters.update(filters)

    def generate_types(self):
        fh = codecs.open(self.headers_out_path + "/types.h", "w")

        tmpl = self.env.get_template("all_types_header.h")

        cont = tmpl.render(all_types=self.jmodel['types'])
        fh.write(cont)

        # TODO: Implement Objects headers generation
        # tmpl = self.env.get_template("types_header.h")
        # # Gen the types classes
        # for type_name, type_def in raml['types'].items():
        #     cont = tmpl.render(this_type=type_def,
        #                        type_name=type_name,
        #                        all_types=raml['types'])
        #
        #     path = os.path.join(self.headers_out_path, "types")
        #     fh = codecs.open(path + type_name + ".h",
        #                      "w")
        #     fh.write(cont)

    def check_service_class_exist(self):
        if not self.input_model:
            return

        ret = False

        self.jmodel = {}

        self.parse()

        # Create the headers and sources
        file_name = "/" + self.jmodel['service_class_name']
        header = self.headers_out_path + file_name + ".h"
        source = self.source_out_path + file_name + ".cpp"

        ret |= os.path.exists(header)
        ret |= os.path.exists(source)

        return ret

    def generate_service_class(self):
        if not self.input_model:
            return

        self.jmodel = {}

        self.parse()

        # Create the headers and sources
        file_name = "/" + self.jmodel['service_class_name']
        header = codecs.open(self.headers_out_path + file_name + ".h", "w")
        source = codecs.open(self.source_out_path + file_name + ".cpp", "w")

        tmpl = self.env.get_template("service/class_header.h")
        header_cont = tmpl.render(model=self.jmodel, maps=self.maps)

        tmpl = self.env.get_template("service/class_source.c")
        source_cont = tmpl.render(model=self.jmodel, maps=self.maps)

        header.write(header_cont)
        source.write(source_cont)

    def generate_app_class(self, jmodel):
        # Create the headers and sources
        file_name = "/" + jmodel['class_name']
        header = codecs.open(self.headers_out_path + file_name + ".h", "w")
        source = codecs.open(self.source_out_path + file_name + ".cpp", "w")

        tmpl = self.env.get_template("app/class_header.h")
        header_cont = tmpl.render(model=self.jmodel, maps=self.maps)

        tmpl = self.env.get_template("app/class_source.c")
        source_cont = tmpl.render(model=self.jmodel, maps=self.maps)

        header.write(header_cont)
        source.write(source_cont)

    def generate_app_superclass(self, jmodel):
        # Create the headers and sources
        header = codecs.open(self.headers_out_path + "/" + "WebSocketApi.h",
                             "w",
                             "utf-8")
        source = codecs.open(self.source_out_path + "/" + "WebSocketApi.cpp",
                             "w",
                             "utf-8")

        tmpl = self.env.get_template("types/app/WebSocketApi.h")
        header_cont = tmpl.render(model=jmodel, maps=self.maps)

        tmpl = self.env.get_template("types/app/WebSocketApi.cpp")
        source_cont = tmpl.render(model=jmodel, maps=self.maps)

        header.write(header_cont)
        source.write(source_cont)

    def generate_agl_service(self, jmodel):
        source = codecs.open(self.source_out_path + "/" + jmodel['api_name'] +
                             "-binding-auto.cpp", "w")
        tmpl = self.env.get_template("service/agl_service.c")
        source_cont = tmpl.render(model=self.jmodel, maps=self.maps)

        source.write(source_cont)

    def set_source_out_path(self, path):
        self.source_out_path = path

    def set_headers_out_path(self, path):
        self.headers_out_path = path

    def set_input_model(self, path):
        self.input_model = path

    def add_custom_types_filters(self):
        if 'types' not in self.jmodel:
            return

        for tname, type_att in self.jmodel['types'].items():
            logging.info(tname)
            if type_att['type'] is 'enum':
                self.maps['type_to_json_new_fn'][tname] = "json_object_new_int"
                self.maps['type_to_json_get_fn'][tname] = "json_object_get_int"
                self.maps['type_to_cpp'][tname] = tname

    def parse(self):
        # nonlocal custom_types
        # Parse the model
        parser = rp.RamlParser()
        self.jmodel = parser.parse(self.input_model)

        self.add_custom_types_filters()

    def generate_service(self):
        if not self.input_model:
            return

        self.jmodel = {}

        self.parse()

        self.generate_agl_service(self.jmodel)

    def generate_app(self):
        if not self.input_model:
            return

        self.jmodel = {}

        self.parse()

        self.generate_app_class(self.jmodel)
        self.generate_app_superclass(self.jmodel)
        self.generate_types()
