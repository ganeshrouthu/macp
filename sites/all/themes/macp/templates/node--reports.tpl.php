<?php
if (!defined('LANG')) {
  define('LANG', 'und');
}
global $base_url;
?>

<?php
$report_groups = _get_mis_report_groups();
$mis_reports = array();
if (!empty($report_groups)) {
	foreach ($report_groups as $group_id => $report_group) {
		$nids = taxonomy_select_nodes($group_id);
		$mis_reports[$report_group] = node_load_multiple($nids);
	}
}
?>

<?php
if (!empty($mis_reports)) {
?>
<ul>
<?php
	foreach ($mis_reports as $report_group => $mis_report_nodes) {
		?>
		<li><?php print $report_group;?></li>
		
<?php
if (!empty($mis_report_nodes)) {
	foreach ($mis_report_nodes as $mis_report_node) {
?>
		
	<?php
if (!empty($mis_report_node->field_reports[LANG])) {
?>
<ol>
<?php
  foreach ($mis_report_node->field_reports[LANG] as $report) {
    if (strlen($report['first']) && strlen($report['second'])) {
    ?>
    <li><a href="<?php print $base_url .'/'.$report['second'];?>" target="_blank"><?php print $report['first'];?></a></li>
    <?php
    }
  }
?>
</ol>
<?php  
}
else {
  print '<ul><li>No ' . $report_group . ' report available.</li></ul>';
}
	}
}
}
?>
</ul>
<?php
}
else {
?>
<div>No MIS Report Available</div>
<?php
}
?>
