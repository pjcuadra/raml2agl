.. _r_deployment_view:

Deployment View
===============

:numref:`r_deployment_raml2agl_root` and
:numref:`r_deployment_raml2agl_root_templates` show the structure
of the :code:`raml2agl` repository. Note that :code:`src/template/` directory
holds all the templates that feed the *Jinja2 Environment* to generate the
components also shown in the corresponding diagram.

.. _r_deployment_raml2agl_root:
.. uml:: deployment_raml2agl_root.uml
  :caption: Deployment Diagram of RAML2AGL Root
  :align: center
  :scale: 80

.. _r_deployment_raml2agl_root_templates:
.. uml:: deployment_raml2agl_root_templates.uml
  :caption: Deployment Diagram of RAML2AGL Root (With Templates)
  :align: center
  :scale: 80

Moreover, the Application and Service source files are separately compiled
and deployed at different abstraction layers within the AGL architecture.
:numref:`r_deployment_runtime`

.. _r_deployment_runtime:
.. uml:: deployment_runtime.uml
  :caption: Deployment Diagram of RAML2AGL Root (Runtime)
  :align: center
  :scale: 40


.. only:: html

  .. bibliography:: ../_static/references.bib
    :style: plain
    :filter: docname in docnames
