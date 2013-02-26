<?php
global $base_url;
$current_page = $base_url . '/' . implode('/', arg());
if (empty($records)) {
  print 'No financial report available.';
}
?>

</div>
<br clear="all"/>
<?php
if (!empty($records)) {
?>
<table border="1" cellpadding="1" cellspacing="0">
  <tr>
  <th><a href="<?php print $base_url?>/finance-reports">Reports Listing</a></th>
  </tr>
</table>
<?php
}
else {
?>
  No financial report available
<?php
}
?>

<?php
require_once 'filters.php';
?>