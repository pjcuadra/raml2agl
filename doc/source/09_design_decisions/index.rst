Design Decisions
================

RESTful Modeling Language Selection
-----------------------------------

There is a handful of Modeling Language that can be used for modeling RESTful
APIs. The main criteria to select the modeling language to be used was that it
has to be machine- and human-readable format, filtering the possibilities to
those using JSON and YAML formats. Options like *API Blueprint* were filtered
out because it's written using Markdown which is more human-readable but much
less machine-readable. In contrast, XML-based modeling languages were also left
out, because it is not a human-readable format.

The analysis was, therefore, focus on *OpenAPI* and *RAML*. Nevertheless, after
analyzing their specifications :cite:`openapi_spec` and :cite:`raml_spec`, RAML
was considered to be equally descriptive and much less verbose.

Python for :code:`raml2agl`
---------------------------

Python was selected to develop :code:`raml2agl`, because of its simplicity.
Also, there are many Python libraries that make the development process faster
and easier. For instance, Jinja2 makes the entire automatic code generation with
relatively less effort. Python YAML parsing library is also used for RAML
parsing. Moreover, Python's dictionaries are a key language feature that proofs
to be useful for parsing file's content. As shown in :cite:`python_perf` doesn't
perform the best compared to a comparable implementation in other languages.
Nevertheless, a high performance isn't required from :code:`raml2agl` since the
code generation isn't being done online nor frequently.

RAML Parser vs :code:`pyraml-parser`/:code:`ramlifications`
-----------------------------------------------------------

Even though there are reference implementations of a RAML parser called,
:code:`pyraml-parser` and :code:`ramlifications`, it was decided to not use them
for now since they only support up to RAML 0.8, whereas :code:`raml2agl` plans
to support RAML 1.0.

This fact adds a little overhead to the development and also includes some risks
(discussed in :ref:`r_risk_python_raml`). Nevertheless, the RAML Parser didn't
represent much effort to develop and generates the expected behavior
efficiently.

Since :code:`ramlifications` plans to support RAML 1.0
:cite:`ramlfications_repo`, it might be a good idea to integrate it into the
RAML Parser once it's supported.

.. _r_design_rpc:

RPC over Web Socket Communication
---------------------------------

Web Socket communication is a powerful communication and design pattern. For
instance, Web Socket Communication enables bi-directional and asynchronous
communication. Whereas, RPC is a unidirectional and synchronous communication.

Therefore, implementing an RPC on top of Web Socket Communication means losing
some communication capabilities. This design decision is probably the most
important done regarding the MDD approach.

Even when the RPC communication model isn't desired, :code:`raml2agl` can still
be used. For instance, it can still be used to automatically generate the
*AGL Service* and the *Service Class*, since the RPC model is only implemented
in *APP Class* and *WebSocketApi*.

.. only:: html

  .. bibliography:: ../_static/references.bib
    :style: plain
    :filter: docname in docnames
