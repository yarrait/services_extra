<?php

/**
 * Gets access rules.
 *
 * @param $op
 *  $op - Permission requested.
 * @return Boolean
 *  Boolean TRUE if the current user has the requested permission.
 */
function _accessrule_resource_access($op = 'view', $args = array()) {
  switch ($op) {
    case 'view':
      return user_access('administer users');
      break;
    case 'update':
      return user_access('administer users');
      break;
    case 'delete':
      return user_access('administer users');
      break;
    case 'create':
      return user_access('administer users');
      break;
    }
}

/**
 * Gets all resource definitions.
 *
 * @return array
 *  An array containing all resources.
 */
function accessrule_resources() {
  $resource = array(
    'retrieve' => array(
       'callback' => '_accessrule_resource_retrieve',
        'args' => array(
          array(
           'name' => 'aid',
           'optional' => FALSE,
           'source' => array('path' => 0),
           'type' => 'int',
           'description' => 'The aid of the accessrule to get',
          ),
        ),
        'access callback' => '_accessrule_resource_access',
        'access arguments' => array('view'),
        'access arguments append' => TRUE,
    ),
    'create' => array(
      'callback' => '_accessrule_resource_create',
        'args' => array(
          array(
            'name' => 'access',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The aid data to create',
            'type' => 'array',
          ),
        ),
        'access callback' => '_accessrule_resource_access',
        'access arguments' => array('create'),
        'access arguments append' => TRUE,
    ),
    'update' => array(
      'callback' => '_accessrule_resource_update',
        'args' => array(
          array(
            'name' => 'aid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The aid of the accessrule to get',
          ),
          array(
            'name' => 'data',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The accessrule data to update',
            'type' => 'array',
          ),
        ),
        'access callback' => '_accessrule_resource_access',
        'access arguments' => array('update'),
        'access arguments append' => TRUE,
    ),
    'delete' => array(
      'callback' => '_accessrule_resource_delete',
        'args' => array(
          array(
            'name' => 'aid',
            'optional' => FALSE,
            'type' => 'int',
            'source' => array('path' => 0),
          ),
        ),
        'access callback' => '_accessrule_resource_access',
        'access arguments' => array('delete'),
        'access arguments append' => TRUE,
    ),
    'index' => array(
      'callback' => '_accessrule_resource_index',
        'args' => array(
          array(
            'name' => 'page',
            'optional' => TRUE,
            'type' => 'int',
            'description' => 'The zero-based index of the page to get, defaults to 0.',
            'default value' => 0,
            'source' => array('param' => 'page'),
          ),
          array(
            'name' => 'fields',
            'optional' => TRUE,
            'type' => 'string',
            'description' => 'The fields to get.',
            'default value' => '*',
            'source' => array('param' => 'fields'),
          ),
          array(
            'name' => 'parameters',
            'optional' => TRUE,
            'type' => 'array',
            'description' => 'Parameters array',
            'default value' => array(),
            'source' => array('param' => 'parameters'),
          ),
          array(
            'name' => 'pagesize',
            'optional' => TRUE,
            'type' => 'init',
            'description' => 'Number of records to get per page.',
            'default value' => variable_get('services_node_index_page_size', 20),
            'source' => array('param' => 'pagesize'),
          ),
        ),
        'access arguments' => array('administer users'),
        'access arguments append' => FALSE,
    ),
  );
  return $resource;
}

/**
 * Returns the accessrule retrived. 
 *
 * @param $id
 *   The aid of the accessrule to get.
 * @return
 *   Accessrule retrived.
 */
function _accessrule_resource_retrieve($id) {
  $accessrule_retrive = db_query('SELECT * FROM access WHERE aid = %d', $id);
  return db_fetch_object($accessrule_retrive);
}

/**
 * Returns available the accessrules list. 
 *
 * @param $page $fields $parameters $page_size
 *   $page - The page number.
 *   $fields - accessrule fields
 *   $parameters - accessrule parameters (name, type etc..)
 *   $page_size - page size
 * @return
 *   An array of accessrule retrived.
 */
function _accessrule_resource_index($page, $fields, $parameters, $page_size) { 
  $query = services_resource_build_index_query('access', 'td.aid desc', $page, $fields, $parameters, 'td', 'aid', 10, 'access');
  $results = array();
  while ($node = db_fetch_object($query)) {
      $accessrule_index[] = $node;
  }
  return services_resource_build_index_list($accessrule_index, 'access','aid');
}

/**
 * Returns newly created accessrule. 
 *
 * @param $access_data
 *   $access_data - Data to create accessrule.
 * @return
 *   An accessrule created.
 */
function _accessrule_resource_create($access_data) {
  module_load_include('inc', 'user', 'user.admin');
  $access = array();
  $access['mask'] = $access_data['access']['mask'];
  $access['type'] = $access_data['access']['type'];
  $access['status'] = $access_data['access']['status'];
  $form_state = array();
  $form_state['values'] = $access;
  $form_state['values']['op'] = variable_get('services_rule_save_button_resource_create', t('Add rule'));
  drupal_execute('user_admin_access_form',$form_state);
  if ($errors = form_get_errors()) {
     return services_error(implode(" ", $errors), 406, array('form_errors' => $errors));
  }
  else {
   $aid = db_query("SELECT aid FROM access ORDER By aid DESC limit 1");
   $accessid = db_fetch_object($aid); 
   $result['aid'] = $accessid -> aid;
   $result['uri'] = services_resource_uri(array('user',$accessid -> aid));
  return $result;
  }
}

/**
 * Returns updated accessrule. 
 *
 * @param $aid
 *   $aid - accessrule Id.
 *   $access - accessrule data.
 * @return
 *   An updated accessrule.
 */
function _accessrule_resource_update($aid,$access) {
  $access_data = array();
  $access_data['mask'] = $access['access']['mask'];
  $access_data['type'] = $access['access']['type'];
  $access_data['status'] = $access['access']['status'];
  if (empty($access_data['mask'])) {
    return services_error("mask filed is required", 406,array('form_errors' => "access[mask] field is required."));
  }
  if (db_result(db_query("SELECT COUNT(*) FROM {access} WHERE  aid = %d", $aid))) {
    db_query("UPDATE {access} SET mask = '%s', type = '%s', status = '%s' WHERE aid = %d", $access_data['mask'], $access_data['type'], $access_data['status'], $aid);
    $access_data['aid'] = $aid;
    return $access_data;
   }
  return services_error("There is no accessrule with id ".$aid,406,null);
}

function _accessrule_resource_delete($aid) {
  if (db_result(db_query('SELECT aid FROM {access} WHERE aid = %d', $aid))) {
  db_query('DELETE FROM {access} WHERE aid = %d',$aid);
  return TRUE;
  }
  return services_error("There is no accessrule with id ".$aid,406,null);
}
