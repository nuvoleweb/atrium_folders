<?php

function atrium_folder_import_exclude() {
  return array('.', '..', 'CVS', '.git', '.svn', '__MACOSX', 'Thumbs.db', '.DS_Store', '.db');
}

/**
 * Main import API: specify source directory and group 
 * to which the repository will belong to.
 *
 * @param $dir
 *    Full path on your local filesystem
 * @param $groups
 *    Array of group NIDs
 * @param $parent
 *    Used in recursion; leave empty.
 */
function atrium_folders_import_create_book($dir, $groups = array(), $parent = NULL, $name = NULL) {
  static $data = array(); 
  static $first = TRUE;

  if (!$data) {
    $data['folders'] = 0;
    $data['files'] = 0;
    $data['parent'] = $parent;
  }

  if (substr($dir, -1) == DIRECTORY_SEPARATOR) {
    $dir = substr($dir, 0, -1);
  }
  
  if (is_dir($dir) && $handle = opendir($dir)) {

    // The current dir name
    $files = atrium_folders_import_get_files($dir);
    if ($parent && $first) {
      atrium_folders_import_attach_files($parent, $files);
      node_save($parent);
    }
    else {
      $title = $name ? $name : array_pop(explode(DIRECTORY_SEPARATOR, $dir));    
      $parent = atrium_folders_import_create_book_page($title, $files, $groups, $parent);
      $data['folders'] += 1;
    }

    $data['files'] += count($files);
    $first = FALSE;

    if (!$data['parent']) {
      $data['parent'] = $parent;
    }

    while (FALSE !== ($file = readdir($handle))) {
      // Skip unwanted files
      if (!in_array($file, atrium_folder_import_exclude()) && $file[0] != '.') { 
        if (is_dir($dir . DIRECTORY_SEPARATOR . $file)) {
          atrium_folders_import_create_book($dir . DIRECTORY_SEPARATOR . $file, $groups, $parent);
        }
      }
    }
    closedir($handle);
  }
  return $data;
}

function atrium_folders_import_get_files($dir) {
  $return = array();
  $files = file_scan_directory($dir, '.*', atrium_folder_import_exclude(), 0, FALSE);
  foreach ($files as $key => $value) {
    if ($value->name) {
      $return[] = $value->filename;
    }
  }
  return $return;
}

function atrium_folders_import_save_file($source) {
  global $user;

  $extensions = '';
  foreach ($user->roles as $rid => $name) {
    $extensions .= ' '. variable_get("upload_extensions_$rid",
    variable_get('upload_extensions_default', 'jpg jpeg gif png txt html doc xls pdf ppt pps odt ods odp'));
  }
  $filename = file_munge_filename(trim(basename($source), '.'), $extensions);

  $file->uid = $user->uid;
  $file->filemime = file_get_mimetype($source);
  $file->status = 1;
  $file->filesize = filesize($source);
  $file->filepath = $source;
  $file->new = TRUE;

  if (file_copy($file, $filename)) {
    $file->timestamp = time();
    $file->description = $file->origname = $file->filename = basename($file->filepath);
    drupal_write_record('files', $file);
    $file->fid = db_last_insert_id('files', 'fid');
    return $file;
  }
  return FALSE;
}


function atrium_folders_import_create_book_page($title, $files = array(), $groups = array(), $parent = NULL) {
    $node = new stdClass();
    $node->type = 'folder';
    $node->title = $title;
    module_load_include('inc', 'node', 'node.pages');
    // Assign groups.
    $node->og_groups = is_array($groups) ? $groups : array($groups);
    
    // Skip book module prepare due to conflict with spaces_menu_access()
    // in _menu_link_translate()
    $node->book['skip'] = TRUE;
    node_object_prepare($node);
    unset($node->book['skip']);
    if ($parent) {
      $_GET['parent'] = $parent->book['mlid'];
    }
    else {
      $node->book['bid'] = 'new';
    }
    atrium_folder_import_book_prepare($node);
    
    unset($node->menu);
    $node->book['module'] = 'book';
    // Assign files.
    atrium_folders_import_attach_files($node, $files);
    node_save($node);
    return $node;
}

function atrium_folders_import_attach_files(&$node, &$files) {
  foreach ($files as $filepath) {
    $file = atrium_folders_import_save_file($filepath);
    $node->files[$file->fid] = $file;
  }
}


function atrium_folder_import_book_prepare(&$node) {
  // Prepare defaults for the add/edit form.
  if (empty($node->book) && (user_access('add content to books') || user_access('administer book outlines'))) {
    $node->book = array();
    if (empty($node->nid) && isset($_GET['parent']) && is_numeric($_GET['parent'])) {
      // Handle "Add child page" links:
      $parent = db_fetch_array(db_query("SELECT * FROM {menu_links} ml INNER JOIN {book} b ON b.mlid = ml.mlid LEFT JOIN {menu_router} m ON m.path = ml.router_path WHERE ml.mlid = %d", $_GET['parent']));          
      if ($parent) {
        $node->book['bid'] = $parent['bid'];
        $node->book['plid'] = $parent['mlid'];
        $node->book['menu_name'] = $parent['menu_name'];
      }
    }
    // Set defaults.
    $node->book += _book_link_defaults(!empty($node->nid) ? $node->nid : 'new');
  }
  else {
    if (isset($node->book['bid']) && !isset($node->book['original_bid'])) {
      $node->book['original_bid'] = $node->book['bid'];
    }
  }
  // Find the depth limit for the parent select.
  if (isset($node->book['bid']) && !isset($node->book['parent_depth_limit'])) {
    $node->book['parent_depth_limit'] = _book_parent_depth_limit($node->book);
  }      
}