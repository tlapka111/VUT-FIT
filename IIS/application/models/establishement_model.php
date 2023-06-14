<?php
class Establishement_model extends CI_Model
{

	function loadEstablishements()
	{
		return $this->db->get('establishment')->result();
	}
        
        function getEstablishementById($id) {
            $this->db->where('establishmentId', $id);
            return $this->db->get('establishment')->row();
        }

        function ordersStart($id)
	{
		$this->db->set('isOpen','true');
		$this->db->where('establishmentId',$id);
		$this->db->update('establishment');
		return;
	}

	function ordersEnd($id)
	{
		$this->db->set('isOpen','false');
		$this->db->where('establishmentId',$id);
		$this->db->update('establishment');
		return;
	}
}
