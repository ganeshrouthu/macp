<?php
define(LANG,'und');


/**
 * Display the list of all success-stories content type
 */
function job_listings() {
  global $user,$base_path;
  $header = array('Job Title', 'Candidate Name', 'Applied Date', 'Resume');
  $header = array(
    'job_title' => array('data' => t('Job Title'), 'field' => 'n.job_title'),
    'app_name' => array('data' => t('Candidate Name')),
    'app_date' => array('data' => t('Applied Date'), 'field' => 'n.app_date'),
    'resume' => array('data' => t('Resume')),
  );
  
 
  $nodes = node_load_multiple(array(), array('type' => 'job_application'));
  foreach($nodes as $node){
  	$job_id = $node->field_job_id['und'][0]['value'];
    $job_detail = node_load($job_id);
		$file_path = str_replace('public://', $base_path . 'sites/default/files/', $node->field_upload_your_resume['und'][0]['uri']);
    $options[$node->nid] = array(
      'job_title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $job_detail->title,
          '#href' => 'node/' . $job_detail->nid,
        ),
      ),
      'app_name' => $node->title,
      'app_date' => format_date($node->created, 'custom', 'D j M Y'),
      'resume' => '<a href =' . $file_path . '>Download</a>',
    );
  }
  
   
  $form['accounts'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No people available.'),
  );
  $form['pager'] = array('#markup' => theme('pager'));
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#attributes' => array('onclick' => 'if(!confirm("Really Delete?")){return false;}'),
    '#validate' => array('job_nodes_validate'),
    '#submit' => array('job_listings_submit'),
  );
  
  return $form;
  
  
}

/**
 * Validate node_admin_nodes form submissions.
 *
 * Check if any nodes have been selected to perform the chosen
 * 'Update option' on.
 */
function job_nodes_validate($form, &$form_state) {
  // Error if there are no items to select.
  if (!is_array($form_state['values']['accounts']) || !count(array_filter($form_state['values']['accounts']))) {
    form_set_error('', t('No items selected.'));
  }
}


function job_listings_submit($form, &$form_state) {
  global $user;
  
  $nodes = array_filter($form_state['values']['accounts']);
  if ($user->uid) {
    node_delete_multiple(array_values($form_state['values']['accounts']));
    $count = count(array_filter($form_state['values']['accounts']));
    $message = t("Deleted @count posts.", array('@count' => $count));
    drupal_set_message($message);
    watchdog('content', 'Deleted @count posts.', array('@count' => $count));
  }
  else {
    // We need to rebuild the form to go to a second step. For example, to
    // show the confirmation form for the deletion of nodes.
    $form_state['rebuild'] = TRUE;
  }
}
