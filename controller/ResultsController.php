<?php


class ResultsController {
    static private $_instance;

    static public function getInstance() {
        if (is_null(self::$_instance)) {
            self::$_instance=new ResultsController();
        }
        return self::$_instance;
    }
} 