<?php
function rei_import_users($url = FALSE) {
  if (!$url || strlen($url) < 4) {
    return TRUE;
  }
  $users = array();
  $prefixes = array('Mr.', 'Miss.', 'Mrs.');
  $pref_replace = array('', '', '');

  if (($handle = fopen($url, "r")) !== FALSE) {
    fgetcsv($handle, 1000, ',');
    $imports = array();
    while (($data = fgetcsv($handle, 1000, ",")) !== FALSE)  {
      $user_name = trim(str_replace($prefixes, $pref_replace, $data[0]));
      if (strlen($user_name)) {
        $name = explode(' ', $user_name);
        $first_name = trim($name[0]);
        $final_last_name = '';
        $cnt = count($name);
        if ($cnt > 1) {
          $last_name = trim(end($name));
          $last_dot = strrpos($last_name, '.');
          $final_last_name = $last_name;
          if ($last_dot) {
            $final_last_name = substr($last_name, ($last_dot+1));
          }
        }
        if (strlen($final_last_name)) {
          $drupal_username = strtolower($first_name .'_'.$final_last_name);
        }
        else {
          $drupal_username = strtolower($first_name);
        }
        $drupal_pwd = $drupal_username;
        $email = $drupal_username.'@macp.com';
        $fields = array(
          'name' => $drupal_username,
          'mail' => $email,
          'pass' => $drupal_pwd,
          'status' => 1,
          'init' => $email,
          'timezone' => 'Asia/Kolkata',
          'roles' => array(DRUPAL_AUTHENTICATED_RID => 'authenticated user',),
        );
        $users[] = $fields;
      }
    }
    if (!_check_user_exists($users)) {
      _save_user($users);
    }
  }
  fclose($handle);
}

function _save_user($users = array()) {
  if (empty($users)) {
    return TRUE;
  }
  foreach ($users as $fields) {
    //set up the user fields
    extract($fields);
    $fields = array(
      'name' => $name,
      'mail' => $mail,
      'pass' => $pass,
      'status' => 1,
      'init' => $mail,
      'timezone' => 'Asia/Kolkata',
      'roles' => array('authenticated user',),
    );
    //the first parameter is left blank so a new user is created
    $account = user_save('', $fields);
  }
}

function _get_all_users() {
  $query = db_select('users', 'u');
  $query->fields('u', array('name'));
  $query->fields('u', array('mail'));
  $result = $query->execute();
  $records = array();
  while($record = $result->fetchAssoc()) {
    if ($record['name'] && $record['mail']) {
      $records[$record['name']] = $record['mail'];
    }
  }
  return $records;
}

function _check_user_exists($users) {
  $db_users = _get_all_users();
  $exists = 0;
  if (!empty($users)) {
    foreach ($users as $user) {
      extract($user, EXTR_PREFIX_ALL, 'U');
      if (in_array($U_name, array_keys($db_users)) || in_array($U_mail, $db_users)) {
        $exists++;
      }
    }
  }
  return ($exists > 0) ? TRUE : FALSE;
}