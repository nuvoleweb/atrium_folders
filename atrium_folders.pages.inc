<?php

/**
 * Implementation of #validate callback.
 */
function atrium_folders_import_validate($form, &$form_state) {
  $form_state['zip'] = file_save_upload('zip');
  if ($form_state['zip'] && $form_state['zip']->filemime != 'application/zip') {
    drupal_set_message(t('The provided file is not a valid ZIP archive'), 'error');
  }
}

/**
 * Implementation of #submit callback.
 */
function atrium_folders_import_submit(&$form, $form_state) {
  if ($form_state['zip']) {
    $group = spaces_get_space();
    $parent = node_load($form_state['values']['nid']);    
    $temp = dirname($form_state['zip']->filepath) . DIRECTORY_SEPARATOR . 'atrium_folders_' . time();
    $zip = new ZipArchive();
    if ($zip->open($form_state['zip']->filepath)) {
      $zip->extractTo($temp);
      module_load_include('inc', 'atrium_folders', 'includes/atrium_folders.import');
      $data = atrium_folders_import_create_book($temp, $group->group->nid, $parent);
      drupal_set_message(t('Created !folders folders and !files files.', array('!folders' => $data['folders'], '!files' => $data['files'])));
      $form['#redirect'] ='node/'. $data['parent']->nid;
    }
    else {
      drupal_set_message(t('Error opening the ZIP archive'), 'error');
    }
  }  
}

/**
 * Page callback for folders feature.
 */
function atrium_folders_overview() {

  $result = views_get_view_result('folders_folders', 'page_1');
  if (count($result) == 1) {
    drupal_goto('node/'. $result[0]->nid);
  }
  return views_embed_view('folders_folders', 'page_1');
}

/**
 * Page callback: toolbox controller.
 */
function atrium_folders_toolbox($op, $type, $id) {
  ctools_include('ajax');
  ctools_include('modal');

  $toolbox = _atrium_folders_get_toolbox_info($op, $type);
  $node = _atrium_folders_get_node($op, $type, $id);
  $form_state = array('ajax' => TRUE, 'title' => 'Title', 'node' => $node, 'folder' => array('type' => $type, 'id' => $id));
  $commands = ctools_modal_form_wrapper($toolbox['form callback'], $form_state);
  if (!$commands && function_exists($toolbox['ajax callback'])) {
    $commands = array();
    $callback = $toolbox['ajax callback'];
    $commands = $callback($form_state);
  }
  elseif ($commands) {
    $show_modal[] = atrium_folders_command_show_modal_dialog($op, $type, $id);
    $commands = array_merge($show_modal, $commands);
  }
  ctools_ajax_render($commands);
}

/**
 * Show modal dialog on ajax call success.
 */
function atrium_folders_command_show_modal_dialog($op, $type, $id) {
  return array(
    'command' => 'show_modal_dialog',
    'id' => _atrium_folders_dom_id($op, $type, $id),
  );
}

/**
 * Dismiss modal dialog.
 */
function atrium_folders_command_dismiss_modal_toolbox() {
  return array(
    'command' => 'dismiss_modal_toolbox',
  );
}
