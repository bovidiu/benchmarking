<?php

module_load_include('php', 'benchmarking', 'model/DbModelSQL');

class RunController {
    static private $_instance;

    static public function getInstance() {
        if (is_null(self::$_instance)) {
            self::$_instance=new RunController();
        }
        return self::$_instance;
    }
    public function initRun($arg){
        $urlResults =  self::makeURLRequest($arg);
        $dataRecording = self::recordData($urlResults,$arg);
        return $dataRecording;
    }
    private function getGeneralSettings(){
        return DbModelSQL::getInstance()->selectSettings();
    }
    private function getWebsiteName($arg){
        return DbModelSQL::getInstance()->selectWebsiteURL($arg);
    }
    private function getSettings(){
        $settings = self::getGeneralSettings();
        $makeRequestURL= "f=json&k=".$settings[0]->api."&runs=".$settings[0]->runs;
        return $makeRequestURL;
    }
    private function makeURLRequest($arg){
        $settings = self::getSettings();
        $website = self::getWebsiteName($arg);
        return file_get_contents("http://www.webpagetest.org/runtest.php?".$settings.'&url='.$website["website_name"]);
    }
    private function recordData($arg,$websiteID){
        $data = json_decode($arg);

        $data = array(
            'code'      => $data->data->testId,
            'date'      => time(),
            'fk_bid'    => $websiteID
        );
        DbModelSQL::getInstance()->insertWebsiteTrackingCode($data);
        drupal_set_message(t('Test running.'),'status');
        drupal_goto('admin/config/benchmarking');
        return true;
    }
} 