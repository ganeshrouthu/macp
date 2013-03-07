<table align="left" border="0" cellspacing="0" cellpadding="0" width="100%" class="custom-nodes">	
	<tr align="left" valign="top">
		<td><?php print $node->label_title;?></td>
		<td colspan="3"><?php print $node->title?></td>
	</tr>
	<tr align="left" valign="top">
		<td><?php print $node->label_type;?></td>
		<td colspan="3"><?php print $node->node_type?></td>
	</tr>
	<tr align="left" valign="top">
		<td><?php print $node->label_locations;?></td>
		<td colspan="3"><?php print $node->node_locations?></td>
	</tr>
	<tr align="left" valign="top">
		<td><?php print $node->label_body;?></td>
		<td colspan="3"><?php print $node->node_body?></td>
	</tr>
	
	<tr align="left" valign="top">
		<td>Documents Attached</td>
		<td colspan="3"><?php print $node->node_documents?></td>
	</tr>
	<tr align="left" valign="top">
		<td><?php print $node->label_images?></td>
		<td><?php print $node->node_images?></td>
		<td><?php print $node->label_videos?></td>
		<td><?php print $node->node_videos?></td>
	</tr>
</table>
