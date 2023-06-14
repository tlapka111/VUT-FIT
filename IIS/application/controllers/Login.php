<?php

defined('BASEPATH') OR exit('No direct script access allowed');

class Login extends CI_Controller
{
	public function __construct()
	{
		parent::__construct();
		$this->load->model('user_model');
	}

	public function index()
	{
		$this->load->view('templates/header');
		$this->load->view('login_view');
		$this->load->view('templates/footer');
	}

	public function auth()
	{
		$email    = $this->input->post('email',TRUE);
		$password = md5($this->input->post('password',TRUE));
		$validate_user = $this->user_model->validate_login($email,$password);
		if ($validate_user->num_rows() > 0)
		{
			$data  = $validate_user->row_array();
			$id = $data['userId'];
			$role = $data['roleName'];
			$email = $data['email'];
			$firstName  = $data['firstName'];
			$lastName = $data['lastName'];

			$session_login_data = array(
				'id' => $id,
				'role' => $role,
				'email' => $email,
				'firstName' => $firstName,
				'lastName' => $lastName,
			);
			$this->session->set_userdata($session_login_data);
			redirect('Home');
		}
		else
		{
			echo $this->session->set_flashdata('error_msg','Špatný email nebo heslo');
			redirect('Login');
		}
	}

	public function logout()
	{
		$this->session->unset_userdata('id');
		$this->session->unset_userdata('email');
		$this->session->unset_userdata('role');
		$this->session->unset_userdata('firstName');
		$this->session->unset_userdata('lastName');
		redirect('Home');
	}
}
