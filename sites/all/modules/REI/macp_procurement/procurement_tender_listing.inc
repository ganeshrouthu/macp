<?php
/**
 * This Form for add tender
 */
function procurement_add_tender($form, $form_state, $tender = NULL, $addendum = NULL) {
  global $base_url;
	$form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
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
  $form['group_id'] = array(
    '#type' => 'select', 
    '#title' => t('Tender Group'),
    '#options' => _tender_groups(),
    '#default_value' => empty($tender->group_id) ? '' : $tender->group_id, 
		'#required' => TRUE,
  );

  $form['date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Submission Date'),
    '#default_value' => empty($tender->created) ? '' : date("Y-m-d H:i:s", $tender->created),
    '#date_type' => DATE_DATETIME,
    '#date_timezone' => date_default_timezone(),
    '#date_format' => 'd/m/Y h:i A',
		'#date_text_parts' => array('hour', 'minute'),
    '#date_increment' => 1,
    '#date_year_range' => '0:+3',
    '#required' => TRUE,
		'#datepicker_options' => array('minDate' => 'today'),
		'#attributes' => array('readonly' => 'true'),
		'#prefix' => '<div class="date-time-picker">',
		'#suffix' => '</div>',
  );
  
  $form['desc'] = array(
  	'#title' => t('Brief Description'),
    '#type' => 'textarea',
    '#default_value' => empty($tender->tdesc) ? '' : $tender->tdesc, 
    '#cols' => 60, 
    '#rows' => 5,
  );

  $form['upload'] = array(
    '#type' => 'managed_file',
    '#title' => t('Upload Document 1'),
    '#upload_location' => 'public://procurement/',
    '#required' => TRUE,
		'#description' => 'Please upload files with these extensions only: pdf doc docx',
    '#default_value' => empty($tender->tfid) ? '' : $tender->tfid, 
		"#upload_validators" => array(
														"file_validate_extensions" => array("pdf PDF doc docx"),
														'file_validate_size' => array(8 * 1048576),
														),
 	);

	$form['upload-another'] = array(
  	'#title' => t('Upload another file'),
    '#type' => 'checkbox',
    '#prefix' => '<div id="upload-another-file">',
    '#suffix' => '</div>',
		'#class' => array('description'),
    '#default_value' => 0,
  );

	$form['upload_2'] = array(
    '#type' => 'managed_file',
    '#title' => t('Upload Document 2'),
    '#upload_location' => 'public://procurement/',
		'#description' => 'Please upload files with these extensions only: pdf doc docx',
		'#default_value' => empty($tender->tender_files) ? '' : $tender->tender_files, 
		"#upload_validators" => array(
														"file_validate_extensions" => array("pdf PDF doc docx"),
														'file_validate_size' => array(8 * 1048576),
														),
		'#states' => array(
											'visible' => array('input[name="upload-another"]' => array('checked' => TRUE))
											),
 	);
	$cancel_url = $base_url . '/procurement/manage-tenders';
 	$form['add_submit'] = array(
 	  '#type' => 'submit',
 	  '#value' => t('Save'),
 	  '#prefix' => '<div class="submit-buttons">',
		'#suffix' => '<span class="cancel-btn-div"><input class="cancel-btn" type="button" value="Cancel" alt="'.$cancel_url.'" /></span></div>'
 	);
  return $form;
}

function _upload_tender_files($form, &$form_state) {
	$tender_files = '';
	if (isset($form_state['values']['upload_2']) && $form_state['values']['upload_2']> 0) {
		$file_2 = file_load($form_state['values']['upload_2']);
		$file_2->status = FILE_STATUS_PERMANENT;
		file_save($file_2);
		file_usage_add($file_2, 'procurement', $file_2->filename, 1);
		if ($form_state['values']['upload_2'] > 0) {
			$tender_files = $form_state['values']['upload_2'];
		}
	}
	return $tender_files;
}

/*
 * hook_form_submit for add tender form
 */
