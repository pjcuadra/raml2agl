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

from jinja2 import Environment, FileSystemLoader
import os

# FIXME: Add all RAML supported types
ramltype_to_cpp_map = {"integer": "int",
                       "string": "char *",
                       "number": "double",
                       "boolean": "json_bool",
                       "any": "void *"}

ramltype_to_json_new_fn_map = {"integer": "json_object_new_int",
                               "string": "json_object_new_string",
                               "number": "json_object_new_double",
                               "boolean": "json_object_new_boolean"}

ramltype_to_json_get_fn_map = {"integer": "json_object_get_int",
                               "string": "json_object_get_string",
                               "number": "json_object_get_double",
                               "boolean": "json_object_get_boolean"}


def ramltype_to_cpp(value):
    if value in ramltype_to_cpp_map:
        return ramltype_to_cpp_map[value]
    return ""


def strip_ilegal_chars(value):
    value = value.replace(" ", "").replace("[]", "").replace("*", "")
    return value


def json_get_fn(value):
    if value in ramltype_to_json_get_fn_map:
        return ramltype_to_json_get_fn_map[value]
    return ""


def json_new_fn(value):
    if value in ramltype_to_json_new_fn_map:
        return ramltype_to_json_new_fn_map[value]
    return ""


filters = {"ramltype_to_cpp": ramltype_to_cpp,
           "strip_ilegal_chars": strip_ilegal_chars,
           "json_get_fn": json_get_fn,
           "json_new_fn": json_new_fn}


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
            keep_trailing_newline=False
        )

        self.env.filters.update(filters)

    def generate_types(self, raml):
        fh = open(self.headers_out_path + "/all_types.h", "w")

        tmpl = self.env.get_template("all_types_header.h")

        if 'types' not in raml:
            cont = tmpl.render(all_types=None)
            fh.write(cont)
            return
        else:
            cont = tmpl.render(all_types=raml['types'])
            fh.write(cont)

        tmpl = self.env.get_template("types_header.h")
        # Gen the types classes
        for type_name, type_def in raml['types'].items():
            cont = tmpl.render(this_type=type_def,
                               type_name=type_name,
                               all_types=raml['types'])

            fh = open(self.headers_out_path + "/types/" + type_name + ".h",
                      "w")
            fh.write(cont)

    def generate_service_class(self, jmodel):
        # Create the headers and sources
        file_name = "/" + jmodel['service_class_name']
        header = open(self.headers_out_path + file_name + ".h", "w")
        source = open(self.source_out_path + file_name + ".cpp", "w")

        tmpl = self.env.get_template("service/class_header.h")
        header_cont = tmpl.render(model=jmodel)

        tmpl = self.env.get_template("service/class_source.c")
        source_cont = tmpl.render(model=jmodel)

        header.write(header_cont)
        source.write(source_cont)

    def generate_app_class(self, jmodel):
        # Create the headers and sources
        file_name = "/" + jmodel['class_name']
        header = open(self.headers_out_path + file_name + ".h", "w")
        source = open(self.source_out_path + file_name + ".cpp", "w")

        tmpl = self.env.get_template("app/class_header.h")
        header_cont = tmpl.render(model=jmodel)

        tmpl = self.env.get_template("app/class_source.c")
        source_cont = tmpl.render(model=jmodel)

        header.write(header_cont)
        source.write(source_cont)

    def generate_app_superclass(self, jmodel):
        # Create the headers and sources
        header = open(self.headers_out_path + "/" + "WebSocketApi.h", "w")
        source = open(self.source_out_path + "/" + "WebSocketApi.cpp", "w")

        tmpl = self.env.get_template("types/app/WebSocketApi.h")
        header_cont = tmpl.render(model=jmodel)

        tmpl = self.env.get_template("types/app/WebSocketApi.cpp")
        source_cont = tmpl.render(model=jmodel)

        header.write(header_cont)
        source.write(source_cont)

    def generate_agl_service(self, jmodel):
        source = open(self.source_out_path + "/" + jmodel['api_name'] +
                      "-binding-auto.cpp", "w")
        tmpl = self.env.get_template("service/agl_service.c")
        source_cont = tmpl.render(model=jmodel)

        source.write(source_cont)

    def set_source_out_path(self, path):
        self.source_out_path = path

    def set_headers_out_path(self, path):
        self.headers_out_path = path

    def generate_service(self, jmodel):
        self.generate_agl_service(jmodel)
        self.generate_service_class(jmodel)

    def generate_app(self, jmodel):
        self.generate_app_class(jmodel)
        self.generate_app_superclass(jmodel)
