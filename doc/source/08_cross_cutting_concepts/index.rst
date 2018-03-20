Cross-cutting Concepts
======================

RPC over Web Socket
-------------------

Since the :code:`raml2agl` implements an RPC over a Web Socket,
:numref:`r_concepts_rpc` shows a generic RPC and
:numref:`r_concepts_ws` shows a generic Web Socket communication. Note that
in order to communicate over Web Socket a connection between *Client* and
*Server* has to be acknowledged. Similarly, the connection has to be closed
once it's not going to be used anymore. This part is handled in the
*WebSocketApi* constructor and destructor, respectively. Moreover, the
*APP Class* and the *AGL Service* handle the messaging and thus simulating an
RPC.

.. _r_concepts_rpc:
.. uml:: concepts_rpc.uml
  :caption: RPC Model
  :align: center
  :scale: 80

.. _r_concepts_ws:
.. uml:: concepts_ws.uml
  :caption: Web Socket Model
  :align: center
  :scale: 80


.. only:: html

  .. bibliography:: ../_static/references.bib
    :style: plain
    :filter: docname in docnames