function procurement_add_tender_submit($form, &$form_state) {
  $created_date = strtotime($form_state['values']['date']);
  $date = date('m/d/Y h:i:s', time());
  $updated_date = strtotime($date);
	$tender_files = _upload_tender_files($form, $form_state);
  if(!empty($form_state['values']['tid'])) {
    db_update('tender') // Table name no longer needs {}
  		->fields(array(
   		   'title' => $form_state['values']['title'],
   		   'tdesc' => $form_state['values']['desc'],
   		   'tfid'  => $form_state['values']['upload'],
   		   'tender_files'  => $form_state['values']['upload_2'],
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
		drupal_set_message('Tender edited successfully.', 'success');
    $form_state['redirect'] = 'procurement/manage-tenders';
  }
	else {
		$added_date = time();
  	$result = db_query("INSERT INTO {tender} 
									(title, group_id, tdesc, tfid,
									tender_files, created, 
									changed, added) 
									VALUES (
									'".$form_state['values']['title']."', 
									'".$form_state['values']['group_id']."', 
									'".$form_state['values']['desc']."', 
									".$form_state['values']['upload'].", 
									'".$form_state['values']['upload_2']."',
									".$created_date.",
									".$created_date.", 
									".$added_date.")");

		if (strlen($form_state['complete form']['upload']['#file']->filename)) {
			//$file->filename = 'Tender-'.$form_state['complete form']['upload']['#file']->filename;
		watchdog('macp', 'upload file:@upload_file', 
							$variables = array('@upload_file' => $form_state['complete form']['upload']['#file']->filename), 
							WATCHDOG_NOTICE, NULL);
		}
		//$file->filename = '';
    $file = file_load($form_state['values']['upload']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
		file_usage_add($file, 'procurement', $file->filename, 1);
    drupal_set_message('Tender added successfully.', 'success');
  	$form_state['redirect'] = 'procurement/manage-tenders';
  }
}

/*
 * Group Tenders by Tender For.
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
    array('data' => t('Title')),
		array('data' => t('Submit to MACP office on or before'), 'align' =>'right'),
    array('data' => t('Downloaded By'), 'class' => array('align-center')), 
		array('data' => t('Actions'), 'class' => array('align-center')),
  );
  $limit = variable_get('codes_per_page',10);
  $query = db_select('tender', 't')
						->extend('PagerDefault')
						->limit($limit); 

  $current_date = DATE('m/d/Y');
  $current_date = strtotime($current_date);
  $query->fields('t')
        ->condition('created', $current_date, '>=')
				->orderBy('added', 'DESC');	//ORDER BY created
  $result = $query->execute();
	// Get tenders by Tender Fors
	$tender_sets = _tenders_group_by_tender_group($result);
	if (!empty($tender_sets)) {
		foreach($tender_sets as $tender_group => $tender_set) {
			if (!empty($tender_set)) {
				$table_rows[] = array(
					array(
						'data' => '<b>Tenders/Advetisements ('.$tender_group.')</b>',
						'colspan' => 7,
						'align' => 'center',
					),
				);
				foreach($tender_set as $tender) {
					$tender_addendums = get_addendum_for_tenders($tender->tid);
					$file = file_load($tender->tfid);
					$get = get_addendum_by_tid($tender->tid);
					$add = l(t('Add Addendum'),  'procurement/' . $tender->tid . '/add/addendum/');
					$edit = l(t('Edit'),  'procurement/' . $tender->tid . '/edit/');
					$delete = l(t('Delete'),  'tender/' . $tender->tid . '/delete', array('attributes' => array('class' => array('confirm-delete'))));
					$tender_title = l($tender->title,  'procurement/view-tender/' . $tender->tid);
					$date_time_str = date('d M Y ', $tender->created) .' upto '. date('g:i a', $tender->created);
					$table_rows[] = array(
						array('data' => $tender_title),
						array('data' => $date_time_str),
						array('data' => l(t('Agencies'), 'procurement/tender/downloaded_user/'.$tender->tid.'/' . $tender->title)),
						array('data' => $add . ' | ' . $edit . ' | ' . $delete, 'class' => array('addendum-links')),
					);
					if (strlen($tender_addendums)) {
						$table_rows[] = array(
							array('data' => $tender_addendums, 'colspan' => 4),
						);
					}
				}
			}
		}
	}
  $attributes = array('class' => array('macp-table', 'tender-table'));
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
    array('data' => t('Title')),
    array('data' => t('Submit to MACP office on or before'), 'align' =>'right'),
  );
  $limit = variable_get('codes_per_page',10);
  $query = db_select('tender', 't')->extend('PagerDefault')->limit($limit); 
  $current_date = date('m/d/Y');
  $current_timestamp = strtotime($current_date);
  $query->fields('t')
        ->condition('created', $current_timestamp, '>=')
				->orderBy('added', 'DESC');	//ORDER BY created
  $result = $query->execute();

	// Get tenders by Tender Groups	
	$tender_sets = _tenders_group_by_tender_group($result);
	if (!empty($tender_sets)) {
		foreach($tender_sets as $tender_group => $tender_set) {
			if (!empty($tender_set)) {
				$table_rows[] = array(
					array(
						'data' => '<b>Tenders/Advetisements ('.$tender_group.')</b>',
						'colspan' => 5,
						'align' => 'center',
					),
				);
				foreach($tender_set as $tender) {
					$tender_addendums = get_addendum_for_tenders($tender->tid);
					$file = file_load($tender->tfid);
					$tender_title = l($tender->title,  'procurement/view-tender/' . $tender->tid);
					$date_time_str = date('d M Y ', $tender->created) .' upto '. date('g:i a', $tender->created);
					$table_rows[] = array(
						array('data' => $tender_title),
						array('data' => $date_time_str, 'align' =>'right'),
					);
					if (strlen($tender_addendums)) {
						$table_rows[] = array(
							array('data' => $tender_addendums, 'colspan' => 4),
						);
					}
				}
			}
		}
	}
  $attributes = array('class' => array('macp-table', 'tender-table'));
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
	global $base_url;
	if(isset($_SESSION['tender_user']) && arg(2) > 0 && arg(3) > 0) {
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
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  
  
  $form['pass'] = array(
    '#title' => t('Password'),
    '#type' => 'password',
    '#size' => 60,
    '#maxlength' => 15,
    '#required' => TRUE,
  );
  $cancel_url = $base_url . '/procurement/tenders';
  $form['submit'] = array(
 	  '#type' => 'submit', 
 	  '#value' => t('Login'),
		'#prefix' => '<div class="submit-buttons">',
		'#suffix' => '<span class="cancel-btn-div"><input class="cancel-btn" type="button" value="Cancel" alt="'.$cancel_url.'" /></span></div>'
 	);

 	$items = array();
  $items[] = l(t('Register new'), 'procurement/register/download-tender/' . arg(2) . '/' .arg(3), array('attributes' => array('title' => t('Create a new tender user account.'))));
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
	global $user, $base_url;
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
	    if(!$user->uid && $form_state['values']['fid'] > 0) { 
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
			$_SESSION['tender_user'] = $form_state['values']['email_id'];
			variable_set(session_id(), $form_state['values']['email_id']);
			$goto = $base_url . '/procurement/tenders';
			drupal_goto($goto);
		}
	  else {
	  	form_set_error('email_id', 'Your email or password is incorrect');
 		  //drupal_set_message('Your email or password is incorrect', 'error');
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
 * hook_form for user to regiser anonymous user and download the tender
 */
function user_register_download_tender($form, $form_state, $tid, $fid) {
	global $base_url;
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
    '#maxlength' => 128,
    '#prefix' => '<div id="errorContainer" clear="all"></div>',
    '#required' => TRUE,
  );
  
  $form['new_pass'] = array(
    '#title' => t('Password'),
    '#type' => 'password',
    '#size' => 25,
    '#maxlength' => 15,
		'#required' => TRUE,
  );
  
  $form['confirm_passord'] = array(
    '#type' => 'password',
    '#title' => t('Confirm Password'),
    '#size' => 25,
		'#required' => TRUE,
  );
  
  $form['agency_name'] = array(
    '#type' => 'textfield', 
    '#title' => t('Name of Agency'),
    '#size' => 60,
    '#maxlength' => 55,
    '#required' => TRUE,
  );
  
  $form['person_name'] = array(
    '#type' => 'textfield', 
    '#title' => t('Name of Person'),
    '#size' => 60,
    '#maxlength' => 55,
    '#required' => TRUE,
  );
  
  $form['cont_number'] = array(
    '#type' => 'textfield', 
    '#title' => t('Contact Number'),
    '#size' => 60,
    '#maxlength' => 10,
    //'#required' => TRUE,
  );
  
  $form['mobile_number'] = array(
    '#type' => 'textfield', 
    '#title' => t('Mobile Number'),
    '#size' => 60,
    '#maxlength' => 10,
    '#required' => TRUE,
  );
  
  $form['website'] = array(
    '#type' => 'textfield', 
    '#title' => t('Website'),
    '#size' => 60,
    '#maxlength' => 55,
  );
  $cancel_url = $base_url . '/procurement/tenders';
  $form['submit'] = array(
 	  '#type' => 'submit', 
 	  '#value' => t('Register'),
		'#prefix' => '<div class="submit-buttons">',
		'#suffix' => '<span class="cancel-btn-div"><input class="cancel-btn" type="button" value="Cancel" alt="'.$cancel_url.'" /></span></div>'
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
	}
	else {
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
	  	drupal_set_message('Your account is registered successfully.');
      $tid = $form_state['values']['tid'];
      $fid = $form_state['values']['fid'];

			////////// SEND ACCOUNT REGISTRATION EMAIL ////////////
			$email = $form_state['values']['email_id'];
			if(valid_email_address($email)) {
				$from = 'info@reisystems.in';
				$parm = base64_encode(serialize(array($email)));
				$password_chage_url = '<a href="'.$base_url.'/change_password/'.$parm.'">Change Password</a>';

				extract($form_state['values'], EXTR_PREFIX_ALL, 'fv');
				$str = 'Hello ' . $fv_person_name .',';
				$str .= 'Your account is created with MACP with following details: \n\n';
				$str .= 'Email ID: ' . $fv_email_id .'\n';
				$str .= 'Name of Agency: ' . $fv_agency_name .'\n';
				$str .= 'Name of Person: ' . $fv_person_name .'\n';
				$str .= 'Contact Number: ' . $fv_cont_number .'\n';
				$str .= 'Mobile Number: ' . $fv_mobile_number .'\n';
				$str .= 'Website: ' . $fv_website .'\n';
				$str .= '\n\n\n';
				$str .= 'Thanks and regards,\n';
				$str .= 'MACP';

				$body[] = $str;
				drupal_set_message($base_url.'/change_password/'.$parm);
				$to = $email; 
				$params = array(
				'body' => $body,
				'subject' => 'MACP: Website Information Request',
				);
				if (drupal_mail('user_register_download_tender', 'uri request', $to, language_default(), $params, $from, TRUE)) {
					drupal_set_message('Thanks, your account has been registered successfully.', 'success');
				}
				else {
					drupal_set_message('There was an error registering you. Please try again later', 'error');
				}
			}
			////////// SEND ACCOUNT REGISTRATION EMAIL ////////////
			
			/* Login user automatically */
			$_SESSION['tender_user'] = $form_state['values']['email_id'];
			variable_set(session_id(), $form_state['values']['email_id']);
			/* Login user automatically */

			$goto_url = 'procurement/tenders';
			drupal_goto($goto_url);
	}
}

/*
 * Function for chnage tender user chnage password
 */
function tuser_chnage_password($form, $form_state) {
	global $base_url;
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#size' => 50,
  );

  $cancel_url = $base_url . '/procurement/register/download-tender/0/0';
  $form['submit'] = array(
 	  '#type' => 'submit', 
 	  '#value' => t('Submit'),
		'#prefix' => '<div class="submit-buttons">',
		'#suffix' => '<span class="cancel-btn-div"><input class="cancel-btn" type="button" value="Cancel" alt="'.$cancel_url.'" /></span></div>'
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

function user_register_download_tender_mail($key, &$message, $params) {
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
    $from = 'info@reisystems.in';
    $parm = base64_encode(serialize(array($email)));
		$password_chage_url = '<a href="'.$base_url.'/change_password/'.$parm.'">Change Password</a>';
    $body[] = "
		Hello User,
		Please click on the link below to reset your password.\n"
		. $password_chage_url;
    drupal_set_message($base_url.'/change_password/'.$parm);
    $to = $email; 
    $params = array(
    'body' => $body,
    'subject' => 'MACP: Website Information Request',
    );
    if (drupal_mail('tuser_chnage_password', 'uri request', $to, language_default(), $params, $from, TRUE)) {
        drupal_set_message('Thanks, the change password mail was successfully sent');    
    }
		else {
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

function procurement_archives_edit_list($record) {
  global $user,$base_path;
  $output = '';
  $table_rows = array();
  $output .= "<h1>".$record['month']." Tender's List</h1>";
  $header = array(
    array('data' => t('Title')),
		array('data' => t('Submit to MACP office on or before'), 'align' =>'right'),
    array('data' => t('Downloaded By'), 'class' => array('align-center')),
		array('data' => t('Actions'), 'class' => array('align-center')),
  );
  $current_date = DATE('m/d/Y');
  $current_timestamp = strtotime($current_date);
  $limit = variable_get('codes_per_page',10);
  $query = db_select('tender', 't')->extend('PagerDefault')->limit($limit); 
  $query->fields('t')
        ->where("FROM_UNIXTIME(t.created, '%M %Y') = :date AND t.created < $current_timestamp", array(':date' => $record['month']))
        ->orderBy('added', 'DESC');
				/*
				->extend('TableSort')  
        ->orderByHeader($header);
				*/
  $result = $query->execute();
	// Get tenders by Tender Groups
	$tender_sets = _tenders_group_by_tender_group($result);
	if (!empty($tender_sets)) {
		foreach($tender_sets as $tender_group => $tender_set) {
			if (!empty($tender_set)) {
				$table_rows[] = array(
					array(
						'data' => '<b>Tenders/Advetisements ('.$tender_group.')</b>',
						'colspan' => 6,
						'align' => 'center',
					),
				);
				foreach($tender_set as $tender) {
					$tender_addendums = get_addendum_for_tenders($tender->tid);
					$file = file_load($tender->tfid);
					$get = get_addendum_by_tid($tender->tid);
					$add = l(t('Add Addendum'),  'procurement/' . $tender->tid . '/add/addendum');
					$edit = l(t('Edit'),  'procurement/' . $tender->tid . '/edit');
					$delete = l(t('Delete'),  'tender/' . $tender->tid . '/delete', array('attributes' => array('class' => array('confirm-delete'))));
					$tender_title = l($tender->title,  'procurement/view-tender/' . $tender->tid);
					$date_formatter = 'j-m-Y';
					$date_time_str = date('d M Y ', $tender->created) .' upto '. date('g:i a', $tender->created);
					$table_rows[] = array(
						array('data' => $tender_title),
						array('data' => $date_time_str),
						array('data' => l(t('Agencies'), 'procurement/tender/downloaded_user/'.$tender->tid.'/' . $tender->title)),
						array('data' => $add . ' | ' . $edit . ' | ' . $delete ),
					);
					if (strlen($tender_addendums)) {
						$table_rows[] = array(
							array('data' => $tender_addendums, 'colspan' => 4),
						);
					}
			}
			}
		}
	}
  $attributes = array('class' => array('macp-table', 'tender-table'));
  $output .=  theme('table', array('header' => $header, 
                                'rows' => $table_rows,
                                'attributes' => $attributes,
                                )). theme('pager');
  return $output;  
}

function procurement_download_tender_files($tender_id, $file_id = FALSE) {
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
  $header = array(array('data' => t('View Tender Details'), 'colspan' => 5));
  $limit = variable_get('codes_per_page',10);
  $query = db_select('tender', 't')->extend('PagerDefault')->limit($limit); 
  $current_date = date('m/d/Y');
  $current_timestamp = strtotime($current_date);
  $query->fields('t')
        ->condition('t.tid', $tender_id, '=');
  $result = $query->execute();
	$groups = variable_get('tender_groups');
	if (!empty($result)) {
		foreach ($result as $tender) {
			$tender->group = (isset($groups[$tender->group_id])) ? $groups[$tender->group_id] : '';
			$tender->addendums = get_user_addendum_by_tid($tender->tid);
			$tender->tender_files = get_tender_files($tender->tid);
			$tender->addendum_files = get_user_addendum_file_tid($tender->tid);
			$tender->tender_date = date('d M Y g:i a', $tender->created);
		}
	}
	$theme_parameters = array('records' => $tender,
                            'headings' => 'View Tender Details',
                      );
  return theme('tender-view', $theme_parameters);
}
