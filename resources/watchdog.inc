<?php

function _watchdog_resource_access($op = 'view', $args = array()) {
  switch ($op) {
    case 'view':
      return user_access('access administration pages');
      break;
  }
}

function watchdog_resources() {
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
  $watchdog = db_query('SELECT w.*, u.name, u.uid FROM {watchdog} w INNER JOIN {users} u ON w.uid = u.uid WHERE w.wid = %d', $id);
  return db_fetch_object($watchdog);
}

function _watchdog_resource_index($page, $fields, $parameters, $page_size) { 
  $no_records = db_result(db_query('Select count(*) from watchdog'));
  $watchdog_data['total'] = $no_records;
  $result = db_query('Select w.*, u.name from watchdog w left outer join users u on w.uid = u.uid order by wid desc');
  $watchdog = array();
  while ($results = db_fetch_object($result)){
    $watchdog[] = $results;
  }
  $start_index = 0;
  $page_no = 0;
  $last_index = 10;
  for ($i = 0; $i<count($watchdog); $i = $i + $page_size) {
    $page_watchdog[$page_no] = array_slice($watchdog,$start_index,$page_size);
    $start_index = $start_index + $page_size;
    $last_index = $last_index + $page_size;
    $page_no = $page_no + 1;
  }
  $watchdog_data['array'] = $page_watchdog[$page]; 
  return $watchdog_data;
} 
