<?php global $base_url;?>
<table align="left" border="0" cellspacing="0" cellpadding="0" width="auto" class="macp-table tender-table">
<tr>
<th colspan="2">Details for Addendum: <?php print $records->atitle;?></th>
</tr>
<tr align="left" valign="top">
		<td class="td-label"><strong>Title: </strong></td>
		<td class="td-data"><?php print $records->atitle;?></td>
	</tr>
	<tr align="left" valign="top">
		<td><strong>Tender Title</strong></td>
		<td><?php print $tender->title;?></td>
	</tr>
	<tr align="left" valign="top">
		<td><strong>Brief Description: </strong></td>
		<td><?php print $records->adesc;?></td>
	</tr>
	<tr align="left" valign="top">
		<td><strong>Uploaded Documents: </strong></td>
<td>

<?php
if (isset($_SESSION['tender_user']) || FALSE != variable_get(session_id(), FALSE)) {
	$download_link = $base_url . '/procurement/download/tender/'.$tender->tid.'/'.$records->afid;
}
else {
	$download_link = $base_url . '/procurement/register/download-tender/'.$tender->tid.'/'.$records->afid;
}
?>
<a href="<?php print $download_link;?>" target="_blank"><?php print $file->filename;?></a>
</td>
	</tr>
	</table>