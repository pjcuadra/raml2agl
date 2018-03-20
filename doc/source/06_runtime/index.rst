.. _r_runtime_view:

Runtime View
============

RAML2AGL Generation
~~~~~~~~~~~~~~~~~~~

:numref:`r_run_time_raml2agl_gen` presents the sequence of the :code:`raml2agl`
run for automatically generate *APP Class*, *WebSocketApi*, *AGL Service* and
*Service Class*.

.. _r_run_time_raml2agl_gen:
.. uml:: run_time_raml2agl_gen.uml
  :caption: RAML2AGL Generation
  :align: center
  :scale: 50


AGL Service Start
~~~~~~~~~~~~~~~~~

It's important to have some insight on how *AGL Services* are initialized and
how the *Unix Web Socket* gets created. Therefore,
:numref:`r_run_time_service_start` shows this process.

.. _r_run_time_service_start:
.. uml:: run_time_service_start.uml
  :caption: AGL Service Start
  :align: center
  :scale: 50


Web Socket Communication
~~~~~~~~~~~~~~~~~~~~~~~~

The *Web Socket Communication* can only happen after the AGL Service is already
running, thus the *Unix Web Socket* was already created and the RESTfull API
is being served. :numref:`r_run_time_web_socket` shows the sequence how the
entire communication takes place.

.. _r_run_time_web_socket:
.. uml:: run_time_web_socket.uml
  :caption: Web Socket Communication
  :align: center
  :scale: 40

Note that the *Application* using the *APP Class* will have the entire Web
Socket communication abstracted as simple method calls. Hence, an RPC model
is implemented on top of the RESTful API. :numref:`r_run_time_web_socket_2`
shows this abstracted communication sequence.

.. _r_run_time_web_socket_2:
.. uml:: run_time_web_socket_2.uml
  :caption: Web Socket Communication
  :align: center
  :scale: 50

.. only:: html

  .. bibliography:: ../_static/references.bib
    :style: plain
    :filter: docname in docnames
