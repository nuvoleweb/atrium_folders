<?php
/**
 * A handler to provide files or folders management toolbox.
 *
 * @ingroup views_field_handlers
 */
class atrium_folders_handler_field_folder_icon extends views_handler_field {

  function allow_advanced_render() {
    return FALSE;
  }

  function query() {
  }

  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    unset($form['alter']);
    unset($form['empty']);
    unset($form['hide_empty']);
    unset($form['empty_zero']);
  }

  function render($values) {
    $type = isset($values->nid) ? DOCUMENT_FOLDER : DOCUMENT_FILE;

    if ($type == DOCUMENT_FILE && isset($values->files_filemime)) {
      $extension = strtolower(substr(strrchr($values->files_filename, '.'), 1));
      return '<div class="mime-'. $extension .'"><div class="fileview mime"></div>';
    }
    elseif ($type == DOCUMENT_FOLDER) {
      $icon = l('', 'node/'. $values->nid, array('attributes' => array('class' => 'folder')));
      return "<div class='fileview'>". $icon ."</div>";
    }
    return '';
  }
}
