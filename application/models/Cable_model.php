<?php
	class Cable_model extends CI_Model {
		public function __construct() {
			$this->load->database();
		}

		public function get_cables() {
            $this->db->from('cables');
            $query = $this->db->get();
            return $query->result_array();
		}
        
        public function get_cable($id) {
            $this->db->where('id', $id); 
            $this->db->from('cables');
            $query = $this->db->get();
            return $query->row_array();
		}
	}
