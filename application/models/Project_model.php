<?php
	class Project_model extends CI_Model {
		public function __construct() {
			$this->load->database();
		}

		public function get_projects($pName = '') {
			$this->db->where('userEmail', $this->session->userdata('email')); 

			if($pName == '') {
				$query = $this->db->get('projects');
				return $query->result_array();
			}

			$query = $this->db->get_where('projects', array('pName' => $pName));
			return $query->row_array();
		}

		public function create_project() {
			$pName = $this->input->post('project_name');

			$data = array(
				'pName' => $pName,
				'userEmail' => $this->session->userdata('email') //checkHere trocar para id
			);
			
			return $this->db->insert('projects', $data);
		}

		public function update_project($oldName) {
			$project = array(
				"pName" => $this->input->post('project_name')
			);

			$this->db->where("pName", $oldName);
			$this->db->update("projects", $project); 
		}
	}
