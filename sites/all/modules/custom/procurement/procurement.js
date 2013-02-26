jQuery(document).ready(function () { 
	jQuery(".delete-add").click(function() {
    return confirm('Are you sure you want to delete?')
  });
  
  jQuery(".delete-link").click(function() {
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
      email_id: "Please enter a valid email address",
    },
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
      },
      person_name: { 
        required: true,
        minlength: 5,
      },
      cont_number: {
        required: true,
        minlength: 10,
        maxlength: 10,
      },
      mobile_number: {
        required: true,
        minlength: 10,
        maxlength: 10,
      },
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
      },
      mobile_number: {
        required: "Mobile number is required",
        minlength: "Mobile number must be at least 5 cheractors",
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
  
})
