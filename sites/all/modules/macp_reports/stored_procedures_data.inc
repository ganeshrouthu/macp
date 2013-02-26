<?php
function _prepare_report_filters($key, $val) {
  global $base_url;
	$pars = _split_request_get_params();
	if (isset($pars[$key]) && strlen($pars[$key])) {
		$pars[$key] = $val;
	}
	$key_type = '';
	$key_financial_year = '';
	$key_district = '';
	
	$url = '';
	$nodes = node_load_multiple(array(), array('type' => 'report_template'));
	if (!empty($nodes)) {
		$report_node = current($nodes);
		$path_url = drupal_get_path_alias('node/' . $report_node->nid);
		extract($pars, EXTR_PREFIX_ALL, 'P');
		$url = $base_url . '/' . $path_url . '?' . implode('/', $pars);
	}
  return $url;
}
