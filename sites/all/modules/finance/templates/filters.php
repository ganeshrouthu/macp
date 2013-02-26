<?php
$key_report_type = 1;
$key_fin_year = 2;
$key_district = 3;
$arg_fin_year = (arg($key_fin_year)) ? arg($key_fin_year) : '2012-2013';
$arg_district = (arg($key_district)) ? arg($key_district) : 0;
$arg_report_type = (arg($key_report_type)) ? arg($key_report_type) : 0;
?>

<?php
$districts = _get_query_results(_sql_districts());
$report_types = _get_report_types();
$years = _get_query_results(_sql_financial_years());
?>
<table align="left" border="0" cellspacing="0" cellpadding="0" width="100%">
	<tr align="left" valign="top">
		<td>State</td>
		<td>Maharashtra</td>
    <td>District</td>
		<td>
    <?php
    $dist_all_url = _prepare_report_filters($key_district, 'all');
    ?>
    <select name="macp_districts" id="macp_districts" class="reports-filter">
		<option value="<?php print $dist_all_url;?>" selected="selected">All</option>
    <?php
    if (!empty($districts)) {
      foreach ($districts as $district) {
        extract($district, EXTR_PREFIX_ALL, 'DT');
        $dist_url = _prepare_report_filters($key_district, $DT_Dist_ID);
        $selected_dist = (arg(3) == $DT_Dist_ID) ? 'selected="selected"' : '';
      ?>
      <option value="<?php print $dist_url;?>" <?php print $selected_dist;?>><?php print $DT_Dist_Name;?>
      </option>
      <?php
      }
    }
    ?>
</select></td>
	</tr>

  <tr align="left" valign="top">
		<td>Report Type</td>
		<td><select name="macp_report_types" id="macp_report_types" class="reports-filter">
		<option value="" selected="selected">All</option>
    <?php
    if (!empty($report_types)) {
      foreach ($report_types as $report_type => $report_type_desc) {
        $report_type_url = _prepare_report_filters($key_report_type, $report_type);
        $selected_report_type = (arg(1) == $report_type) ? 'selected="selected"' : '';
      ?>
      <option value="<?php print $report_type_url;?>" <?php print $selected_report_type;?>><?php print $report_type_desc;?>
      </option>
      <?php
      }
    }
    ?>
</select></td>
    <td>Financial Year</td>
		<td>
    <?php
    $fin_all_url = _prepare_report_filters($key_fin_year, 'all');
    ?>
    <select name="macp_fys" id="macp_fys" class="reports-filter">
		<option value="<?php print $fin_all_url;?>" selected="selected">All</option>
    <?php
    if (!empty($years)) {
      foreach ($years as $year) {
        extract($year, EXTR_PREFIX_ALL, 'FY');
        $fin_url = _prepare_report_filters($key_fin_year, $FY_Financial_Year_Name);
        $selected_fy = (arg(2) == $FY_Financial_Year_Name) ? 'selected="selected"' : '';
    ?>
      <option value="<?php print $fin_url;?>" <?php print $selected_fy;?>><?php print $FY_Financial_Year_Name;?>
      </option>
      <?php
      }
    }
    ?>
</select>
    </td>
	</tr>
</table>