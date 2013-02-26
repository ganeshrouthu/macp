<?php
if (!defined('LANG')) {
  define('LANG', 'und');
}
global $base_url;
?>

<?php
print $node->body[LANG][0]['safe_value'];
?>

<?php
if (!empty($node->field_reports[LANG])) {
?>
<ol>
<?php
  foreach ($node->field_reports[LANG] as $report) {
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
  print 'No financial report available.';
}
?>