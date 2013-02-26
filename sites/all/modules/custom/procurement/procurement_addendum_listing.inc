<?php
/**
 * add addendum to selected tender
 */
function procurement_add_addendum($form, $form_state, $tender = NULL, $addendum = NULL) {
  
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
    
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 15,
      '#submit' => array('addendum_delete'),
      '#attributes' => array('class' => array('delete-add')),
    );
  	
  }else {
    $form['action'] = array(
    	'#type' => 'hidden', 
    	'#default_value' => 'add', 
  	);
  }
  
  
  $form['add_title_add'] = array(
    '#type' => 'textfield', 
    '#title' => t('Title of the addendum'),
    '#size' => 60, 
    '#maxlength' => 128, 
    '#default_value' => empty($addendum->atitle) ? '' : $addendum->atitle, 
    '#required' => TRUE,
  );
  
  $form['add_desc'] = array(
  	'#title' => t('Brief of the addendum'),
    '#type' => 'textarea',
    '#default_value' => empty($addendum->adesc) ? '' : $addendum->adesc, 
    '#cols' => 60, 
    '#rows' => 5,
  );
  
  $form['add_upload'] = array(
    '#type' => 'managed_file',
    '#title' => t('Upload addendum'),
    '#upload_location' => 'public://addendum/',
    '#required' => TRUE,
    '#default_value' => empty($addendum) ? '' :$addendum->afid,
 	);
 	
 	$form['add_submit'] = array(
 	  '#type' => 'submit', 
 	  '#value' => empty($addendum) ? t('Add Addendum') :t('Edit Addendum')
 	);
 	return $form;
}


/* 
 * hook_submit for add_addendum form to selected tender
 */
function procurement_add_addendum_submit($form, &$form_state) {
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
    
     
     $form_state['redirect'] = 'procurement/tenders';
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
  	
  	//$result = db_query("INSERT INTO {tender} (title, tdesc, tfid, created, changed) VALUES ('".$form_state['values']['title']."', '".$form_state['values']['desc']."', ".$form_state['values']['upload'].", ".$created_date.", ".$created_date.")");
  	$form_state['redirect'] = 'procurement/tenders';
  }
}

/*
 * function for delete selected addendum with file
 */
function addendum_delete($form, $form_state) {
  db_delete('addendum')
    ->condition('aid', $form_state['values']['aid'])
    ->execute();
  drupal_goto('/admin/tenders');
}

/*
 * function todisplay selected addendum 
 * with there all details to anonymous user
 */
function addendum_detail($tender = NULL, $addendum = NULL) {
	$file = file_load($addendum->afid);
  $output = '';
  $output .= '<div class="">';
  $output .= '<table>';
  $output .= '<tr><td align="center" colspan=2><b>'.$addendum->atitle . ' details</b></td></tr>';
  $output .= '<tr><td>Tender Title</td><td>' . $tender->title .'</td></tr>';
  $output .= '<tr><td>Addendum Title</td><td>' . $addendum->atitle .'</td></tr>';
  $output .= '<tr><td>Addendum Desc</td><td>' . $addendum->adesc .'</td></tr>';
  $output .= '<tr><td>Upload Doc</td><td><a href =' . file_create_url($file->uri) . '>' . $file->filename . '</a></td></tr>';
  $output .= '</table>';
  $output .= '</div>';
  
  return $output;
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
    );
   
  }
  $output .= theme('table', array('header' => $header, 'rows' => $table_rows)). theme('pager');
  return $output;
}
  
