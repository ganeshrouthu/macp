<?php
extract($headings, EXTR_PREFIX_ALL, 'H');
?>
<?php print $index;?>
<table border="1" cellpadding="1" cellspacing="0" class="macp-table">
<tr>
<th>#</th>
<th>Dy. Comm. AHD</th>
<th>Phase</th>
<th>Funds Released</th>
<th>Expdit. by Mar-2012</th>
<th>Exp. Year Till Dt.</th>
<th>Expdit.</th>
<th>In %</th>
</tr>
<?php
if (!empty($records)) {
  $i=0;
  foreach ($records as $record) {
    ++$i;
     extract($record, EXTR_PREFIX_ALL, 'R');
?>
<tr>
<td><?php echo $i;?></td>
<td><?php echo $R_Accounting_Center;?></td>
<td><?php echo $R_Phase;?></td>
<td><?php echo $R_Funds_Released;?></td>
<td><?php echo $R_Expdit_By_Last_Fin_Year;?></td>
<td><?php echo $R_Expdit_Year_Till_Date;?></td>
<td><?php echo $R_Expenditure;?></td>
<td><?php echo $R_in_per;?>%</td>
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