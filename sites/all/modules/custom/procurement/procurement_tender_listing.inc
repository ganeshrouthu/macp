<?php
/**
 * This Form for add tender
 */
function procurement_add_tender($form, $form_state, $tender = NULL, $addendum = NULL) {
  
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
    '#default_value' => empty($tender) ? '' :$tender->tfid,
 	);
 	
 	$form['submit'] = array(
 	  '#type' => 'submit', 
 	  '#value' => empty($tender) ? t('Add Tender') :t('Edit Tender')
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
   		   'created' => $created_date,
    	   'changed' => $updated_date,
  	  ))
     ->condition('tid', $form_state['values']['tid'], '=')
     ->execute();
     
     $form_state['redirect'] = 'procurement/tenders';
  }else {
  	$result = db_query("INSERT INTO {tender} (title, tdesc, tfid, created, changed) VALUES ('".$form_state['values']['title']."', '".$form_state['values']['desc']."', ".$form_state['values']['upload'].", ".$created_date.", ".$created_date.")");
  	
  	/*db_insert('addendum') // Table name no longer needs {}
			->fields(array(
  			'atitle'  => $form_state['values']['title'],
  			'adesc'   => $form_state['values']['tdesc'],
  			'afid'    => $form_state['values']['upload'],
  			'created' => $created_date,
  			'changed' => $updated_date,
		*/
  	$form_state['redirect'] = 'procurement/tenders';
  }
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
    array('data' => t('Brief of tenders')),
    array('data' => t('addendum')),
    array('data' => t('Submission date'), 'field' => 't.created', 'sort' => 'DESC'),
    array('data' => t('Tender Doc.')),
    array('data' => t('Action')), 
  );
  $limit = variable_get('codes_per_page',5);
  $query = db_select('tender', 't')->extend('PagerDefault')->limit($limit); 
  $query->fields('t')
        ->extend('TableSort')  // Add table sort extender.
        ->orderByHeader($header);  // Add order by headers.
  $result = $query->execute();
  
  foreach($result as $tender) {
  	$file = file_load($tender->tfid);
  	$get = get_addendum_by_tid($tender->tid);
  	$add = l(t('Add Addendum'),  'procurement/' . $tender->tid . '/add/addendum');
  	$edit = l(t('Edit'),  'procurement/' . $tender->tid . '/edit');
  	$delete = l(t('Delete'),  'tender/' . $tender->tid . '/delete', array('attributes' => array('class' => array('delete-link'))));
    $table_rows[] = array(
      array('data' => $tender->title),
      array('data' => $tender->tdesc),
      array('data' => get_addendum_by_tid($tender->tid)),
      array('data' =>  format_date($tender->created, 'custom', 'D j M Y')),
      array('data' => get_addendum_file_tid($tender->tid)),
      array('data' => $add . ' | ' . $edit . ' | ' . $delete ),
    );
   
  }
  $output .= theme('table', array('header' => $header, 'rows' => $table_rows)). theme('pager');
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
    array('data' => t('Title of tenders'), 'field' => 't.title', 'sort' => 'DESC'),
    array('data' => t('Brief of tenders')),
    array('data' => t('addendum')),
    array('data' => t('Submission date'), 'field' => 't.created', 'sort' => 'DESC'),
    array('data' => t('Tender Doc.')),
    
  );
  $limit = variable_get('codes_per_page',5);
  $query = db_select('tender', 't')->extend('PagerDefault')->limit($limit); 
  $query->fields('t')
        ->extend('TableSort')  // Add table sort extender.
        ->orderByHeader($header);  // Add order by headers.
  $result = $query->execute();
  
  foreach($result as $tender) {
  	$file = file_load($tender->tfid);
  	$get = get_addendum_by_tid($tender->tid);
    $table_rows[] = array(
      array('data' => $tender->title),
      array('data' => $tender->tdesc),
      array('data' => get_user_addendum_by_tid($tender->tid)),
      array('data' =>  format_date($tender->created, 'custom', 'D j M Y')),
      array('data' => get_user_addendum_file_tid($tender->tid)),
      
    );
   
  }
  $output .= theme('table', array('header' => $header, 'rows' => $table_rows)). theme('pager');
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
    '#title' => t('Email ID (User ID)'),
    '#size' => 60,
    '#maxlength' => 128,
    //'#required' => TRUE,
  );
  
  
  /*$form['pass'] = array(
    '#title' => t('Password'),
    '#type' => 'password',
    '#size' => 60,
    '#maxlength' => 15,
    //'#required' => TRUE,
  );*/
  
  $form['submit'] = array(
 	  '#type' => 'submit', 
 	  '#value' => t('Download Tender'),
 	);
 	

 	$items = array();
  $items[] = l(t('Register new'), 'register/download/tender/' . arg(2) . '/' .arg(3), array('attributes' => array('title' => t('Create a new tender user account.'))));
  //$items[] = l(t('Forgot password'), 'tuser/chnage_password', array('attributes' => array('title' => t('Request new password via e-mail.'))));
  $form['links'] = array('#markup' => theme('item_list', array('items' => $items)));
  
  return $form;
}

