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
  if (!$conn || !function_exists('odbc_exec')|| !function_exists('odbc_fetch_array')) {
    return array();
  }
  $result = odbc_exec($conn, $sql);
  $records = array();
  while($temp = odbc_fetch_array($result)) {
    $records[] = $temp;
  }
  return $records;
}

function _get_query_results_id_key($sql, $id_field = FALSE) {
  $conn = _odbc_connect();
  if (!$conn || !function_exists('odbc_exec')|| !function_exists('odbc_fetch_array')) {
    return array();
  }
  $result = odbc_exec($conn, $sql);
  $records = array();
  while($temp = odbc_fetch_array($result)) {
		if ($id_field) {
			$records[$temp[$id_field]] = $temp;
		}
		else {
			$records[] = $temp;
		}
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
      if ($vocab->machine_name == 'report_types') {
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

function _get_ddrcs_records($type, $financial_year, $district_id) {
	$sql_ddrcs = _sql_ddrcs_yearly_report($type, $financial_year, arg(3));
	$result = _odbc_exec($sql_ddrcs);
	$records = array();
	$fields = array(
			'Project_Funds_Released',
			'Project_Funds_Expenditure',
			'Beneficiary_Funds_Collection',
			'Beneficiary_Funds_Expenditure'
		);
	if (function_exists('odbc_fetch_array')) {
		while($temp = odbc_fetch_array($result)) {
			$temp = _format_lacs($temp, $fields);
			extract($temp, EXTR_PREFIX_ALL, 'TM');
			$temp['overall_proj_plus_ben'] = $TM_Project_Funds_Released + $TM_Beneficiary_Funds_Collection;
			$temp['overall_expenditure'] = 
			$TM_Project_Funds_Expenditure + $TM_Beneficiary_Funds_Expenditure;
			$temp['in_per_one'] = _calculate_percentage($TM_Project_Funds_Expenditure, $TM_Project_Funds_Released);
			$temp['in_per_two'] = _calculate_percentage($TM_Beneficiary_Funds_Expenditure, $TM_Beneficiary_Funds_Collection);
			unset($temp['ACC_ID']);
			$records[] = $temp;
		}
	}
	
	//_p($records);
	return $records;
}

function _get_districts_rhs_results($district_id) {
	$records = array();
	$rows = _get_query_results(_sql_districts_rhs($district_id));
	$total_cnt_Centers = 0;
	if (!empty($rows)) {
		$i=0;
		foreach ($rows as $row) {
			extract($row, EXTR_PREFIX_ALL, 'U');
			$total_cnt_Centers += ($U_Num_Centers > 0) ? $U_Num_Centers : 0;
			$i++;
		}
	}
	$totals['total_cnt_Centers'] = $total_cnt_Centers;
	$records['Totals'] = $totals;
	$records['records'] = $rows;
	return $records;
}

function _get_atma_records($type, $financial_year, $district_id) {
	$records = array();
	$district_id = arg(3);
	$sql_atma = _sql_atma_yearly_report($type, $financial_year, $district_id);
//	_p($sql_atma, 1);
	$result = _odbc_exec($sql_atma);
	$fields = array(
			'Funds_Released',
			'Expenditure_Year_Till_Date',
			'Expenditure',
			'Last_Financial_Year_Expenditure',
		);
	if (function_exists('odbc_fetch_array')) {
		while($temp = odbc_fetch_array($result)) {
			extract($temp, EXTR_PREFIX_ALL, 'T');
			$temp['in_per'] = _calculate_percentage($T_Expenditure, $T_Funds_Released);
			$temp = _format_lacs($temp, $fields);
			$records[] = $temp;
		}
	}
	return $records;
}

function _get_dyAHD_records($type, $financial_year, $district_id) {
	$records = array();
	$sql_dyAHD = _sql_dyAHD_yearly_report($type, $financial_year, $district_id);
	$result = _odbc_exec($sql_dyAHD);
	$fields = array(
			'Funds_Released',
			'Expdit_By_Last_Fin_Year',
			'Expdit_Year_Till_Date',
			'Expenditure'
		);
	if (function_exists('odbc_fetch_array') && $result) {
		while($temp = odbc_fetch_array($result)) {
			extract($temp, EXTR_PREFIX_ALL, 'T');
			$temp['in_per'] = _calculate_percentage($T_Expenditure, $T_Funds_Released);
			$temp = _format_lacs($temp, $fields);
			$records[] = $temp;
		}
	}
	return $records;
}

function _get_complaints_deptwise_records($financial_year) {
	$records = array();
	$sql_compl_dept = _sql_complaints_deptwise_yearly_report($financial_year);
	$result = _odbc_exec($sql_compl_dept);
	if (function_exists('odbc_fetch_array')) {
		while($temp = odbc_fetch_array($result)) {
			$records[] = $temp;
		}
	}
	return $records;
}

function _get_rh_cwp_records($financial_year) {
	$sql_rh_cwp = _sql_rh_cwp($financial_year);
	$result = _odbc_exec($sql_rh_cwp);
	$rows = array();
	$fields = array(
			'Total_Payment_Done',
			'Total_Proposed_Cost'
		);
	if (function_exists('odbc_fetch_array')) {
		while($temp = odbc_fetch_array($result)) {
			extract($temp, EXTR_PREFIX_ALL, 'T');
			$temp['Fin'] = _calculate_percentage($T_Total_Payment_Done, $T_Total_Proposed_Cost);
			$temp['Phy'] = _format_percentage($T_Phy);
			$temp['total_RHs'] = $temp['Per_25'] + $temp['Per_50'] + $temp['Per_75'] + $temp['Per_100'];
			$temp = _format_lacs($temp, $fields);
			$rows[] = $temp;
		}
	}

	$totals = array();
	$total_cnt_Centers = 0;
	$total_cnt_per_25 = 0;
	$total_cnt_per_50 = 0;
	$total_cnt_per_75 = 0;

	$total_cnt_per_100 = 0;
	$Total_Proposed_Cost = 0;
	$Total_Payment_Done = 0;
	$total_cnt_Total_Proposed_Cost = 0;
	$total_cnt_Total_Payment_Done = 0;

	$total_cnt_Phy = 0;
	$total_cnt_Fin = 0;
	if (!empty($rows)) {
		$i=0;
		foreach ($rows as $row) {
			extract($row, EXTR_PREFIX_ALL, 'U');
			$total_cnt_Centers += ($U_total_RHs > 0) ? $U_total_RHs : 0;
			$total_cnt_per_25 += ($U_Per_25 > 0) ? $U_Per_25 : 0;
			$total_cnt_per_50 += ($U_Per_50 > 0) ? $U_Per_50 : 0;
			$total_cnt_per_75 += ($U_Per_75 > 0) ? $U_Per_75 : 0;;
			$total_cnt_per_100 += ($U_Per_100 > 0) ? $U_Per_100 : 0;

			$total_cnt_Phy += ($U_Phy > 0) ? $U_Phy : 0;
			$total_cnt_Fin += ($U_Fin > 0) ? $U_Fin : 0;
			$total_cnt_Total_Proposed_Cost += ($U_Total_Proposed_Cost > 0) ? $U_Total_Proposed_Cost : 0;
			$total_cnt_Total_Payment_Done += ($U_Total_Payment_Done > 0) ? $U_Total_Payment_Done : 0;
			$i++;
		}
		$totals['total_cnt_Centers'] = $total_cnt_Centers;
		$totals['total_cnt_per_25'] = $total_cnt_per_25;
		$totals['total_cnt_per_50'] = $total_cnt_per_50;
		$totals['total_cnt_per_75'] = $total_cnt_per_75;
		$totals['total_cnt_per_100'] = $total_cnt_per_100;
		$totals['total_cnt_Total_Proposed_Cost'] = $total_cnt_Total_Proposed_Cost;
		$totals['total_cnt_Total_Payment_Done'] = $total_cnt_Total_Payment_Done;
		$totals['total_cnt_Phy'] = $total_cnt_Phy / $i;
		$totals['total_cnt_Fin'] = $total_cnt_Fin / $i;
	}
	$records['Totals'] = $totals;
	$records['records'] = $rows;
	return $records;
}

function _get_apmc_cwp_records($financial_year) {
	$records = array();
	$sql_apmc_cwp = _sql_apmc_cwp($financial_year);
	$result = _odbc_exec($sql_apmc_cwp);
	$rows = array();
	$fields = array(
			'Total_Payment_Done',
			'Total_Proposed_Cost'
		);
	if (function_exists('odbc_fetch_array')) {
		while($temp = odbc_fetch_array($result)) {
			extract($temp, EXTR_PREFIX_ALL, 'T');
			$temp['Fin'] = _calculate_percentage($T_Total_Payment_Done, $T_Total_Proposed_Cost);
			$temp['Phy'] = _format_percentage($T_Phy);
			$temp['total_inst'] = $temp['Per_25'] + $temp['Per_50'] + $temp['Per_75'] + $temp['Per_100'];
			$temp = _format_lacs($temp, $fields);
			$rows[] = $temp;
		}
	}
	$totals = array();
	$total_cnt_Centers = 0;
	$total_cnt_per_25 = 0;
	$total_cnt_per_50 = 0;
	$total_cnt_per_75 = 0;

	$total_cnt_per_100 = 0;
	$Total_Proposed_Cost = 0;
	$Total_Payment_Done = 0;
	$total_cnt_Total_Proposed_Cost = 0;
	$total_cnt_Total_Payment_Done = 0;

	$total_cnt_Phy = 0;
	$total_cnt_Fin = 0;
	if (!empty($rows)) {
		$i=0;
		foreach ($rows as $row) {
			extract($row, EXTR_PREFIX_ALL, 'U');
			$total_cnt_Centers += ($U_total_inst > 0) ? $U_total_inst : 0;
			$total_cnt_per_25 += ($U_Per_25 > 0) ? $U_Per_25 : 0;
			$total_cnt_per_50 += ($U_Per_50 > 0) ? $U_Per_50 : 0;
			$total_cnt_per_75 += ($U_Per_75 > 0) ? $U_Per_75 : 0;;
			$total_cnt_per_100 += ($U_Per_100 > 0) ? $U_Per_100 : 0;

			$total_cnt_Phy += ($U_Phy > 0) ? $U_Phy : 0;
			$total_cnt_Fin += ($U_Fin > 0) ? $U_Fin : 0;
			$total_cnt_Total_Proposed_Cost += ($U_Total_Proposed_Cost > 0) ? $U_Total_Proposed_Cost : 0;
			$total_cnt_Total_Payment_Done += ($U_Total_Payment_Done > 0) ? $U_Total_Payment_Done : 0;
			$i++;
		}
		$totals['total_cnt_Centers'] = $total_cnt_Centers;
		$totals['total_cnt_per_25'] = $total_cnt_per_25;
		$totals['total_cnt_per_50'] = $total_cnt_per_50;
		$totals['total_cnt_per_75'] = $total_cnt_per_75;
		$totals['total_cnt_per_100'] = $total_cnt_per_100;
		$totals['total_cnt_Total_Proposed_Cost'] = $total_cnt_Total_Proposed_Cost;
		$totals['total_cnt_Total_Payment_Done'] = $total_cnt_Total_Payment_Done;
		$totals['total_cnt_Phy'] = $total_cnt_Phy / $i;
		$totals['total_cnt_Fin'] = $total_cnt_Fin / $i;
	}
	$records['Totals'] = $totals;
	$records['records'] = $rows;
	return $records;
}

function _get_districts_apmcs_records($district_id) {
	$rows = _get_query_results(_sql_districts_apmcs(arg(3)));
	$records = array();
	$total_cnt_Centers = 0;
	if (!empty($rows)) {
		$i=0;
		foreach ($rows as $row) {
			extract($row, EXTR_PREFIX_ALL, 'U');
			$total_cnt_Centers += ($U_Num_Centers > 0) ? $U_Num_Centers : 0;
			$i++;
		}
	}
	$totals['total_cnt_Centers'] = $total_cnt_Centers;
	$records['Totals'] = $totals;
	$records['records'] = $rows;
	return $records;
}

function _get_dist_phase_complaints($financial_year) {
	$sql_compl_dist_phase = _sql_complaints_dist_phasewise_yearly_report($financial_year);
	$result = _odbc_exec($sql_compl_dist_phase);
	$records = array();
	if (function_exists('odbc_fetch_array')) {
		while($temp = odbc_fetch_array($result)) {
			$records[] = $temp;
		}
	}
	return $records;
}

function _split_request_get_params() {
	$arr['report_type'] = '';
	$arr['financial_year'] = '';
	$arr['district_id'] = '';
	$req = (array_keys($_REQUEST)) ? array_keys($_REQUEST) : array();
	if (isset($req[0]) && strlen($req[0])) {
		$pars = explode('/', $req[0]);
		$default_financial_year = (date('Y')-1) . '-' . date('Y');
		$arr['report_type'] = (isset($pars[0]) && strlen($pars[0])) ? $pars[0] : 'all';
		$arr['financial_year'] = (isset($pars[1]) && strlen($pars[1])) ? $pars[1] : $default_financial_year;
		$arr['district_id'] = (isset($pars[2]) && strlen($pars[2])) ? $pars[2] : 'all';
		return $arr;
	}
}

function _get_ddrcs_header_titles() {
	return array(
					'District Dy. Registar Co-Operative Soc. (DDRC)',
					'Phase',
					'Financial Year',
					'Project Funds-Funds Released',
					'Project Funds-Expenditure',
					'In %',
					'Beneficiary Funds-Collection',
					'Beneficiary Funds-Expenditure',
					'In %',
					'Overall-Project+Beneficiary',
					'Overall-Expenditure',
				);
}

function _get_atma_header_titles() {
	return array(
					'ATMA - District Level',
					'Phase',
					'Funds Released',
					'Expenditure by Mar-2012',
					'Expenditure Year Till Date',
					'Expenditure',
					'In %'
				);
}

function _get_dyADH_header_titles() {
	return array(
					'Dy. Comm. AHD',
					'Phase',
					'Funds Released',
					'Expdit. by Mar-2012',
					'Exp. Year Till Dt.',
					'Expdit.',
					'In %'
				);
}

function _get_complaints_deptwise_header_titles() {
	return array(
						'Department',
						'Total Complaints',
						'New Complaints',
						'Interim Reply Sent',
						'Forwarded',
						'Resolved'
				);
}

function _get_districts_header_titles() {
	return array(
					'District Dy. Registar Co-Operative Soc. (DDRC)',
					'Phase',
					'Financial Year',
					'Project Funds-Funds Released',
					'Project Funds-Expenditure',
					'In %',
					'Beneficiary Funds-Collection',
					'Beneficiary Funds-Expenditure',
					'In %',
					'Overall-Project+Beneficiary',
					'Overall-Expenditure',
				);
}
function _get_district_wise_rhs_header_titles() {
	return array(
					'District Name',
					'Total RH',
					'In-house',
					'SP',
					'UC Stage',
					'Three tranches',
					'First or Second tranche'
				);
}
function _get_district_wise_apmcs_header_titles() {
	return array(
					'District Name',
					'Total APMC',
					'In-house',
					'SP',
					'UC Stage',
					'Three tranches',
					'First or Second tranche'
				);
}
function _get_district_phase_complaints_header_titles() {
	return array(
					'Dist Phase No',
					'District Name',
					'Total Complaints',
					'New Complaints',
					'Interim Reply Sent',
					'Forwarded',
					'Resolved'
				);
}
function _get_rh_cwp_header_titles() {
	return array(
					'PARTICULARS OF FULL PROJECT PROPOSAL',
					'No of RH',
					'25%',
					'50%',
					'75%',
					'100%',
					'Proposed Cost',
					'Payment Done',
					'Phy',
					'Fin'
				);
}
function _get_apmc_cwp_header_titles() {
	return array(
					'PARTICULARS OF FULL PROJECT PROPOSAL',
					'No of APMC',
					'25%',
					'50%'
					,'75%',
					'100%',
					'Proposed Cost',
					'Payment Done',
					'Phy',
					'Fin'
				);
}

function _get_mis_report_groups() {
  $arr = array();
  $vocabs = taxonomy_get_vocabularies();
  $vid = NULL;
  if (! empty($vocabs)) {
    foreach ($vocabs as $vocab) {
      if ($vocab->machine_name == 'mis_report_groups') {
        $vid = $vocab->vid;
      }
    }
  }
  if (! $vid) {
    return $arr;
  }
  $report_groups = taxonomy_get_tree($vid);
  if (! empty($report_groups)) {
    foreach ($report_groups as $report_group) {
      $arr[$report_group->tid] = $report_group->description;
    }
  }
  return $arr;
}