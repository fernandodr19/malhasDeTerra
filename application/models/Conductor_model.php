<?php
	class Conductor_model extends CI_Model {
		public function __construct() {
			$this->load->database();
		}

		public function get_conductors($gsId) {
            $this->db->where('gsId', $gsId); 
            $this->db->from('conductors');
            $query = $this->db->get();
            return $query->result_array();
		}
        
        public function create_conductors($gsId) {
            
            $conductors = $this->input->post('conductors');
            $conductorCables = $this->input->post('conductorCables');
            for ($i = 0; $i < sizeof($conductors['x1']); $i++) {
                $conductorData = array(
                    'gsId' => $gsId,
                    'x1' => $conductors['x1'][$i],
                    'y1' => $conductors['y1'][$i],
                    'z1' => $conductors['z1'][$i],
                    'x2' => $conductors['x2'][$i],
                    'y2' => $conductors['y2'][$i],
                    'z2' => $conductors['z2'][$i],
                    'cableId' => $conductorCables[$i]
                 );

                $this->db->insert('conductors', $conductorData);
            }
		}

		public function delete_conductors($gsId) {
            $this->db->where("gsId", $gsId);
			$this->db->delete("conductors"); 
		}
	}
