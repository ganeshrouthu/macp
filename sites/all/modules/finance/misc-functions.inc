<?php
function _odbc_connect() {
  $conn = FALSE;
  if (!function_exists('odbc_connect')) {
    return $conn;
  }

  $conn = odbc_connect('MACPDB','','');
  if (!$conn) {
    print 'Error: Could not connect to MSSQL Server!';
    return FALSE;
  }
  else {
    return $conn;
  }
}

function _get_last_financial_year($financial_year) {
  $temp_one = substr($financial_year, 0, 4);
  $temp_two = (int)($temp_one - 1);
  $last_financial_year = $temp_two . '-' . $temp_one;
  return $last_financial_year;
}

function _format_lacs($nums, $fields) {
  $arr = $nums;
  $one_lac = 100000;
  if (!empty($nums) && !empty($fields)) {
    foreach ($nums as $key => $num) {
      if (in_array($key, $fields)) {
        $figure = ($num > 0) ? $num / $one_lac : 0;
        $amt = number_format($figure, 2, '.', '');
        $arr[$key] = ($amt > 0) ? $amt : 0;
      }
    }
  }
  return $arr;
}

function _calculate_percentage($par_one, $par_two) {
  $percentage = 0;
  if ($par_two > 0 && $par_one > 0) {
    $percentage = ($par_one/$par_two)*100;
    $percentage = number_format($percentage, 0, '', '');
  }
  return $percentage;
}

function _format_percentage($par_one) {
  $percentage = 0;
  if ($par_one > 0) {
    $percentage = number_format($par_one, 0, '', '');
  }
  return $percentage;
}

function _display_reports_listing() {
  $reports_nodes = node_load_multiple(array(), array('type' => 'reports'));
  $reports_node = current($reports_nodes);
  $headings = array(
      'heading' => 'Financial Reports',
    );
  $theme_parameters = array('records' => $reports_node->field_reports[LANG],
                            'headings' => $headings
                        );
  return theme('reports-listing', $theme_parameters);
}

function _display_listing_theme($type, $financial_year, $records = array()) {
  $headings = array(
    'agency_type' => $type,
    'financial_year' => $financial_year
  );
  $theme_parameters = array('records' => $records,
                            'headings' => $headings,
                            'index' => _display_reports_listing()
                      );
  //return (!empty($records)) ? theme($type, $theme_parameters) : 'No data available.';
  return theme($type, $theme_parameters);
}

function _get_page_heading($type) {
  $page_headings = array();
  $page_headings['DEPT-COMPL'] = 'Departmentwise Complaint/Appeal Listing';
}

function _get_query_results($sql) {
  $conn = _odbc_connect();
  $sql_districts = $sql;
  $result = odbc_exec($conn, $sql_districts);
  $records = array();
  while($temp = odbc_fetch_array($result)) {
    $records[] = $temp;
  }
  return $records;
}

function starts_with($haystack, $needle) {
  return !strncmp($haystack, $needle, strlen($needle));
}

function _get_report_types() {
  $arr = array();
  $vocabs = taxonomy_get_vocabularies();
  $vid = NULL;
  if (! empty($vocabs)) {
    foreach ($vocabs as $vocab) {
      if ($vocab->name == 'Report Types') {
        $vid = $vocab->vid;
      }
    }
  }
  if (! $vid) {
    return $arr;
  }
  $report_types = taxonomy_get_tree($vid);
  if (! empty($report_types)) {
    foreach ($report_types as $report_type) {
      $arr[$report_type->name] = $report_type->description;
    }
  }
  return $arr;
}