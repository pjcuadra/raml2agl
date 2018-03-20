Solution Strategy
=================

The :abbr:`MDD (Model Drive Development)` approach developed is focused on
developing applications to run on top of AGL. The AGL architecture specifies
different layers of abstraction and the :abbr:`MDD (Model Drive Development)`
workflows shall be compliant with this architecture. Therefore, the
:abbr:`MDD (Model Drive Development)` process presented in this work focuses
on the development of AGL Services that use AGL's Applications Framework
APIs.

AGL services expose functionality to all the applications that might run
on top :cite:`agl_spec`. To be more specific AGL services are implemented
as :code:`systemd` user-defined services in AGL. The way they expose the
functionality is exposing a RESTfull API through a Web Sockets (or dbus).
Meaning that in order to access functionality exposed by an AGL service, the
application has to open a Web Socket use the RESTfull API.

The :abbr:`MDD (Model Drive Development)` approach presented in this document
focuses in defining a model of the RESTfull API. The model is then used as an
input for automatically generate the communication components of both the AGL
service and the AGL application.

For modeling the RESTfull API, :abbr:`RAML (RESTfull API)` was used.
:abbr:`RAML (RESTfull API Modeling Language)` is a recently developed community
standard that has already been widely adopted in other projects like;
*API Workbench* and *API Designer* :cite:`raml_projects`. It's a markup language
based in YAML, which makes it both; machine readable and human readable.

:code:`raml2agl` is written in Python (Python 3), which makes it really fast to
develop and portable. Although Python has already two reference implementations
of a RAML parser called :code:`pyraml-parser` :cite:`pyraml_repo` and
:code:`ramlifications` (developed by Spotify) :cite:`ramlfications_repo`, they
were not used for developing :code:`raml2agl` since they only support RAML 0.8
and :code:`raml2agl` plans to support RAML 1.0. Therefore, a custom RAML 1.0
parser was designed and implemented. :code:`ramlifications` plans to support
RAML 1.0 in the future. :cite:`ramlfications_repo` Therefore, :code:`raml2agl`
could adopt it in the future.

Another reason to use Python to write raml2agl is the variety of already
implemented components. Especially the support for Jinja2 templating language
was of high importance here. Jinja2 is a very powerful and complete templating
language with bindings for Python. :cite:`jinja2_doc` The code generation was
implemented using Jinja2 templates, which makes the code generation highly
flexible and fast to develop.

The final outcome of the automatic code generation is a set of C++ classes
that implement the entire RESTfull API communication. Moreover, simple C++
classes methods abstract the complex Web Socket handling and RESTfull API
message marshaling and unmarshaling. This approach can be compared with other
projects like Google's protobuffer :cite:`protobuffer_doc` that aims to
automatically generate the communication components.

.. only:: html

  .. bibliography:: ../_static/references.bib
    :style: plain
    :filter: docname in docnames
