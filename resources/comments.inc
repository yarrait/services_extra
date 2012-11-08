<?php

function _comments_resource_access($op = 'view', $args = array()) {
  switch ($op) {
    case 'view':
      return user_access('administer comments');
      break;
    }
  }

function comments_resources() {
  $resource = array(
    'retrieve' => array(
      'callback' => '_comments_resource_retrieve',
        'args' => array(
          array(
            'name' => 'cid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'int',
            'description' => 'Get recently added commnets',
          ),
          array(
            'name' => 'page',
            'optional' => TRUE,
            'source' => array('param' => 'page'),
            'type' => 'int',
            'description' => 'The page number which want to display',
          ),
        ),
        'access callback' => '_comments_resource_access',
        'access arguments' => array('view'),
        'access arguments append' => TRUE,
    ),
    'index' => array(
      'callback' => '_comments_resource_index',
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
        'access arguments' => array('administer comments'),
        'access arguments append' => FALSE,
      ),
  );
  return $resource;
}

function _comments_resource_retrieve($cid = 0,$page = 0) {
  if($cid && $page){
  } 
  else if($cid){
   $page = 0;
  }
  $total_num_comment = db_result(db_query("SELECT count(*) from  comments"));
  $result = db_query("SELECT * from  comments where cid>%d ORDER BY cid desc",$cid);
  while ($total = db_fetch_object($result)) {
    $new_comments[] = $total;
  }
  $start_index = 0;
  $page_no = 0;
  $last_index = 10;
  for($index = 0; $index < count($new_comments); $index = $index + 10) {
    $page_comments[$page_no] = array_slice($new_comments,$start_index,$last_index);
    $start_index = $start_index + 10;
    $last_index = $last_index + 10;
    $page_no++;
  }
  $recent_comment['total'] = $total_num_comment;
  $recent_comment['array'] = $page_comments[$page]; 
  return $recent_comment;
}

function _comments_resource_index($page, $fields, $parameters, $page_size) { 
  $key = array_keys($parameters);
  $value = array_values($parameters);
  if (empty($parameters))
    $total_no_comments = db_result(db_query("select count(*) from comments"));
  else if ($key[1])
    $total_no_comments = db_result(db_query("select count(*) from comments where $key[0]=%d && $key[1]=%d",$value[0],$value[1]));
  else if ($key[0])
    $total_no_comments = db_result(db_query("select count(*) from comments where $key[0]=%d",$value[0]));
  $query = services_resource_build_index_query('comments', 'c.timestamp DESC', $page, $fields, $parameters, 'c', 'cid', 10, 'comment');
  $results = array();
  while ($results = db_fetch_object($query)) {
    $comment[] = $results;
  }
  $comments['total'] = $total_no_comments;
  $comments['array'] = $comment; 
  return $comments;
}
