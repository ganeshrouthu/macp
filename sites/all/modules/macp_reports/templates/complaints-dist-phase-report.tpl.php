<?php
extract($headings, EXTR_PREFIX_ALL, 'H');
?>
<?php print $index;?>
<table border="1" cellpadding="1" cellspacing="0" class="macp-table">
<tr>
<th align="center">#</th>
<th align="center">Dist Phase No</th>
<th align="center">District Name</th>
<th align="center">Total Complaints</th>
<th align="center">New Complaints</th>
<th align="center">Interim Reply Sent</th>
<th align="center">Forwarded</th>
<th align="center">Resolved</th>
</tr>
<?php
if (!empty($records)) {
  $i=0;
  foreach ($records as $record) {
    ++$i;
     extract($record, EXTR_PREFIX_ALL, 'R');
?>
<tr>
<td align="center"><?php print $i;?></td>
<td align="center"><?php print $R_Dist_Phase;?></td>
<td align="center"><?php print $R_District;?></td>
<td align="center"><?php print $R_Total_Complaints;?></td>
<td align="center"><?php print $R_Count_New_Complaints;?></td>
<td align="center"><?php print $R_Count_Interim_Reply_Sent;?></td>
<td align="center"><?php print $R_Count_Forwarded;?></td>
<td align="center"><?php print $R_Count_Resolved;?></td>
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