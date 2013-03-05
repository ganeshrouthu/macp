jQuery(document).ready(function () {
	jQuery(".messages").click(function () {
		jQuery(this).hide();
	});
  jQuery(".delete-add").click(function() {
    return confirm('Are you sure you want to delete?')
  });
	//procurement-add-tender
  //edit-upload-upload-button
	//edit-upload-2-ajax-wrapper
/*	
	jQuery("#procurement-add-tender #edit-upload-upload-button").click(function() {
		jQuery("#edit-upload-2-ajax-wrapper").show();
	});
	
	jQuery("#upload-another").click(function() {
		jQuery("#procurement-add-tender #edit-upload-2-ajax-wrapper").css({display:'block !important'});
		return false;
	});
*/
  jQuery(".confirm-delete").click(function() {
    return confirm('Are you sure you want to delete?')
  });
  
  jQuery("#user-login-download-tender").validate({
    rules: {
      email_id: {
        required: true,
        email: true
      },
      pass: {
        required: true,
        minlength: 5
      },
    },
    messages: {
      pass: {
        required: "Please provide a password",
        minlength: "Your password must be at least 5 characters long"
      },
			email_id: {
        required: "Please enter a valid email address"
      }
		},
		  errorPlacement: function (error, element) {
				error.insertAfter(element);
			},
			errorElement:'span'
  });	
  
  jQuery("#user-register-download-tender").validate({
    rules: {
      email_id: {
        required: true,
        email: true
      },
      new_pass: {
        required: true,
        minlength: 5
      },
      confirm_passord: {
        required: true,
        equalTo:"#edit-new-pass",
        minlength: 5
      },
      agency_name: {
        required: true,
        minlength: 5,
				maxlength: 10
      },
      person_name: { 
        required: true,
        minlength: 5,
        maxlength: 10
      },
      cont_number: {
				required: false,
        minlength: 5
      },
      mobile_number: {
        required: true,
        minlength: 10,
				maxlength: 11
      },
    },
		invalidHandler: function(form, validator) {
			var errors = validator.numberOfInvalids();
			if (errors) {
				jQuery("#errorContainer").css('color', 'red');
				var message	=	'Highlighted fields are mandatory.';
				var email_id = jQuery.trim(jQuery("#edit-email-id").val());
				var new_pass = jQuery.trim(jQuery("#edit-new-pass").val());
				var confirm_passord = jQuery.trim(jQuery("#edit-confirm-passord").val());
				var agency_name = jQuery.trim(jQuery("#edit-agency-name").val());
				var person_name = jQuery.trim(jQuery("#edit-person-name").val());
				var cont_number = jQuery.trim(jQuery("#edit-cont-number").val());
				var mobile_number = jQuery.trim(jQuery("#edit-mobile-number").val());

				if (!valid_email(email_id)) {
					message = 'Please enter valid Email ID.';
				}
				else if (new_pass.length < 5) {
					message = 'Password should be at least 5 characters long.';
				}
				else if (confirm_passord.length < 5) {
					message = 'Confirm Password should be at least 5 characters long.';
				}
				else if (new_pass != confirm_passord) {
					message = 'Password and Confirm Password should be same.';
				}
				else if (agency_name.length > 0 && agency_name.length < 5) {
					message = 'Agency Name should be at least 5 characters long.';
				}
				else if (person_name.length > 0 && person_name.length < 5) {
					message = 'Person Name should be at least 5 characters long.';
				}
				else if (cont_number.length > 0 && cont_number.length < 5) {
					message = 'Contact Number should be at least 5 digits long.';
				}
				else if (mobile_number.length > 0 && mobile_number.length < 10) {
					message = 'Mobile Number should be at least 10 digits long.';
				}
				else if (mobile_number.length > 0 && mobile_number.length > 10) {
					message = 'Agency Name should not be more than 10 characters long.';
				}
				jQuery("#errorContainer").html(message);
			}
		},
		errorPlacement: function(error, element) {
			return true;
		},
		
    messages: {
    	email_id: "Please enter a valid email address",
      pass: {
        required: "Please provide a password",
        minlength: "Your password must be at least 5 characters long"
      },
      agency_name: {
      	required: "Agency name is required",
        minlength: "Agency name must be at least 5 cheractors",
      },
      person_name: {
      	required: "Person name is required",
        minlength: "Person name must be at least 5 cheractors",
      },
      cont_number: {
      	required: "Contact number is required",
        minlength: "Contact number must be at least 5 cheractors",
        maxlength: "Please enter no more than 10 numbers.",
      },
      mobile_number: {
        required: "Mobile number is required",
        minlength: "Mobile number must be at least 5 cheractors",
				maxlength: "Please enter no more than 10 numbers.",
      }
    },
  });
  
  jQuery("#tuser-chnage-password").validate({
    rules: {
      old_pass: {
        required: true,
        minlength: 5
      },
      new_password: {
        required: true,
        minlength: 5
      },
      confirm_pass: {
        required: true,
        equalTo:"#edit-new-password",
        minlength: 5
      },
    },
    messages: {
      old_pass: {
        required: "Please provide a password",
        minlength: "Your password must be at least 5 characters long"
      },
      confirm_pass: {
        required: "Please provide a password",
        minlength: "Your password must be at least 5 characters long"
      },
      
    },
  });

	jQuery(".cancel-btn").click(function () {
		var url_to = jQuery(this).attr("alt");
		window.location = url_to;
		//alert(url_to);
	});
})

function valid_email(email) {
	var regex = /^([a-zA-Z0-9_\.\-\+])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})+$/;
	return regex.test(email);
}