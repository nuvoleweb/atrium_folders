<?php

class atrium_folders_handler_argument_menu_trail extends views_handler_argument {

  function uses_breadcrumb() { return FALSE; }
  function needs_style_plugin() { return FALSE; }

  function query() {
    $nids = array();
    $node = menu_get_object($type = 'node', 1, 'node/'. $this->argument);
    if (!empty($node)) {
      $links = book_get_flat_menu($node->book);
      foreach ($links as $link) {
        list(, $nid) = explode('/', $link['link_path']);
        $nids[] = $nid;
      }
      if ($nids) {
        $query = implode(', ', $nids);
        $this->ensure_my_table();
        $this->query->add_where($this->options['group'], $this->query->table_queue['node']['alias'] .".nid IN (%s)", $query);
      }
    }
  }
}
