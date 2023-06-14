<?php
class Restaurant_model extends CI_Model
{

	function loadEstablishements($city)
	{
		if ($city == "")
		{
			$query = $this->db->get('establishment')->result();
			return $query;
		}
		$query = $this->db->get_where('establishment', array('city' => $city))->result();
		return $query;
    }
}