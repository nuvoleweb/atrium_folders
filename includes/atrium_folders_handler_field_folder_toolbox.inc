<?php
/**
 * A handler to provide files or folders management toolbox.
 *
 * @ingroup views_field_handlers
 */
class atrium_folders_handler_field_folder_toolbox extends views_handler_field {

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
    static $result = array();
    static $done = FALSE;

    $output = '';
    $node = menu_get_object('node');
    if ($node && $node->type == 'folder' && node_access('update', $node)) {
      ctools_include('ajax');
      ctools_include('modal');
      if (!$done) {
        $settings = array('ModalToolbox' => array(
          'closeText' => t('Close'),
          'throbber' => theme('image', drupal_get_path('module', 'atrium_folders') .'/images/ajax-loader.gif', t('Loading...'), t('Loading')),
        ));
        drupal_add_js($settings, 'setting');
        drupal_add_js('misc/jquery.form.js');
        ctools_add_js('ajax-responder');
        ctools_add_js('modal');
        drupal_add_js(drupal_get_path('module', 'atrium_folders') .'/js/atrium_folders_modal_toolbox.js'); // CTools modal.js overrides
        $done = TRUE;
      }

      $output = '';
      $type = isset($values->nid) ? DOCUMENT_FOLDER : DOCUMENT_FILE;
      $id = isset($values->nid) ? $values->nid : $values->fid;
      $result = $result ? $result : module_invoke_all('folder_toolbox');
      foreach ($result as $toolbox) {
        if ($toolbox['type'] == $type) {
          $path = sprintf('folders/toolbox/%s/%s/%s', $toolbox['op'], $type, $id);
          $output .= theme('toolbox_link', $toolbox['title'], $path, _atrium_folders_dom_id($toolbox['op'], $type, $id), $toolbox['op']);
        }
      }

      ctools_include('plugins');
    }
    return $output;
  }
}

