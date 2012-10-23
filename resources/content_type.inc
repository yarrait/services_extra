<?php

function _content_type_resource_access($op = 'view', $args = array()) {
  switch ($op) {
    case 'view':
     return user_access('administer content types');
   }
}

function content_type_resources() {
  $resource = array(
     'retrieve' => array(
        'callback' => '_content_type_resource_retrieve',
	  'args' => array(
          array(
            'name' => 'type',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'string',
            'description' => 'The type of the node to get',
          ),
        ),
        'access callback' => '_content_type_resource_access',
        'access arguments' => array('view'),
        'access arguments append' => TRUE,
      ),
     'index' => array(
        'callback' => '_content_type_resource_index',
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
        'access arguments' => array('administer content types'),
        'access arguments append' => FALSE,
      ),
  );
  return $resource;
}

function _content_type_resource_retrieve($value) {
  $result = db_query("SELECT * FROM node_type WHERE type = '$value' ");
  return db_fetch_object($result);
}

function _content_type_resource_index($page, $fields, $parameters, $page_size){ 
  $query = services_resource_build_index_query('node_type', 'td.name asc', $page, $fields, $parameters, 'td', 'type', 10, 'node_type');
  $results = array();
  while ($node = db_fetch_object($query)) {
      $results[] = $node;
  }
  return services_resource_build_index_list($results, 'node_type','type');
}
