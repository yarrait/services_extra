<?php

function _user_update_resource_access($op = 'view', $args = array()) {
  switch ($op) {
     case 'create':
      return user_access('administer users');
   }
}

function user_update_resources() {
  $resource = array(
      'create' => array(
        'callback' => '_user_update_resource_create',
        'args' => array(
         array(
            'name' => 'user_data',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The user data to update',
            'type' => 'array',
          ),
        ),
        'access callback' => '_user_update_resource_access',
        'access arguments' => array('create'),
        'access arguments append' => TRUE,
      ),
  );
  return $resource;
}

function _user_update_resource_create($user_data){
  $return =array();
  $count=count($user_data['user']);
  $count1=0;
  $option=$user_data['option'];
  if(empty($option) || empty($user_data['user']))
    return services_error("option or uid field is requried" ,406,array('form_errors' => "option or uid field is requried."));

  if($option=="block")
      $values=0;
  if($option=="active")
      $values=1;  

  foreach($user_data['user'] as $key => $value)
  {
    if(is_numeric($key)) 
    {
      $uid=$value['uid'];
      if(db_result(db_query("SELECT COUNT(*) FROM {users} WHERE  uid = %d", $uid))) 
      {   
          $total_uid[]=$uid;
         if($uid!=null && ($option=="block" || $option=="active" ))
         {
            $result = db_query("UPDATE users SET status = '%d' WHERE uid = %d", $values, $uid);
            $count1++;
         }
         else if($uid!=null && $option="delete")
         {
            $result = db_query("DELETE from users WHERE uid = %d",$uid);
            $count1++;
         }
      }
      else
      {
          $error[]= $uid;
      }
    }
    else 
    {
       services_error('JSON format is not proper', 400);
    }  
  }
  if($count1==$count){
     /* $final_string=implode(",",$total_uid);
      $return['uid']=$final_string;
      $return['message']="update has been performed.";*/
     return true;
  }
  if(!empty($error))	
  {
      $final_string=implode(",",$error);
      return services_error("There is no user with id ".$final_string ,406,null);
  }
}  
