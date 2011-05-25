<?php

/**
 * Implementation of hook_views_handlers().
 */
function atrium_folders_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'atrium_folders') .'/includes',
    ),
    'handlers' => array(
      'atrium_folders_handler_field_folder_toolbox' => array(
         'parent' => 'views_handler_field',
      ),
      'atrium_folders_handler_field_folder_icon' => array(
         'parent' => 'views_handler_field',
      ),
      'atrium_folders_handler_argument_menu_trail' => array(
         'parent' => 'views_handler_argument',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data().
 */
function atrium_folders_views_data() {
  $data = array();
  $data['atrium_folders']['table']['group'] = t('Folders');
  $data['atrium_folders']['table']['join'] = array('#global' => array());
  $data['atrium_folders']['toolbox'] = array(
    'title' => t('Toolbox'),
    'help' => t('Folder management tools.'),
    'field' => array(
      'handler' => 'atrium_folders_handler_field_folder_toolbox',
    ),
  );
  $data['atrium_folders']['icon'] = array(
    'title' => t('Icon'),
    'help' => t('Icon for files and folders.'),
    'field' => array(
      'handler' => 'atrium_folders_handler_field_folder_icon',
    ),
  );
  $data['node']['menu_trail'] = array(
    'title' => t('Nodes in Menu Trail'),
    'help' => t('Select nodes in the current node menu trail.'),
    'argument' => array(
      'handler' => 'atrium_folders_handler_argument_menu_trail',
    ),
  );

  return $data;
}

