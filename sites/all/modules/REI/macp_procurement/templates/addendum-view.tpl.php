<?php global $base_url;?>
<table align="left" border="0" cellspacing="0" cellpadding="0" width="auto" class="macp-table view-addendum">
<tr>
<th colspan="2">Details for Addendum: <?php print $records->atitle;?></th>
</tr>
<tr align="left" valign="top">
		<td class="td-label"><strong>Title: </strong></td>
		<td class="td-data"><?php print $records->atitle;?></td>
	</tr>
	<tr align="left" valign="top">
		<td><strong>Teder Title</strong></td>
		<td><?php print $tender->title;?></td>
	</tr>
	<tr align="left" valign="top">
		<td><strong>Brief Description: </strong></td>
		<td><?php print $records->adesc;?></td>
	</tr>
	<tr align="left" valign="top">
		<td><strong>Uploaded Documents: </strong></td>
<td>
<a href ="<?php print file_create_url($file->uri)?>"><?php print $file->filename;?></a>
</td>
	</tr>
	</table>