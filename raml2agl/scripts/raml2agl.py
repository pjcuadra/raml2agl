#!/usr/bin/env python
from raml2agl import Raml2Agl as r2g
import os
import yaml
import json
import logging


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


def create_dir(path):
    if not os.path.exists(path):
        os.makedirs(path)


if __name__ == '__main__':
    from sys import argv
    myargs = getopts(argv)
    model_file = ''
    gen_service = False
    gen_service_class = False
    gen_app = False
    out_path = "./out"
    headers_out_path = out_path
    source_out_path = out_path

    # Parse all command line parameter
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

    if '--service' in myargs:
        gen_service = True
        headers_out_path = out_path + "/service"
        source_out_path = out_path + "/service"

    if '--service-class' in myargs:
        gen_service_class = True
        headers_out_path = out_path + "/service"
        source_out_path = out_path + "/service"

    if '--app' in myargs:
        gen_app = True
        headers_out_path = out_path + "/app"
        source_out_path = out_path + "/app"

    logging.info("Provided params - " + json.dumps(myargs,
                                                   sort_keys=True,
                                                   indent=4))

    # Create output directories
    create_dir(out_path)

    headers_out_path = headers_out_path + "/include"
    source_out_path = source_out_path + "/src"

    create_dir(source_out_path)
    create_dir(headers_out_path)

    # Generate the output
    generator = r2g.Raml2Agl()
    generator.set_source_out_path(source_out_path)
    generator.set_headers_out_path(headers_out_path)
    generator.set_input_model(model_file)

    if gen_service_class:
        generator.generate_service_class()

    if gen_service:
        generator.generate_service()

    if gen_app:
        generator.generate_app()
