<?php
/**
 * add addendum to selected tender
 */
function procurement_add_addendum($form, $form_state, $tender = NULL, $addendum = NULL) {
  global $base_url;
  $form['add_title'] = array(
    '#type' => 'textfield', 
    '#title' => t('Title of the Tender'),
    '#size' => 60, 
    '#maxlength' => 128,
    '#attributes' => array('disabled' => 'true'), 
    '#default_value' => empty($tender->title) ? '' : $tender->title, 
    '#required' => TRUE,
  );
  
  $form['tender_id'] = array(
    '#type' => 'hidden', 
    '#title' => t('Tender ID'),
    '#size' => 60, 
    '#maxlength' => 128,
    '#attributes' => array('disabled' => 'true'), 
    '#default_value' => empty($tender->tid) ? '' : $tender->tid, 
    '#required' => TRUE,
  );
  
  if(!empty($addendum)) {
    $form['action'] = array(
    	'#type' => 'hidden', 
    	'#default_value' => 'edit', 
  	);
  	
  	$form['aid'] = array(
    	'#type' => 'hidden', 
    	'#title' => t('Addendum ID'),
    	'#size' => 60, 
    	'#maxlength' => 128,
    	'#attributes' => array('disabled' => 'true'), 
    	'#default_value' => empty($addendum->aid) ? '' : $addendum->aid, 
    	'#required' => TRUE,
    );
  }else {
    $form['action'] = array(
    	'#type' => 'hidden', 
    	'#default_value' => 'add', 
  	);
  }

  $form['add_title_add'] = array(
    '#type' => 'textfield', 
    '#title' => t('Title'),
    '#size' => 60, 
    '#maxlength' => 128, 
    '#default_value' => empty($addendum->atitle) ? '' : $addendum->atitle, 
    '#required' => TRUE,
  );
  
  $form['add_desc'] = array(
  	'#title' => t('Brief Description'),
    '#type' => 'textarea',
    '#default_value' => empty($addendum->adesc) ? '' : $addendum->adesc, 
    '#cols' => 60, 
    '#rows' => 5,
  );

  $form['add_upload'] = array(
    '#type' => 'managed_file',
    '#title' => t('Upload Documents'),
    '#upload_location' => 'public://addendum/',
    '#required' => TRUE,
		'#description' => 'Please upload files with these extensions only: pdf doc docx',
    '#default_value' => empty($addendum) ? '' :$addendum->afid,
		"#upload_validators" => array(
														"file_validate_extensions" => array("pdf PDF doc docx"),
														'file_validate_size' => array(8 * 1048576),
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

/* 
 * hook_submit for add_addendum form to selected tender
 */
function procurement_add_addendum_submit($form, &$form_state) {
	global $base_url;
	if ($form_state['values']['op'] == 'Cancel') {
		drupal_goto($base_url . '/procurement/manage-tenders');
	}
	$date = date('m/d/Y h:i:s', time());
  $updated_date = strtotime($date);
  if($form_state['values']['action'] == 'add') {
  	db_insert('addendum') // Table name no longer needs {}
			->fields(array(
  			'tid'     => $form_state['values']['tender_id'],
  			'atitle'  => $form_state['values']['add_title_add'],
  			'adesc'   => $form_state['values']['add_desc'],
  			'afid'    => $form_state['values']['add_upload'],
  			'created' => $updated_date,
  			'changed' => $updated_date,
			))->execute();
		//start chnage by Nitin 1 Feb 2013
    $file = file_load($form_state['values']['add_upload']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    file_usage_add($file, 'procurement', $file->filename, 1);
    $form_state['redirect'] = 'procurement/manage-tenders';
  }else {
  	
  	db_update('addendum') // Table name no longer needs {}
  		->fields(array(
  		  'atitle'  => $form_state['values']['add_title_add'],
  			'adesc'   => $form_state['values']['add_desc'],
  			'afid'    => $form_state['values']['add_upload'],
  			'changed' => $updated_date,
  	  ))
     ->condition('aid', $form_state['values']['aid'], '=')
     ->execute();
     
   //start chnage by Nitin 1 Feb 2013
    $add_file = addendum_load($form_state['values']['aid']);
    $file = file_load($form_state['values']['add_upload']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    file_usage_add($file, 'procurement', $file->filename, 1);
    //end chnage by Nitin 1 Feb 2013
  	
  	$form_state['redirect'] = 'procurement/manage-tenders';
  }
}

/*
 * function for delete selected addendum with file
 */
function addendum_delete($form, $form_state) {
	delete_addendum($form_state['values']['aid']);
}

function delete_addendum($aid) {
  $file = addendum_load($aid);
  db_delete('addendum')
    ->condition('aid', $aid)
    ->execute();
  drupal_goto('procurement/manage-tenders');
}

/*
 * function todisplay selected addendum 
 * with there all details to anonymous user
 */
function addendum_detail($tender = NULL, $addendum = NULL) {
	$file = file_load($addendum->afid);
	$theme_parameters = array(
											'records' => $addendum,
											'tender' => $tender,
											'file' => $file,
                      'headings' => 'View Addendum Details',
                      );
  return theme('addendum-view', $theme_parameters);
}

/*
 * function show the list of user that download tenders
 */
 
function tender_download_user() {
  $output = '';
  $table_rows = array();
  $header = array(
    array('data' => t('Person Name'), 'field' => 'tu.person_name', 'sort' => 'DESC'),
    array('data' => t('Agency Name')),
    array('data' => t('Tender Name')),
    array('data' => t('Tender Doc.')),
    array('data' => t('download date'), 'field' => 'td.created', 'sort' => 'DESC'), 
  );
  $limit = variable_get('codes_per_page',10);
  $query = db_select('tuser', 'tu')->extend('TableSort')->extend('PagerDefault')->limit($limit); 
  $query->join('tdownload', 'td', 'td.uid = tu.uid');
  $query->fields('td', array('tid', 'fid', 'created'));
  $query->fields('tu', array('agency_name', 'person_name'));
  $query->orderByHeader($header);
  $result = $query->execute();
  
  while($record = $result->fetchAssoc()) {
  	$file = file_load($record['fid']);
  	$tender = tender_load($record['tid']);
    $table_rows[] = array(
      array('data' => $record['person_name']),
      array('data' => $record['agency_name']),
      array('data' =>  $tender->title),
      array('data' => $file->filename),
      array('data' =>  format_date($record['created'], 'custom', 'D j M Y')),
      array('data' =>  format_date($record['created'], 'custom', 'D j M Y')),
    );
   
  }
  $output .= theme('table', array('header' => $header, 'rows' => $table_rows)). theme('pager');
  return $output;
}
