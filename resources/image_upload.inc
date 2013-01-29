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
function _image_upload_resources_create($imagedata) { 
  $validators = array(
  /**
   * Defaults already allow png, jpg etc. If more needed to be supported,
   * edit here.
   */
  'file_validate_extensions' => array(),
  'file_validate_size' => array(),
  );

  $image_str = 'abc'; //$imagedata['img_string'];

  $image = base64_decode($image_str); 
  //  $saved_img = file_put_contents('/var/image.png', $image);

  $field = content_fields('field_product_image', 'product'); //content_fields($element['#field_name'], $element['#type_name']);
  $dest = filefield_widget_file_path($field); 

  $filepath = file_save_data($image, variable_get('file_directory_temp'));
  $file = field_file_save_file($filepath, $validators, $dest);

  /* following is the content what $file will have
Array
(
    [uid] => 1
    [filename] => fileMysu0k
    [filepath] => sites/default/files/fileMysu0k
    [filemime] => application/octet-stream
    [source] => field_file_save_file
    [destination] => sites/default/files/fileMysu0k
    [filesize] => 19281
    [status] => 0
    [timestamp] => 1359461443
    [fid] => 20
)
  */
//  print_r($file); exit;

}
