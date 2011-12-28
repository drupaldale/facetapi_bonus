<?php

/**
 * @file
 *
 * Filter to rewrite facet items via a configurable callback function
 */

/**
* Facetapi plugin for rewriting facet items via a configurable callback function
*/
class FacetapiFilterRewriteItems extends FacetapiFilter {

  /**
   * Filters facet items.
   */
  public function execute(array $build) {
    $rewrite_callback = check_plain(trim($this->settings->settings['rewrite_callback_function']));
    if (function_exists($rewrite_callback)) {
      $rewrite_callback($build, $this);
    }
    else {
      drupal_set_message(t("Rewrite callback '$rewrite_callback' is not defined (FacetapiFilterRewriteItems::execute)."), 'warning');
    }
    return $build;
  }

  /**
   * Adds configurable rewriting callback
   */
  function settingsForm(&$form, &$form_state) {
    $form['rewrite_callback_function'] = array(
      '#title' => t('Rewrite callback function name'),
      '#type' => 'textfield',
      '#description' => t("The name of the PHP function used for rewriting the facet items before rendering, following the pattern <i>function_name(&\$build, &\$widget)</i>"),
      '#default_value' => isset($this->settings->settings['rewrite_callback_function']) ? $this->settings->settings['rewrite_callback_function'] : NULL,
    );

    parent::settingsForm($form, $form_state);
  }

  /**
   * Returns defaults for the settings this widget provides.
   */
  function getDefaultSettings() {
    return array('rewrite_callback_function' => '');
  }
}
