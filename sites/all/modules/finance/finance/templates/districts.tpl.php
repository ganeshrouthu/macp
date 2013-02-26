<?php
//_p($records['Totals'], 0);
//_p($records['records'], 1);
?>

<?php
extract($headings, EXTR_PREFIX_ALL, 'H');
global $base_url;
?>
<?php print $index;?>
<table border="1" cellpadding="1" cellspacing="0">
<tr>
<th align="center">#</th>
<th align="right">District ID</th>
<th align="right">Phase</th>
<th align="center">District Name</th>
<th align="center">RH Work Progress</th>
</tr>
<?php
if (!empty($records)) {
  $i=0;
  foreach ($records as $record) {
    ++$i;
     extract($record, EXTR_PREFIX_ALL, 'R');
     $rh_cwp_url = $base_url . '/yearly-report/RH-CWP/2012-2013/'.$R_Dist_ID;
?>
<tr>
<td align="right"><?php print $i;?></td>
<td align="right"><?php print $R_Dist_ID;?></td>
<td align="right"><?php print $R_Dist_PhaseNo;?></td>
<td align="center"><?php print $R_Dist_Name;?></td>
<td align="center">
<a href="<?php print $rh_cwp_url?>" target="_blank">RH Work Progess</a></td>
</tr>
<?php
  }
}
else {
?>
<tr>
<td colspan="4" align="center">No records found.</td>
</tr>
<?php
}
?>
</table>
<h4 align="right">*Figures are in Lk.</h4>
