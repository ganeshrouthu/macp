<?php
extract($headings, EXTR_PREFIX_ALL, 'H');
?>
<?php print $index;?>

<?php
global $base_url;
?>
<div class="mis-clear-all">
<table border="1" cellpadding="1" cellspacing="0" width="100%" align="center" class="macp-table">
<thead>
<tr>
<th rowspan="2" align="center">#</th>
<th rowspan="2" align="center">DDRCS</th>
<th rowspan="2" align="center">Phase</th>
<th rowspan="2" align="center">Fin. Year</th>
<th colspan="2" align="center">Proj. Funds</th>
<th rowspan="2" align="center">In %</th>
<th colspan="2" align="center">Benf. Funds</th>
<th rowspan="2" align="center">In %</th>
<th colspan="2" align="center">Overall</th>
</tr>
<tr>
<th>Funds Relsd.</th>
<th>Expen.</th>
<th>Collec.</th>
<th>Expendt.</th>
<th>Proj.+Benef.</th>
<th>Expendt.</th>
</tr>
</thead>
<tbody>
<?php
if (!empty($records)) {
  $i=0;
  foreach ($records as $record) {
    ++$i;
     extract($record, EXTR_PREFIX_ALL, 'R');
?>
<?php
$R_in_per_one = ($R_in_per_one > 100) ? $R_in_per_one - 100 : $R_in_per_one;
$R_in_per_two = ($R_in_per_two > 100) ? $R_in_per_two - 100 : $R_in_per_two;
?>
<tr>
<td><?php echo $i;?></td>
<td><?php echo $R_DDRCS_Name;?></td>
<td><?php echo $R_Phase;?></td>
<td><?php echo $R_Financial_Year;?></td>
<td><?php echo $R_Project_Funds_Released;?></td>
<td><?php echo $R_Project_Funds_Expenditure;?></td>
<td><?php echo $R_in_per_one;?>%</td>
<td><?php echo $R_Beneficiary_Funds_Collection;?></td>
<td><?php echo $R_Beneficiary_Funds_Expenditure;?></td>
<td><?php echo $R_in_per_two;?>%</td>
<td><?php echo $R_overall_proj_plus_ben;?></td>
<td><?php echo $R_overall_expenditure;?></td>
</tr>
<?php
  }
}
else {
?>
<tr>
<td colspan="12" align="center">No records found.</td>
</tr>
<?php
}
?>
</tbody>
</table>
</div>