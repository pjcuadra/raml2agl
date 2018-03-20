.. _r_constraints:

Constraints
===========

.. _r_constraints_technical:

Technical Constraints
---------------------

The technical constraints are shown in :numref:`r_t_technical_constraints`.

.. _r_t_technical_constraints:
.. table:: Technical Constraints Table
  :widths: 6 15

  +--------+----------------------+--------------------------------------------+
  | **ID** | **Constraint**       | **Description**                            |
  +========+======================+============================================+
  | Software and programming constraints                                       |
  +--------+----------------------+--------------------------------------------+
  | TC1    | Programming Language | There's no explicit constraint regarding   |
  |        |                      | the programming language to be used.       |
  +--------+----------------------+--------------------------------------------+
  | Operating system constraints                                               |
  +--------+----------------------+--------------------------------------------+
  | TC2    | AGL distribution     | The developed MDD methodology shall apply  |
  |        |                      | for developing for AGL Linux Distribution  |
  +--------+----------------------+--------------------------------------------+
  | Hardware Constraints                                                       |
  +--------+----------------------+--------------------------------------------+
  | TC3    | Memory friendly      | The applications developed with the MDD    |
  |        |                      | approach shall consider good memory        |
  |        |                      | management practices.                      |
  +--------+----------------------+--------------------------------------------+

.. _r_constraints_convention:

Conventions
-----------

Finally, conventions used by this project are shown in
:numref:`r_t_conventions`.

.. _r_t_conventions:
.. table:: Organizational Constraints Table
  :widths: 6 15

  +--------+----------------------+--------------------------------------------+
  | **ID** | **Constraint**       | **Description**                            |
  +========+======================+============================================+
  | C1     | Documentation        | The documentation is written using the     |
  |        |                      | arc42 document structure and using Sphinx. |
  +--------+----------------------+--------------------------------------------+
  | C2     | Coding conventions   | For C/C++ and Python (used for the MDD)    |
  |        |                      | development the coding styles used were    |
  |        |                      | the Linux Kernel coding style              |
  |        |                      | :cite:`kernel_style` and PEP8 :cite:`pep8`,|
  |        |                      | respectively.                              |
  +--------+----------------------+--------------------------------------------+


.. only:: html

  .. bibliography:: ../_static/references.bib
    :style: plain
    :filter: docname in docnames
