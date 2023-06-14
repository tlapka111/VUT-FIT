<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class EstablishmentAdministratory extends CI_Controller
{
	public function __construct()
	{
		parent::__construct();
		$this->load->model('establishement_model');
	}

	public function closeOrder($establishement_id)
	{
		if (($this->session->userdata('role')) == 'admin' || (($this->session->userdata('role')) == 'operator'))
		{
			$this->establishement_model->ordersEnd($establishement_id);
			redirect(EstablishmentAdministratory);
		}
		else // Neoprávněný přístup doplnit error stranku
		{
			show_404();
		}
	}

	public function openOrder($establishement_id)
	{
		if (($this->session->userdata('role')) == 'admin' || (($this->session->userdata('role')) == 'operator'))
		{
		$this->establishement_model->ordersStart($establishement_id);
		redirect(EstablishmentAdministratory);
		}
		else // Neoprávněný přístup doplnit error stranku
		{
			show_404();
		}
	}
        
        public function editEstablishement($establishement_id)
	{
		if (($this->session->userdata('role')) == 'admin' || (($this->session->userdata('role')) == 'operator'))
		{
                    
                    
		$establishement = $this->establishement_model->getEstablishementById($establishement_id);
                $data['establishement'] = $establishement;
		//redirect(EstablishmentAdministratory);
                $this->load->view('templates/header');
			$this->load->view('edit-establishement_view', $data);
			$this->load->view('templates/footer');
		}
		else // Neoprávněný přístup doplnit error stranku
		{
			show_404();
		}
	}
        
        public function offerEstablishement($establishement_id)
	{
		if (($this->session->userdata('role')) == 'admin' || (($this->session->userdata('role')) == 'operator'))
		{
                    
                    
		$establishement = $this->establishement_model->getEstablishementById($establishement_id);
                $data['establishement'] = $establishement;
		//redirect(EstablishmentAdministratory);
                $this->load->view('templates/header');
			$this->load->view('offer-establishement_view', $data);
			$this->load->view('templates/footer');
		}
		else // Neoprávněný přístup doplnit error stranku
		{
			show_404();
		}
	}
        public function showEstablishement($establishement_id)
	{ 
		$establishement = $this->establishement_model->getEstablishementById($establishement_id);
                $data['establishement'] = $establishement;
                $this->load->view('templates/header');
		$this->load->view('establishement_view', $data);
		$this->load->view('templates/footer');
	}

	public function newEstablishement()
	{
		if (($this->session->userdata('role')) == 'admin' || (($this->session->userdata('role')) == 'operator'))
		{
			$this->load->view('templates/header');
			$this->load->view('new-establishement_view');
			$this->load->view('templates/footer');
		}
		else // Neoprávněný přístup doplnit error stranku
		{
			show_404();
		}
	}

	public function AddEstablishement()
	{
		$name    = $this->input->post('name',TRUE);
		$description = $this->input->post('description', TRUE);
		$priceCategory = $this->input->post('priceCategory',TRUE);
		$city = $this->input->post('city',TRUE);
		$street	 = $this->input->post('street',TRUE);
		$houseNumber = $this->input->post('houseNumber',TRUE);
		$test= $this->input->post('isOpen',TRUE);

		if($test == 'true')
		{
			$isOpen = 'true';
		}
		else
		{
			$isOpen = 'false';
		}
		$model = array('name' => $name, 'description'=> $description,'priceCategory'=>$priceCategory,'city'=>$city,'street'=>$street,'houseNumber'=>$houseNumber,'isOpen'=>$isOpen);
		$this->db->insert('establishment',$model);
		redirect('EstablishmentAdministratory');
	}

        public function SaveEstablishement() {
            $id = $this->input->post('establishmentId', TRUE);
            $name = $this->input->post('name',TRUE);
            $description = $this->input->post('description', TRUE);
            $priceCategory = $this->input->post('priceCategory',TRUE);
            $city = $this->input->post('city',TRUE);
            $street	 = $this->input->post('street',TRUE);
            $houseNumber = $this->input->post('houseNumber',TRUE);
            
            //$model = array('name' => $name, 'description'=> $description,'priceCategory'=>$priceCategory,'city'=>$city,'street'=>$street,'houseNumber'=>$houseNumber);
            $this->db->set('name', $name);
            $this->db->set('description', $description);
            $this->db->set('priceCategory', $priceCategory);
            $this->db->set('city', $city);
            $this->db->set('street', $street);
            $this->db->set('houseNumber', $houseNumber);
            $this->db->where('establishmentId',$id);
            $this->db->update('establishment');
            redirect('EstablishmentAdministratory');
        }

	public function index()
	{
		if (($this->session->userdata('role')) == 'admin' || (($this->session->userdata('role')) == 'operator'))
		{
			$establishements = $this->establishement_model->loadEstablishements();
			$this->load->view('templates/header');
			$data['establishements'] = $establishements;
			$this->load->view('establishment-administratory_view',$data);
			$this->load->view('templates/footer');
		}
		else // Neoprávněný přístup doplnit error stranku
		{
			show_404();
		}
	}
}
