<?php

class DbModelSQL {

  private static $_instance;
  private $_conn;

  public function __construct() {
    $this->_conn = Database::getConnection();
  }

  public static function getInstance() {
    if (!isset(self::$_instance)) {
      self::$_instance = new DbModelSQL();
    }
    return self::$_instance;
  }
  /*CHECKS*/
  public function checkDuplicateWebsite($url){
    $res = $this->_conn->query("SELECT * FROM benchmarking_website_list WHERE website_name ='$url' LIMIT 1");
    $arr=array();
    while($row = $res->fetchObject()){
      $arr[] = $row;
    }
    return $arr;
  }
  public function checkActivationWebsite($arg){
    $res = $this->_conn->query("SELECT activate FROM benchmarking_website_list WHERE id_bwl ='$arg' LIMIT 1");
    $arr=array();
    while($row = $res->fetchObject()){
      $arr[] = $row;
    }
    return $arr;
  }
  /*SELECTS*/
  public function selectSettings(){
    $res = $this->_conn->query("SELECT * FROM benchmarking_website_settings ORDER BY id_bws DESC LIMIT 1");
    $arr=array();
    while($row = $res->fetchObject()){
      $arr[] = $row;
    }
    return $arr;
  }
    public function selectWebsiteURL($arg){
        $res = $this->_conn->query("SELECT website_name FROM benchmarking_website_list WHERE id_bwl = '$arg' ");
        $arr=array();
        while($row = $res->fetchObject()){
            $arr["website_name"] = $row->website_name;
        }
        return $arr;
    }

  /*UPDATE*/
    public function disableWebsite($arg){
        $res = $this->_conn->update("benchmarking_website_list")
            ->fields(array(
                'activate' => '0'
            ))
            ->condition('id_bwl',$arg,'=')
            ->execute();
        return $res;
    }
    public function activateWebsite($arg){
        $res = $this->_conn->update("benchmarking_website_list")
            ->fields(array(
                'activate' => '1'
            ))
            ->condition('id_bwl',$arg,'=')
            ->execute();
        return $res;
    }

  /*INSERT*/
  public function insertWebsiteName($arg){
    $res = $this->_conn->insert("benchmarking_website_list")->fields($arg)->execute();
    return $res;
  }
  public function insertWebsiteSettings($arg){
    $res = $this->_conn->insert("benchmarking_website_settings")->fields($arg)->execute();
    return $res;
  }
    public function insertWebsiteTrackingCode($arg){
        $res = $this->_conn->insert("benchmarking_website_tracking_data")->fields($arg)->execute();
        return $res;
    }
  /*DELETE*/
  public function removeWebsiteFromList($arg){
      $res = $this->_conn->delete('benchmarking_website_list')
          ->condition('id_bwl', $arg,'=')
          ->execute();
      return $res;
  }

} 