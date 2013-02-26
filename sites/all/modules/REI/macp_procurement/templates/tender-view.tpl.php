<table align="left" border="0" cellspacing="0" cellpadding="0" width="auto" class="macp-table">
	<tr align="left" valign="top">
		<td><strong>Tender Title: </strong></td>
		<td><?php print $records->title;?></td>
	</tr>
	<tr align="left" valign="top">
		<td><strong>Tender Group</strong></td>
		<td><?php print $records->group;?></td>
	</tr>
	<tr align="left" valign="top">
		<td><strong>Brief Description: </strong></td>
		<td><?php print $records->tdesc;?></td>
	</tr>
	<tr align="left" valign="top">
		<td><strong>Tender Files: </strong></td>
<td><?php print $records->addendum_files;?></td>
	</tr>
	<tr align="left" valign="top">
		<td><strong>Created Date: </strong></td>
		<td><?php print $records->created_date;?></td>
	</tr>
	<tr align="left" valign="top">
		<td><strong>Updated: </strong></td>
		<td><?php print $records->updated_date;?></td>
	</tr>
	
</table>