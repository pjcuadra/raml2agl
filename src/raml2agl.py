from jinja2 import Template, Environment, FileSystemLoader
import sys, os, yaml, json;

"""Collect command-line options in a dictionary"""

out_path = "./out"
headers_out_path = out_path + "/include"
source_out_path = out_path + "/src"

def getopts(argv):
    opts = {}  # Empty dictionary to store key-value pairs.
    while argv:  # While there are arguments left to parse...
        if argv[0][0] == '-':  # Found a "-name value" pair.
            opts[argv[0]] = argv[1]  # Add key and value to the dictionary.
        argv = argv[1:]  # Reduce the argument list by copying it starting from index 1.
    return opts

# FIXME: Add all RAML supported types
ramltype_to_cpp_map = {"integer" : "int",
    "string" : "std::string",
    "number" : "double",
    "boolean" : "json_bool",
    "any" : "void *",
    }
ramltype_to_json_new_fn_map = {"integer" : "json_object_new_int",
    "string" : "json_object_new_string",
    "number" : "json_object_new_double",
    "boolean" : "json_object_new_boolean",
    }
ramltype_to_json_get_fn_map = {"integer" : "json_object_get_int",
    "string" : "json_object_get_string",
    "number" : "json_object_get_double",
    "boolean" : "json_object_get_boolean",
    }

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

filters = {
    "ramltype_to_cpp": ramltype_to_cpp,
    "strip_ilegal_chars" : strip_ilegal_chars,
    "json_get_fn" : json_get_fn,
    "json_new_fn" : json_new_fn
}

def generate_types(env, raml):
    fh = open(headers_out_path + "/all_types.h","w")

    if not 'types' in raml:
        fh.write(env.get_template("all_types_header.h").render(all_types = None))
        return
    else:
        fh.write(env.get_template("all_types_header.h").render(all_types = raml['types']))


    # Gen the types classes
    for type_name, type_def in raml['types'].items():
        cont = env.get_template("types_header.h").render(this_type = type_def,
            type_name = type_name,
            all_types = raml['types'])
        fh = open(headers_out_path + "/types/" + type_name + ".h", "w")
        fh.write(cont)




def generate_api_class(env, raml):
    class_name = "Service" + raml['title'].replace(" ", "").capitalize()

    # Create the headers and sources
    header = open(headers_out_path + "/service/" + class_name + ".h","w")
    source = open(source_out_path + "/service/" + class_name + ".cpp","w")

    verbs = {}

    # Filter all the verbs
    for key, value in raml.items():
        if '/' is not key[:1]:
            continue

        verbs[key[1:]] = value

    print(verbs)

    header_cont = env.get_template("service_class_header.h").render(
            class_name = class_name,
            verbs = verbs
            )

    source_cont = env.get_template("service_class_source.c").render(
            class_name = class_name,
            verbs = verbs
            )

    header.write(header_cont)
    source.write(source_cont)

def generate_agl_service(env, raml):
    api_name = raml['title'].replace(" ", "").lower()

    verbs = {}

    # Filter all the verbs
    for key, value in raml.items():
        if '/' is not key[:1]:
            continue

        verbs[key[1:]] = value

    source = open(source_out_path + "/service/" + api_name + "-binding-auto.cpp","w")
    source_cont = env.get_template("agl_service.c").render(
            class_name = "Service" + api_name.capitalize(),
            api_name = api_name,
            verbs = verbs
            )

    source.write(source_cont)


def creat_dir(path):
    if not os.path.exists(path):
        os.makedirs(path)


if __name__ == '__main__':
    from sys import argv
    myargs = getopts(argv)


    if '-i' in myargs:
        print(myargs['-i'])
        f = open(myargs['-i'], "r")
        y=yaml.load(f.read())
    else:
        exit(1)

    if '-o' in myargs:
        out_path = myargs['-o']

    if '-h' in myargs:
        headers_out_path = myargs['-h']

    if '-s' in myargs:
        source_out_path = myargs['-s']

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

    jy = json.loads(json.dumps(y));

    # Generate the Types
    generate_types(env, jy)
    generate_api_class(env, jy)
    generate_agl_service(env, jy)

    print(y)
    print(json.dumps(y))

    print(myargs)
