<?php
/**
 * @file
 * report_template.features.inc
 */

/**
 * Implements hook_node_info().
 */
function report_template_node_info() {
  $items = array(
    'report_template' => array(
      'name' => t('Report Template'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
