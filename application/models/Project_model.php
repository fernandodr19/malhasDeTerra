<?php
	class Project_model extends CI_Model {
		public function __construct() {
			$this->load->database();
		}

		public function get_projects($pName = '') {
			if($pName == '') {
				$query = $this->db->get('projects');
				return $query->result_array();
			}

			$query = $this->db->get_where('projects', array('pName' => $pName));
			return $query->row_array();
		}

		public function create_project() {
			$this->load->library('form_validation');

			$this->form_validation->set_rules('newProjectName', 'NewProjectName', 'required');

			if ($this->form_validation->run() == FALSE){
				$this->form_validation->set_message("The %s value does not exist");//checkHere validation
			} else {
				$pName = $this->input->post('newProjectName');

				$data = array(
					'pName' => $pName,
					'userEmail' => 'fdr'	//checkHere sesion..
				);
				
				return $this->db->insert('projects', $data);
			}
		}
	}
