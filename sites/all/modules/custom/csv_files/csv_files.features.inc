<?php
/**
 * @file
 * csv_files.features.inc
 */

/**
 * Implements hook_node_info().
 */
function csv_files_node_info() {
  $items = array(
    'import_users_csv' => array(
      'name' => t('Import Users CSV'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
