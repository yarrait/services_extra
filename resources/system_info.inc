<?php

function _system_info_resource_access($op = 'view', $args = array()) {
  return TRUE;
}

function system_info_resources() {
  $resource = array(
    'index' => array(
      'callback' => '_system_info_resource_index',
       'access callback' => '_system_info_resource_access',
        'access arguments' => array('view'),
        'access arguments append' => TRUE,
      ),
  );
  return $resource;
}

function _system_info_resource_index() { 
  $var['version'] = VERSION;
  
  //getting version of service extra module
  $services_extra_path = drupal_get_path('module', 'services_extra') . '/services_extra' . '.info';
  $services_extra = drupal_parse_info_file($services_extra_path);
  $var['services_extra_version'] = $services_extra['version'];

  return $var;
}
