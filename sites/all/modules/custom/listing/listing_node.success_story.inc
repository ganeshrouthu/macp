<?php
/**
 * Display the list of all success-stories content type
 */
function listing_success_story() {
  global $user;
  $header = array(
    'title' => array('data' => t('Title'), 'field' => 'n.title'),
    'operations' => array('data' => t('Operations')),
  );
  
 
  $nids = db_select('node', 'n')
    ->fields('n',array('nid'))
    ->condition('type', 'success_story','=')
    ->condition('uid', $user->uid, '=')
    ->addTag('node_access')
    ->execute()
    ->fetchCol();
  //dsm($nids, 'nid');
  $nodes = node_load_multiple($nids);
  
  $destination = drupal_get_destination();
  $options = array();
  foreach($nodes as $node){
    $options[$node->nid] = array(
        'title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $node->title,
          '#href' => 'node/' . $node->nid,
        ),
      ),
      'operations' => array(
        'data' => array(
      		'#type' => 'link', 
      		'#title' => t('edit'), 
      		'#href' => "node/$node->nid/edit", 
      		'#options' => array('query' => $destination)
      	)
      ),
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
    '#validate' => array('listing_nodes_validate'),
    '#submit' => array('listing_success_story_submit'),
  );
  
  return $form;
  
  
}

/**
 * Validate node_admin_nodes form submissions.
 *
 * Check if any nodes have been selected to perform the chosen
 * 'Update option' on.
 */
function listing_nodes_validate($form, &$form_state) {
  // Error if there are no items to select.
  if (!is_array($form_state['values']['accounts']) || !count(array_filter($form_state['values']['accounts']))) {
    form_set_error('', t('No items selected.'));
  }
}


function listing_success_story_submit($form, &$form_state) {
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
