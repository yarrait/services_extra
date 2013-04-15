<?php

function _site_configuration_resource_access($op = 'view', $args = array()) {
  switch ($op) {
     case 'create':
       return user_access('administer users'); 
       break;
     case 'view':
       return user_access('administer users'); 
       break;
  }
}

function site_configuration_resources() {
  $resource = array(
    'retrieve' => array(
      'callback' => '_site_configuration_resource_retrieve',
	'args' => array(
          array(
            'name' => 'configuration',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'string',
            'description' => 'The name of configuration of the site configuration to get',
          ),
        ),
        'access callback' => '_site_configuration_resource_access',
        'access arguments' => array('view'),
        'access arguments append' => TRUE,
    ),
    'create' => array(
      'callback' => '_site_configuration_resource_create',
        'args' => array(
          array(
            'name' => 'site_configuration',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The site_configuration data to update',
            'type' => 'array',
          ),
        ),
        'access callback' => '_site_configuration_resource_access',
        'access arguments' => array('create'),
        'access arguments append' => TRUE,
    ),
  );
  return $resource;
}

function _site_configuration_resource_create($site_data){
  $option = $site_data['site_update']['option'];
  $value  = $site_data['site_update']['value'];
 
  switch ($option) {
   
    case "cache_clear":
      drupal_flush_all_caches();
      return TRUE;
      break;
 
    case "clean_url":
      module_load_include('inc', 'system', 'system.admin');

      if ($value == "reset") {
        $form_state['values']['op'] = t('Reset to defaults');
        drupal_execute('system_clean_url_settings', $form_state);
        return TRUE;
        break;
      }
      $clean_url = $value['clean_url'];
      variable_set('clean_url', $clean_url);
      return TRUE;
      break;

    case "site_maintenance":
      module_load_include('inc', 'system', 'system.admin');
      if ($value == "reset") {
        $form_state['values']['op'] = t('Reset to defaults');
        drupal_execute('system_site_maintenance_settings', $form_state);
        return TRUE;
        break;
      }
      variable_set('site_offline', $value['site_offline']); 
      variable_set('site_offline_message', $value['site_offline_message']);
      return TRUE;
      break;
    
    case "site_information":
      module_load_include('inc', 'system', 'system.admin');
      if ($value == "reset") {
        $form_state['values']['op'] = t('Reset to defaults');
        drupal_execute('system_site_information_settings', $form_state);
        return TRUE;
        break;
      }
      $form_state = array();
      $form_state['values']['site_name'] = $value['site_name'];
      $form_state['values']['site_mail'] = $value['site_mail'];
      $form_state['values']['site_slogan'] = $value['site_slogan'];
      $form_state['values']['site_mission'] = $value['site_mission'];
      $form_state['values']['site_footer'] = $value['site_footer'];
      $form_state['values']['anonymous'] = $value['anonymous'];
      $form_state['values']['site_frontpage'] = $value['site_frontpage'];
      $form_state['values']['op'] = t('Save configuration');
      drupal_execute('system_site_information_settings', $form_state);
      if ($errors = form_get_errors()) {
        return  services_error(implode(" ", $errors), 406, array('form_errors' => $errors));
      } 
      return TRUE;
      break;
    
   case "site_performance":

     module_load_include('inc', 'system', 'system.admin');
     if ($value == "reset") {
       $form_state['values']['op'] = t('Reset to defaults');
       drupal_execute('system_performance_settings', $form_state);
       return TRUE;
       break;
     }
     $form_state = array();
     $form_state['values']['cache'] = $value['cache'];
     $form_state['values']['cache_lifetime'] = $value['cache_lifetime'];
     $form_state['values']['page_compression'] = $value['page_compression'];
     $form_state['values']['block_cache'] = $value['block_cache'];
     $form_state['values']['preprocess_css'] = $value['preprocess_css'];
     $form_state['values']['preprocess_js'] = $value['preprocess_js'];
     $form_state['values']['op'] = t('Save configuration');
     drupal_execute('system_performance_settings', $form_state);
     if ($errors = form_get_errors()) {
       return  services_error(implode(" ", $errors), 406, array('form_errors' => $errors));
     } 
     return TRUE;
     break;

    case "file_system":
      module_load_include('inc', 'system', 'system.admin');
      if ($value == "reset") {
       $form_state['values']['op'] = t('Reset to defaults');
       drupal_execute('system_file_system_settings', $form_state);
       return TRUE;
       break;
      }
      $form_state = array();
      $form_state['values']['file_directory_path'] = $value['file_directory_path'];
      $form_state['values']['file_directory_temp'] = $value['file_directory_temp'];
      $form_state['values']['file_downloads'] = $value['file_downloads'];
      $form_state['values']['op'] = t('Save configuration');
      drupal_execute('system_file_system_settings', $form_state);
      if ($errors = form_get_errors()) {
        return  services_error(implode(" ", $errors), 406, array('form_errors' => $errors));
      } 
      return TRUE;
      break;
      
    case "database_logging":
      $dblog_row_limit = $value['db_log_limit'];
      variable_set('dblog_row_limit', $dblog_row_limit);     
      return TRUE;
      break;

    case "date_time":
      module_load_include('inc', 'system', 'system.admin');
      if ($value == "reset") {
       $form_state['values']['op'] = t('Reset to defaults');
       drupal_execute('system_date_time_settings', $form_state);
       return TRUE;
       break;
      }
      $form_state = array();
      $form_state['values']['date_default_timezone'] = $value['date_default_timezone'];
      $form_state['values']['configurable_timezones'] = $value['configurable_timezones'];
      $form_state['values']['date_first_day'] = $value['week_first_day'];

      if ($value['date_format_short_custom']) {
        $custom_format  = $value['date_format_short_custom'];
      }
      else if ($value['date_format_medium_custom']) {
        $custom_format = $value['date_format_medium_custom'];
      }
      else {
        $custom_format = $value['date_format_long_custom'];
      }
      if ($value['date_format_short_custom'] || $value['date_format_medium_custom'] || $value['date_format_long_custom']) {
        $format = array();
        $format['format'] = $custom_format;
        $format['type'] = 'custom';
        $format['locked'] = 0;
        $format['is_new'] = 1;
        //date_format_save($format);
      }  
      if ($value['date_format_short_custom']) {
        variable_set('date_format_short_custom', $value['date_format_short_custom']);
        variable_set('date_format_short', $value['date_format_short_custom']);
      }
      else {
        variable_set('date_format_short', $value['date_format_short']);
      }
      if ($value['date_format_medium_custom']) {
        variable_set('date_format_medium_custom', $value['date_format_medium_custom']);
        variable_set('date_format_medium', $value['date_format_medium_custom']);
      }
      else {
        variable_set('date_format_medium', $value['date_format_medium']);
      }
      if ($value['date_format_long_custom']) {
        variable_set('date_format_long_custom', $value['date_format_long_custom']);
        variable_set('date_format_long', $value['date_format_long_custom']); 
      }
      else {
        variable_set('date_format_long', $value['date_format_long']);
      }
      $form_state['values']['op'] = t('Save configuration');
      drupal_execute('system_date_time_settings', $form_state);
      if ($errors = form_get_errors()) {
        return  services_error(implode(" ", $errors), 406, array('form_errors' => $errors));
      } 
      return TRUE;
      break;

    case "error_reporting":
      module_load_include('inc', 'system', 'system.admin');
     
      if ($value == "reset") {
        $form_state['values']['op'] = t('Reset to defaults');
        drupal_execute('system_error_reporting_settings', $form_state);
        return TRUE;
        break;
     }
     $form_state = array();
     $form_state['values']['site_403'] = $value['site_403'];
     $form_state['values']['site_404'] = $value['site_404'];
     $form_state['values']['error_level'] = $value['error_level'];
     $form_state['values']['op'] = t('Save configuration');
     drupal_execute('system_error_reporting_settings', $form_state);
     if ($errors = form_get_errors()) {
       return  services_error(implode(" ", $errors), 406, array('form_errors' => $errors));
     } 
     return TRUE;
     break;
     
  }

}

