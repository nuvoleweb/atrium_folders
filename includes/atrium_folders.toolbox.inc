<?php

/**
 * Move file toolbox ==================================================
 */

/**
 * Form callback.
 */
function atrium_folders_move_file(&$form_state) {
  $node = $form_state['node'];
  $fid = $form_state['folder']['id'];
  
  $form = array();
  $form['move'] = array(
    '#type' => 'fieldset',
    '#title' => t('Move'),
    '#collapsable' => FALSE,
    '#tree' => FALSE,
  );
  $form['move']['folder'] = array(
    '#type' => 'select',
    '#title' => t('Move to folder'),
    '#default_value' => $node->book['plid'],
    '#options' => book_toc($node->book['bid'], array($node->nid), MENU_MAX_DEPTH),
    '#attributes' => array('class' => 'book-title-select'),
  );

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  $form['buttons']['#prefix'] = '<div class="buttons" style="display: block;">';
  $form['buttons']['#suffix'] = '</div>';
  return $form;
}

/**
 * Form submit callback.
 */
function atrium_folders_move_file_submit(&$form, &$form_state) {

  $node = $form_state['node'];
  if ($node->book['mlid'] != $form_state['values']['folder']) {   // Don't move to the same folder.
    $fid = $form_state['folder']['id'];
    $file = $node->files[$fid];
    $menu_link = menu_link_load($form_state['values']['folder']);
    $dest = menu_get_object('node', 1, $menu_link['link_path']);
    $file->nid = $dest->nid;
    $file->vid = $dest->vid;
    drupal_write_record('upload', $file, array('fid'));
  }
}

/**
 * Ajax callback.
 */
function atrium_folders_move_file_ajax_callback($form_state) {
  
  $commands = array();
  $node = $form_state['node'];
  $commands[] = atrium_folders_command_dismiss_modal_toolbox();
  if ($node->book['mlid'] != $form_state['values']['folder']) {   // Don't move to the same folder.
    $commands[] = ctools_ajax_command_remove('.file-'. $form_state['folder']['id']);
  }
  return $commands;
}

/**
 * Rename file toolbox ================================================
 */

/**
 * Form callback.
 */
function atrium_folders_rename_file(&$form_state) {
  $node = $form_state['node'];
  $fid = $form_state['folder']['id'];
  $file = $node->files[$fid];

  $form = array();
  $form['rename'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rename'),
    '#collapsable' => FALSE,
    '#tree' => FALSE,
  );
  $form['rename']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#required' => TRUE,
    '#description' => t('A brief description of the file.'),
    '#default_value' => $file->description,
  );

  $form['rename']['filename'] = array(
    '#type' => 'textfield',
    '#title' => t('Filename'),
    '#required' => TRUE,
    '#description' => t('Filename, including extension. For security reasons, not-standard characters in the filename will be automatically converted.'),
    '#default_value' => $file->filename,
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  $form['buttons']['#prefix'] = '<div class="buttons" style="display: block;">';
  $form['buttons']['#suffix'] = '</div>';
  return $form;
}

/**
 * Form submit callback.
 */
function atrium_folders_rename_file_submit(&$form, &$form_state) {

  global $user;
  $node = $form_state['node'];
  $fid = $form_state['folder']['id'];

  $extensions = '';
  foreach ($user->roles as $rid => $name) {
    $extensions .= ' '. variable_get("upload_extensions_$rid",
    variable_get('upload_extensions_default', 'jpg jpeg gif png txt html doc xls pdf ppt pps odt ods odp'));
  }

  $filename = file_munge_filename(trim(basename($form_state['values']['filename']), '.'), $extensions);
  $dest = dirname($node->files[$fid]->filepath) . DIRECTORY_SEPARATOR . $filename;
  $node->files[$fid]->description = $form_state['values']['description'];
  $source = $node->files[$fid];
  node_save($node);
  if ($source->filename != $filename && file_move($source, $dest)) {
    $source->timestamp = time();
    $source->filename = basename($source->filepath);
    $result =  drupal_write_record('files', $source, array('fid'));
  }
  $form_state['dest'] = $source;
}

/**
 * Ajax callback.
 */
function atrium_folders_rename_file_ajax_callback($form_state) {
  $commands = array();
  $file = $form_state['dest'];
  $link = '.file-'. $form_state['folder']['id'] .' .filename';
  $description_href = '.file-'. $form_state['folder']['id'] .' .description a';
  $icon_href = '.file-'. $form_state['folder']['id'] .' .icon a';
  $description = '.file-'. $form_state['folder']['id'] .' .link';
  $commands[] = ctools_ajax_command_html($link, $file->filename);
  $commands[] = ctools_ajax_command_attr($description_href, 'href', base_path() . $file->filepath);
  $commands[] = ctools_ajax_command_attr($icon_href, 'href', base_path() . $file->filepath);
  $commands[] = ctools_ajax_command_html($description, $file->description);
  $commands[] = atrium_folders_command_dismiss_modal_toolbox();
  return $commands;
}

/**
 * Delete file toolbox ================================================
 */

/**
 * Form callback.
 */
function atrium_folders_delete_file(&$form_state) {
  $form = array();

  global $user;
  $node = $form_state['node'];
  $fid = $form_state['folder']['id'];
  $form['fid'] = array(
    '#type' => 'hidden',
    '#value' => $fid,
  );

  $form['delete'] = array(
    '#type' => 'fieldset',
    '#title' => t('Delete'),
    '#description' => t('Are you sure you want to delete this file?'),
    '#collapsable' => FALSE,
    '#tree' => FALSE,
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Yes'),
  );
  $form['buttons']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('No'),
  );
  $form['buttons']['#prefix'] = '<div class="buttons" style="display: block;">';
  $form['buttons']['#suffix'] = '</div>';

  return $form;
}

/**
 * Form submit callback.
 */
function atrium_folders_delete_file_submit(&$form, &$form_state) {
  $node = $form_state['node'];
  $fid = $form_state['folder']['id'];
  if ($form_state['clicked_button']['#value'] == t('Yes')) {
    $node->files[$fid]->remove = TRUE;
    drupal_set_message(t('The file has been deleted.'));
    node_save($node);
  }
}

/**
 * Ajax callback.
 */
function atrium_folders_delete_file_ajax_callback($form_state) {
  $commands = array();
  if ($form_state['clicked_button']['#value'] == t('Yes')) {
    $commands[] = ctools_ajax_command_remove('.file-'. $form_state['folder']['id']);
  }
  $commands[] = atrium_folders_command_dismiss_modal_toolbox();
  return $commands;
}


