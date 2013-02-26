jQuery(function() {
  jQuery("#reports-dd").change(function () {
    if (jQuery(this).val()) {
      window.location = jQuery(this).val();
    }
  });

  jQuery(".reports-filter").change(function () {
    if (jQuery(this).val()) {
      window.location = jQuery(this).val();
    }
  });
});
