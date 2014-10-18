<?php

module_load_include('php', 'benchmarking', 'controller/SettingsController');

function benchmarking_admin_form() {

  $form['website'] = array(
    '#type' 	=> 'fieldset',
    '#title' 	=> t('<div class="title">Web performance URLs</div>'),
  );

  $form['website']['url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Add website'),
    '#size'          => 80,
    '#default_value' => '',
    '#value' 		 => '',
    '#required' 	 => TRUE,
    '#description'   => t('Add a new URL to the list.'),
  );

   $form['website']['websites'] = benchmarking_admin_list_websites(array(), array(), TRUE);

  $form['submit'] = array(
    '#type' 		=> 'submit',
    '#value' 		=> t('Save Website'),
    '#submit' 		=> array('benchmarking_validate_website'),
    '#collapsible'  => TRUE,
  );

  return $form;
}
function benchmarking_validate_website($form, &$form_state){
  if(!validURL($form_state["input"]["url"])){
    drupal_set_message('Invalid website URL','error');
    return false;
  }
  return SettingsController::getInstance()->recordWebsite($form_state["input"]["url"]);
}

function benchmarking_admin_list_websites($query = array(), $conditions = array(), $tableselect = FALSE){
  $query += array(
    'conditions' 	=> array(),
    'args' 			=> array(),
    'limit' 		=> 50
  );

  $header = array(
    'id_bwl' 		=> array('data' => t('#'), 'field' => 'id_bwl'),
    'website_name'	=> array('data' => t('Name'), 'field' => 'website_name'),
    'activation' 	=> array('data' => t('Activation'), 'field' => ''),
    'operations' 	=> array('data' => t('Operations'), 'field' => ''),
    'requests' 		=> array('data' => t('Requests'), 'field' => '')
  );

  $select = db_select('benchmarking_website_list', 'b')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->orderByHeader($header)
    ->limit($query['limit'])
    ->fields('b');

  foreach($query['conditions'] as $c){
    $select->condition($c['field'], $c['value'], $c['operator']);
  }


  $result = $select->execute();


  $websites = array();
  while($r = $result->fetchAssoc()){
    $websites[] = $r;
  }


  $rows = array();

  foreach($websites as $w){

    $row = array();

    foreach($header as $title => $data){
      if(isset($w[$data['field']]) && $title != 'operations'){
        if(strlen($w[$data['field']]) > 100){
          $row[$data['field']] = substr(htmlspecialchars($w[$data['field']]), 0, 100) . '...';
        }else{
          $row[$data['field']] = htmlspecialchars($w[$data['field']]);
        }
      }
    }


    $isOn = SettingsController::getInstance()->checkWebsiteActivation($w['id_bwl']);

    if(isset($header['activation'])){
      $activation = array();

      if($isOn){

        $activation["disable"] = array(
          'title' => t('Disable'),
          'href'  => 'benchmarking/disable-website/' . $w['id_bwl']
        );
      }else{

        $activation["enable"] = array(
          'title' => t('Enable' ),
          'href'  => 'benchmarking/activate-website/' . $w['id_bwl']
        );

      }

      $row['activation'] = theme_links(array(
        'links' => $activation,
        'attributes' => array(
          'class'  => array(
            'links',
            'inline nowrap'
          )
        ),
        'heading' => ''
      ));
    }


    if(isset($header['operations'])){
      $operations = array();
      $operations['del'] = array(
        'title' => t('Delete '),
        'href'  => 'benchmarking/remove-website/' . $w['id_bwl']
      );

      $row['operations'] = theme_links(array(
        'links' => $operations,
        'attributes' => array(
          'class'  => array(
            'links',
            'inline nowrap'
          )
        ),
        'heading' => ''
      ));
    }

    if(isset($header['requests'])){
      $requests = array();
      $requests['run'] = array(
        'title' => t('Run Test'),
        'href'  => 'benchmarking/make-run/'. $w['id_bwl']
      );

      $requests['getRun'] = array(
        'title' => t('Check Results '),
        'href'  => 'benchmarking/get-results-api/'. $w['id_bwl']
      );

      $row['requests'] = theme_links(array(
        'links' => $requests,
        'attributes' => array(
          'class'  => array(
            'links',
            'inline nowrap'
          )
        ),
        'heading' => ''
      ));
    }

    $rows[(string)$w['id_bwl']] = $row;
  }

  return array(
    '#markup' 			=> theme_table(array(
        'header' 		=> $header,
        'rows' 			=> $rows,
        'attributes' 	=> array(),
        'caption' 		=> '',
        'colgroups' 	=> array(),
        'sticky' 		=> true,
        'empty' 		=> t('No websites currently configured')
      )).theme('pager')
  );

}

function validURL($url) {
  if (!filter_var($url, FILTER_VALIDATE_URL, FILTER_FLAG_HOST_REQUIRED)){
    return false;
  }
  return true;
}

/* 
 * Settings page setup
 */

function benchmarking_settings_form() {
    drupal_add_js(drupal_get_path('module','benchmarking').'/js/location.js');
  $settings = SettingsController::getInstance()->getSettings();

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('<div class="title">Web performance settings</div>'),
  );

  $form['settings']['api'] = array(
    '#type'          	=> 'textfield',
    '#title'         	=> t('API'),
    '#size'         	=> 40,
    '#default_value'	=> '',
    '#value' 			=> (empty($settings[0]->api) ? '': $settings[0]->api),
    '#required' 			=> TRUE,
    '#description'   	=> t('WebPageTest Performance API'),
  );

  $form['settings']['runs'] = array(
    '#type'         	=> 'textfield',
    '#title'         	=> t('Runs'),
    '#size'         	=> 10,
    '#default_value' 	=> '',
    '#value' 			=> (empty($settings[0]->runs)? '': $settings[0]->runs ),
    '#required' 			=> TRUE,
    '#description'   	=> t('How many runs the script should do? Up to 9 runs available'),
  );
  $form['settings']['location'] = array(
    '#type'          	=> 'textfield',
    '#title'         	=> t('Location'),
    '#suffix'			=>'<div id="loadLocation"></div>',
    '#size'         	=> 20,
    '#default_value' 	=> '',
    '#value' 			=> (empty($settings[0]->location) ? '': $settings[0]->location ),
    '#description'   	=> t('<a href="#" id="selectServerLocation" >Select a server location.</a> Default location is Dulles, VA USA '),
  );

  $form['submit'] = array(
    '#type' 	=> 'submit',
    '#value' 	=> t('Save Configuration'),
    '#submit' 	=> array('benchmarking_settings_submit_form'),
  );

  return $form;
}

function benchmarking_settings_submit_form($form,  &$form_state){

  if($form_state["input"]["api"] == "" || $form_state["input"]["runs"] == ""){
    drupal_set_message(t("Fill in all fields"),"error");
  }else{
    $insert = array(
      "api" 				=> trim($form_state['input']['api']),
      "runs" 				=> trim($form_state['input']['runs']),
      "location"			=> $form_state['input']['location']
    );
    drupal_set_message(t("Settings recorded"),"status");
    return  SettingsController::getInstance()->recordSettings($insert);

  }

}





















