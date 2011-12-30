<?php

/**
 * @file
 *
 * Filter to exclude specified facet items from being shown on the facet
 *
 * This is heavily inspired by the excellent blog post from Trellon
 * http://www.trellon.com/content/blog/apachesolr-and-facetapi
 */

/**
 * Plugin that excludes specified facet items.
 */
class FacetapiFilterExcludeItems extends FacetapiFilter {

  /**
   * Filters facet items.
   */
  public function execute(array $build) {
    $exclude_string = $this->settings->settings['exclude'];
    $exclude_array = explode(',', $exclude_string);
    // Exclude item if its markup or indexed_value is one of excluded items
    // or is matched by one of the excluded item treated as regexp if the
    // exclude items starts with "/".
    $filtered_build = array();
    foreach ($build as $key => $item) {
      $exclude = FALSE;
      foreach ($exclude_array as $exclude_item) {
        $exclude_item = trim($exclude_item);
        if (!empty($exclude_item) &&
            (($item['#indexed_value'] == $exclude_item) ||
              ($item['#markup'] == $exclude_item) ||
              (($exclude_item[0] == '/') && preg_match($exclude_item, $item['#indexed_value'])) ||
              (($exclude_item[0] == '/') && preg_match($exclude_item, $item['#markup']))
            )
          ) {
          $exclude = TRUE;
        }
      }
      if (!$exclude) {
        $filtered_build[$key] = $item;
      }
    }

    return $filtered_build;
  }

  /**
   * Adds settings to the filter form.
   */
  public function settingsForm(&$form, &$form_state) {
    $form['exclude'] = array(
      '#title' => t('Exclude items'),
      '#type' => 'textfield',
      '#description' => t("Comma separated list of titles or values that should be excluded. If an exclude item starts with '/^' and ends with '/', it is used as a regular expression with preg_match to match against an item's title or value."),
      '#default_value' => $this->settings->settings['exclude'],
    );
  }

  /**
   * Returns an array of default settings
   */
  public function getDefaultSettings() {
    return array('exclude' => '');
  }
}
