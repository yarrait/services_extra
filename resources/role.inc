<?php

/**
 * Gets access rules.
 *
 * @param $op
 *  $op - Permission requested
 * @return Boolean
 *  Boolean TRUE if the current user has the requested permission
 */
function _role_resource_access($op = 'view', $args = array()) {
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
function role_resources() {
  $resource = array(
    'retrieve' => array(
      'callback' => '_role_resource_retrieve',
        'args' => array(
          array(
            'name' => 'rid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The rid of the node to get',
          ),
        ),
        'access callback' => '_role_resource_access',
        'access arguments' => array('view'),
        'access arguments append' => TRUE,
    ),
    'create' => array(
      'callback' => '_role_resource_create',
        'args' => array(
          array(
            'name' => 'role',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The role data to create',
            'type' => 'array',
          ),
        ),
        'access callback' => '_role_resource_access',
        'access arguments' => array('create'),
        'access arguments append' => TRUE,
    ),
    'update' => array(
      'callback' => '_role_resource_update',
        'args' => array(
          array(
            'name' => 'rid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The rid of the node to get',
          ),
          array(
            'name' => 'data',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The role data to update',
            'type' => 'string',
          ),
        ),
        'access callback' => '_role_resource_access',
        'access arguments' => array('update'),
        'access arguments append' => TRUE,
    ),
    'delete' => array(
      'callback' => '_role_resource_delete',
        'args' => array(
          array(
            'name' => 'rid',
            'optional' => FALSE,
            'type' => 'int',
            'source' => array('path' => 0),
          ),
        ),
        'access callback' => '_role_resource_access',
        'access arguments' => array('delete'),
        'access arguments append' => TRUE,
    ),
    'index' => array(
      'callback' => '_role_resource_index',
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
 * Returns the role retrieved. 
 *
 * @param $id
 *   $id - role id
 * @return
 *   role retrieved
 */
function _role_resource_retrieve($id) {
  $role = db_query('SELECT * FROM role WHERE rid = %d', $id);
  return db_fetch_object($role);
}

/**
 * Returns role list. 
 *
 * @param $page $fields $parameters $page_size
 *   $page - The page number
 *   $fields - role fields
 *   $parameters - role parameters
 *   $page_size - page size
 * @return
 *   role list
 */
function _role_resource_index($page, $fields, $parameters, $page_size) { 
  $query = db_query("select * from role order by rid asc");
  $role = array();
  while ($result = db_fetch_object($query)) {
      $role[] = $result;
  }
 return services_resource_build_index_list($role, 'role','rid');
}

/**
 * Returns created role
 *
 * @param $role
 *   $role - data to create role
 * @return
 *   created role
 */
function _role_resource_create($role){
  $role = _services_arg_value($role, 'role');
  module_load_include('inc', 'user', 'user.admin');
  $form_state = array();
  $form_state['values'] = $role;
  $form_state['values']['op'] = variable_get('services_role_save_button_resource_create', t('Add role'));
  drupal_execute('user_admin_new_role', $form_state);
  if ($errors = form_get_errors()) {
    return services_error(implode(" ", $errors), 406, array('form_errors' => $errors));
  } 
  else {
    $rid = db_query("select rid from role order by rid desc limit 1");
    $roleid = db_fetch_object($rid); 
    $role_service_uri['rid']=$roleid -> rid;
    $role_service_uri['uri']=services_resource_uri(array('user',$roleid -> rid));
    return $role_service_uri;
  }
}

/**
 * Returns updated role
 *
 * @param $rid $role
 *   $rid - role id
 *   $role - data to update role
 * @return
 *   updated role
 */
function _role_resource_update($rid, $role){
  $role = _services_arg_value($role, 'roles');
  $role['rid'] = $rid;
  $role_name =$role['role']['name'];
  if (empty($role)) {
    return services_error("role name filed is required", 406,array('form_errors' => "role[name] field is required."));
  }
  if (db_result(db_query("SELECT COUNT(*) FROM {role} WHERE  rid = %d", $rid))) {
    db_query("UPDATE {role} SET name = '%s' WHERE rid = %d", $role_name, $rid);
    $role_service_uri['role'] =$role;
    $role_service_uri['rid'] =$rid;
    return $role_service_uri;
  }
  return services_error("There is no role with id ".$rid, 406,null);
}

/**
 * Returns Boolean value
 *
 * @param $rid
 *   $rid - role id
 * @return
 *  Boolean TRUE if the role is deleted else FALSE
 */
function _role_resource_delete($rid){
  if ($rid == 1 || $rid == 2) {
    return services_error("your request will not accepted ",212,null);
  }	
  if (db_result(db_query('SELECT rid FROM {role} WHERE rid = %d', $rid))) {
    db_query('DELETE FROM {role} WHERE rid = %d',$rid);
    db_query('DELETE FROM {permission} WHERE rid = %d',$rid);
    db_query('DELETE FROM {users_roles} WHERE rid = %d',$rid);
  return TRUE;
  }
  return services_error("There is no role with id ".$rid,406,null);
}
