jQuery(function() {
	jQuery("#quicktabs-test ul").css({
																background:'none !important'
																})


  jQuery("#reports-dd").change(function () {
    if (jQuery(this).val()) {
      window.location = jQuery(this).val();
    }
  });
	jQuery("#edit-field-report-url").blur(function () {
		if (jQuery(this).val() != '') {
			jQuery("#edit-field-upload-doc-ajax-wrapper").hide();
		}
		else {
			jQuery("#edit-field-upload-doc-ajax-wrapper").show();
		}
	});
  jQuery(".reports-filter").change(function () {
    if (jQuery(this).val()) {
      window.location = jQuery(this).val();
    }
  });
});
