<?php

/**
 * Gets access rules.
 *
 * @param $op
 *  $op - Permission requested
 * @return Boolean
 *  Boolean TRUE if the current user has the requested permission
 */
function _content_update_resource_access($op = 'view', $args = array()) {
  switch ($op) {
    case 'create':
      return user_access('administer nodes');
      break;
  }
}

/**
 * Gets all resource definitions.
 *
 * @return array
 *  An array containing all resources.
 */
function content_update_resources() {
  $resource = array(
    'create' => array(
      'callback' => '_content_update_resource_create',
        'args' => array(
          array(
            'name' => 'content_data',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The content data to update',
            'type' => 'array',
          ),
        ),
        'access callback' => '_content_update_resource_access',
        'access arguments' => array('create'),
        'access arguments append' => TRUE,
    ),
  );
  return $resource;
}

/**
 * Returns updated content. 
 *
 * @param $content_data
 *   $content_data - content data to update
 * @return 
 *   true if updated else service error
 */
function _content_update_resource_create($content_data){
  $result = array();
  $option=$content_data['option'];
  if(empty($option) || empty($content_data['node']))
   return services_error("option or nid field is requried" ,406,array('form_errors' => "option or nid field is requried."));

  if($option == "publish" || $option == "promote to front page" || $option == "make sticky")
      $values=1;
  if($option == "unpublish" || $option == "demote from front page" || $option == "remove stickiness")
      $values=0;
  
  $count_node = count($content_data['node']);
  $count_update_node = 0;
  foreach ($content_data['node'] as $key => $value) {
    if (is_numeric($key)) {
      $nid = $value['nid'];
        if (db_result(db_query("SELECT COUNT(*) FROM {node} WHERE  nid = %d", $nid))) {
          $total_nid[] = $nid;
   	    if ($nid != null && ($option == "publish" || $option == "unpublish")) { 
   	      $result = db_query("UPDATE node SET status = '%d' WHERE nid = %d", $values, $nid);
   	      $count_update_node++;
   	    }
   	    else if ($nid != null && ($option == "promote to front page" || $option == "demote from front page")){
   	      $result = db_query("UPDATE {node} SET promote = '%d' WHERE nid = %d", $values, $nid);
	      $count_update_node++;
	    }
            else if ($nid != null && ($option == "make sticky" || $option == "remove stickiness")) { 
	      $result = db_query("UPDATE {node} SET sticky = '%d' WHERE nid = %d", $values, $nid);
	      $count_update_node++;
	    }
            else if ($nid != null && $option == "delete") { 
              $result = db_query("DELETE from node WHERE nid = %d",$nid);
	      $count_update_node++;
            }
            else {
              return services_error("There is no option of this type ".$option, 406, null);   
            }
        }
        else {
          $error_nid[] = $nid;
        } 
    }
    else {
      services_error('JSON format is not proper',400);
    }
  }
  if ($count_update_node == $count_node) {
    return TRUE;
  }
  if (!empty($error_nid)) {
    $not_found_nid = implode(",",$error_nid);
    return services_error("There is no node with id ".$not_found_nid , 406, null);
  }
}
