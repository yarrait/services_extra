<?php

function _vocabulary_term_resource_access($op = 'view', $args = array()) {
  switch ($op) {
    case 'view':
     return user_access('administer taxonomy');
  }
}

function vocabulary_term_resources() {
  $resource = array(
    'retrieve' => array(
      'callback' => '_vocabulary_term_resource_retrieve',
        'args' => array(
          array(
            'name' => 'vid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The vid of the vocabulary to get',
          ),
        ),
        'access callback' => '_vocabulary_term_resource_access',
        'access arguments' => array('view'),
        'access arguments append' => TRUE,
    ),
    'index' => array(
      'callback' => '_vocabulary_term_resource_index',
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
        'access arguments' => array('administer taxonomy'),
        'access arguments append' => FALSE,
    ),
  );
  return $resource;
}

function _vocabulary_term_resource_retrieve($id) {
  $vocabulary_term = taxonomy_get_tree($id);
  return $vocabulary_term;
}

function _vocabulary_term_resource_index($page, $fields, $parameters, $page_size) { 
  $query = services_resource_build_index_query('term_data', 'vd.vid desc', $page, $fields, $parameters, 'vd', 'vid', 10, 'term_data');
  while ($results = db_fetch_object($query)) {
      $vocabulary_term[] = $results;
  }
  return $vocabulary_term;
}
