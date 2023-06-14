<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Home extends CI_Controller
{
    function __construct()
    {
        parent::__construct();
        $this->load->model('Ajax_model');
    }
    
    public function index()
    {
        $this->load->view('templates/header');
        $this->load->view('homepage_view');
        $this->load->view('templates/footer');
    }

    //autocomplete v navbaru
    function get_autocomplete()
    {
        if (isset($_GET['term'])) 
        {
            $result = $this->Ajax_model->search_blog($_GET['term']);
        
            if (count($result) > 0) 
            {
                foreach ($result as $row)
                    $arr_result[] = $row->city;
                echo json_encode($arr_result);
            }
        }
    }
}