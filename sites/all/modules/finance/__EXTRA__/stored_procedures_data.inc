<?php
function yearly_report($type = 'DDRCS', $financial_year = '2012-2013') {
  if (!strlen($type)) {
    return 'Please enter Agency Type.';
  }
  // DB Connection
  $conn = _odbc_connect();
  switch ($type) {
    case 'DDRCS':
      $sql_ddrcs = _sql_ddrcs_yearly_report($type, $financial_year, arg(3));
      $result = odbc_exec($conn, $sql_ddrcs);
      $fields = array(
          'Project_Funds_Released',
          'Project_Funds_Expenditure',
          'Beneficiary_Funds_Collection',
          'Beneficiary_Funds_Expenditure'
        );
      $records = array();
      while($temp = odbc_fetch_array($result)) {
        $temp = _format_lacs($temp, $fields);
        extract($temp, EXTR_PREFIX_ALL, 'TM');
        $temp['overall_proj_plus_ben'] = $TM_Project_Funds_Released + $TM_Beneficiary_Funds_Collection;
        $temp['overall_expenditure'] = 
        $TM_Project_Funds_Expenditure + $TM_Beneficiary_Funds_Expenditure;
        $temp['in_per_one'] = _calculate_percentage($TM_Project_Funds_Expenditure, $TM_Project_Funds_Released);
        $temp['in_per_two'] = _calculate_percentage($TM_Beneficiary_Funds_Expenditure, $TM_Beneficiary_Funds_Collection);
        $records[] = $temp;
      }
//      _p($records, 1);

      /*
      $arr = _get_summed_records($records);
      _p($arr, 1);
      */

      return _display_listing_theme($type, $financial_year, $records);
    break;

    case 'ATMA':
      $district_id = arg(3);
      $sql_atma = _sql_atma_yearly_report($type, $financial_year, $district_id);
      $result = odbc_exec($conn, $sql_atma);
      $fields = array(
          'Funds_Released',
          'Expenditure_Year_Till_Date',
          'Expenditure',
          'Last_Financial_Year_Expenditure',
        );
      $records = array();
      while($temp = odbc_fetch_array($result)) {
        extract($temp, EXTR_PREFIX_ALL, 'T');
        $temp['in_per'] = _calculate_percentage($T_Expenditure, $T_Funds_Released);
        $temp = _format_lacs($temp, $fields);
        $records[] = $temp;
      }
      return _display_listing_theme($type, $financial_year, $records);
    break;
    
    case 'dyAHD':
      $sql_dyAHD = _sql_dyAHD_yearly_report($type, $financial_year, arg(3));
      $result = odbc_exec($conn, $sql_dyAHD);
      $fields = array(
          'Funds_Released',
          'Expdit_By_Last_Fin_Year',
          'Expdit_Year_Till_Date',
          'Expenditure'
        );
      $records = array();
      while($temp = odbc_fetch_array($result)) {
        extract($temp, EXTR_PREFIX_ALL, 'T');
        $temp['in_per'] = _calculate_percentage($T_Expenditure, $T_Funds_Released);
        $temp = _format_lacs($temp, $fields);
        $records[] = $temp;
      }
      return _display_listing_theme($type, $financial_year, $records);
    break;
    default:
      'Please select a proper Agency Type.';
    break;

    case 'DEPT-COMPL':
      $sql_compl_dept = _sql_complaints_deptwise_yearly_report($financial_year);
      $result = odbc_exec($conn, $sql_compl_dept);
      $records = array();
      while($temp = odbc_fetch_array($result)) {
        $records[] = $temp;
      }
      return _display_listing_theme($type, $financial_year, $records);
    break;

    case 'DISTRICTS':
      $records = _get_query_results(_sql_districts());
      return _display_listing_theme($type, $financial_year, $records);
    break;

    case 'DISTRICT-WISE-RHS':
      $rows = _get_query_results(_sql_districts_rhs(arg(3)));
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
      return _display_listing_theme($type, $financial_year, $records);
    break;

    case 'DISTRICT-WISE-APMCS':
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
      return _display_listing_theme($type, $financial_year, $records);
    break;

    default:
      return 'Please select a proper Agency Type.';
    break;

    case 'DIST-PHASE-COMPL':
      $sql_compl_dist_phase = _sql_complaints_dist_phasewise_yearly_report($financial_year);
      $result = odbc_exec($conn, $sql_compl_dist_phase);
      $records = array();
      while($temp = odbc_fetch_array($result)) {
        $records[] = $temp;
      }
      return _display_listing_theme($type, $financial_year, $records);
    break;
    default:
      'Please select a proper Agency Type.';
    break;

    case 'RH-CWP':
      $sql_rh_cwp = _sql_rh_cwp($financial_year);
      $result = odbc_exec($conn, $sql_rh_cwp);
      $rows = array();
      $fields = array(
          'Total_Payment_Done',
          'Total_Proposed_Cost'
        );
      while($temp = odbc_fetch_array($result)) {
        extract($temp, EXTR_PREFIX_ALL, 'T');
        $temp['Fin'] = _calculate_percentage($T_Total_Payment_Done, $T_Total_Proposed_Cost);
        $temp['Phy'] = _format_percentage($T_Phy);
        $temp['total_RHs'] = $temp['Per_25'] + $temp['Per_50'] + $temp['Per_75'] + $temp['Per_100'];
        $temp = _format_lacs($temp, $fields);
        $rows[] = $temp;
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
      return _display_listing_theme($type, $financial_year, $records);
    break;

    case 'APMC-CWP':
      $sql_apmc_cwp = _sql_apmc_cwp($financial_year);
      $result = odbc_exec($conn, $sql_apmc_cwp);
      $rows = array();
      $fields = array(
          'Total_Payment_Done',
          'Total_Proposed_Cost'
        );
      while($temp = odbc_fetch_array($result)) {
        extract($temp, EXTR_PREFIX_ALL, 'T');
        $temp['Fin'] = _calculate_percentage($T_Total_Payment_Done, $T_Total_Proposed_Cost);
        $temp['Phy'] = _format_percentage($T_Phy);
        $temp['total_inst'] = $temp['Per_25'] + $temp['Per_50'] + $temp['Per_75'] + $temp['Per_100'];
        $temp = _format_lacs($temp, $fields);
        $rows[] = $temp;
      }
      //_p($rows, 0);
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
      return _display_listing_theme($type, $financial_year, $records);
    break;
    default:
      'Please select a proper Agency Type.';
    break;
  }
}

function _prepare_report_filters($key, $val) {
  global $base_url;
  $fin_url = arg();
  $fin_url[$key] = $val;
  $url = $base_url . '/' . implode('/', $fin_url);
  return $url;
}


function _get_summed_records($records) {
  $arr = array();
  if (count($records)) {
    foreach ($records as $record) {
      //$arr[]
      //_p($record['ACC_ID']);
      $arr[$record['ACC_ID']][] = $record;
    }
  }

  $newarr = array();
  if (!empty($arr)) {
    foreach ($arr as $k => $v) {
      foreach($v as $value) {
        foreach($value as $key=>$secondValue) {
          if(!isset($newarr[$key])) {
            $newarr[$k][$key]=0;
          }
          $newarr[$k][$key]+=$secondValue;
        }
      }
    }
  }
  return $newarr;
}