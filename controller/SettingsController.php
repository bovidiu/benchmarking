<?php
module_load_include('php', 'benchmarking', 'model/DbModelSQL');

class SettingsController {

  static private $_instance;

  static public function getInstance() {
    if (is_null(self::$_instance)) {
      self::$_instance=new SettingsController();
    }
    return self::$_instance;
  }

  public function recordWebsite($url){
    //check duplicate
    self::checkDuplicateWebsite($url);
    //record
    self::recordWebsiteName($url);
  }
  private function checkDuplicateWebsite($url){
    $duplicate = DbModelSQL::getInstance()->checkDuplicateWebsite($url);
    if(!empty($duplicate)){
      drupal_set_message('Website already registered.','error');
      return false;
    }
    return true;
  }
  private function recordWebsiteName($url){
    $arg =array(
      'website_name'  => $url,
      'activate'      => '1'
    );
    $record = DbModelSQL::getInstance()->insertWebsiteName($arg);

    if($record){
      drupal_set_message('Website recorded','status');
    }
    return true;
  }

  public function checkWebsiteActivation($arg){
    $db = DbModelSQL::getInstance()->checkActivationWebsite($arg);
    if($db[0]->activate != 1){
      return false;
    }
    return true;
  }
  public function getSettings(){
    return DbModelSQL::getInstance()->selectSettings();
  }

  public function recordSettings($arg){
    return DbModelSQL::getInstance()->insertWebsiteSettings($arg);
  }

    public function disableWebsite($arg){
        if(DbModelSQL::getInstance()->disableWebsite($arg)){
            drupal_set_message(t('Website disabled'),'warning');
            drupal_goto('admin/config/benchmarking');
        }
        return false;
    }
    public function activateWebsite($arg){
        if(DbModelSQL::getInstance()->activateWebsite($arg)){
            drupal_set_message(t('Website activated'),'status');
            drupal_goto('admin/config/benchmarking');
        }
        return false;
    }
  public function removeWebsite($arg){

      if(DbModelSQL::getInstance()->removeWebsiteFromList($arg)){
          drupal_set_message(t('Website removed'),'status');
          drupal_goto('admin/config/benchmarking');
      }
      return false;
  }

  public function getServerResults(){
      $location = "http://www.webpagetest.org/getLocations.php?f=json";
      header('Content-Type: application/json');
      return file_get_contents($location);
  }
} 