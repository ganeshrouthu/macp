<?php
extract($headings, EXTR_PREFIX_ALL, 'H');
?>
<?php print $index;?>
<!--<h1 align="center"><?php print $H_agency_type;?> Report For FY <?php print $H_financial_year;?></h1>-->
<table border="1" cellpadding="1" cellspacing="0">
<tr>
<th rowspan="2" align="center">#</th>
<th rowspan="2" align="center"> District Dy. Registar Co-Operative Soc. (DDRC)</th>
<th rowspan="2" align="center">Phase</th>
<th rowspan="2" align="center">Financial Year</th>
<th colspan="2" align="center">Project Funds</th>
<th rowspan="2" align="center">In %</th>
<th colspan="2" align="center">Beneficiary Funds</th>
<th rowspan="2" align="center">In %</th>
<th colspan="2" align="center">Overall</th>
</tr>

<tr>
<th>Funds Released</th>
<th>Expenditure</th>
<th>Collection</th>
<th>Expenditure</th>
<th>Project+Beneficiary</th>
<th>Expenditure</th>
</tr>
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
<td colspan="11" align="center">No records found.</td>
</tr>
<?php
}
?>
</table>
<h4 align="right">*Figures are in Lk.</h4>