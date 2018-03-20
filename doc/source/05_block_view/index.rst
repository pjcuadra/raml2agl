.. _r_block_view:

Building Block View
===================

To understand where the proposed :abbr:`MDD (Model Drive Development)` approach
has its importance, the components involved in the Unix Web Socket communication
have to be presented. :numref:`r_block_web_socket` presents these components.

.. _r_block_web_socket:
.. uml:: block_web_socket.uml
  :caption: Web Socket Communication Component Diagram
  :align: center
  :scale: 50

Since the *AGL Application Framework* and its API are already given in the
AGL architecture, the rationale behind the design was to wrap the
*AGL Application Framework API* and the Web Socket communication in an RPC-like
approach. Moreover, the components were encapsulated applying functional
decomposition. :numref:`r_t_block_top_resp` shows the responsibilities for
each of the components in :numref:`r_block_web_socket`.

.. _r_t_block_top_resp:
.. table:: Top Block Components Responsibilities
  :widths: 6 15

  +---------------------------+------------------------------------------------+
  | **Name**                  | **Responsibility**                             |
  +===========================+================================================+
  | AGL Application Framework | Manage all AGL Services and their life cycle,  |
  |                           | Create Unix Web Socket for the RESTfull API to |
  |                           | be served by the AGL Services, Forward         |
  |                           | RESTfull API verb calls to AGL Services verbs  |
  |                           | callbacks, Verbs authentication process        |
  |                           | handling.                                      |
  +---------------------------+------------------------------------------------+
  | AGL Service               | Initialize service resources, serve the        |
  |                           | RESTfull API, Forward the RESTfull API verbs   |
  |                           | to the corresponding Service Class method,     |
  |                           | Unmarshal JSON messages as to parse            |
  |                           | corresponding Service Class method parameters, |
  |                           | Marshal output parameters of Service Class as  |
  |                           | JSON to reply through Unix Web Socket.         |
  +---------------------------+------------------------------------------------+
  | Service Class             | Implements the intended functionality at       |
  |                           | service side for each RESTfull API verb.       |
  +---------------------------+------------------------------------------------+
  | Application               | Use functionality exposed by the AGL Services  |
  |                           | to achieve a user-visible purpose.             |
  +---------------------------+------------------------------------------------+
  | APP Class                 | Exposes all RESTfull API verbs as methods      |
  |                           | with input and output parameters, Marshal      |
  |                           | parameters as JSON to send requests to the     |
  |                           | Unix Web Socket, Unmarshal JSON replies to     |
  |                           | update output parameters.                      |
  +---------------------------+------------------------------------------------+
  | WebSocketApi              | Handle Unix Web Socket connection, Form        |
  |                           | RESTfull API request, Wait for RESTfull API    |
  |                           | replies.                                       |
  +---------------------------+------------------------------------------------+


:code:`raml2agl` features an automatic code generation tool developed.
:numref:`r_block_raml2agl` shows the building blocks of the tool and its
relations with the possible outputs.

.. _r_block_raml2agl:
.. uml:: block_raml2agl.uml
  :caption: RAML2AGL Block Diagram
  :align: center

As shown in :numref:`r_block_raml2agl`, :code:`raml2agl` generates code for the
*Service Class*, *App Class*, and the *AGL Service*; the last two are fully
generated. Note that the automatically generated components are the ones with
more responsibilities, as shown in :numref:`r_t_block_top_resp`. This fact was
also the rationale behind the definition of the components, to automate most of
the process and reduce the overhead of creating a new *Service* and/or
*Application*. Moreover, :numref:`r_t_block_raml2agl_resp` shows the
responsibilities of each of the :code:`raml2agl` components.

.. _r_t_block_raml2agl_resp:
.. table:: RAML2AGL Components Responsibilities
  :widths: 6 15

  +---------------------------+------------------------------------------------+
  | **Name**                  | **Responsibility**                             |
  +===========================+================================================+
  | RAML Parser               | Read the RAML model and create a JSON model to |
  |                           | be pass to the Jinja2 templates.               |
  +---------------------------+------------------------------------------------+
  | Jinja2 Environment        | Manage the templates, render the templates     |
  |                           | using the JSON model.                          |
  +---------------------------+------------------------------------------------+
  | raml2agl main             | Read the RAML model from a file, Control the   |
  |                           | entire generation flow, reads input command    |
  |                           | line parameters, Calls the RAML Parser to      |
  |                           | generate JSON model, Calls the Jinja2          |
  |                           | Environment to render the corresponding        |
  |                           | templates.                                     |
  +---------------------------+------------------------------------------------+


Service Class
~~~~~~~~~~~~~

:numref:`r_block_generated_example` shows an example of the output of
:code:`raml2agl` using the following model;

.. code::

  #%RAML 1.0
  title: Example
  mediaType: application/json
  version: v1
  baseUri: localhost:8000/api?token=x
  /method_1:
    post:
      body:
        properties:
          param_in_1:
            type: integer
    get:
      responses:
        200:
          body:
            properties:
              param_out_1:
                type: integer
  /method_2:
    post:
      body:
        properties:
          param_in_1:
            type: string
    get:
      responses:
        200:
          body:
            properties:
              param_out_1:
                type: string


.. _r_block_generated_example:
.. uml:: block_generated_example.uml
  :caption: Generated Example
  :align: center
  :scale: 50

