<?php
	class GroundingSystem_model extends CI_Model {
		public function __construct() {
			$this->load->database();
		}

		public function get_groundingSystems($projectId = '') {
            if($projectId == '')
                return;
            
			$this->db->where('projectId', $projectId); 
            $this->db->from('groundingSystems');
            $query = $this->db->get();
            return $query->result_array();
		}
        
        public function get_groundingSystem($id = '') {
            if($id == '')
                return;
            
            $this->db->where('id', $id); 
            $this->db->from('groundingSystems');
			$query = $this->db->get();
			return $query->row_array();
        }
        
		public function create_groundingSystem($projectId) {
			$name = $this->input->post('newGSName');

			$projectData = array(
				'name' => $name,
                'projectId' => $projectId
			);
            
            return $this->db->insert('groundingSystems', $projectData);
		}

		public function update_groundingSystem($id) {
            if($id == '')
                return;
            
			$data = array(
				'name' => $this->input->post('gs_name')
			);

			$this->db->where("id", $id);
			return $this->db->update("groundingSystems", $data); 
		}
	}
