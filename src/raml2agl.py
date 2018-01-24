from jinja2 import Environment, FileSystemLoader
from ramlParser import parse
import os
import yaml
import json
import logging

"""Collect command-line options in a dictionary"""

out_path = "./out"
headers_out_path = out_path + "/include"
source_out_path = out_path + "/src"


def getopts(argv):
    opts = {}  # Empty dictionary to store key-value pairs.
    while argv:  # While there are arguments left to parse...
        if argv[0][0] is '-':  # Found a "-name value" pair.
            if len(argv) > 1:
                if argv[1][0] != '-':
                    opts[argv[0]] = argv[1]
                else:
                    opts[argv[0]] = True
            elif len(argv) == 1:
                opts[argv[0]] = True

        # Reduce the argument list by copying it starting from index 1.
        argv = argv[1:]
    return opts


# FIXME: Add all RAML supported types
ramltype_to_cpp_map = {"integer": "int",
                       "string": "std::string",
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


def generate_types(env, raml):
    fh = open(headers_out_path + "/all_types.h", "w")

    tmpl = env.get_template("all_types_header.h")

    if 'types' not in raml:
        cont = tmpl.render(all_types=None)
        fh.write(cont)
        return
    else:
        cont = tmpl.render(all_types=raml['types'])
        fh.write(cont)

    tmpl = env.get_template("types_header.h")
    # Gen the types classes
    for type_name, type_def in raml['types'].items():
        cont = tmpl.render(this_type=type_def,
                           type_name=type_name,
                           all_types=raml['types'])

        fh = open(headers_out_path + "/types/" + type_name + ".h", "w")
        fh.write(cont)


def generate_api_class(env, jmodel):
    # Create the headers and sources
    file_name = "/service/" + jmodel['class_name']
    header = open(headers_out_path + file_name + ".h", "w")
    source = open(source_out_path + file_name + ".cpp", "w")

    tmpl = env.get_template("service/class_header.h")
    header_cont = tmpl.render(model=jmodel)

    tmpl = env.get_template("service/class_source.c")
    source_cont = tmpl.render(model=jmodel)

    header.write(header_cont)
    source.write(source_cont)


def generate_agl_service(env, jmodel):
    source = open(source_out_path + "/service/" + jmodel['api_name'] +
                  "-binding-auto.cpp", "w")
    tmpl = env.get_template("service/agl_service.c")
    source_cont = tmpl.render(model=jmodel)

    source.write(source_cont)


def creat_dir(path):
    if not os.path.exists(path):
        os.makedirs(path)


if __name__ == '__main__':
    from sys import argv
    myargs = getopts(argv)
    model_file = ''

    if '-i' in myargs:
        model_file = myargs['-i']
        f = open(model_file, "r")
        y = yaml.load(f.read())
    else:
        exit(1)

    if '-v' in myargs:
        logging.basicConfig(level=logging.INFO)

    if '-o' in myargs:
        out_path = myargs['-o']

    if '-h' in myargs:
        headers_out_path = myargs['-h']

    if '-s' in myargs:
        source_out_path = myargs['-s']

    logging.info("Provided params - " + json.dumps(myargs,
                                                   sort_keys=True,
                                                   indent=4))

    # Create output dir
    creat_dir(out_path)
    creat_dir(headers_out_path)
    creat_dir(headers_out_path + "/types")
    creat_dir(headers_out_path + "/service")
    creat_dir(headers_out_path + "/app")
    creat_dir(source_out_path + "/service")

    # Load templates
    path = os.path.dirname(os.path.realpath(__file__)) + "/templates"
    env = Environment(
        loader=FileSystemLoader(path),
        trim_blocks=True,
        lstrip_blocks=True,
        keep_trailing_newline=False
    )

    env.filters.update(filters)

    jy = json.loads(json.dumps(y))

    jmodel = parse(model_file)

    # Generate the Types
    # generate_types(env, jy)
    # generate_api_class(env, jy)
    generate_agl_service(env, jmodel)
    generate_api_class(env, jmodel)
