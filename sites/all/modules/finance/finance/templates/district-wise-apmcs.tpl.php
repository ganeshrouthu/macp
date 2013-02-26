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
<th align="center">District Name</th>
<th align="center">Total APMC</th>
<th align="center">In-house</th>
<th align="center">SP</th>
<th align="center">UC Stage</th>
<th align="center">Three tranches</th>
<th align="center" colspan="4">First or Second tranche</th>
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
<td align="left"><?php print $R_Dist_Name;?></td>
<td align="right"><?php print $R_Num_Centers;?></td>
<td align="right"><?php print '';?></td>
<td align="right"><?php print '';?></td>
<td align="right"><?php print '';?></td>
<td align="right"><?php print '';?></td>
<td align="right" colspan="4"><?php print '';?></td>
</tr>
<?php
  }
  ?>
<tr>
<td align="right" colspan="2">Total</td>
<td align="right"><?php print $records['Totals']['total_cnt_Centers'];?>
<td align="right"><?php print '';?></td>
<td align="right"><?php print '';?></td>
<td align="right"><?php print '';?></td>
<td align="right"><?php print '';?></td>
<td align="right" colspan="4"><?php print '';?></td>
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