<?php

function _user_filter_resource_access($op = 'view', $args = array()) {
  switch ($op) {
    case 'create':
      return user_access('administer users');
      break;
  }
}

function user_filter_resources() {
  $resource = array(
    'create' => array(
      'callback' => '_user_filter_resource_create',
        'args' => array(
          array(
            'name' => 'user_data',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The user data to filter',
            'type' => 'array',
          ),
        ),
        'access callback' => '_user_filter_resource_access',
        'access arguments' => array('create'),
        'access arguments append' => TRUE,
      ),
    );
  return $resource;
}

function _user_filter_resource_create($user_data){
  $return = array();
  $option = $user_data['user']['option'];
  $filter = $user_data['user']['filter'];
  $page = $user_data['user']['page'];
  
  if (empty($option) || empty($filter)) {
    return services_error("option or filter field is requried" ,406,array('form_errors' => "option or uid field is requried."));
  }
  if ($option == "role") {
     $no_records = db_result(db_query('Select count(*) from users_roles r inner join users u on r.uid = u.uid where r.rid=%d',$filter));
     $result = db_query('Select u.uid from users_roles r inner join users u on r.uid = u.uid where r.rid=%d ORDER BY uid DESC',$filter);
     while($row = db_fetch_object($result)){
           $user_details[] = user_load($row->uid);
     }
  }
  else if ($option == "permission") {
    $role = db_query('select rid from role where rid!=1');
    while($row = db_fetch_object($role)) {
      $result[] = $row->rid;
    }
    for ($i = 0; $i<count($result); $i++) {
      $perm = db_query('select perm from permission where rid=%d',$result[$i]);
      while ($rows = db_fetch_object($perm)){
        $results = $rows->perm;
      }
      $explode = explode(', ' ,$results);
      if (in_array($filter, $explode)) { 
        if ($result[$i] == 2) {
          $no_records = db_result(db_query('Select count(*) from users WHERE uid !=0'));
          $user = db_query("SELECT uid FROM users WHERE uid !=0  ORDER BY uid DESC");
          while ($row = db_fetch_object($user)){
            $user_details[] = user_load($row->uid);
          }
          $page_size = 20;
          $start_index = 0;
          $page_no = 0;
          $last_index = 20;
          for ($m = 0; $m<count($user_details); $m= $m + $page_size) {
            $page_user[$page_no] = array_slice($user_details,$start_index,$last_index);
            $start_index = $start_index + $page_size;
            $last_index = $last_index + $page_size;
            $page_no++;
          }
          $user_filter_data['total'] = $no_records;
          $user_filter_data['users'] = $page_user[$page];
          return $user_filter_data;
        }
        else { 
          $role_uid = db_query('select uid from users_roles where rid=%d ORDER BY uid DESC',$result[$i]);
          while ($user = db_fetch_object($role_uid)) {
            $user_uid[] = $user->uid;
          }
          for ($j = 0;$j<count($user_uid);$j++) {
            $user_details[] = user_load($user_uid[$j]);
          }
          $final_array = $user_details;unset($user_details);
          $final_no_records = $no_records;unset($no_records);
        } 
      }
    }
    if (empty($final_array)) {
      $user_root = db_fetch_object(db_query("SELECT uid FROM users WHERE uid =1")); 
      $root_user_detail['total'] = count($user_root);
      for ($i = 0; $i < count($user_root); $i++){
        $root_user_detail['users'][$i] = user_load($user_root->uid);
      }
      return $root_user_detail;
    }
    
//    $user_root = db_fetch_object(db_query("SELECT uid FROM users WHERE uid =1")); 
    
    $root_user_detail[] = user_load(1);
    $merge_user = array_merge($final_array,$root_user_detail);
    $size_array = count($merge_user);
    for ($i = 0; $i < $size_array; $i++) {
      for ($j = 0; $j < $size_array; $j++) {
        if ($i == $j) {
          continue;
        }
        else if ($merge_user[$i]->uid == $merge_user[$j]->uid) {
          unset ($merge_user[$i]);
        }
      }
    }
    foreach ($merge_user as $key =>$val) {
      $all_user[] = $val;
    }
    $page_size = 20;
    $start_index = 0;
    $page_no = 0;
    $last_index = 20;
    for ($i = 0; $i<count($all_user); $i= $i + $page_size) {
      $page_user[$page_no] = array_slice($all_user,$start_index,$last_index);
      $start_index = $start_index + $page_size;
      $last_index = $last_index + $page_size;
      $page_no++;
    }
    $user_filter_data['total'] = count($all_user);
    $user_filter_data['users'] = $page_user[$page];
    return $user_filter_data;
  }
  else if ($option == "status"){
    if ($filter == "active"){
      $no_records = db_result(db_query('Select count(*) from users WHERE status = 1 && uid!=0'));
      $user = db_query("SELECT uid FROM users WHERE status = 1 && uid!=0 ORDER BY uid DESC");
      while ($row = db_fetch_object($user)){
        $user_details[] = user_load($row->uid);
      }
    }
    else if ($filter == "blocked"){
      $no_records = db_result(db_query('Select count(*) from users WHERE status = 0 && uid!=0'));
      $user = db_query("SELECT uid FROM users WHERE status = 0 && uid!=0 ORDER BY uid DESC");
      while ($row = db_fetch_object($user)){
        $user_details[] = user_load($row->uid);
      }
    }
  }
  if (empty($user_details)) {
    return services_error("No users available.", 404);
  }
  $page_size = 20; 
  $start_index = 0;
  $page_no = 0;
  $last_index = 20;
  for ($i = 0; $i<count($user_details); $i = $i + $page_size) {
    $page_user[$page_no] = array_slice($user_details,$start_index,$last_index);
    $start_index = $start_index + $page_size;
    $last_index = $last_index + $page_size;
    $page_no++;
  }

  $user_filter_data['total'] = $no_records;
  $user_filter_data['users'] = $page_user[$page];
  return $user_filter_data;
}
