<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class UserAdministratory extends CI_Controller
{
	public function __construct()
	{
		parent::__construct();
		$this->load->model('user_model');
	}

	public function index()
	{
		if (($this->session->userdata('role')) == 'admin')
		{
			$role = '';
			$users = null;

			if($this->input->post('role',FALSE))
			{
				$tmp  = $this->input->post('role',TRUE);
				if($tmp == "běžný uživatel")
				{
					$role = 'user';
				}
				else if($tmp == "admin")
				{
					$role = 'admin';
				}

				else if($tmp == "řidič")
				{
					$role = 'driver';
				}

				else if($tmp == "operátor")
				{
					$role = 'operator';
				}

				else if($tmp == "všichni")
				{
					$role = 'all';
				}
			}
			if ($role == '' || $role == 'all')
			{
				$users = $this->user_model->getAllUsers();
			}
			else
			{
				$users= $this->user_model->getAllwithRole($role);
			}

			$data['users'] = $users;
			$this->load->view('templates/header');
			$this->load->view('user-administratory_view',$data);
			$this->load->view('templates/footer');

		}
		else
		{
			show_404();
		}
	}

	function  SeachUser()
	{
		if (($this->session->userdata('role')) == 'admin') {
			$shouldRender = false;
			$users = null;
			$noData = false;
			if ($this->input->post('email', TRUE))
			{
				$email = $this->input->post('email', TRUE);
				$shouldRender = true;
				$users = $this->user_model->getByEmail($email);
			}

			if ($this->input->post('name', FALSE))
			{
				$firstName = $this->input->post('name', TRUE);
				$lastName = $this->input->post('lastName', TRUE);
				$shouldRender = true;
				$users = $this->user_model->getByName($firstName,$lastName);
				foreach ($users as $user)
				{

					echo $user->firstName;
				}
			}

			if ($this->input->post('city', TRUE))
			{
				$city = $this->input->post('city', TRUE);
				$shouldRender = true;
				$users = $this->user_model->getByCity($city);
			}

			if ($users == null)
			{
				$noData = true;
			}

			$data['shouldRender'] = $shouldRender;
			$data['noData'] = $noData;
			$data['users'] = $users;
			$this->load->view('templates/header');
			$this->load->view('search-user_view', $data);
			$this->load->view('templates/footer');
		}
		else{ show_404();}
	}


	function editUser ($userId)
	{
		if (($this->session->userdata('role')) == 'admin')
		{
			$user = $this->user_model->getUserById($userId);

			if ($user == null)
			{
				show_404();
			}

			$firstName = '';
			$lastName = '';
			$phoneNumber = '';
			$role = '';
			$city = '';
			$street = '';
			$houseNumber = '';

			if($this->input->post('firstName',TRUE))
			{
				$firstName = $this->input->post('firstName',TRUE);
				$lastName = $this->input->post('lastName',TRUE);
				$phoneNumber = $this->input->post('phoneNum',TRUE);
				if($this->input->post('role',TRUE))
				{
					$tmp  = $this->input->post('role',TRUE);
					if($tmp == "běžný uživatel")
					{
						$role = 'user';
					}
					else if($tmp == "admin")
					{
						$role = 'admin';
					}

					else if($tmp == "řidič")
					{
						$role = 'driver';
					}

					else if($tmp == "operátor")
					{
						$role = 'operator';
					}
				}
				$city  = $this->input->post('city',TRUE);
				$street  = $this->input->post('street',TRUE);
				$houseNumber = $this->input->post('houseNumber',TRUE);
				$this->user_model->updateProfileByAdmin($userId, $firstName, $lastName, $phoneNumber, $role, $city, $street, $houseNumber);
				echo $this->session->set_flashdata('ok_msg','Změna dat proběhla úspěšně');
			}



			$user = $this->user_model->getUserById($userId);
			$selectedValue  = null;

			if($user->roleName == "admin")
			{
				$selectedValue = '1';
			}
			else if($user->roleName == 'operator')
			{
				$selectedValue = '2';
			}
			else if ($user->roleName == 'driver')
			{
				$selectedValue = '3';
			}
			else
			{
				$selectedValue = '4';
			}

			$data['selectedValue']  = $selectedValue;

			if($user == null)
			{
				show_404();
			}
			else
			{
				$data['user'] = $user;
				$this->load->view('templates/header');
				$this->load->view('edit-user_view',$data);
				$this->load->view('templates/footer');
			}
		}
		else
		{
			show_404();
		}
	}
}
