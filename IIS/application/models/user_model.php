<?php
class User_model extends CI_Model
{
	function validate_login($email,$password){
		$this->db->where('email',$email);
		$this->db->where('password',$password);
		$result = $this->db->get('users',1);
		return $result;
	}

	function getAllUsers()
	{
		$this->db->order_by('lastName ASC');
		return $this->db->get('users')->result();
	}

	function getAllwithRole($role)
	{
		$this->db->where('roleName',$role);
		$this->db->order_by('lastName ASC');
		return $this->db->get('users')->result();
	}
	function getUserById($id)
	{
		$this->db->where('userId',$id);
		return $this->db->get('users')->row();
	}
	function getByEmail($email)
	{
		$this->db->where('email',$email);
		return $this->db->get('users')->result();
	}

	function getByName($firstName, $lastName)
	{
		$this->db->where('firstName',$firstName);
		$this->db->where('lastName',$lastName);
		return $this->db->get('users')->result();
	}

	function getByCity($city)
	{
		$this->db->where('deliveryCity',$city);
		return $this->db->get('users')->result();
	}

	function updateProfileByUser($id, $firstName, $lastName, $phoneNumber, $city, $street, $houseNumber)
	{
		$this->db->where('userId',$id);
		$this->db->set('firstName',$firstName);
		$this->db->set('lastName',$lastName);
		$this->db->set('phoneNumber',$phoneNumber);
		$this->db->set('deliveryCity',$city);
		$this->db->set('deliveryStreet',$street);
		$this->db->set('deliveryHouseNumber',$houseNumber);
		$this->db->update('users');
	}


	function updateProfileByAdmin($id, $firstName, $lastName, $phoneNumber, $role, $city, $street, $houseNumber)
	{
		$this->db->where('userId',$id);
		$this->db->set('firstName',$firstName);
		$this->db->set('lastName',$lastName);
		$this->db->set('phoneNumber',$phoneNumber);
		$this->db->set('roleName',$role);
		$this->db->set('deliveryCity',$city);
		$this->db->set('deliveryStreet',$street);
		$this->db->set('deliveryHouseNumber',$houseNumber);
		$this->db->update('users');
	}
}
