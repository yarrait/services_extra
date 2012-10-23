<?php

function _watchdog_resource_access($op = 'view', $args = array()) {
  switch($op) 
  {
    case 'view':
      return user_access('access administration pages');
  }
}

function watchdog_resources() 
{
  $resource = array(
      'retrieve' => array(
        'callback' => '_watchdog_resource_retrieve',
        'args' => array(
          array(
            'name' => 'wid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The wid of the watchdog to get',
          ),
        ),
        'access callback' => '_watchdog_resource_access',
        'access arguments' => array('view'),
        'access arguments append' => TRUE,
      ),
     'index' => array(
        'callback' => '_watchdog_resource_index',
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
        'access arguments' => array('access administration pages'),
        'access arguments append' => FALSE,
      ),
  );
  return $resource;
}

function _watchdog_resource_retrieve($id) {
  $result = db_query('SELECT w.*, u.name, u.uid FROM {watchdog} w INNER JOIN {users} u ON w.uid = u.uid WHERE w.wid = %d', $id);
  return db_fetch_object($result);
}

function _watchdog_resource_index($page, $fields, $parameters, $page_size) 
{ 
  $result = db_query('Select w.*, u.name from watchdog w left outer join users u on w.uid = u.uid order by wid desc');
  $results = array();
  $no_records=db_result(db_query('Select count(*) from watchdog'));
  while ($watchdog = db_fetch_object($result))
  {
      $results[] = $watchdog;
  }

  $j=0;$k=0;
  for($i = 0; $i<count($results); $i= $i+10)
  {
      $page_watchdog[$k]=array_slice($results,$j,10);
      $j=$j+10;
      $l=$l+10;
      $k++;
   }
   $total['total'] = $no_records;
   $total['array'] = $page_watchdog[$page]; 
   return $total;
  
} 
