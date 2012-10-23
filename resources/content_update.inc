<?php

function _content_update_resource_access($op = 'view', $args = array()) {
   switch ($op) {
    case 'create':
     return user_access('administer nodes');
   }
}

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

function _content_update_resource_create($content_data){
  $result =array();$error=array();
  $option=$content_data['option'];
  if(empty($option) || empty($content_data['node']))
   return services_error("option or nid field is requried" ,406,array('form_errors' => "option or nid field is requried."));

  if($option=="publish" || $option=="promoted" || $option=="sticky")
      $values=1;
  if($option=="unpublish" || $option=="unpromoted" || $option=="unsticky")
      $values=0;
  
  $count=count($content_data['node']);
  $count1=0;
  
      foreach($content_data['node'] as $key => $value)
      {
        if(is_numeric($key)) 
         {
            $nid=$value['nid'];
             if (db_result(db_query("SELECT COUNT(*) FROM {node} WHERE  nid = %d", $nid)))
             {
               $total_nid[]=$nid;
   	       if($nid!=null && ($option=="publish" || $option=="unpublish" ))
               { 
   	          $result = db_query("UPDATE node SET status = '%d' WHERE nid = %d", $values, $nid);
   	          $count1++;
   	       }
   	       else if($nid!=null && ($option=="promoted" || $option=="unpromoted" ))
               {
   	          $result = db_query("UPDATE {node} SET promote = '%d' WHERE nid = %d", $values, $nid);
	   	  $count1++;
	       }
               else if($nid!=null && ($option=="sticky" || $option=="unsticky" ))
               { 
	          $result = db_query("UPDATE {node} SET sticky = '%d' WHERE nid = %d", $values, $nid);
		  $count1++;
	       }
               else if($nid!=null && $option=="delete")
               { 
                  $result = db_query("DELETE from node WHERE nid = %d",$nid);
		  $count1++;
               }
              else
              {
                return services_error("There is no option of this type ".$option, 406, null);   
             }
          }
          else 
          {
               $error[]=$nid;
          } 
        }
        else 
        {
          services_error('JSON format is not proper',400);
        }
     }
     if($count1==$count){
        /*$final_string=implode(",", $total_nid);
        $return['nid']=$final_string;
        $return['message']="update has been performed.";*/
       return true;
    }
    if(!empty($error))	
    {
         $final_string=implode(",",$error);
         return services_error("There is no node with id ".$final_string , 406, null);
    }
}
