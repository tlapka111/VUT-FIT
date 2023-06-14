<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class UserProfile extends CI_Controller
{
	public function __construct()
	{
		parent::__construct();
		$this->load->model('user_model');
	}

	function detail($userId)
	{
		if ((($this->session->userdata('role')) == 'admin') || (($this->session->userdata('id')) == $userId))
		{
			$user = $this->user_model->getUserById($userId);
			$role = $this->session->userdata('role');

			$data['user'] = $user;
			$data['role'] = $role;

			$this->load->view('templates/header');
			$this->load->view('profile-detail_view',$data);
			$this->load->view('templates/footer');
		}
		else
		{
			show_404();
		}
	}


	function edit($userId)
	{
		if ((($this->session->userdata('role')) == 'admin') || (($this->session->userdata('id')) == $userId))
		{
			if($this->input->post('firstName',FALSE)){
				$firstName = $this->input->post('firstName',TRUE);
				$lastName = $this->input->post('lastName',TRUE);
				$phoneNumber = $this->input->post('phoneNumber',TRUE);
				$city = $this->input->post('city',TRUE);
				$street = $this->input->post('street',TRUE);
				$houseNumber = $this->input->post('houseNumber',TRUE);

				$this->user_model->updateProfileByUser($userId, $firstName, $lastName, $phoneNumber, $city, $street, $houseNumber);
				redirect(base_url()."detail-profilu/".$userId);
			}

			$user = $this->user_model->getUserById($userId);
			$data['user'] = $user;

			$this->load->view('templates/header');
			$this->load->view('profile-edit_view',$data);
			$this->load->view('templates/footer');
		}
		else
		{
			show_404();
		}
	}
}
