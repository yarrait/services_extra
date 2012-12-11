<?php


function _form_content_resource_access($op = 'view', $args = array()) {
  switch ($op) {
    case 'view':
      return user_access('administer users');
      break;
  }
}



function form_content_resources() {
  $resource = array(
    'retrieve' => array(
      'callback' => '_form_content_resources_retrieve',
        'args' => array(
          array(
            'name' => 'type',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'string',
            'description' => 'Provide name of content type to get form structure',
          ),
        ),
        'access callback' => '_form_content_resource_access',
        'access arguments' => array('view'),
        'access arguments append' => TRUE,
    ),
  );
  return $resource;
}


function _form_content_resources_retrieve($type) { 
  module_load_include('inc', 'node', 'node.pages');
  global $user;

  $types = node_get_types();
  $type = isset($type) ? str_replace('-', '_', $type) : NULL;

  // If a node type has been specified, validate its existence.
  if (isset($types[$type]) && node_access('create', $type)) {
    // Initialize settings:
    $node = array(
      'uid' => $user->uid,
      'name' => (isset($user->name) ? $user->name : ''),
      'type' => $type,
      'language' => '',
    );

    $form = druproid_get_form($type.'_node_form', $node);
    druproid_fix_node_form_admin_order($form);
    druproid_convert_form_options_to_strict_array($form);
 
  }

  return $form;

}


/**
 * Apply the same modification that theme_node_form does on component order
 *
 * This allows us to present form components in the same order as the HTML UI
 *
 * @see theme_node_form()
 */

function druproid_fix_node_form_admin_order(&$form) {
  if ($form['author']) {
    $form['author']['#weight'] = 99997;
  }
  if (isset($form['options'])) {
    $form['options']['#weight'] = 99998;
  }
  if (isset($form['buttons'])) {
    $form['buttons']['#weight'] = 99999;
  }
}


/**
 * Converts form component's options to strict array
 *
 * Walk through each form component recursively for #options array.
 * Converts #options array to a strict array (not a associative array)
 * in order to keep the order revelant, as hashes don't have specific order.
 *
 * Also replace 0 value as "0" (string) so that it isn't interpreted as BOOL value
 * by Objective-C.
 *
 * @param &$form
 */

function druproid_convert_form_options_to_strict_array(&$form) {
  $children_keys = element_children($form);
  foreach ($children_keys as $child_key) {
    // Reached an optionable component (select or radios)
    if (isset($form[$child_key]['#options']) && is_array($form[$child_key]['#options'])) {
      $options = array();
      // Replaces #options with a strict array since
      // hashes don't keep order outside of PHP
      foreach ($form[$child_key]['#options'] as $key => $value) {
        if (is_object($value)) {
          $option = $value->option;
          reset($option); // Shouldn't be useful
          $options[] = array(key($option) => current($option));
        }
        else {
          // Dirty hack, but no choice
          if ($value === 0 && $key === 0) {
            $key = "0";
          }
          $options[] = array((string) $key => $value);
        }
      }
      // Shiny new options ordered
      $form[$child_key]['#options'] = $options;
    }
    // Continue deeper in the form structure
    druproid_convert_form_options_to_strict_array($form[$child_key]);
  }
}



function druproid_get_form($form_id) { 

  $form_state = array('storage' => NULL, 'submitted' => FALSE);

  $args = func_get_args();
  $cacheable = FALSE;

  if (isset($_SESSION['batch_form_state'])) { 
    // We've been redirected here after a batch processing : the form has
    // already been processed, so we grab the post-process $form_state value
    // and move on to form display. See _batch_finished() function.
    $form_state = $_SESSION['batch_form_state'];
    unset($_SESSION['batch_form_state']);
  }
  else { 
    // If the incoming $_POST contains a form_build_id, we'll check the
    // cache for a copy of the form in question. If it's there, we don't
    // have to rebuild the form to proceed. In addition, if there is stored
    // form_state data from a previous step, we'll retrieve it so it can
    // be passed on to the form processing code.
    if (isset($_POST['form_id']) && $_POST['form_id'] == $form_id && !empty($_POST['form_build_id'])) { 
      $form = form_get_cache($_POST['form_build_id'], $form_state);
    }

    // If the previous bit of code didn't result in a populated $form
    // object, we're hitting the form for the first time and we need
    // to build it from scratch.
    if (!isset($form)) {
      $form_state['post'] = $_POST;
      // Use a copy of the function's arguments for manipulation
      $args_temp = $args;
      $args_temp[0] = &$form_state;
      array_unshift($args_temp, $form_id);

      $form = call_user_func_array('drupal_retrieve_form', $args_temp);
      $form_build_id = 'form-'. md5(uniqid(mt_rand(), TRUE));
      $form['#build_id'] = $form_build_id;
      drupal_prepare_form($form_id, $form, $form_state);
      // Store a copy of the unprocessed form for caching and indicate that it
      // is cacheable if #cache will be set.
      $original_form = $form;
      $cacheable = TRUE;
      unset($form_state['post']);
    }
    $form['#post'] = $_POST;

    // Now that we know we have a form, we'll process it (validating,
    // submitting, and handling the results returned by its submission
    // handlers. Submit handlers accumulate data in the form_state by
    // altering the $form_state variable, which is passed into them by
    // reference.
    drupal_process_form($form_id, $form, $form_state);
    if ($cacheable && !empty($form['#cache'])) {
      // Caching is done past drupal_process_form so #process callbacks can
      // set #cache.
      form_set_cache($form_build_id, $original_form, $form_state);
    }
  }

  // Most simple, single-step forms will be finished by this point --
  // drupal_process_form() usually redirects to another page (or to
  // a 'fresh' copy of the form) once processing is complete. If one
  // of the form's handlers has set $form_state['redirect'] to FALSE,
  // the form will simply be re-rendered with the values still in its
  // fields.
  //
  // If $form_state['storage'] or $form_state['rebuild'] has been set
  // and input has been processed, we know that we're in a complex
  // multi-part process of some sort and the form's workflow is NOT
  // complete. We need to construct a fresh copy of the form, passing
  // in the latest $form_state in addition to any other variables passed
  // into drupal_get_form().

  if ((!empty($form_state['storage']) || !empty($form_state['rebuild'])) && !empty($form_state['process_input']) && !form_get_errors()) {
    $form = drupal_rebuild_form($form_id, $form_state, $args);
  }

  return $form;
}
