<?php
extract($headings, EXTR_PREFIX_ALL, 'H');
?>
<?php print $index;?>
<table border="1" cellpadding="1" cellspacing="0" class="macp-table">
<tr>
<th align="center">#</th>
<th align="center">ATMA - District Level</th>
<th align="center">Phase</th>
<th align="center">Funds Released</th>
<th align="center">Expenditure by Mar-<?php print substr($H_financial_year, 0, 4);?>
</th>
<th align="center">Expenditure Year Till Date</th>
<th align="center">Expenditure</th>
<th align="center">In %</th>
</tr>

<?php
$records = array();
$temp = array(
'Accounting_Centers_Name' => 'ATMA - Ahmadnagar',
'phase' => '1',
'Funds_Released' => '170.28',
'Expenditure_Year_Till_Date' => '50.45',
'Expenditure' => '114.32',
'Last_Financial_Year_Expenditure' => '63.87',
'in_per' => '67%');
$records[] = $temp;
?>


<?php
if (!empty($records)) {
  $i=0;
  foreach ($records as $record) {
    ++$i;
     extract($record, EXTR_PREFIX_ALL, 'R');
?>
<tr>
<td align="center"><?php print $i;?></td>
<td align="center"><?php print $R_Accounting_Centers_Name;?></td>
<td align="center"><?php print $R_phase;?></td>
<td align="center"><?php print $R_Funds_Released;?></td>
<td align="center"><?php print $R_Expenditure_Year_Till_Date;?></td>
<td align="center"><?php print $R_Expenditure;?></td>
<td align="center"><?php print $R_Last_Financial_Year_Expenditure;?></td>
<td align="center"><?php print $R_in_per;?>%</td>
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