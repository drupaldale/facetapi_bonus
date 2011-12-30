


INSTALLATION


Simply save this module's subdir in your contrib module directory,
and enable the module.



USAGE


Within the Facet API UI of a specific facet, go to "Dependency" or "Filters" to
add one of this module's plugins. The usage of the specific plugins are well
explained within their settings forms.



MODULE DESCRIPTION


Facet API Bonus for Drupal 7 is a collection of additional Facet API plugins and
functionality, foremost filter and dependency plugins – And a place to collect
more additional Facet API extensions.

Currently Facet API Bonus includes:

* Facet Dependency: Dependency plugin to make one facet (say "product category")
  to show up depending on other facets or specific facet items being active
  (say "content type" is "product" or "service"). Very flexible,
   supports multiple facets to be dependencies, as well as regexp for specifying
   facet item dependencies, as well as option how to behave if a dependency is
   being lost.
* Exclude Items: Filter plugin to exclude certain facet items by their
  markup/title or internal value (say excluding "page" from "content types").
  Regexp are also possible.
* Rewrite Items: Filter plugin to rewrite labels or internals of the facet items
  via a admin-configurable callback function (in a structured array, before
  rendering). Very handy to rewrite list field values or totally custom encoded
  facet values for user friendly output.


===> Further additions are very welcome! <===

Facet API Bonus is written for Drupal 7, and is stable, tested,
and ready to be used in production environments.

Requirements:

* Facet API is obviously required, as well as
  a compatible search module (e.g. apachesolr, search_api).

Similar projects:

* http://drupal.org/project/facetapi_extra, created at the same time,
  seems to aim for the same goal, containing but one smaller dependency plugin
  at the moment. Facet API Bonus offers more different plugins for more use
  cases, the plugins in Facet API Bonus are richer in function and number
  at the moment. Maintainers will later discuss further distinction or merge.



MODULE URL


More information and issues, see the module page, currently at:

  http://drupal.org/sandbox/daniel.nolde/1387636
