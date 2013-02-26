<?php
global $base_url;
$districts = _get_query_results(_sql_districts());
$fin_years = _get_query_results(_sql_financial_years());
$current_dist = (arg(3) && arg(3) > 0) ? arg(3) : 0;
$current_fin_year = (arg(2) && arg(2) > 0) ? arg(2) : 0;

$key_fin_year = 2;
$key_district = 3;
$arg_fin_year = (arg($key_fin_year)) ? arg($key_fin_year) : '2012-2013';
$arg_district = (arg($key_district)) ? arg($key_district) : 0;

extract($headings, EXTR_PREFIX_ALL, 'H');
?>
<?php print $index;?>
<table border="1" cellpadding="1" cellspacing="0">
<tr>
<th align="center">#</th>
<th align="center">PARTICULARS OF FULL PROJECT PROPOSAL</th>
<th align="center">No of APMC</th>
<th align="center">25%</th>
<th align="center">50%</th>
<th align="center">75%</th>
<th align="center">100%</th>
<th align="center">Proposed Cost</th>
<th align="center">Payment Done</th>
<th align="center">Phy</th>
<th align="center">Fin</th>
</tr>

<tr>
<td colspan="7">
</td>
<td align="center">Financial Year</td>
<td align="center">
<select name="fin-years-RH-dd" id="fin-years-RH-dd" class="reports-filter">
<option value="">--Please Select--</option>
<?php
  foreach ($fin_years as $fin_year) {
    if (strlen($fin_year['Financial_Year_ID']) && strlen($fin_year['Financial_Year_Name'])) {
      extract($fin_year, EXTR_PREFIX_ALL, 'F');
      $selected_dist = ($F_Financial_Year_Name == $current_fin_year) ? 'selected="selected"' : '';
      $fin_url = _prepare_report_filters($key_fin_year, $F_Financial_Year_Name);
    ?>
    <option value="<?php print $fin_url;?>" <?php print $selected_dist;?>><?php print $F_Financial_Year_Name;?></option>
    <?php
    }
  }
?>
</select>
</td>

<td align="center">District</td>
<td align="center">
<select name="districts-RH-dd" id="districts-RH-dd" class="reports-filter">
<option value="state">--Please Select--</option>
<?php
  foreach ($districts as $district) {
    if (strlen($district['Dist_ID']) && strlen($district['Dist_Name'])) {
      extract($district, EXTR_PREFIX_ALL, 'D');
      $selected_dist = ($D_Dist_ID == $current_dist) ? 'selected="selected"' : '';
      $dist_url = _prepare_report_filters($key_district, $D_Dist_ID);
    ?>
    <option value="<?php print $dist_url;?>" <?php print $selected_dist;?>><?php print $D_Dist_Name;?></option>
    <?php
    }
  }
?>
</select>
</td>
</tr>

<?php
if (!empty($records['records'])) {
  $i=0;
  foreach ($records['records'] as $record) {
    ++$i;
     extract($record, EXTR_PREFIX_ALL, 'R');
?>
<tr>
<td align="right"><?php print $i;?></td>
<td align="left"><?php print $R_Work_Progress_Stage;?></td>
<td align="right"><?php print $R_total_inst;?></td>
<td align="right"><?php print $R_Per_25;?></td>
<td align="right"><?php print $R_Per_50;?></td>
<td align="right"><?php print $R_Per_75;?></td>
<td align="right"><?php print $R_Per_100;?></td>
<td align="right"><?php print $R_Total_Proposed_Cost;?></td>
<td align="right"><?php print $R_Total_Payment_Done;?></td>
<td align="right"><?php print $R_Phy;?>%</td>
<td align="right"><?php print $R_Fin;?>%</td>
</tr>
<?php
  }
?>
<tr>
<td align="right" colspan="2">Progress</td>
<td align="right"><?php print $records['Totals']['total_cnt_Centers'];?></td>
<td align="right"><?php print $records['Totals']['total_cnt_per_25'];?></td>
<td align="right"><?php print $records['Totals']['total_cnt_per_50'];?></td>
<td align="right"><?php print $records['Totals']['total_cnt_per_75'];?></td>
<td align="right"><?php print $records['Totals']['total_cnt_per_100'];?></td>
<td align="right"><?php print $records['Totals']['total_cnt_Total_Proposed_Cost'];?></td>
<td align="right"><?php print $records['Totals']['total_cnt_Total_Payment_Done'];?></td>
<td align="right"><?php print $records['Totals']['total_cnt_Phy'];?>%</td>
<td align="right"><?php print $records['Totals']['total_cnt_Fin'];?>%</td>
</tr>
<?php
}
else {
?>
<tr>
<td colspan="11" align="center">No records found.</td>
</tr>
<?php
}
?>
</table>
<h4 align="right">*Figures are in Lk.</h4>