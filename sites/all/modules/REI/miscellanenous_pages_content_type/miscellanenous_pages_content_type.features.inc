<?php
/**
 * @file
 * miscellanenous_pages_content_type.features.inc
 */

/**
 * Implements hook_node_info().
 */
function miscellanenous_pages_content_type_node_info() {
  $items = array(
    'misc_pages' => array(
      'name' => t('Miscellanenous Pages'),
      'base' => 'node_content',
      'description' => t('These are pages who have specific meanings.
For example, if we want to display some data on page for which we can not create a view.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
