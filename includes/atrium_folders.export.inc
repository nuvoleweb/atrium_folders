<?php

/**
 * Main export API: create archives for group files to be exported.
 */

/**
* Create a zip file
* @param array
*   Array containing file paths
* @param string
*   Destination directory
* @param boolean
*   Overwrite if existing zip file
* @return boolean
*   Succes or failure
*/
function atrium_folders_export_create_zip($files = array(),$destination = '',$overwrite = true) {
  if(file_exists($destination) && !$overwrite) { return false; }

  $valid_files = array();
  if(is_array($files)) {
    foreach($files as $file) {
      if(file_exists($file)) {
        $valid_files[] = $file;
      }
    }
  }

  if(count($valid_files)) {
    $zip = new ZipArchive();
    if($zip->open($destination,$overwrite ? ZIPARCHIVE::OVERWRITE : ZIPARCHIVE::CREATE) !== true) {
      return false;
    }
    foreach($valid_files as $file) {
      $zip->addFile($file,basename($file));
    }
    $zip->close();

    if (!file_exists($destination)) return false;

    header("Content-type: application/zip");
    header("Content-Disposition: attachment; filename=".basename($destination));
    header("Pragma: no-cache");
    header("Expires: 0");
    readfile("$destination");
    exit;

  }
  else
  {
    return false;
  }
}

/**
* Find which are the children nodes of a book node.
* @param object
*   Book link ($node->book)
* @return array
*   An array of node id
*/
function atrium_folders_export_get_children_nids($book_link) {
  $unflat = book_menu_subtree_data($book_link);
  $nids    = array();
  $iterator = new RecursiveIteratorIterator(new ParentIterator(new RecursiveArrayIterator($unflat)), RecursiveIteratorIterator::SELF_FIRST);
  foreach ($iterator as $key => $value) {
    if (array_key_exists('nid', $value)) {
      $nids[] = $value['nid'];
    }
  }
  return $nids;
}

/**
* Find all downloadable files within nids.
* @param array
*   Array of nid's
* @return array
*   An array of files
*/
function atrium_folders_export_get_deep_downed ($nids) { 
    $file_array = array();
    foreach ($nids as $value) {
        $file_array = array_merge ($file_array, upload_load(node_load($value)));
    }
    return $file_array;
}


/**
* Find all downloadable files within nids.
* @param array
*   Array of nid's
* @return array   
*   An array of files
*/
function atrium_folders_export_retrieve_paths ($file_array) {
  $path_array;
  foreach ($file_array as $value) {
    $path_array[] = $value->filepath;
  }
  return $path_array;
}