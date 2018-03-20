Introduction and Goals
======================

:abbr:`AGL (Automotive Grade Linux)` provides many development interfaces. For
instance, HTML5, JavaScript, and C/C++ applications can be developed to run on
top :abbr:`AGL (Automotive Grade Linux)`. However, development methodologies
aren't explicitly mentioned from :abbr:`AGL (Automotive Grade Linux)`'s
development team.


Requirements Overview
---------------------

This documentation presents an :abbr:`MDD (Model Drive Development)` methodology
to simplify and abstract the development process.

.. _r_intro_quality_reqs:

Quality Requirements
--------------------

Below, the quality requirements are presented.

.. req:: Transparency
  :id: req_001
  :tags: quality
  :hide_status:
  :hide_tags:
  :hide_links:

  The :abbr:`MDD (Model Drive Development)` methodology shall show a clear
  mapping between the components from involved layers.

.. req:: Abstraction
  :id: req_002
  :tags: quality
  :hide_status:
  :hide_tags:

  The :abbr:`MDD (Model Drive Development)` methodology shall provide a
  simplified abstract of the concepts in the underlying layers; e.g. Application
  Framework.

.. req:: Standardization
  :id: req_003
  :tags: quality
  :hide_status:
  :hide_tags:

  The developed solutions for the :abbr:`MDD (Model Drive Development)`
  methodology, shall use standard and predefined processes, methodologies,
  tools, and interfaces to facilitate their adoption.

.. req:: Flexibility
  :id: req_004
  :tags: quality
  :hide_status:
  :hide_tags:

  The :abbr:`MDD (Model Drive Development)` methodology should provide
  customization mechanisms.

.. req:: Testability and Debugability
  :id: req_005
  :tags: quality
  :hide_status:
  :hide_tags:

  The :abbr:`MDD (Model Drive Development)` methodology should provide
  mechanisms for testing and debug all main components.

.. only:: html

  .. bibliography:: ../_static/references.bib
    :style: plain
    :filter: docname in docnames
