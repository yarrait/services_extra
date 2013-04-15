<?php

/**
 * Gets access rules.
 *
 * @param string $op
 *  $op - Permission requested.
 * @return Boolean
 *  Boolean TRUE if the current user has the requested permission.
 */
function _backup_migrate_resource_access($op = 'view', $args = array()) {
  switch ($op) {
    case 'view':
      return user_access('administer users');
      break;
    case 'create':
      return user_access('administer users');
      break;
  }
}

/**
 * Gets all resource definitions.
 *
 * @return array
 *  An array containing all resources.
 */
function backup_migrate_resources() { 
  $resource = array(
    'index' => array(
        'callback' => '_backup_migrate_index',
        'access arguments' => array('administer nodes'),
        'access arguments append' => FALSE,
    ),
    'create' => array(
      'callback' => '_backup_migrate_quickbackup_create',
        'args' => array(
          array(
            'name' => 'backup_migrate',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'Quick Backup',
            'type' => 'array',
          ),
        ),
        'access callback' => '_backup_migrate_resource_access',
        'access arguments' => array('create'),
        'access arguments append' => TRUE,
    ),
  );
 
  return $resource;
}

/**
 * Returns list of available Sources, Destinations and Profiles . 
 *
 * @return
 *   list of available Sources, Destinations and Profiles.
 */
function _backup_migrate_index() { 
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


/**
 * Returns success message on creation of backup else service error. 
 * @param $backup_data
 *  $backup_data - Required data to create backup (Like, source_id, destination_id, profile_id).
 * @return
 *   success message on creation of backup else service error.
 */
function _backup_migrate_quickbackup_create($backup_data) { 
  include_once(DRUPAL_ROOT . '/' . drupal_get_path('module', 'backup_migrate') . '/backup_migrate.module');
  module_load_include('inc', 'backup_migrate', 'includes/destinations');
  module_load_include('inc', 'backup_migrate', 'includes/profiles');

  if (user_access('perform backup')) {

    // For a quick backup use the default settings.
    $settings = _backup_migrate_profile_saved_default_profile($backup_data['profile_list']);

    // Set the destination to the one chosen in the pulldown.
    $settings->destination_id = $backup_data['destination_list'];
    $settings->source_id = $backup_data['source_list'];

    // Save the settings for next time.
    variable_set("backup_migrate_source_id", $backup_data['source_list']);
    variable_set("backup_migrate_destination_id", $backup_data['destination_list']);
    variable_set("backup_migrate_profile_id", $backup_data['profile_list']);

    // Do the backup.
    $backup_info = backup_migrate_perform_backup($settings);
    $file = $backup_info->file_info['filename'];
    $dest = $settings->get_destination_name();
    $source = $settings->get_source_name();
    $message['backup_migrate'] = array(
      'message' => "$source". ' backed up successfully to '. "$file" .' in destination ' . "$dest",
    );
    return $message;
  }
  else {
    return services_error("Please try again..",406,null);
  }
}
