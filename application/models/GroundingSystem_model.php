<?php
	class GroundingSystem_model extends CI_Model {
		public function __construct() {
			$this->load->database();
		}

		public function get_groundingSystems($projectId) {
            $this->db->where('projectId', $projectId); 
            $this->db->from('groundingSystems');
            $query = $this->db->get();
            return $query->result_array();
		}
        
        public function get_groundingSystem($id) {
            $this->db->where('id', $id); 
            $this->db->from('groundingSystems');
			$query = $this->db->get();
			return $query->row_array();
        }
        
		public function create_groundingSystem($projectId) {
			$gsData = array(
                'projectId' => $projectId,
				'name' => $this->input->post('newGSName')
			);
            
            $this->db->insert('groundingSystems', $gsData);
            return $this->db->insert_id();
		}

		public function update_groundingSystem($id) {
            $data = array(
				'name' => $this->input->post('gs_name'),
                'conductorsMaxLength' => $this->input->post('gs_conductorsMaxLength'),
                'nLayers' => $this->input->post('gs_nLayers'),
                'firstLayerDepth' => $this->input->post('gs_firstLayerDepth'),
                'firstLayerResistivity' => $this->input->post('gs_firstLayerResistivity'),
                'secondLayerResistivity' => $this->input->post('gs_secondLayerResistivity'),
                'crushedStoneLayerDepth' => $this->input->post('gs_crushedStoneLayerDepth'),
                'crushedStoneLayerResistivity' => $this->input->post('gs_crushedStoneLayerResistivity'),
                'injectedCurrent' => $this->input->post('gs_injectedCurrent')
			);

			$this->db->where("id", $id);
			return $this->db->update("groundingSystems", $data); 
		}
	}
