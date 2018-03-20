Quality Requirements
====================

In this chapter, the quality requirement presented in
:ref:`r_intro_quality_reqs` are evaluated. Besides, other quality aspects are
also introduced an evaluated.

:code:`raml2agl` tool fulfills :need:`req_001` by maintaining a clear mapping
between the *Service Class*'s and the *APP Class*'s methods. Hence creating as
well an Object Oriented interface that abstracts the Unix Web Socket
communication and thus fulfilling :need:`req_002` as well.

The adoption of RAML as the interface modeling language speaks for the
fulfillment :need:`req_003`. Moreover, :code:`raml2agl` uses broadly adopted
tools, such as Jinja2. Also, :code:`raml2agl` follows standard coding styles
such as the Kernel's coding style and PEP8. Both broadly adopted tools and
the use of standard coding styles, also contribute towards :need:`req_003`
fulfillment.

:code:`raml2agl` allows the user to set the output directories and decide
what components to generate. Additionally, by supporting RAML :code:`raml2agl`
enables the user to generate a wide variety of interfaces. These are two
already-implemented customization mechanisms for the proposed MDD approach.
Therefore fulfilling :need:`req_004`.

As for :need:`req_005`, generates intermediate probing points with well-defined
interfaces which allows the user to develop unit testing for the system's
main components. For instance, the AGL service developer can create unit testing
for the *Service Class*, which would test the actual AGL Service's purpose.
Similarly, the AGL Service developer could interact with the RESTful interface
directly using tools like Postman :cite:`postman_site`. This will test a
different aspect of the components interaction, which is the marshaling and
unmarshaling of the JSON in the AGL Service side, as well as the mapping with
the *Service Class*'s methods. In the *APP Service* side, a similar testing can
be done to verify the marshaling and unmarshaling of the methods' parameters
into JSON.

By defining a standard interface also enables a decoupled development process,
where AGL Service and AGL Application can be developed in parallel. Moreover,
mocking :cite:`mock_object` mechanisms can be easily implemented using the
interface's definition. For instance, the *APP Service* interface could be
mocked using *Google Test* :cite:`googletest_repo`, thus enabling testing at
AGL application level without the need of running in the actual system, which
at the same time enables faster development.

Interestingly, the mocking and components unit tests can be also automatically
generated out of the RAML model, also contributing towards :need:`req_004`.
Moreover, by having a deterministic mapping between the RAML model and the
components' behavior correctness can be verified once and guaranteed for
everyone :cite:`model_base_code_verification`, thus minimizing the testing
effort. Note that correct memory management is also considered part of the
code's behavior correctness as it's one of the system's constraints (*TC3*) as
specified in :ref:`r_constraints`. For instance, all the developed unit testing
could be tested under memory management checking tools such as :code:`valgrind`
to validate its correctness. By doing so, the memory management correctness is
verified without any more testing effort since the same unit tests are run, but
on top of :code:`valgrind`. In fact, this was done while testing
:code:`raml2agl`'s behavior.

.. only:: html

  .. bibliography:: ../_static/references.bib
    :style: plain
    :filter: docname in docnames