function _site_configuration_resource_retrieve($configuration) {
//1 enabled 0 disabled  
  switch ($configuration) {
    case "clean_url":
      $result['clean_url'] = variable_get('clean_url', 0);
      break;

    case "site_maintenance":
      $value =  variable_get('site_offline', 0); 
      $message = variable_get('site_offline_message', t('@site is currently under maintenance. We should be back shortly. Thank you for your patience.', array('@site' => variable_get('site_name', 'Drupal'))));
      $result['site_offline'] = $value;
      $result['message'] = $message;
      break;    

    case "site_information":
      $result['site_name'] = variable_get('site_name', 'Drupal');
      $result['site_mail'] = variable_get('site_mail', '');
      $result['site_slogan'] = variable_get('site_slogan', '');
      $result['site_mission'] = variable_get('site_mission', '');
      $result['site_footer'] =  variable_get('site_footer', '');
      $result['anonymous'] =  variable_get('anonymous', t('Anonymous'));
      $result['site_frontpage'] =  variable_get('site_frontpage', 'node');
      break;

    case "site_performance":
      $result['cache'] = variable_get('cache', CACHE_DISABLED);
      $result['cache_lifetime'] = variable_get('cache_lifetime', 0);
      $result['page_compression'] = variable_get('page_compression', 1);
      $result['block_cache'] = variable_get('block_cache', CACHE_DISABLED);
      $result['preprocess_css'] = variable_get('preprocess_css', 0);
      $result['preprocess_js'] = variable_get('preprocess_js', 0);
      break;
   
    case "file_system":
      $result['file_directory_path'] = file_directory_path();
      $result['file_directory_temp'] = file_directory_temp();
      $result['file_downloads'] = variable_get('file_downloads', FILE_DOWNLOADS_PUBLIC);
      break;

    case "database_logging":
      $result['dblog_row_limit'] = variable_get('dblog_row_limit', 1000);     
      break;

    case "date_time":
 
      $zone_list = _system_zonelist();
      $i = 0;
      foreach($zone_list as $key => $value) {
         $default_zone[$i]['value'] = $key;
         $default_zone[$i]['format'] = $value;   
         $i++;
      }
      $default_time_zone['selected'] =  variable_get('date_default_timezone', 0);
      $default_time_zone['time_zone'] = $default_zone;
      $result['default_time_zone'] = $default_time_zone;
    
      $result['configurable_timezones'] = variable_get('configurable_timezones', 1);
 
      $result['week_first_day'] = variable_get('date_first_day', 0);

       // Date settings: possible date formats
      $date_short = array('Y-m-d H:i', 'm/d/Y - H:i', 'd/m/Y - H:i', 'Y/m/d - H:i',
           'd.m.Y - H:i', 'm/d/Y - g:ia', 'd/m/Y - g:ia', 'Y/m/d - g:ia',
           'M j Y - H:i', 'j M Y - H:i', 'Y M j - H:i',
           'M j Y - g:ia', 'j M Y - g:ia', 'Y M j - g:ia');
     
      $date_medium = array('D, Y-m-d H:i', 'D, m/d/Y - H:i', 'D, d/m/Y - H:i',
          'D, Y/m/d - H:i', 'F j, Y - H:i', 'j F, Y - H:i', 'Y, F j - H:i',
          'D, m/d/Y - g:ia', 'D, d/m/Y - g:ia', 'D, Y/m/d - g:ia',
          'F j, Y - g:ia', 'j F Y - g:ia', 'Y, F j - g:ia', 'j. F Y - G:i');
  
      $date_long = array('l, F j, Y - H:i', 'l, j F, Y - H:i', 'l, Y, F j - H:i',
        'l, F j, Y - g:ia', 'l, j F Y - g:ia', 'l, Y, F j - g:ia', 'l, j. F Y - G:i');


      $short_date_format['selected'] = variable_get('date_format_short', $date_short[1]);

       $i = 0;
      foreach ($date_short as $format) {
        $short_format_list[$i]['value'] = $format;
        $short_format_list[$i]['format'] = format_date(time(), 'custom', $format);
        $i++;
      }
      $short_date_format['custom_short_format'] = variable_get('date_format_short_custom', 'd/m - H:i');
      $short_date_format['short_format'] = $short_format_list;
      $result['short_date_format'] = $short_date_format;
 
      $medium_date_format['selected'] =  variable_get('date_format_medium', $date_medium[1]);

      $i = 0;
      foreach ($date_medium as $format) {
        $medium_format_list[$i]['value'] = $format;
        $medium_format_list[$i]['format'] = format_date(time(), 'custom', $format);
         $i++;
      }
      $medium_date_format['custom_medium_format'] = variable_get('date_format_medium_custom', 'D, m/d/Y - H:i');      
      $medium_date_format['medium_format'] = $medium_format_list;
      $result['medium_date_format'] = $medium_date_format;
         
      $long_date_format['selected'] =  variable_get('date_format_long', $date_long[0]);
     
      $i = 0;
      foreach ($date_long as $format) {
         $long_format_list[$i]['value'] = $format;
         $long_format_list[$i]['format'] = format_date(time(), 'custom', $format);
         $i++;
      }
      $long_date_format['custom_long_format'] = variable_get('date_format_long_custom', 'l, F j, Y - H:i');
      $long_date_format['long_format'] = $long_format_list;
      $result['long_date_format'] = $long_date_format;
      return $result;
      break;
 
    case "error_reporting":
      $result['site_403'] = variable_get('site_403', '');
      $result['site_404'] = variable_get('site_404', '');
      $result['error_level'] = variable_get('error_level', 1); 
      break;
  }
   return $result;
}
