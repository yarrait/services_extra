<?php

function _permission_module_user_resource_access($op = 'view', $args = array()) {
  switch ($op) {
    case 'view':
     return user_access('administer permissions');
    case 'update':
     return user_access('administer permissions');
    }
}

function user_permission_resources() {
  $resource = array(
      'retrieve' => array(
        'callback' => '_permission_module_user_resource_retrieve',
        'args' => array(
          array(
            'name' => 'modules',
            'optional' => TRUE,
            'source' => array('path' => 0),
            'type' => 'string',
            'description' => 'The permission of the module to get',
          ),
          array(
            'name' => 'roles',
            'optional' => TRUE,
            'source' => array('param' => 'role'),
            'type' => 'int',
            'description' => 'The enabled permission of the role to get',
          ),
        ),
        'access callback' => '_permission_module_user_resource_access',
        'access arguments' => array('view'),
        'access arguments append' => TRUE,
      ),
     'update' => array(
        'callback' => '_permission_module_user_resource_update',
        'args' => array(
          array(
            'name' => 'rid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The rid of the role to get',
          ),
          array(
            'name' => 'permission',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The permission data to update',
            'type' => 'array',
          ),
        ),
        'access callback' => '_permission_module_user_resource_access',
        'access arguments' => array('update'),
        'access arguments append' => TRUE,
      ),
      'index' => array(
        'callback' => '_permission_module_user_resource_index',
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
     'access arguments' => array('administer permissions'),
        'access arguments append' => FALSE,
       ),
  );
  return $resource;
}

function _permission_module_user_resource_retrieve($module = NULL, $role = 0) {
//permission_module_user/block?role=1
  if($module && $role) {
    $role_object=db_query("select name from role where rid=%d",$role);
    $role_name=db_fetch_object($role_object);
    $result = db_query('SELECT p.perm FROM {permission} p WHERE p.rid = %d', $role);
    while ($role_record = db_fetch_object($result)) {
      $role_permissions = $role_record->perm;
    } 
    $role_permissions = explode(", ",$role_permissions);
    $permissions = module_invoke($module, 'perm');
    $count1=count($role_permissions);$count=count($permissions);
    for ($i = 0; $i<$count1; $i++) {
      for ($j=0;$j<$count;$j++) {
        if ($permissions[$j] == $role_permissions[$i]) {
          $match_array['Enable'][] = $permissions[$j];
        }
      }
    }
  if (empty($match_array))
     return services_error("There are no anabled permissions for ".$role_name->name , 404,null);
  else
     return $match_array;
}
  else if ($module && !$role) {
    $output[$module]=module_invoke($module, 'perm');
    return $output;
  }
 return services_error('No module with ' .$module.' found.', 404);
}

function _permission_module_user_resource_index($page, $fields, $parameters, $page_size) {
  foreach (module_list(FALSE, FALSE, TRUE) as $module) {
    if ($permissions = module_invoke($module, 'perm')) {
           $module_list["modules"][] = $module;
       }
    } 
  return $module_list;
}

function _permission_module_user_resource_update($rid, $permissions) {
  $return = array();
  $final_perm = array();
  $role = db_fetch_object(db_query('SELECT * FROM {role} where rid=%d', $rid));

  if(empty($permissions['perm']['module']) || empty($permissions['perm']['enabled']))
    return services_error("module name or permission field is requried" ,406,array('form_errors' => "module name or permission field is requried."));

  if(!empty($role)) {
     $perm = db_fetch_object(db_query('SELECT perm FROM {permission} where rid=%d', $role->rid));
     $current_perm = explode(', ',$perm->perm);
 
     if(!empty($perm)){
       $role_permissions = explode(", ",$perm->perm);
       $module_perm = module_invoke($permissions['perm']['module'], 'perm');
       $module_enabled_perm =array_intersect($role_permissions, $module_perm);
       $perm_diff = array_diff($current_perm, $module_enabled_perm);
       db_query('DELETE FROM {permission} WHERE rid = %d', $role->rid);

       if(count($permissions['perm']['enabled'])) {
         $final_perm = array_merge($perm_diff, $permissions['perm']['enabled']);
         db_query("INSERT INTO {permission} (rid, perm) VALUES (%d, '%s')", $role->rid, implode(', ', $final_perm));
         $return['perm'] = db_fetch_object(db_query('SELECT pid, rid FROM {permission} where rid=%d', $role->rid));
         $return['status_message'] = 'The changes have been saved.';
       }
     } 
     else {
        if(count($permissions['perm']['enabled'])) {
	  db_query("INSERT INTO {permission} (rid, perm) VALUES (%d, '%s')", $role->rid, implode(', ', $permissions['perm']['enabled']));
          $return['perm'] = db_fetch_object(db_query('SELECT pid, rid FROM {permission} where rid=%d', $role->rid));
          $return['status_message'] = 'The changes have been saved.';
        }
     }
  }
  else {
     return services_error("There is no role with id ".$rid, 406,null);
   } 
  return $return;   
}

