<?php

function _role_resource_access($op = 'view', $args = array()) {
   switch ($op) {
    case 'view':
     return user_access('administer users');
    case 'update':
     return user_access('administer users');
    case 'delete':
     return user_access('administer users');
    case 'create':
     return user_access('administer users'); 
    }
}

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

function _role_resource_retrieve($id) {
  $result = db_query('SELECT * FROM role WHERE rid = %d', $id);
  return db_fetch_object($result);
}

function _role_resource_index($page, $fields, $parameters, $page_size) { 
  $query = services_resource_build_index_query('role', 'rd.rid asc', $page, $fields, $parameters, 'rd', 'rid', 10, 'role');
  $results = array();
  while ($node = db_fetch_object($query)) {
      $results[] = $node;
  }
 return services_resource_build_index_list($results, 'role','rid');
}

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
  else{
   $rid=db_query("select rid from role order by rid desc limit 1");
   $roleid=db_fetch_object($rid); 
   $result['rid']=$roleid->rid;
   $result['uri']=services_resource_uri(array('user',$roleid->rid));
  return $result;
  }
}

function _role_resource_update($rid, $role){
  $role = _services_arg_value($role, 'roles');
  $role['rid'] = $rid;
  $role =$role['role']['name'];
  if(empty($role)){
    return services_error("role name filed is required", 406,array('form_errors' => "role[name] field is required."));
  }
  if(db_result(db_query("SELECT COUNT(*) FROM {role} WHERE  rid = %d", $rid))) {
      db_query("UPDATE {role} SET name = '%s' WHERE rid = %d", $role, $rid);
      $return['role'] =$role;
      $return['rid'] =$rid;
     return $return;
    }
  return services_error("There is no role with id ".$rid, 406,null);
}

function _role_resource_delete($rid){
  if($rid==1 || $rid==2){
    return services_error("your request will not accepted ",212,null);
  }	
  if(db_result(db_query('SELECT rid FROM {role} WHERE rid = %d', $rid))){
    db_query('DELETE FROM {role} WHERE rid = %d',$rid);
    db_query('DELETE FROM {permission} WHERE rid = %d',$rid);
    db_query('DELETE FROM {users_roles} WHERE rid = %d',$rid);
  return true;
  }
  return services_error("There is no role with id ".$rid,406,null);
}
