<?php
	class Point_model extends CI_Model {
		public function __construct() {
			$this->load->database();
		}

		public function get_points($gsId) {
            $this->db->where('gsId', $gsId);
            $this->db->from('points');
            $query = $this->db->get();
            return $query->result_array();
		}
        
        public function create_points($gsId) { 
            $points = $this->input->post('points');
            for ($i = 0; $i < sizeof($points['x']); $i++) {
                $conductorData = array(
                    'gsId' => $gsId,
                    'x' => $points['x'][$i],
                    'y' => $points['y'][$i]
                 );

                $this->db->insert('points', $conductorData);
            }
		}

		public function delete_points($gsId) {
            $this->db->where("gsId", $gsId);
			$this->db->delete("points"); 
		}
	}
