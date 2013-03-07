<?php
@session_start();
global $base_url;
?>
<table align="left" border="0" cellspacing="0" cellpadding="0" width="auto" class="macp-table view-tender tender-table">
<tr>
<th colspan="2">Details for Tender: <?php print $records->title;?></th>
</tr>
<tr align="left" valign="top">
		<td class="td-label"><strong>Title: </strong></td>
		<td class="td=data"><?php print $records->title;?></td>
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
		<td><strong>Uploaded Documents: </strong></td>
		<td><?php print $records->tender_files;?></td>
	</tr>
<?php
if (_is_content_admin_logged_in()) {
?>
	<tr align="left" valign="top">
		<td><strong>Downloaded By: </strong></td>
<td>
<a href="<?php print $base_url;?>/procurement/tender/downloaded_user/<?php print $records->tid.'/' . $records->title;?>">Agencies</a>
</td>
	</tr>
<?php
}
?>
</table>