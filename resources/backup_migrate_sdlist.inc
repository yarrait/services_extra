<?php

function _backup_migrate_sdlist_resource_access($op = 'view', $args = array()) {
  switch ($op) {
    case 'view':
      return user_access('administer users');
      break;
  }
}


function backup_migrate_sdlist_resources() { 
  $resource = array(
    'index' => array(
      'callback' => '_backup_migrate_sdlist_index',
        ),
        'access arguments' => array('administer nodes'),
        'access arguments append' => FALSE,
      );
 
  return $resource;
}


function _backup_migrate_sdlist_index() { 
  module_load_include('inc', 'backup_migrate', 'includes/destinations');
  module_load_include('inc', 'backup_migrate', 'includes/profiles');
  $list = array();
  $list['source_list'] = _backup_migrate_get_destination_form_item_options('source');
  $destinations = _backup_migrate_get_destination_form_item_options('manual backup');
  if($destinations) {
     foreach($destinations as $key => $val) {
         if($key != 'download') {
           $opt[$key] = $val;
         }
     }
     $list['destination_list'] = $opt;
  }
  $list['profile_list'] = _backup_migrate_get_profile_form_item_options();
  return $list;
}


