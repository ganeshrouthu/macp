<?php
/**
 * This Form for add tender
 */
function procurement_add_tender($form, $form_state, $tender = NULL, $addendum = NULL) {
  global $base_url;
	$form['title'] = array(
    '#type' => 'textfield', 
    '#title' => t('Title of the tender/EOI'),
    '#size' => 60,
    '#maxlength' => 128, 
    '#default_value' => empty($tender->title) ? '' : $tender->title, 
    '#required' => TRUE,
  );
	$form['tid'] = array(
    '#type' => 'hidden', 
    '#title' => t('tender id'),
    '#size' => 60, 
    '#maxlength' => 128, 
    '#default_value' => empty($tender->tid) ? '' : $tender->tid, 
  );
	$form['title'] = array(
    '#type' => 'textfield', 
    '#title' => t('Title of the tender/EOI'),
    '#size' => 60,
    '#maxlength' => 128, 
    '#default_value' => empty($tender->title) ? '' : $tender->title, 
    '#required' => TRUE,
  );
  
  $form['group_id'] = array(
    '#type' => 'select', 
    '#title' => t('Tender Group'),
    '#description' => 'Please select a Tender Group',
    '#options' => _tender_groups(),
    '#default_value' => empty($tender->group_id) ? '' : $tender->group_id, 
		'#required' => TRUE,
  );

  $form['date'] = array(
    '#type' => 'date_popup',
    '#title' => 'Last date of tender/EOI',
    '#default_value' => empty($tender->created) ? '' : date("Y-m-d H:i:s", $tender->created),
    '#date_type' => DATE_DATETIME,
    '#date_timezone' => date_default_timezone(),
    '#date_format' => 'm/d/Y',
    '#date_increment' => 1,
    '#date_year_range' => '0:+3',
    '#required' => TRUE,
    '#min_date' => 'today',
		'#attributes' => array('readonly' => 'true'),
  );
  
  $form['desc'] = array(
  	'#title' => t('Brief of the tender/EOI'),
    '#type' => 'textarea',
    '#default_value' => empty($tender->tdesc) ? '' : $tender->tdesc, 
    '#cols' => 60, 
    '#rows' => 5,
  );
  
  $form['upload'] = array(
    '#type' => 'managed_file',
    '#title' => t('Upload tender/EOI'),
    '#upload_location' => 'public://procurement/',
    '#required' => TRUE,
		'#description' => 'Please upload files with these extensions only: txt pdf PDF doc docx ppt pptx xls xlsx<br/>',
    '#default_value' => empty($tender) ? '' :$tender->tfid,
		"#upload_validators" => array(
														"file_validate_extensions" => array("txt pdf PDF doc docx ppt pptx xls xlsx"),
														'file_validate_size' => array(8 * 1048576),
														),
 	);
 	
 	$form['submit'] = array(
 	  '#type' => 'submit', 
 	  //'#value' => empty($tender) ? t('Add Tender') :t('Edit Tender')
 	  '#value' => t('Save')
 	 );

	 $form['cancel'] = array(
 	  '#type' => 'link',
 	  '#href' => $base_url . '/procurement/manage-tenders',
 	  '#title' => t('Cancel')
 	);
  return $form;
}

/*
 * hook_form_submit for add tender form
 */
