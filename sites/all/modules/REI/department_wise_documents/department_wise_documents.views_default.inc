<?php
/**
 * @file
 * department_wise_documents.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function department_wise_documents_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'deptwise_documents';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Departmentwise Documents';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Departmentwise Documents';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-left',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['label'] = 'No records found.';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  /* No results behavior: Global: Result summary */
  $handler->display->display_options['empty']['result']['id'] = 'result';
  $handler->display->display_options['empty']['result']['table'] = 'views';
  $handler->display->display_options['empty']['result']['field'] = 'result';
  $handler->display->display_options['empty']['result']['label'] = 'No records found.';
  $handler->display->display_options['empty']['result']['empty'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Upload Doc */
  $handler->display->display_options['fields']['field_upload_doc']['id'] = 'field_upload_doc';
  $handler->display->display_options['fields']['field_upload_doc']['table'] = 'field_data_field_upload_doc';
  $handler->display->display_options['fields']['field_upload_doc']['field'] = 'field_upload_doc';
  $handler->display->display_options['fields']['field_upload_doc']['click_sort_column'] = 'fid';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Field: Dept/Cell (field_department) */
  $handler->display->display_options['arguments']['field_department_value']['id'] = 'field_department_value';
  $handler->display->display_options['arguments']['field_department_value']['table'] = 'field_data_field_department';
  $handler->display->display_options['arguments']['field_department_value']['field'] = 'field_department_value';
  $handler->display->display_options['arguments']['field_department_value']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_department_value']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['field_department_value']['default_argument_options']['index'] = '2';
  $handler->display->display_options['arguments']['field_department_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_department_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_department_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_department_value']['limit'] = '0';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'upload_doucments' => 'upload_doucments',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'departmentwise-documents';
  $export['deptwise_documents'] = $view;

  return $export;
}
