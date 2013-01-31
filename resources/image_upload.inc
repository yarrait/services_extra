<?php
/**
 * Gets access rules.
 *
 * @param string $op
 *  $op - Permission requested.
 * @return Boolean
 *  Boolean TRUE if the current user has the requested permission.
 */
function _image_upload_resource_access($op = 'view', $args = array()) {
  switch ($op) {
    case 'view':
      return user_access('administer users');
      break;
    case 'delete':
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
function image_upload_resources() {
  $resource = array(
    'create' => array(
      'callback' => '_image_upload_resources_create',
        'args' => array(
          array(
            'name' => 'image upload',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'Image Upload',
            'type' => 'array',
          ),
        ),
        'access callback' => '_image_upload_resource_access',
        'access arguments' => array('create'),
        'access arguments append' => TRUE,
    ),
    'delete' => array(
      'callback' => '_image_upload_resources_delete',
        'args' => array(
          array(
            'name' => 'fid',
            'optional' => FALSE,
            'type' => 'int',
            'source' => array('path' => 0),
          ),
        ),
        'access callback' => '_image_upload_resource_access',
        'access arguments' => array('delete'),
        'access arguments append' => TRUE,
    ),

  );
 
  return $resource;
}

/**
 * Returns image information on success otherwise service error message 
 * @param $imagedata
 *  $imagedata - Required data to upload image (Like, imagename, image type).
 * @return
 *   image information on success otherwise service error message
 */
function _image_upload_resources_create($imagedata) { //print_r($imagedata['#upload_validators']); exit;
  if(!empty($imagedata)) {
    $validators_decoded = (array) json_decode($imagedata['#upload_validators']); //print_r($validators_decoded); exit;
    $validators = array(
      'file_validate_extensions' => $validators_decoded['file_validate_extensions'],
      'file_validate_size' => $validators_decoded['filefield_validate_size'],
    );
    $image_str = $imagedata['imagefield_byte_code'];
    $image = base64_decode($image_str); 

    $field = content_fields($imagedata['#field_name'], $imagedata['#type_name']);
    $dest = filefield_widget_file_path($field); 

    $dest_path = variable_get('file_directory_temp')."/"."{$imagedata['file_name']}"; //print_r($dest_path); exit;
    $filepath = file_save_data($image, $dest_path);
    $file = field_file_save_file($filepath, $validators, $dest);
    if(!empty($file)) {
      return $file;
    }
  }
  return FALSE;
}

/**
 * Returns Boolean value
 *
 * @param $fid
 *   $fid - file id
 * @return
 *  Boolean TRUE if the file is deleted else FALSE
 */
function _image_upload_resources_delete($fid) {
  if(isset($fid)) {
    $removed_file = field_file_load($fid);
    if ($removed_file['status'] == 0) {
      if(field_file_delete($removed_file)) {
        return TRUE;
      } else return FALSE;
    }
  }
  return FALSE;
}
