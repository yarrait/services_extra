<?php

function _vocabulary_resource_access($op = 'view', $args = array()) {
  switch ($op) {
    case 'view':
     return user_access('administer taxonomy');
   }
}

function vocabulary_resources() {
  $node_resource = array(
      'retrieve' => array(
       'help' => 'Retrieve  a latestcomment',
        'callback' => '_vocabulary_resource_retrieve',
	  'args' => array(
          array(
            'name' => 'vid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'The vid of the vocabulary to get',
          ),
        ),
        'access callback' => '_vocabulary_resource_access',
        'access arguments' => array('view'),
        'access arguments append' => TRUE,
      ),
      'index' => array(
        'callback' => '_vocabulary_resource_index',
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
  return $node_resource;
}

function _vocabulary_resource_retrieve($id) {
  $result = db_query('SELECT w.*, u.type FROM {vocabulary} w INNER JOIN {vocabulary_node_types} u ON w.vid = u.vid WHERE w.vid = %d', $id);
  return db_fetch_object($result);
}

function _vocabulary_resource_index($page, $fields, $parameters, $page_size) { 
  $page_size=10;
  $return =array();
  $j=0;$k=0;$l=$page_size;
  $output=taxonomy_get_vocabularies();
  foreach($output as $key => $vocab)
    $vacab_list[] = $vocab;
  for($i = 0; $i<count($vacab_list); $i= $i+$page_size)
  {
     $page_vocab[$k]=array_slice($vacab_list,$j,$l);
     $j=$j+$page_size;
     $l=$l+$page_size;
     $k++;
  }
  return services_resource_build_index_list($page_vocab[$page], 'vocabulary','vid');
}
