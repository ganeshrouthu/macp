<?php
$print_drop_downs = TRUE;
if (arg(0) == 'printpdf') {$print_drop_downs = FALSE;}
?>
<div class="mis-clear-all">
<?php
if (arg(0) == 'printpdf') {
?>
<style type="text/css">
/* Added for styling reporting tables. */
.macp-table {
width: 100% !important;
border: 1px solid #B0B0B0;
}
.macp-table tbody {
/* Kind of irrelevant unless your .css is alreadt doing something else */
margin: 0;
padding: 0;
border: 0;
outline: 0;
font-size: 100%;
vertical-align: baseline;
background: transparent;
}
.macp-table thead {
text-align: left;
}
.macp-table thead th {
background: -moz-linear-gradient(top, #F0F0F0 0, #DBDBDB 100%);
background: -webkit-gradient(linear, left top, left bottom, color-stop(0%, #F0F0F0), color-stop(100%, #DBDBDB));
filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='#F0F0F0', endColorstr='#DBDBDB', GradientType=0);
border: 1px solid #B0B0B0;
color: #444;
font-size: 12px;
font-weight: bold;
padding: 3px 10px;
}
.macp-table td {
padding: 1px 5px;
}
.macp-table tr:nth-child(even) {
background: #F2F2F2;
}

.mis-clear-all {
	clear:all;
}

.pdf-span {
	width:200px;
	float:left;
}

.pdf-div {
	width:400px;
	float:left;
}
</style>
<?php
}
?>
<?php
global $base_url;
$current_page = $base_url . '/' . implode('/', arg());
if (empty($records)) {
  print 'No financial report available.';
}
$pars = _split_request_get_params();
extract($pars, EXTR_PREFIX_ALL, 'P');
$reports_link = drupal_get_path_alias('node/' . arg(1));
$export_csv_link = $base_url .'/'.$reports_link .'?'. implode('/', $pars);
$districts = _get_query_results_id_key(_sql_districts(), 'Dist_ID');
$report_types = _get_report_types();
$years = _get_query_results(_sql_financial_years());
?>


<?php
$url_district = 'All';
if ($P_district_id != 'all' && !empty($districts)) {
	$url_district = $districts[$P_district_id]['Dist_Name'];
}
?>

<?php
if (arg(0) == 'printpdf' || arg(0) == 'print') {
?>
<div>
<strong>Report Type:</strong> <?php print $P_report_type;?> | <strong>Financial Year:</strong> <?php print $P_financial_year;?> | <strong>State</strong>: Maharashtra | <strong>District:</strong> <?php print $url_district;?> 
</div>
<?php
}
else {
?>
<table border="0" cellspacing="0" cellpadding="0" align="center" class="macp-table report-filters">
	<thead>
  <tr align="left" valign="top">
		<td><div class="heading"><strong>State: </strong></div>Maharashtra</td>
    <td><div class="heading"><strong>District: </strong></div><?php
    $dist_all_url = _prepare_report_filters('district_id', 'all');
    $report_all_url = _prepare_report_filters('report_type', 'all');
		$selected_dist_all = (isset($pars['district_id']) && $pars['district_id'] == 'all') ? 'selected="selected"' : '';
		$selected_fy_all = (isset($pars['financial_year']) && $pars['financial_year'] == 'all') ? 'selected="selected"' : '';
    ?>
		<?php
		$dist_text = 'All';
		if ($print_drop_downs) {
		?>
    <select name="macp_districts" id="macp_districts" class="reports-filter">
		<option value="<?php print $dist_all_url;?>" <?php print $selected_dist_all;?>>All</option>
		<?php
		}
		?>
    <?php	
    if (!empty($districts)) {
      foreach ($districts as $district) {
        extract($district, EXTR_PREFIX_ALL, 'DT');
        $dist_url = _prepare_report_filters('district_id', $DT_Dist_ID);
        $selected_dist = ($P_district_id == $DT_Dist_ID) ? 'selected="selected"' : '';
				if ($pars['district_id'] == $DT_Dist_ID) {
					$dist_text = $DT_Dist_Name;
				}
				if ($print_drop_downs) {
      ?>
      <option value="<?php print $dist_url;?>" <?php print $selected_dist;?>><?php print $DT_Dist_Name;?> </option>
      <?php
				}
      }
    }
    ?>
		<?php print ($print_drop_downs) ? '</select>' : $dist_text;?></td>
	</tr>

  <tr align="left" valign="top">
		<td><div class="heading"><strong>Report Type: </strong></div>
		<?php
		if (isset($pars['report_type'])) {
			print $pars['report_type'];
		}
		?>
		</td>
		<td>
		<div class="heading"><strong>Financial Year: </strong></div>
		<?php
		$financial_year_text = '';
		if ($print_drop_downs) {
		?>
		<?php
    $fin_all_url = _prepare_report_filters('financial_year', 'all');
    ?>
		<select name="macp_fys" id="macp_fys" class="reports-filter">
		<option value="<?php print $fin_all_url;?>" <?php print $selected_fy_all;?>>All</option>
		<?php
		}
		?>
    <?php
    if (!empty($years)) {
      foreach ($years as $year) {
        extract($year, EXTR_PREFIX_ALL, 'FY');
        $fin_url = _prepare_report_filters('financial_year', $FY_Financial_Year_Name);
        $selected_fy = ($P_financial_year == $FY_Financial_Year_Name) ? 'selected="selected"' : '';
				if ($pars['financial_year'] == $FY_Financial_Year_Name) {
					$financial_year_text = $FY_Financial_Year_Name;
				}
    ?>
		<?php
		if ($print_drop_downs) {
		?>
      <option value="<?php print $fin_url;?>" <?php print $selected_fy;?>><?php print $FY_Financial_Year_Name;?></option>
			<?php
				}
			}
    }
    ?>
<?php print ($print_drop_downs) ? '</select>' : $financial_year_text;?>
</td>
	</tr>
  </thead>
</table>
<?php
}
?>
</div>