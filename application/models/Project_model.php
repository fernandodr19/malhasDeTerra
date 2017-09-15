<?php
	class Project_model extends CI_Model {
		public function __construct() {
			$this->load->database();
		}

		public function get_projects() {
			$this->db->where('userId', $this->session->userdata('id')); 
            $this->db->from('userProjects');
            $this->db->join('projects', 'userProjects.projectId= projects.id');
            $query = $this->db->get();
            return $query->result_array();
		}
        
        public function get_project($id = '') {
            $this->db->where('userId', $this->session->userdata('id')); 
            $this->db->where('id', $id);
            $this->db->from('userProjects');
            $this->db->join('projects', 'userProjects.projectId= projects.id');
			$query = $this->db->get();
			return $query->row_array();
        }

		public function create_project() {
			$name = $this->input->post('newProjectName');

			$projectData = array(
				'name' => $name
			);
            $this->db->insert('projects', $projectData);
            
            $userProjectsData = array(
                'userId' => $this->session->userdata('id'),
                'projectId' => $this->db->insert_id(),
                'owner' => true,
                'readOnly' => false
            );
			
			return $this->db->insert('userProjects', $userProjectsData);
		}

		public function update_project($id) {
			$data = array(
				'name' => $this->input->post('project_name'),
                'substation' => $this->input->post('project_substation'),
                'description' => $this->input->post('project_description'),
                'designer' => $this->input->post('project_designer')
			);

			$this->db->where("id", $id);
			$this->db->update("projects", $data); 
		}
	}
