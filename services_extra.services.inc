<?php

/**
 * Implementation of hook_default_services_endpoint().
 */
function services_extra_default_services_endpoint() {
  $export = array();

  $endpoint = new stdClass;
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'druproid';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'druproid';
  $endpoint->authentication = array(
    'services' => 'services',
  );
  $endpoint->server_settings = array(
    'rest_server' => array(
      'formatters' => array(
        'json' => TRUE,
        'bencode' => FALSE,
        'ical' => FALSE,
        'jsonp' => FALSE,
        'php' => FALSE,
        'rss' => FALSE,
        'xcal' => FALSE,
        'xml' => FALSE,
        'yaml' => FALSE,
      ),
      'parsers' => array(
        'application/json' => TRUE,
        'application/vnd.php.serialized' => TRUE,
        'application/x-www-form-urlencoded' => TRUE,
        'application/x-yaml' => TRUE,
        'multipart/form-data' => TRUE,
      ),
    ),
  );
  $endpoint->resources = array(
    'comment' => array(
      'operations' => array(
        'create' => array(
          'enabled' => 1,
        ),
        'retrieve' => array(
          'enabled' => 1,
        ),
        'update' => array(
          'enabled' => 1,
        ),
        'delete' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
      'actions' => array(
        'countAll' => array(
          'enabled' => 1,
        ),
        'countNew' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'file' => array(
      'operations' => array(
        'create' => array(
          'enabled' => 1,
        ),
        'retrieve' => array(
          'enabled' => 1,
        ),
        'delete' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'node' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'create' => array(
          'enabled' => 1,
        ),
        'update' => array(
          'enabled' => 1,
        ),
        'delete' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
      'relationships' => array(
        'files' => array(
          'enabled' => 1,
        ),
        'comments' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'system' => array(
      'actions' => array(
        'connect' => array(
          'enabled' => 1,
        ),
        'get_variable' => array(
          'enabled' => 1,
        ),
        'set_variable' => array(
          'enabled' => 1,
        ),
        'del_variable' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'taxonomy_term' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'create' => array(
          'enabled' => 1,
        ),
        'update' => array(
          'enabled' => 1,
        ),
        'delete' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
      'actions' => array(
        'selectNodes' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'taxonomy_vocabulary' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'create' => array(
          'enabled' => 1,
        ),
        'update' => array(
          'enabled' => 1,
        ),
        'delete' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
      'actions' => array(
        'getTree' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'user' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'create' => array(
          'enabled' => 1,
        ),
        'update' => array(
          'enabled' => 1,
        ),
        'delete' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
      'actions' => array(
        'login' => array(
          'enabled' => 1,
        ),
        'logout' => array(
          'enabled' => 1,
        ),
        'register' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'watchdog' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'system_info' => array(
      'operations' => array(
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'user_permission' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'update' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'vocabulary_term' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'accessrule' => array(
      'operations' => array(
        'create' => array(
          'enabled' => 1,
        ),
        'retrieve' => array(
          'enabled' => 1,
        ),
        'update' => array(
          'enabled' => 1,
        ),
        'delete' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'content_type' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'content' => array(
      'operations' => array(
        'create' => array(
          'enabled' => 1,
        ),
        'retrieve' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'role' => array(
      'operations' => array(
        'create' => array(
          'enabled' => 1,
        ),
        'retrieve' => array(
          'enabled' => 1,
        ),
        'update' => array(
          'enabled' => 1,
        ),
        'delete' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'user_update' => array(
      'operations' => array(
        'create' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'content_update' => array(
      'operations' => array(
        'create' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'vocabulary' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'comments' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'user_filter' => array(
      'operations' => array(
        'create' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'form_content' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'backup_migrate' => array(
      'operations' => array(
        'create' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'backup_migrate_destination' => array(
      'operations' => array(
        'create' => array(
          'enabled' => 1,
        ),
        'retrieve' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'is_module_installed' => array(
      'operations' => array(
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'image_upload' => array(
      'operations' => array(
        'create' => array(
          'enabled' => 1,
        ),
        'delete' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'site_configuration' => array(
      'operations' => array(
        'create' => array(
          'enabled' => 1,
        ),
        'retrieve' => array(
          'enabled' => 1,
        ),
      ),
    ),
  );
  $endpoint->debug = 1;
  $export['druproid'] = $endpoint;

  return $export;
}
