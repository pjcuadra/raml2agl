#!/usr/bin/env python

from distutils.core import setup

setup(name='RAML 2 AGL',
      version='1.0',
      description='RAML model to AGL Service implementation',
      author='Pedro Cuadra',
      author_email='pjcuadra@gmail.com',
      url='https://github.com/pjcuadra/raml2agl',
      packages=['raml2agl'],
      scripts=['raml2agl/scripts/raml2agl'],
      install_requires=[
          'jinja2',
          'pyyaml'
      ],
      # py_modules=['Raml2Agl', 'RamlParser'],
      package_data={'raml2agl': ['templates/*',
                                 'templates/app/*',
                                 'templates/service/*',
                                 'templates/types/app/*']},

      )
