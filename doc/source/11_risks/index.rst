Risks and Technical Debts
=========================

Each of the subsections discusses a risk and technical debt aspect.

.. _r_risk_python_raml:

PyRAML/ramlifications Adoption
------------------------------

As mentioned before, :code:`pyraml` nor :code:`ramlifications` weren't adopted
to develop :code:`raml2agl`. This leaves an important technical debt since the
compliance with the RAML standard isn't verified in the implemented RAML parser.
Meaning, that some modeling language syntax error in an input RAML model
wouldn't be caught.

Moreover, the by the time of writing this document, the RAML parser doesn't
support all RAML 1.0 features but are being increasingly supported. Thus,
creating a gap between the RAML 1.0 modeling features and the :code:`raml2agl`
features. Nevertheless, the most important RAML 1.0 modeling features are
supported in :code:`raml2agl`. Please review :cite:`raml2agl_repo` for an
updated list of RAML 1.0 supported features.

RPC Limitations
---------------

The use of an RPC communication model on top of Web Socket represents a risk
and technical debt since some applications might work better on top of the
raw Web Socket communication. Nevertheless, :code:`raml2agl` can still be used
for the client side automatically code generation as mentioned in
:ref:`r_design_rpc`.

.. only:: html

  .. bibliography:: ../_static/references.bib
    :style: plain
    :filter: docname in docnames
