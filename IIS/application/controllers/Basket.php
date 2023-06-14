<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Basket extends CI_Controller
{
	public function index()
	{
		$this->load->view('templates/header');
		$this->load->view('basket_view');
		$this->load->view('templates/footer');
	}
}
