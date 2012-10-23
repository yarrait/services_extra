<?php

function _comments_resource_access($op = 'view', $args = array()) {
  switch ($op){
     case 'view':
      return user_access('administer comments');
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
     'actions' => array(
        'retrivecomments' => array(
          'access callback' => '_comment_resource_access',
          'access arguments' => array('view'),
          'callback' => '_comment_resource_count',
          'args'     => array(
            array(
              'name'         => 'cid',
              'type'         => 'int',
              'description'  => t('The node id to count all comments.'),
              'source'       => array('data' => 'cid'),
              'optional'     => FALSE,
            ),
          ),
        ),
     ),
  );
  return $resource;
}

function _comments_resource_retrieve($cid = 0,$page = 0) {
  if($cid && $page){
  } 
  else if($cid){
   $page=0;
  }
  $total_num_comment=db_result(db_query("SELECT count(*) from  comments"));
  $result=db_query("SELECT * from  comments where cid>%d ORDER BY cid desc",$cid);
  while($total=db_fetch_object($result)){
        $new_comments[]=$total;
  }
  $j=0;$k=0;
  for($i = 0; $i<count($new_comments); $i= $i+10)
  {
      $page_comments[$k]=array_slice($new_comments,$j,10);
      $j=$j+10;
      $l=$l+10;
      $k++;
  }
  $total['total'] = $total_num_comment;
  $total['array'] = $page_comments[$page]; 
  return $total;
}

function _comments_resource_index($page, $fields, $parameters, $page_size) { 
  // Limit to published nodes if user doesn't have 'administer nodes'
  // permissions.
  $key=array_keys($parameters);
  $value=array_values($parameters);
  if(empty($parameters))
    $var=db_result(db_query("select count(*) from comments"));
  else if($key[1])
    $var=db_result(db_query("select count(*) from comments where $key[0]=%d && $key[1]=%d",$value[0],$value[1]));
  else if($key[0])
    $var=db_result(db_query("select count(*) from comments where $key[0]=%d",$value[0]));

  $query = services_resource_build_index_query('comments', 'c.timestamp DESC', $page, $fields, $parameters, 'c', 'cid', 10, 'comment');
  // Put together array of matching nodes to return.
  $results = array();
  while ($comments = db_fetch_object($query)) {
    $results[] = $comments;
  }
  $total['total'] = $var;
  $total['array'] = $results; 
  return $total;
}

function _comment_resource_count($cid)
{
     return comment_num_all($cid);
    return _comment_load($cid);
}