function user_login_download_tender_mail($key, &$message, $params) {

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
 * Hook_submit for user login form for download tender
 */
function user_login_download_tender_submit($form, &$form_state) {
	global $user;
  global $base_url;
	/*if(!valid_email_address($form_state['values']['email_id'])) {
    form_set_error('email', 'Your email address appears malformed');
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
      	$file = file_load($form_state['values']['fid']);
      	$file_download = drupal_realpath($file->uri);;
      	
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
	  	form_set_error('email', 'Your email or password is incorrect');
	  }
    
	}*/
  
  $valid_email = $form_state['values']['email_id'];
   $result = db_query('SELECT t.mail FROM {tuser} t WHERE t.mail = :mail', array(':mail' => $valid_email));
   $record = $result->fetchAssoc();
    $from = 'santhosh.kalla@reisystems.in';
    $body[] = 'Email: '.$valid_email.'<br />URL: '.$base_url.'tuser/chnage_password';
    $to = $valid_email; 
    $params = array(
    'body' => $body,
    'subject' => 'Website Information Request',
    );
    if (drupal_mail('user_login_download_tender', 'uri request', $to, language_default(), $params, $from, TRUE) && $record != "")
    {
        drupal_set_message('Thanks, we will be in contact with more information soon.');    
    } else {
        drupal_set_message('There was an error subscribing you. Please try again later');
    }
}

/*
 * function for download selected file 
 * after logn/register anonymous user
 */
function download_file($file) {
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
    '#maxlength' => 128,
    //'#required' => TRUE,
  );
  
  $form['new_pass'] = array(
    '#title' => t('Password'),
    '#type' => 'password',
    '#size' => 60,
    '#maxlength' => 15,
  );
  
  $form['confirm_passord'] = array(
    '#type' => 'password',
    '#title' => t('Confirm password'),
    '#size' => 25, 
  );
  
  $form['agency_name'] = array(
    '#type' => 'textfield', 
    '#title' => t('Name of agency'),
    '#size' => 60,
    '#maxlength' => 55,
    //'#required' => TRUE,
  );
  
  $form['person_name'] = array(
    '#type' => 'textfield', 
    '#title' => t('Name of person'),
    '#size' => 60,
    '#maxlength' => 55,
    //'#required' => TRUE,
  );
  
  $form['cont_number'] = array(
    '#type' => 'textfield', 
    '#title' => t('Contact number (PT)'),
    '#size' => 60,
    '#maxlength' => 55,
    //'#required' => TRUE,
  );
  
  $form['mobile_number'] = array(
    '#type' => 'textfield', 
    '#title' => t('Contact number (Mobile)'),
    '#size' => 60,
    '#maxlength' => 55,
    //'#required' => TRUE,
  );
  
  $form['website'] = array(
    '#type' => 'textfield', 
    '#title' => t('Website'),
    '#size' => 60,
    '#maxlength' => 55,
  );
  
  $form['submit'] = array(
 	  '#type' => 'submit', 
 	  '#value' => t('Download Tender'),
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
	  form_set_error('emial', 'Email ID already exist');
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
	  	
	  	
	  
	  if($rec) {
	    if(!$user->uid) {
	    	
	    	$user_details = db_select('tuser', 'tu')
	    	  ->fields('tu', array('mail', 'uid'))
	    		->condition('tu.mail', $form_state['values']['email_id'], '=')
	    		->execute();
	    	$count = $user_details->rowCount();
	    	
	    	drupal_session_start();
	    	$_SESSION['tender_user'] = $form_state['values']['email_id'];
      	$file = file_load($form_state['values']['fid']);
      	$file_download = drupal_realpath($file->uri);;
    
        if (file_exists($file_download)) {
        	if($count == 1) {
	    			$res = $user_details->fetchAssoc();
	    			$uid = $res['uid'];
	          
	    			
	    			$date = date('m/d/Y h:i:s', time());
    				$created_date = strtotime($date);
	    		  $tdown = db_insert('tdownload')
	    		    ->fields(array(
	    		      'uid' => $res['uid'],
	    		      'tid' => $form_state['values']['tid'],
	    		      'fid' => $form_state['values']['fid'],
	    		      'created' => $created_date,
	    		    ))->execute();
	    		  drupal_set_message('record added');
	    		}
        	//download_file($file_download);          
        }
	  	}	
	  }
	}
    
  
}


/*
 * Function for chnage tender user chnage password
 */
function tuser_chnage_password($form, $form_state) {
  $form['email'] = array(
    '#type' => 'textfield', 
    '#title' => t('Email'),
    '#size' => 60,
    '#maxlength' => 128,
    //'#required' => TRUE,
  );
  
  $form['new_password'] = array(
    '#type' => 'password',
    '#title' => t('New password'),
    '#size' => 60,
    '#maxlength' => 15,
  );
  
  $form['confirm_pass'] = array(
    '#type' => 'password',
    '#title' => t('Confirm password'),
    '#size' => 25, 
  );
  
  $form['submit'] = array(
 	  '#type' => 'submit', 
 	  '#value' => t('Change password'),
 	 );
  
  return $form;
  
}


/*
 * Hook_submit for tender user chnage password
 */
function tuser_chnage_password_submit($form, &$form_state) {
	$old_pass =  md5(utf8_encode($form_state['values']['old_pass']));
	$new_pass =  md5(utf8_encode($form_state['values']['new_password']));
	$result = db_select('tuser', 'tu')
	    ->fields('tu', array('mail', 'pass'))
	    ->condition('tu.pass', $old_pass, '=')
	    ->execute()->rowCount();
	if($result ==1) {
	  $num_updated = db_update('tuser') // Table name no longer needs {}
  		->fields(array(
  		           'pass' => $new_pass,
              ))
      ->condition('pass', $old_pass, '=')
      ->execute();
     drupal_set_message('your password is chnaged');
     $form_state['rebuild'] = TRUE;
	}
	else {
	  form_set_error('password', 'Your old password is wrong');
	  drupal_goto('tuser/chnage_password');
	}
}