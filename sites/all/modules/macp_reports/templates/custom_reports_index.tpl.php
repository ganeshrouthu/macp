<?php
global $base_url;
$report_groups = _get_mis_report_groups();
$mis_reports = array();
if (!empty($report_groups)) {
	foreach ($report_groups as $group_id => $report_group) {
		$nids = taxonomy_select_nodes($group_id);
		$nodes = node_load_multiple($nids, array('type'=> 'custom_reports'));
		if (!empty($nodes) && count($nodes)) {
			$mis_reports[$report_group] = $nodes;
		}
	}
}
?>
<div id="macp-custom-reports">
<ol class="groups">
<?php
if (!empty($mis_reports)) {
?>
<?php
	foreach ($mis_reports as $report_group => $mis_report_nodes) {
?>
		<li><b><?php print $report_group;?></b>
		<ol class="records">
<?php
if (!empty($mis_report_nodes)) {
	foreach ($mis_report_nodes as $mis_report_node) {
		$title = (isset($mis_report_node->title)) ? $mis_report_node->title : '';
		$field_reports_group = (isset($mis_report_node->field_reports_group[LANGUAGE_NONE][0]['tid'])) ? $mis_report_node->field_reports_group[LANGUAGE_NONE][0]['tid'] : '';
		$field_upload_doc = (isset($mis_report_node->field_upload_doc[LANGUAGE_NONE][0]['uri'])) ? $mis_report_node->field_upload_doc[LANGUAGE_NONE][0]['uri'] : '';
		$field_report_url = (isset($mis_report_node->field_report_url[LANGUAGE_NONE][0]['value'])) ? $mis_report_node->field_report_url[LANGUAGE_NONE][0]['value'] : '';
		$field_upload = (isset($mis_report_node->field_upload[LANGUAGE_NONE][0]['value'])) ? $mis_report_node->field_upload[LANGUAGE_NONE][0]['value'] : '';
		$file_download = '';
		if ($field_upload) {
			$file = file_load($field_upload);
			$file_download = file_create_url($file->uri);
		}
?>
<?php
$report_link = (strlen($field_report_url)) ? $field_report_url : $file_download;
?>
<li><a href="<?php print $report_link;?>" target="_blank"><?php print $title;?></a></li>
<?php  
	}
}
else {
?>
<li>No <?php print $report_group;?> Available.</li>
<?php
}
?>
</ol>
</li>
<?php
}
?>
<?php
}
else {
?>
<li>No report group available.</li>
<?php
}
?>
</ol>
</div>