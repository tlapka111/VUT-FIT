<?php
class Ajax_model extends CI_Model{
 
    function search_blog($title){
        $this->db->like('city', $title , 'after');
        $this->db->distinct('city');
        $this->db->group_by('city'); 
        $this->db->order_by('city', 'ASC');
        $this->db->limit(10);
        return  $this->db->get('establishment')->result();   
    }
 
}