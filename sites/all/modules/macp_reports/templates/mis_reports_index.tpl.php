<?php
global $base_url;
$report_groups = _get_mis_report_groups();
$mis_reports = array();
if (!empty($report_groups)) {
	foreach ($report_groups as $group_id => $report_group) {
		$nids = taxonomy_select_nodes($group_id);
		$nodes = node_load_multiple($nids, array('type'=> 'reports'));
		if (!empty($nodes) && count($nodes)) {
			$mis_reports[$report_group] = $nodes;
		}
	}
}
?>
<div id="macp-mis-reports">
<ol class="groups">
<?php
if (!empty($mis_reports)) {
	foreach ($mis_reports as $report_group => $mis_report_nodes) {
		?>
		<li><b><?php print $report_group;?></b>
		<ol class="records">
<?php
if (!empty($mis_report_nodes)) {
	foreach ($mis_report_nodes as $mis_report_node) {
if (!empty($mis_report_node->field_reports[LANGUAGE_NONE])) {
?>
<?php
  foreach ($mis_report_node->field_reports[LANGUAGE_NONE] as $report) {
    if (strlen($report['first']) && strlen($report['second'])) {
    ?>
<li><a href="<?php print $base_url .'/'.$report['second'];?>" target="_blank"><?php print $report['first'];?></a></li>
    <?php
    }
  }
?>
<?php  
}
else {
?>
<tr><td align="center">No <?php print $report_group;?> available.</td></tr>
<?php
}
	}
}
?>
</li>
</ol>
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