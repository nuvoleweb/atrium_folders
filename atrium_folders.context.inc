<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function atrium_folders_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'folders_listing';
  $context->description = '';
  $context->tag = 'Folders';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'folder' => 'folder',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
    'path' => array(
      'values' => array(
        'folders' => 'folders',
      ),
    ),
    'views' => array(
      'values' => array(
        'folders_navigation' => 'folders_navigation',
      ),
    ),
  );
  $context->reactions = array(
    'menu' => 'folders',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Folders');

  $export['folders_listing'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'spaces-feature-folders';
  $context->description = '';
  $context->tag = 'Folders';
  $context->conditions = array(
    'bookroot' => array(
      'values' => array(
        'folder' => 'folder',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
    'node' => array(
      'values' => array(
        'folder' => 'folder',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
    'path' => array(
      'values' => array(
        'folders' => 'folders',
        'node/*/folders/*' => 'node/*/folders/*',
      ),
    ),
    'views' => array(
      'values' => array(
        'folders' => 'folders',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(),
      'layout' => 'wide',
    ),
    'menu' => 'folders',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Folders');

  $export['spaces-feature-folders'] = $context;
  return $export;
}
