<?php

/**
 * Implementation of hook_drush_command().
 */
function atrium_folders_drush_command() {
  $items = array();

  $items['folder-import'] = array(
    'description'   => "Import a folder into Open Atrium group.",
    'arguments' => array(
      'path'        => 'Full path to the folder to be imported',
      'group'       => 'Path of the group to import the folder into',
    ),
    'options' => array(
      '--parent'    => 'Node ID of the parent folder',
      '--uid'       => 'User ID for authoring the newly created folders, Administrator if empty'
    ),
    'examples' => array(
      'drush folder-import /tmp/apples fruits' => 'Import content from /tmp/apples into "fruits" group (es. http://localhost/atrium/fruits).',
      'drush folder-import /tmp/apples fruits --parent=235 --uid=5' => 'Import content from /tmp/apples into "fruits" group (es. http://localhost/atrium/fruits). Parent folder will be node 235 and author user 5.',
    ),
    'aliases'       => array('fi'),
    'bootstrap'     => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );
  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function atrium_folders_drush_help($section) {
  switch ($section) {
    case 'drush:folder-import':
      return dt("Import a folder into an Open Atrium group.");
  }
}

/**
 * Implementation of drush_hook_COMMAND_validate().
 */
function drush_atrium_folders_folder_import_validate($path = NULL, $group = NULL) {
  
  $gid = _drush_atrium_folders_get_group($group);
  if (!$path || !$group) {
    drush_set_error('DRUSH_INPUT_ERROR', dt('Both filesystem path and group path are required. Run "drush help folder-import" for help.'));
  }
  elseif (!$gid) {
    drush_set_error('DRUSH_INPUT_ERROR', dt('Invalid group path'));
  }
  elseif (!is_dir($path)) {
    drush_set_error('DRUSH_INPUT_ERROR', dt('Invalid filesystem path'));
  }
  else {
    if ($parent = drush_get_option('parent')) {
      $node = node_load((int) $parent);
      if (!$node) {
        drush_set_error('DRUSH_INPUT_ERROR', dt('Parent node does not exist'));
      }
      elseif ($node->type != 'folder') {
        drush_set_error('DRUSH_INPUT_ERROR', dt('Parent node is not a folder'));
      }
      elseif (!isset($node->og_groups[$gid])) {
        drush_set_error('DRUSH_INPUT_ERROR', dt('Parent node does not belong to the specified group'));
      }
    }
    if ($uid = drush_get_option('uid')) {
      $user = user_load((int) $uid);
      if (!$user) {
        drush_set_error('DRUSH_INPUT_ERROR', dt('User does not exist'));
      }
    }
  }
}

/**
 * Drush command callback: case-list
 */
function drush_atrium_folders_folder_import($path = NULL, $group = NULL) {
  module_load_include('inc', 'atrium_folders', 'includes/atrium_folders.import');

  $uid = drush_get_option('uid') ? drush_get_option('uid') : 1;
  drush_drupal_login($uid);
  $gid = _drush_atrium_folders_get_group($group);
  $parent = drush_get_option('parent') ? node_load(drush_get_option('parent')) : NULL;
  $data = atrium_folders_import_create_book($path, $gid, $parent);
  drush_log(dt('Created !folders folders and !files files.', array('!folders' => $data['folders'], '!files' => $data['files'])), 'ok');
}

function _drush_atrium_folders_get_group($group) {
  return db_result(db_query("SELECT id FROM purl WHERE provider = 'spaces_og' AND value = '%s'", $group));
}
