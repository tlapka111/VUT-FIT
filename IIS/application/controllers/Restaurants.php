<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Restaurants extends CI_Controller
{
    public function __construct()
	{
		parent::__construct();
		$this->load->model('restaurant_model');
	}

	public function index()
	{
        $city =$this->input->post('mesto',TRUE);
        $this->load->view('templates/header');
        $establishements = $this->restaurant_model->loadEstablishements($city);
		$data['establishements'] = $establishements;
		$this->load->view('restaurants_view',$data);
        $this->load->view('templates/footer');
	}
}