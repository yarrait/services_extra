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
  if(!empty($imagedata['#upload_validators']) && !empty($imagedata['#field_name']) && !empty($imagedata['#type_name']) && !empty($imagedata['imagefield_byte_code'])) {
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