Note that *Service Class* isn't fully automatic generated. Nevertheless, a
skeleton of the entire class with all the methods definition is generated. Is
the task of the *Service* developer to finish the implementation of the
functionality. Moreover, each method represents a verb of the RESTfull API.
Hence, :code:`/example/method_1` will shall be implemented in
:code:`ServiceExample.method_1(...)`. Furthermore, the model title is the parsed
to name the RESTfull API and both classes.

WebSocketApi
~~~~~~~~~~~~

:numref:`r_block_web_socket_api` class diagram shows the definition of the
:code:`WebSocketApi` class.

.. _r_block_web_socket_api:
.. uml:: block_web_socket_api.uml
  :caption: Web Socket API Class Diagram
  :align: center
  :scale: 50

Moreover, below the description of each of the classes members.

.. doxygenclass:: WebSocketApi
  :project: raml2agl
  :members:
  :protected-members:
  :private-members:

APP Class
~~~~~~~~~

As shown in :numref:`r_block_generated_example` the :code:`Example` *APP Class*
has symmetric methods with :code:`ServiceExample`. Therefore, a call to
:code:`Example.method_1` will call :code:`/example/method_1` RESTfull API
through the Unix Web Socket. Note that every *APP Class* is completely
automatically generated. Moreover, *APP Class* inherits :code:`WebSocketApi`
and implements the entire Unix Web Socket communication its methods.

AGL Service
~~~~~~~~~~~

An AGL service is basically the implementation of the
*Application Framework API* shown in :numref:`r_block_af_api`.

.. _r_block_af_api:
.. uml:: block_af_api.uml
  :caption: AGL Application Framework API :cite:`docs_bindings_reference`
  :align: center
  :scale: 50


Furthermore, to implement :numref:`r_block_generated_example`, for instance,
a null-terminated list of verbs has to be defined as follows;

.. code-block:: c

  static const struct afb_verb_v2 verbs[] = {
  	/*Without security*/
  	{.verb = "method_1", .callback = method_1, .auth = NULL, .info = "method_1", .session = 0},
  	{.verb = "method_2", .callback = method_2, .auth = NULL, .info = "method_2", .session = 0},
  	{.verb = NULL, .callback = NULL, .auth = NULL, .info = NULL, .session = 0 }
  };

Note that for an initial implementation the authentication mechanisms weren't
implemented. Nevertheless, it has been included in the :code:`raml2agl`'s road
map, see :cite:`raml2agl_repo`.

And finally, to register the entire API to the *AGL Application Framework* the
:code:`afb_binding_v2` structure is automatically generated as follows.

.. code-block:: c

  const struct afb_binding_v2 afbBindingV2 = {
    .api = "example",
    .specification = "",
    .info = "Auto generated - Example",
  	.verbs = verbs,
  	.preinit = NULL,
    .init = init,
  	.onevent = NULL,
    .noconcurrency = 1
  };

RAML Parser
~~~~~~~~~~~

:numref:`r_block_parser` presents the internals of the RAML Parser component.
Furthermore, the responsibilities of each of the sub-components are stated in
:numref:`r_t_block_raml_parser_resp`

.. _r_block_parser:
.. uml:: block_parser.uml
  :caption: RAML Parser Block Diagram
  :align: center
  :scale: 50

.. _r_t_block_raml_parser_resp:
.. table:: RAML2 Parser Sub-components Responsibilities
  :widths: 6 15

  +---------------------------+------------------------------------------------+
  | **Name**                  | **Responsibility**                             |
  +===========================+================================================+
  | Root Attributes Parser    | Parse the RAML root attributes like; title and |
  |                           | base URI.                                      |
  +---------------------------+------------------------------------------------+
  | Methods Parser            | Parse the RAML verbs as methods                |
  +---------------------------+------------------------------------------------+
  | Input Parameters Parser   | Parse the RAML verbs' input parameters         |
  +---------------------------+------------------------------------------------+
  | Output Parameters Parser  | Parse the RAML verbs' output parameters        |
  +---------------------------+------------------------------------------------+
  | Types Parser              | Parse the RAML verbs' parameters' types        |
  +---------------------------+------------------------------------------------+


raml2agl main
~~~~~~~~~~~~~

:numref:`r_block_raml2agl_main` presents the internals of the *RAML2AGL main*
component. Furthermore, the responsibilities of each of the sub-components are
stated in :numref:`r_t_block_raml2agl_main_resp`

.. _r_block_raml2agl_main:
.. uml:: block_raml2agl_main.uml
  :caption: RAML2AGL main Block Diagram
  :align: center


.. _r_t_block_raml2agl_main_resp:
.. table:: RAML2AGL main Sub-components Responsibilities
  :widths: 6 15

  +---------------------------+------------------------------------------------+
  | **Name**                  | **Responsibility**                             |
  +===========================+================================================+
  | Command Line Arguments    | Parses the command line arguments to configure |
  | Parser                    | the *File Generator*.                          |
  +---------------------------+------------------------------------------------+
  | Templates Filters         | Defines Jinja2 Template filters to convert     |
  |                           | data types from RAML format to C++.            |
  +---------------------------+------------------------------------------------+
  | Files Generator           | Passes the JSON model to render the templates  |
  |                           | to be built and write files to the selected    |
  |                           | output location.                               |
  +---------------------------+------------------------------------------------+


.. only:: html

  .. bibliography:: ../_static/references.bib
    :style: plain
    :filter: docname in docnames
