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
import sys
import logging


def check_single_level_verb(raml):

    error_found = False

    for key, value in raml.items():
        if '/' != key[:1]:
            continue

        # Check for nested verbs
        for subkey, value in value.items():
            if '/' == subkey[:1]:
                logging.error('RAML - {} - nested Verbs not supported'.format(
                    subkey))
                error_found |= True

        # Check for verbs with slashes also finding /verb/{id}
        if key[1:].find('/') != -1:
            logging.error('RAML - {} - nested Verbs not supported'.format(
                key))
            error_found |= True

    if error_found:
        sys.exit(1)


def validate(raml_file_path):
    # Needed to resolve includes
    # model_loc = os.path.dirname(raml_file_path)

    # Read the model
    f = open(raml_file_path, "r")
    yraml = yaml.load(f.read())

    # Check of title
    if 'title' not in yraml:
        logging.error('RAML - Title not specified')
        sys.exit(1)

    check_single_level_verb(yraml)