function procurement_add_tender_submit($form, &$form_state) {
  $created_date = strtotime($form_state['values']['date']);
  $date = date('m/d/Y h:i:s', time());
  $updated_date = strtotime($date);
  if(!empty($form_state['values']['tid'])) {
    db_update('tender') // Table name no longer needs {}
  		->fields(array(
   		   'title' => $form_state['values']['title'],
   		   'tdesc' => $form_state['values']['desc'],
   		   'tfid'  => $form_state['values']['upload'],
   		   'group_id'  => $form_state['values']['group_id'],
   		   'created' => $created_date,
    	   'changed' => $updated_date,
  	  ))
     ->condition('tid', $form_state['values']['tid'], '=')
     ->execute();
    
    //Start chnage by Nitin 1 Feb 2013
    
    $file = file_load($form_state['values']['upload']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    file_usage_add($file, 'procurement', $file->filename, 1);
    //End chnage by Nitin 1 Feb 2013
     
    $form_state['redirect'] = 'procurement/tenders';
  }
	else {
  	$result = db_query("INSERT INTO {tender} 
									(title, group_id, tdesc, tfid, created, changed) 
									VALUES (
									'".$form_state['values']['title']."', 
									'".$form_state['values']['group_id']."', 
									'".$form_state['values']['desc']."', 
									".$form_state['values']['upload'].", 
									".$created_date.", 
									".$created_date.")");
  	
		//Start chnage by Nitin 1 Feb 2013	
    $file = file_load($form_state['values']['upload']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    file_usage_add($file, 'procurement', $file->filename, 1);
    //end chnage by Nitin 1 Feb 2013
  	$form_state['redirect'] = 'procurement/tenders';
  }
}

/*
 * Group Tenders by Tender Group.
 */
function _tenders_group_by_tender_group($tenders) {
	$arr = array();
	$tender_groups = _tender_groups();
	if (!empty($tenders)) {
		foreach($tenders as $tender) {
			$group_id = (isset($tender->group_id)) ? $tender->group_id : 0;
			$tender_group = (isset($tender_groups[$group_id])) ? $tender_groups[$group_id] : 0;
			$arr[$tender_group][] = $tender;
		}
	}
	return $arr;
}

/*
 * Show list of all tenders to content admin
 */
function procurement_tender_list() {
  global $user,$base_path;
  $output = '';
  $table_rows = array();
  $header = array(
    array('data' => t('Title of tenders'), 'field' => 't.title', 'sort' => 'DESC'),
    array('data' => t('Addendum')),
    array('data' => t('Submission date'), 'field' => 't.created', 'sort' => 'DESC'),
    array('data' => t('Tender Doc.')),
    array('data' => t('Action')), 
    array('data' => t('Tender Downloaded By')),
  );
  $limit = variable_get('codes_per_page',5);
  $query = db_select('tender', 't')
						->extend('PagerDefault')
						->limit($limit); 

  $current_date = DATE('m/d/Y');
  $current_date = strtotime($current_date);
  $query->fields('t')
        ->condition('created', $current_date, '>=')
        ->extend('TableSort')  // Add table sort extender.
        ->orderByHeader($header);  // Add order by headers.
  $result = $query->execute();

	// Get tenders by Tender Groups
	$tender_sets = _tenders_group_by_tender_group($result);
	if (!empty($tender_sets)) {
		foreach($tender_sets as $tender_group => $tender_set) {
			if (!empty($tender_set)) {
				$table_rows[] = array(
					array(
						'data' => 'Tenders/Advetisements ('.$tender_group.')',
						'colspan' => 7,
						'align' => 'center',
					),
				);
				foreach($tender_set as $tender) {
					$file = file_load($tender->tfid);
					$get = get_addendum_by_tid($tender->tid);
					$add = l(t('Add Addendum'),  'procurement/' . $tender->tid . '/add/addendum');
					$edit = l(t('Edit'),  'procurement/' . $tender->tid . '/edit');
					$delete = l(t('Delete'),  'tender/' . $tender->tid . '/delete', array('attributes' => array('class' => array('confirm-delete'))));
					$tender_title = l($tender->title,  'procurement/view-tender/' . $tender->tid);
					$table_rows[] = array(
						array('data' => $tender_title),
						array('data' => get_addendum_by_tid($tender->tid)),
						array('data' =>  format_date($tender->created, 'custom', 'j-m-Y')),
						array('data' => get_tender_files($tender->tid)),
						array('data' => $add . ' | ' . $edit . ' | ' . $delete ),
						array('data' => l(t('Users'), 'procurement/tender/downloaded_user/'.$tender->tid.'/' . $tender->title)),
					);
				}
			}
		}
	}
  $attributes = array('class' => array('macp-table'));
  $output .= '<br/>'.theme('table', 
                                      array(
                                        'header' => $header, 
                                        'rows' => $table_rows, 
                                        'attributes' => $attributes,
                                          )
                                        ). theme('pager');
  return $output;
}

/*
 * function for delete selected tender 
 * with there addendum and there file
 */
function procurement_tender_delete($tender = NULL) {
  if(!empty($tender)) {
    $result = db_select('addendum', 'a')
	    ->fields('a', array('aid', 'afid'))
	    ->condition('a.tid', $tender->tid, '=')
	    ->execute();
	  $tender_file = file_load($tender->tfid);
	  
	  if(!empty($tender_file)) {
	  	file_delete($tender_file);
	  }
	  foreach($result as $res) {
	    $file = file_load($res->afid);
	    if(!empty($file)) {
	      file_delete($file);
	    }
	    
	    db_delete('addendum')
      ->condition('aid', $res->aid)
      ->execute();
	  }
	  
    $num_deleted = db_delete('tender')
      ->condition('tid', $tender->tid)
      ->execute();
    drupal_goto('procurement/tenders');
  }
}

/*
 * Display list of tender to anonymous user
 */

function procurement_user_tender_list() {
  global $user,$base_path;
  $output = '';
  $table_rows = array();
  $header = array(
    array('data' => t('Title'), 'field' => 't.title', 'sort' => 'DESC'),
    array('data' => t('Addendum')),
    array('data' => t('Submission date'), 'field' => 't.created', 'sort' => 'DESC'),
    array('data' => t('Tender Doc.')),
  );
  $limit = variable_get('codes_per_page',5);
  $query = db_select('tender', 't')->extend('PagerDefault')->limit($limit); 
  //Date Condition added by santhosh for creating archives on 01Feb2013
  $current_date = DATE('m/d/Y');
  $current_timestamp = strtotime($current_date);
  $query->fields('t')
        ->condition('created', $current_timestamp, '>=')
        ->extend('TableSort')  // Add table sort extender.
        ->orderByHeader($header);  // Add order by headers.
  $result = $query->execute();

	// Get tenders by Tender Groups	
	$tender_sets = _tenders_group_by_tender_group($result);
	if (!empty($tender_sets)) {
		foreach($tender_sets as $tender_group => $tender_set) {
			if (!empty($tender_set)) {
				$table_rows[] = array(
					array(
						'data' => 'Tenders/Advetisements ('.$tender_group.')',
						'colspan' => 5,
						'align' => 'center',
					),
				);
				foreach($tender_set as $tender) {
					$file = file_load($tender->tfid);
					$get = get_addendum_by_tid($tender->tid);
					$tender_title = l($tender->title,  'procurement/view-tender/' . $tender->tid);
					$table_rows[] = array(
						array('data' => $tender_title),
						array('data' => get_user_addendum_by_tid($tender->tid)),
						array('data' =>  format_date($tender->created, 'custom', 'j-m-Y')),
						array('data' => get_user_addendum_file_tid($tender->tid)),
					);
				}
			}
		}
	}
  $attributes = array('class' => array('macp-table'));
  $output .= theme('table', 
                    array(
                      'header' => $header, 
                      'rows' => $table_rows,
                      'attributes' => $attributes,
                      )
                   ). theme('pager');
  return $output;
}

/*
 * Hook_form for user to login aand download the tender
 */
function user_login_download_tender($form, $form_state, $tid, $fid) {
	if(isset($_SESSION['tender_user'])) {
		$user_detail = db_select('tuser', 'tu')
	    ->fields('tu', array('uid'))
	    ->condition('tu.mail', $_SESSION['tender_user'], '=')
	    ->execute()->fetchAssoc();
		
		$file_download = db_select('tdownload', 'td')
    		->fields('td', array('did', 'uid'))
        ->condition('td.tid', arg(2), '=')
        ->condition('td.fid', arg(3), '=')
        ->execute();
    $count = $file_download->rowCount();
    if($count == 0) {
      $date = date('m/d/Y h:i:s', time());
  		$created_date = strtotime($date);
      
       	        		
      $tdown = db_insert('tdownload')
	    	->fields(array(
	    		  'uid' => $user_detail['uid'],
	    	    'tid' => arg(2),
	    	    'fid' => arg(3),
	    	    'created' => $created_date,
	    	))->execute();
		}
		  
		$file = file_load($fid);
		$file_download = drupal_realpath($file->uri);
		return download_file($file_download);
	}
  $form['fid'] = array(
    '#type' => 'hidden', 
    '#title' => t('fid'),
    '#size' => 60,
    '#maxlength' => 128,
    //'#required' => TRUE,
    '#default_value' => arg(3),
    '#attributes' => array('disabled' => 'true'),
  );
  
  $form['tid'] = array(
    '#type' => 'hidden', 
    '#title' => t('tid'),
    '#size' => 60,
    '#maxlength' => 128,
    //'#required' => TRUE,
    '#default_value' => arg(2),
    '#attributes' => array('disabled' => 'true'),
  );
  
  $form['email_id'] = array(
    '#type' => 'textfield', 
    '#title' => t("Email ID (User ID)"),
    '#size' => 60,
    '#prefix' => "<span style='color:red; float:right; position:absolute; left:142px;'>*</span>",
    '#maxlength' => 128,
    //'#required' => TRUE,
  );
  
  
  $form['pass'] = array(
    '#title' => t('Password'),
    '#type' => 'password',
    '#size' => 60,
    '#prefix' => "<span style='color:red; float:right; position:absolute; left:80px;'>*</span>",
    '#maxlength' => 15,
    //'#required' => TRUE,
  );
  
  $form['submit'] = array(
 	  '#type' => 'submit', 
 	  '#value' => t('Download Tender'),
 	);

 	$items = array();
  $items[] = l(t('Register new'), 'register/download/tender/' . arg(2) . '/' .arg(3), array('attributes' => array('title' => t('Create a new tender user account.'))));
  $items[] = l(t('Forgot password'), 'tuser/change_password', array('attributes' => array('title' => t('Request new password via e-mail.'))));
  $form['links'] = array('#markup' => theme('item_list', array('items' => $items)));
  
  return $form;
}

/*
 * Hook_submit for user login form for download tender
 */
function user_login_download_tender_submit($form, &$form_state) {
	@session_start();
	@drupal_session_start();
	global $user;
	if(!valid_email_address($form_state['values']['email_id'])) {
    form_set_error('email_id', 'Your email address appears malformed');
  }
	else {
		$pass =  md5(utf8_encode($form_state['values']['pass']));
		$user_detail = db_select('tuser', 'tu')
	    ->fields('tu', array('mail', 'pass', 'uid'))
	    ->condition('tu.mail', $form_state['values']['email_id'], '=')
	    ->condition('tu.pass', $pass, '=')
	    ->execute();
	  
	  $cnt = $user_detail->rowCount(); 
	    
	  if($cnt == 1) { 
	    if(!$user->uid) { 
	    	drupal_session_start();
	    	$_SESSION['tender_user'] = $form_state['values']['email_id'];
				variable_set(session_id(), $form_state['values']['email_id']);
      	$file = file_load($form_state['values']['fid']);
      	$file_download = drupal_realpath($file->uri); 
        if (file_exists($file_download)) {
          $file_download = db_select('tdownload', 'td')
        			->fields('td', array('did', 'uid'))
        			->condition('td.tid', $form_state['values']['tid'], '=')
        			->condition('td.fid', $form_state['values']['fid'], '=')
        			->execute();
        	$count = $file_download->rowCount();
        	if($count == 0) {
        		$date = date('m/d/Y h:i:s', time());
  					$created_date = strtotime($date);
        		$user_det = $user_detail->fetchAssoc();

        	  $tdown = db_insert('tdownload')
	    		    ->fields(array(
	    		      'uid' => $user_det['uid'],
	    		      'tid' => $form_state['values']['tid'],
	    		      'fid' => $form_state['values']['fid'],
	    		      'created' => $created_date,
	    		    ))->execute();
	    		  drupal_set_message('record added');
        	} 
          download_file($file_download);
        }
			}	
	  }
	  else {
	  	form_set_error('email_id', 'Your email or password is incorrect');
 		  drupal_set_message('Your email or password is incorrect', 'error');
	  }
    
	}
}

/*
 * function for download selected file 
 * after logn/register anonymous user
 */
function download_file($file = FALSE) {
  if (! $file || strlen($file) < 2) {
    return TRUE;
  }
  header('Content-Description: File Transfer');
  header('Content-Type: application/octet-stream');
  header('Content-Disposition: attachment; filename='.basename($file));
  header('Content-Transfer-Encoding: binary');
  header('Expires: 0');
  header('Cache-Control: must-revalidate');
  header('Pragma: public');
  header('Content-Length: ' . filesize($file));
  ob_clean();
  flush();
  readfile($file);
}


/*
 * Hook_form for user to regiser anonymous user and download the tender
 */
function user_register_download_tender($form, $form_state, $tid, $fid) {
  $form['fid'] = array(
    '#type' => 'hidden', 
    '#title' => t('fid'),
    '#size' => 60,
    '#maxlength' => 128,
    //'#required' => TRUE,
    '#default_value' => arg(4),
    '#attributes' => array('disabled' => 'true'),
  );
  
  $form['tid'] = array(
    '#type' => 'hidden', 
    '#title' => t('tid'),
    '#size' => 60,
    '#maxlength' => 128,
    //'#required' => TRUE,
    '#default_value' => arg(3),
    '#attributes' => array('disabled' => 'true'),
  );
  
  $form['email_id'] = array(
    '#type' => 'textfield', 
    '#title' => t('Email ID'),
    '#size' => 60,
    //'#prefix' => "<span style='color:red; float:right; position:absolute; left:77px;'>*</span>",
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  
  $form['new_pass'] = array(
    '#title' => t('Password'),
    '#type' => 'password',
    //'#prefix' => "<span style='color:red; float:right; position:absolute; left:79px;'>*</span>",
    '#size' => 25,
    '#maxlength' => 15,
		'#required' => TRUE,
  );
  
  $form['confirm_passord'] = array(
    '#type' => 'password',
    '#title' => t('Confirm password'),
    //'#prefix' => "<span style='color:red; float:right; position:absolute; left:139px;'>*</span>",
    '#size' => 25,
		'#required' => TRUE,
  );
  
  $form['agency_name'] = array(
    '#type' => 'textfield', 
    '#title' => t('Name of agency'),
    '#size' => 60,
    //'#prefix' => "<span style='color:red; float:right; position:absolute; left:125px;'>*</span>",
    '#maxlength' => 55,
    '#required' => TRUE,
  );
  
  $form['person_name'] = array(
    '#type' => 'textfield', 
    '#title' => t('Name of person'),
    '#size' => 60,
    //'#prefix' => "<span style='color:red; float:right; position:absolute; left:125px;'>*</span>",
    '#maxlength' => 55,
    '#required' => TRUE,
  );
  
  $form['cont_number'] = array(
    '#type' => 'textfield', 
    '#title' => t('Contact number (PT)'),
    '#size' => 60,
    //'#prefix' => "<span style='color:red; float:right; position:absolute; left:161px;'>*</span>",
    '#maxlength' => 55,
    '#required' => TRUE,
  );
  
  $form['mobile_number'] = array(
    '#type' => 'textfield', 
    '#title' => t('Contact number (Mobile)'),
    '#size' => 60,
    //'#prefix' => "<span style='color:red; float:right; position:absolute; left:188px;'>*</span>",
    '#maxlength' => 55,
    '#required' => TRUE,
  );
  
  $form['website'] = array(
    '#type' => 'textfield', 
    '#title' => t('Website'),
    '#size' => 60,
    '#maxlength' => 55,
  );
  
  $form['submit'] = array(
 	  '#type' => 'submit', 
 	  '#value' => t('Submit Form'),
 	 );
  
  return $form;
}

/*
 * Hook_form_submit for user to regiser form
 */
function user_register_download_tender_submit($form, &$form_state) { 
	global $user;
  $date = date('m/d/Y h:i:s', time());
  $created_date = strtotime($date);
  
  $pass =  md5(utf8_encode($form_state['values']['new_pass']));
  $details = db_select('tuser', 'tu')
	  ->fields('tu', array('mail'))
	  ->condition('tu.mail', $form_state['values']['email_id'], '=')
	  ->execute();
	if($details->rowCount() > 0) {
	  $user_det = $details->fetchObject();
	  form_set_error('email_id', 'Email ID already exist');
	}else{
	  $rec = db_insert('tuser') // Table name no longer needs {}
	  	->fields(array(
    		'pass'          => $pass,
    		'mail'          => $form_state['values']['email_id'],
    		'agency_name'   => $form_state['values']['agency_name'],
    		'person_name'   => $form_state['values']['person_name'],
    		'cont_number'   => $form_state['values']['cont_number'],
    		'mobile_number' => $form_state['values']['mobile_number'],
    		'mobile_number' => $form_state['values']['mobile_number'],
    		'website'       => $form_state['values']['website'],
    		'created'       => $created_date,
	  	))->execute();
	  	
	  	drupal_set_message('Your account is registered. Login here to download the tenders');
      $tid = $form_state['values']['tid'];
      $fid = $form_state['values']['fid'];
      drupal_goto("download/tender/$tid/$fid");
	}
}

/*
 * Function for chnage tender user chnage password
 */
function tuser_chnage_password($form, $form_state) {
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#prefix' => "<span style='color:red; float:right; position:absolute; left:57px;'>*</span>",
    '#size' => 50,
  );
  
  $form['submit'] = array(
 	  '#type' => 'submit', 
 	  '#value' => t('Submit'),
 	 );  
  return $form;
}

function tuser_chnage_password_mail($key, &$message, $params) {
  $headers = array(
    'MIME-Version' => '1.0',
    'Content-Type' => 'text/html; charset=UTF-8;',
    'Content-Transfer-Encoding' => '8Bit',
    'X-Mailer' => 'Drupal'
  );

  foreach ($headers as $key => $value) {
    $message['headers'][$key] = $value;
  }

  $message['subject'] = $params['subject'];
  $message['body'] = $params['body'];
}

/*
 * Hook_submit for tender user chnage password
 */
function tuser_chnage_password_submit($form, &$form_state) {
  global $base_url;
  $email = $form_state['values']['email'];
  $result = db_query('SELECT t.mail FROM {tuser} t WHERE t.mail = :mail', array(':mail' => $email));
   $record = $result->fetchAssoc();
  if(valid_email_address($email) && $record != ""){
    $from = 'santhosh.kalla@reisystems.in';
    $parm = base64_encode(serialize(array($email)));
    $body[] = 'Email: '.$email.'<br />URL: '.$base_url.'/change_password/'.$parm;
    drupal_set_message($base_url.'/change_password/'.$parm);
    $to = $email; 
    $params = array(
    'body' => $body,
    'subject' => 'Website Information Request',
    );
    if (drupal_mail('tuser_chnage_password', 'uri request', $to, language_default(), $params, $from, TRUE))
    {
        drupal_set_message('Thanks, the change password mail was successfully sent');    
    } else {
        drupal_set_message('There was an error subscribing you. Please try again later');
    }
  }else{
      form_set_error('email', 'invalid email address');
  }
}

function change_password($form_state, $record) {
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#size' => 50,
    '#default_value' => $record['build_info']['args'][0]['email'],
  );
  $form['new_password'] = array(
    '#type' => 'password_confirm',
    '#size' => 25,
  );
  $form['submit'] = array(
 	  '#type' => 'submit', 
 	  '#value' => t('Change Password'),
  );
  return $form;
}

function change_password_submit($form, &$form_state){
  $email = $form_state['values']['email'];
	$new_pass =  md5(utf8_encode($form_state['values']['new_password']));
	$result = db_select('tuser', 'tu')
	    ->fields('tu', array('mail', 'pass'))
	    ->condition('tu.mail', $email, '=')
	    ->execute()->rowCount();
	if($result ==1) {
	  $num_updated = db_update('tuser') // Table name no longer needs {}
  		->fields(array(
  		           'pass' => $new_pass,
              ))
      ->condition('mail', $email, '=')
      ->execute();
     drupal_set_message('Your password is chnaged');
     $form_state['rebuild'] = TRUE;
	  drupal_goto('procurement/tenders/list');
	}
	else {
	  form_set_error('email', 'Invalid email address');
	  drupal_goto('tuser/change_password');
	}
}
//Added by santhosh for archives on 01Feb2013
function procurement_archives_edit_list($record) {
  global $user,$base_path;
  $output = '';
  $table_rows = array();
  $output .= "<h1>".$record['month']." Tender's List</h1>";
  $header = array(
    array('data' => t('Title'), 'field' => 't.title', 'sort' => 'DESC'),
    array('data' => t('addendum')),
    array('data' => t('Submission date'), 'field' => 't.created', 'sort' => 'DESC'),
    array('data' => t('Tender Doc.')),
    array('data' => t('Tender Downloaded By')),
    array('data' => t('Action')),
  );
  $current_date = DATE('m/d/Y');
  $current_timestamp = strtotime($current_date);
  $limit = variable_get('codes_per_page',5);
  $query = db_select('tender', 't')->extend('PagerDefault')->limit($limit); 
  $query->fields('t')
        ->where("FROM_UNIXTIME(t.created, '%M %Y') = :date AND t.created < $current_timestamp", array(':date' => $record['month']))
        ->extend('TableSort')  
        ->orderByHeader($header);  
  $result = $query->execute();


	// Get tenders by Tender Groups
	$tender_sets = _tenders_group_by_tender_group($result);
	if (!empty($tender_sets)) {
		foreach($tender_sets as $tender_group => $tender_set) {
			if (!empty($tender_set)) {
				$table_rows[] = array(
					array(
						'data' => 'Tenders/Advetisements ('.$tender_group.')',
						'colspan' => 6,
						'align' => 'center',
					),
				);
				foreach($tender_set as $tender) {
					$file = file_load($tender->tfid);
					$get = get_addendum_by_tid($tender->tid);
					$add = l(t('Add Addendum'),  'procurement/' . $tender->tid . '/add/addendum');
					$edit = l(t('Edit'),  'procurement/' . $tender->tid . '/edit');
					$delete = l(t('Delete'),  'tender/' . $tender->tid . '/delete', array('attributes' => array('class' => array('confirm-delete'))));
					$tender_title = l($tender->title,  'procurement/view-tender/' . $tender->tid);
					$table_rows[] = array(
						array('data' => $tender_title),
						array('data' => get_addendum_by_tid($tender->tid)),
						array('data' =>  format_date($tender->created, 'custom', 'j-m-Y')),
						array('data' => get_tender_files($tender->tid)),
						array('data' => l(t('Users'), 'procurement/tender/downloaded_user/'.$tender->tid.'/' . $tender->title)),
						array('data' => $add . ' | ' . $edit . ' | ' . $delete ),
					);
			}
			}
		}
	}
  $attributes = array('class' => array('macp-table'));
  $output .=  theme('table', array('header' => $header, 
                                'rows' => $table_rows,
                                'attributes' => $attributes,
                                )). theme('pager');
  return $output;  
}

function procurement_download_tender_files($file_id = FALSE) {
  if (! $file_id) {
    return TRUE;
  }
  $file = file_load($file_id);
  $file_download = drupal_realpath($file->uri);
  return download_file($file_download);
}

function procurement_user_logout() {
  global $base_url;
  unset($_SESSION['tender_user']);
	variable_del(session_id());
  drupal_goto($base_url . '/procurement/tenders');
}

function sql_tender_downloaded_users($tender_id) {
  $sql = 'SELECT 
            T.tid, T.title, U.agency_name, U.person_name, 
            U.mail, U.cont_number, U.mobile_number, 
            U.website, U.created
        FROM tender T
        LEFT JOIN tdownload D ON D.tid = T.tid
        LEFT JOIN tuser U ON D.uid = U.uid
        WHERE T.tid = :tid
        AND U.uid > 0';
  $result = db_query($sql, array(':tid' => $tender_id));
  return $result;
}

function procurement_tender_downloaded_users($tender_id = FALSE, $tender_title) {
  if (! $tender_id || $tender_id < 1) {
    return 'Please select a tender';
  }
  $output = '';
  $tender_id = arg(3);
  $tender_name = arg(4);
  $tenders = sql_tender_downloaded_users($tender_id);
  $header = array(
    array('data' => t('Person Name'),),
    array('data' => t('Agency Name')),
    array('data' => t('Email')),
    array('data' => t('Contact Number')),
    array('data' => t('Mobile Number')),
    array('data' => t('Website')),
  );
  $table_rows = array();
  $has_records = 0;
  if (!empty($tenders)) {
    foreach ($tenders as $tender) {
      if (count($tender)) {
        ++$has_records;
      }
      $website_link = $tender->website;
      if (strlen($tender->website)) {
        $website_link = '<a href="'.$tender->website.'">'.$tender->website.'</a>';
      }
      $table_rows[] = array(
        array('data' => $tender->agency_name),
        array('data' => $tender->person_name),
        array('data' => $tender->mail),
        array('data' => $tender->cont_number),
        array('data' => $tender->mobile_number),
        array('data' => $tender->website),
      );
    }
  }
  if ($has_records < 1) {
     $table_rows[] = array(
        array(
          'data' => 'No data available.',
          'colspan' => 6,
          'align' => 'center',
        ),
      );
  }
  $attributes = array('class' => array('macp-table'));
  $output .= 'Tender: '.$tender_name .
                theme('table', array('header' => $header, 
                      'rows' => $table_rows,
                      'attributes' => $attributes,
                      '#empty' => t('No items available'),
                      )). theme('pager');
  return $output;
}

function procurement_view_tender_details($tender_id = 0) {
	if (! $tender_id || $tender_id < 0) {
		return 'Please select a tender';
	}
  $output = '';
  $table_rows = array();
  $header = array(
    array('data' => t('View Tender Details'), 'colspan' => 5),
		);
  $limit = variable_get('codes_per_page',5);
  $query = db_select('tender', 't')->extend('PagerDefault')->limit($limit); 
  //Date Condition added by santhosh for creating archives on 01Feb2013
  $current_date = DATE('m/d/Y');
  $current_timestamp = strtotime($current_date);
  $query->fields('t')
        ->condition('t.tid', $tender_id, '=')
        ->extend('TableSort')  // Add table sort extender.
        ->orderByHeader($header);  // Add order by headers.
  $result = $query->execute();
	$groups = variable_get('tender_groups');
	if (!empty($result)) {
		foreach ($result as $tender) {
			$tender->group = (isset($groups[$tender->group_id])) ? $groups[$tender->group_id] : '';
			$tender->created_date = format_date($tender->created, 'custom', 'j-m-Y');
			$tender->updated_date = format_date($tender->changed, 'custom', 'j-m-Y');
			
			$tender->tender_files = get_user_addendum_by_tid($tender->tid);
			$tender->addendum_files = get_user_addendum_file_tid($tender->tid);
		}
	}
	$theme_parameters = array('records' => $tender,
                            'headings' => 'View Tender Details',
                      );
  return theme('tender-view', $theme_parameters);
}
