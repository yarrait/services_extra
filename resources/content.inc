<?php

function _content_resource_access($op = 'view', $args = array()) {
  switch ($op) {
    case 'view':
      return user_access('administer nodes');
      break;
    case 'create':
      return user_access('administer nodes');
      break;
  }
}

function content_resources() {
  $resource = array(
    'retrieve' => array(
      'callback' => '_content_resource_retrieve',
        'args' => array(
          array(
            'name' => 'nid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The nid of the node to get',
          ),
        ),
        'access callback' => '_content_resource_access',
        'access arguments' => array('view'),
        'access arguments append' => TRUE,
    ),
    'create' => array(
      'callback' => '_content_resource_create',
        'args' => array(
          array(
            'name' => 'node_data',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The content data to filter',
            'type' => 'array',
          ),
        ),
        'access callback' => '_content_resource_access',
        'access arguments' => array('create'),
        'access arguments append' => TRUE,
    ),
    'index' => array(
      'callback' => '_content_resource_index',
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
        'access arguments' => array('administer nodes'),
        'access arguments append' => FALSE,
      ),
  );
  return $resource;
}

function _content_resource_retrieve($nid) {
  global $user;
  $node = node_load($nid);
  if ($node) {
    if (module_exists('content') && variable_get('services_use_content_permissions', TRUE)) {
      $fields = content_fields(NULL, $node->type);
      foreach ($fields as $field_name => $field_info) {
        if (isset($node->$field_name)) {
          $access = module_invoke_all('field_access', 'view', $field_info, $user, $node);
          if (in_array(FALSE, $access)) {
            unset($node->$field_name);
          }
        }
      }
    }
    $node->uri = services_resource_uri(array('content', $node->nid));
    return $node;
  }
  return services_error('Node nid '. $nid .' not found', 404);
}

function _content_resource_index($page, $fields, $parameters, $page_size) {
  $page_size = 10;
  $total_no_records = db_result(db_query('Select count(*) from node'));
  $results = array();
  $result = db_query('Select n.nid,n.vid,n.type,n.language,n.title,n.uid,u.name,n.status,n.created,n.changed,n.comment,n.promote,n.moderate,n.sticky,n.tnid,
    n.translate from node n left outer join users u on n.uid = u.uid ORDER BY changed desc');
  while ($node = db_fetch_object($result)){
    $results[] = $node;
  }
  $start_index=0;
  $page_no=0;
  $last_index=10;
  for($index = 0; $index<count($results); $index = $index + $page_size) {
    $page_node[$page_no]=array_slice($results,$start_index,$last_index);
    $start_index = $start_index + $page_size;
    $last_index = $last_index + $page_size;
    $page_no++;
  }
  $content['total'] = $total_no_records;
  $content['contents'] = $page_node[$page]; 
  return $content;
}

function _content_resource_create($node_data){
  $return = array();
  $option = $node_data['node']['option'];
  $filter = $node_data['node']['filter'];
  $page = $node_data['node']['page'];
  $return = array();
  if(!empty($option) && !empty($filter))
  {
     if($option == "status")
     {
	switch($filter){
          case "published":
            $result = db_query('Select n.nid,n.vid,n.type,n.language,n.title,n.uid,u.name,n.status,n.created,n.changed,n.comment,n.promote,n.moderate,
              n.sticky,n.tnid,n.translate from node n left outer join users u on n.uid = u.uid WHERE  n.status = 1 and u.uid != 0');
            while ($rows = db_fetch_object($result)) {
               $results[] = $rows;
            }
            break;
   	  case "not published":
   	    $result = db_query('Select n.nid, n.vid, n.type, n.language, n.title, n.uid, u.name, n.status, n.created, n.changed, n.comment, n.promote, n.moderate, 
              n.sticky, n.tnid, n.translate from node n left outer join users u on n.uid = u.uid WHERE n.status = 0 and u.uid != 0');
   	    while ($rows = db_fetch_object($result)) {
               $results[] = $rows;
            }
            break;
   	  case "promoted":
   	   $result = db_query('Select n.nid,n.vid,n.type,n.language,n.title,n.uid,u.name,n.status,n.created,n.changed,n.comment,n.promote,n.moderate,n.sticky,n.tnid,
             n.translate from node n left outer join users u on n.uid = u.uid WHERE n.promote = 1 and u.uid != 0');
   	   while ($rows = db_fetch_object($result)) {
             $results[] = $rows;
           }
           break;
          case "not promoted":
   	   $result = db_query('Select n.nid,n.vid,n.type,n.language,n.title,n.uid,u.name,n.status,n.created,n.changed,n.comment,n.promote,n.moderate,n.sticky,n.tnid,
             n.translate from node n left outer join users u on n.uid = u.uid WHERE n.promote = 0 and u.uid != 0');
   	   while ($rows = db_fetch_object($result)) {
             $results[] = $rows;
           }
           break;
          case "sticky":
   	   $result = db_query('Select n.nid,n.vid,n.type,n.language,n.title,n.uid,u.name,n.status,n.created,n.changed,n.comment,n.promote,n.moderate,n.sticky,n.tnid,
             n.translate from node n left outer join users u on n.uid = u.uid WHERE n.sticky = 1 and u.uid != 0');
   	   while ($rows = db_fetch_object($result)) {
             $results[]=$rows;
           }
           break;
          case "not sticky":
   	   $result = db_query('Select n.nid,n.vid,n.type,n.language,n.title,n.uid,u.name,n.status,n.created,n.changed,n.comment,n.promote,n.moderate,n.sticky,n.tnid,
             n.translate from node n left outer join users u on n.uid = u.uid WHERE n.sticky = 0 and u.uid != 0');
   	   while ($rows = db_fetch_object($result)) {
             $results[] = $rows;
           }
           break;
   	}
     }
     if ($option == "type") {
       if (db_result(db_query("SELECT COUNT(*) FROM node_type WHERE  type='$filter' "))) { 
              $result = db_query("Select n.nid, n.vid, n.type, n.language, n.title, n.uid, u.name, n.status, n.created, n.changed, n.comment, n.promote, n.moderate, 
                n.sticky, n.tnid, n.translate from node n left outer join users u on n.uid = u.uid WHERE n.type = '$filter' and n.uid != 0 ");
   	      while ($rows = db_fetch_object($result)) {
                $results[] = $rows;
              }
        }
        else {
          return services_error("There is no node with type ".$filter,406,null);
        }
     }   
  }
  else{
     return services_error("node[option] or node[filter] field is requried", 406,array('form_errors' => "node[option] or node[filter] field is requried."));
   }
 if(empty($results))
   return services_error("There is no post available for ".$filter, 404, null);
  $page_size = 10;
  $start_index = 0;
  $page_no = 0;
  $last_index = 10;
  for ($index = 0; $index<count($results); $index = $index + $page_size) {
    $page_content[$page_no] = array_slice($results,$start_index,$last_index);
    $start_index = $start_index + $page_size;
    $last_index = $last_index + $page_size;
    $page_no++;
  }
  $content['total'] = count($results);
  $content['contents'] = $page_content[$page];
  return $content;
}
