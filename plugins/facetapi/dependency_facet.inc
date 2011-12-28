<?php

/**
 * @file
 *
 * Performs a dependency check against other specified facets/facet values
 *
 * ToDos:
 * - remove depending facet from url if this dependency gets lost
 *
 */

/**
 * Adds a dependency on another facet being active.
 */
class FacetapiDependencyFacet extends FacetapiDependency {

  protected $default_settings;

  /**
   * Executes the dependency check.
   */
  public function execute() {
    $active_facets = $this->getActiveFacets();

    // show this facet, if it is active itself and forced deactivation isn't on
    if (!$this->settings['force_deactivation'] && isset($active_facets[$this->facet['name']])) {
      return TRUE;
    }
    else {
      // show this facet if no facet dependencies are configured
      $facets = array_filter($this->settings['facets']);
      if (!count($facets)) {
        return TRUE;
      }
      else {
        // show this facet, if any configured facet dependency ...
        foreach ($facets as $facet_name) {
          // ... is met ...
          if (isset($active_facets[$facet_name])) {
            // ... without fine grained facet items dependencies ...
            if (empty($this->settings['facet_items_' . $facet_name])) {
              return TRUE;
            }
            else {
              $items_string = trim($this->settings['facet_items_' . $facet_name]);
              $facet_items = explode(',', $items_string);
              // ... without any real fine grained facet items values ...
              if (empty($items_string) || !count($facet_items)) {
                return TRUE;
              }
              // ... or with fine grained facet item dependencies ...
              else {
                foreach ($facet_items as $facet_item) {
                  if (in_array(trim($facet_item), $active_facets[$facet_name])) {
                    return TRUE;
                  }
                }
              }
            }
          }
        }

        // ... otherwise hide this facet
        return FALSE;
      }
    }
  }

  /**
   * Adds dependency settings to the form.
   */
  public function settingsForm(&$form, &$form_state) {
    // Get enabled facets (minus this dependency's one)
    $facets = $this->getEnabledFacets($form['#facetapi']['settings']->searcher);
    unset($facets[$form['#facetapi']['settings']->facet]);

    $form[$this->id]['facets'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Show this facet only for specific other facets'),
      '#default_value' => $this->settings['facets'],
      '#options' => array_map('check_plain', $facets),
      '#description' => t('Show this facet only if another one of these selected facet(s) is actively in use. If you select no other facets, the facet will be visible not depend on other facets.'),
    );

    $form[$this->id]['force_deactivation'] = array(
      '#type' => 'checkbox',
      '#title' => t('Force deactivation for unmet dependency.'),
      '#default_value' => $this->settings['force_deactivation'],
      '#description' => t('Deactivate this facet if dependency is not met anymore, even if this facet has had active items itself (recommended)'),
    );

    // add optional value input field and extend default settings for each facet
    foreach ($facets as $facet_name => $facet_title) {
      $id = 'facet_items_' . $facet_name;
      $checkboxid = '#edit-facets-' . str_replace('_', '-', drupal_strtolower($facet_name));
      $form[$this->id][$id] = array(
        '#title' => t('Facet items for %facet', array('%facet' => $facet_title)),
        '#type' => 'textfield',
        '#description' => t("Comma separated list of facet items to depend on. If an exclude item starts with '/^' and ends with '/', it is used as a regular expression with preg_match to match against an item's title or value."),
        '#default_value' => isset($this->settings[$id]) ? $this->settings[$id] : NULL,
        '#states' => array('visible' => array($checkboxid => array('checked' => TRUE))),
      );
      $this->default_settings[$id] = '';
    }
  }

  /**
   * Returns defaults for settings.
   *
   * Has to be dynamic since we want a detail settings field for each dependency
   * but the enabled facets are not known yet (afaik, hence the add. property)
   */
  public function getDefaultSettings() {
    if (!count($this->default_settings)) {
      $this->default_settings = array('facets' => array(), 'force_deactivation' => TRUE);
    }
    return $this->default_settings;
  }

  /**
   * Determine enabled facets
   *
   * Returns an associative array facet name => title
   * of factes enabled for the given searcher
   */
  public function getEnabledFacets($searcher = array()) {
    $facets = array();

    facetapi_get_enabled_facets($searcher, NULL);
    $enabled_facets = &drupal_static('facetapi_get_enabled_facets', array());
    $enabled_facets = $enabled_facets[$searcher . ':'];

    foreach ($enabled_facets as $facet_key => $facet_object) {
      $facets[$facet_key] = t($facet_object['label']) . ' (' . $facet_object['name'] . ')';
    }

    return $facets;
  }

  /**
   * Determine active facets and their values
   *
   * Returns an associative array: active facet name => array of facet value(s)
   */
  public function getActiveFacets() {
    $active_facets = array();
    if (isset($_GET['f'])) {
      $facets_request_arguments = $_GET['f'];
      if (count($facets_request_arguments)) {
        foreach ($facets_request_arguments as $facets_request_argument) {
          $facets_request_argument = explode(':', $facets_request_argument);
          if (count($facets_request_argument) == 2) {
            $active_facets[check_plain($facets_request_argument[0])][] = check_plain($facets_request_argument[1]);
          }
        }
      }
    }
    return $active_facets;
  }

}
