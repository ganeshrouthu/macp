<?php
define(LANG,'und');

/*
 * Display list of question ask by logged in user
 */

function ask_expert_table() {
	global $user,$base_path;
  
  $output = '';
  
  $output .= '<div class="ask_ques"><a href =' . $base_path . 'ask_expert/all>Question ask by all user</a></div>';
  $table_rows = array();
  
  $header = array(
    array('data' => t('Title'), 'field' => 'n.title', 'sort' => 'DESC'),
    array('data' => t('Replies')),
    array('data' => t('Last reply')),
    array('data' => t('Created date'), 'field' => 'n.created', 'sort' => 'DESC'),
  );
  $limit = variable_get('codes_per_page',5);
  
  $query = db_select('node', 'n')->extend('PagerDefault')->limit($limit); 
  $query->fields('n', array('nid', 'title', 'created'))
        ->extend('TableSort')  // Add table sort extender.
        ->condition('type', 'ask_expert', '=')
        ->condition('uid', $user->uid, '=')
        ->orderByHeader($header);  // Add order by headers.
  $result = $query->execute();
  
  foreach($result as $stat_line) {
  	$node = node_load($stat_line->nid);
  	if($node->cid != 0) {
  	  $comment = comment_load($node->cid);
  	  $details = 'By ' . $comment->name . ' ' . format_date($comment->created, 'custom', 'D j M Y');
  	}else {
  	  $details = 'n/a';
  	}
    $table_rows[] = array(
      array('data' => array(
            '#type' => 'link',
            '#title' => $stat_line->title,
            '#href' => 'node/' . $stat_line->nid,
          )),
      array('data' => $node->comment_count),
      array('data' => $details),
      array('data' =>  format_date($node->created, 'custom', 'D j M Y')),
    );
  }
  $output .= theme('table', array('header' => $header, 'rows' => $table_rows)). theme('pager');
  return $output;
}


/*
 * Display list of question ask by logged in user
 */

function ask_expert_all_table() {
	global $user,$base_path;
  $user_name  = '';
  $output = '';
  if (!in_array('administrator', array_values($user->roles))) {
  	$output .= '<a href =' . $base_path . 'ask_expert/my>Question Ask By Me</a>';
  }
  $table_rows = array();
  
  $header = array(
    array('data' => t('Title'), 'field' => 'n.title', 'sort' => 'DESC'),
    array('data' => t('User Name')),
    array('data' => t('Replies')),
    array('data' => t('Last reply')),
    array('data' => t('Created date'), 'field' => 'n.created', 'sort' => 'DESC')
  );
  $limit = variable_get('codes_per_page',5);
  
  $query = db_select('node', 'n')->extend('PagerDefault')->limit($limit); 
  $query->fields('n', array('nid', 'title'))
        ->extend('TableSort')  // Add table sort extender.
        ->condition('type', 'ask_expert', '=')
        ->orderByHeader($header);  // Add order by headers.
        
  $result = $query->execute();
  
  foreach($result as $stat_line) {
  	$node = node_load($stat_line->nid);
  	if($node->cid != 0) {
  	  $comment = comment_load($node->cid);
  	  $details = 'By ' . $comment->name . ' ' . format_date($comment->created, 'custom', 'D j M Y');
  	}else {
  	  $details = 'n/a';
  	}
  	
    if(!empty($node->name)) {
      $user_name = $node->name;
    }else {
      $user_name = 'anonymous ';
    }
    $table_rows[] = array(
      array('data' => array(
            '#type' => 'link',
            '#title' => $stat_line->title,
            '#href' => 'node/' . $stat_line->nid,
          )),
      array('data' => $user_name),
      array('data' => $node->comment_count),
      array('data' => $details),
      array('data' =>  format_date($node->created, 'custom', 'D j M Y')),
    );
  }
  $output .= theme('table', array('header' => $header, 'rows' => $table_rows)). theme('pager');
  return $output;
}
