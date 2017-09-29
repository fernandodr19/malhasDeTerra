<?php
	class Projects extends CI_Controller {
		public function index() {
			$data['title'] = 'Projects';

			$this->load->view('templates/header');
			$this->load->view('projects/index', $data);
			$this->load->view('templates/footer');
		}

		public function view($projectId = '', $tab = '') {
			$data['project'] = $this->project_model->get_project($projectId);
            $data['gs'] = $this->groundingSystem_model->get_groundingSystem($data['project']['lastGsId']);
            $data['conductors'] = $this->conductor_model->get_conductors($data['project']['lastGsId']);
            $data['points'] = $this->point_model->get_points($data['project']['lastGsId']);
            $data['profiles'] = $this->profile_model->get_profiles($data['project']['lastGsId']);
            $data['cables'] = $this->cable_model->get_cables();
            
            if(empty($data['gs']) && isset($this->groundingSystem_model->get_groundingSystems($projectId)[0]))
                    $data['gs'] = $this->groundingSystem_model->get_groundingSystems($projectId)[0];
            
            if($tab == '')
                $tab = 'projectTab';
            $data['tab'] = $tab;
			if(empty($data['project'])) {
				show_404();
			}

			$this->load->view('templates/header');
			$this->load->view('projects/view', $data);
			$this->load->view('templates/footer');
		}

		public function add_project() {
			$this->form_validation->set_rules('newProjectName', 'NewProjectName', 'required');

			if ($this->form_validation->run() == FALSE){
				//print_r('expression<br>expression<br>expression<br>expression<br>expression<br>');
				//$this->form_validation->set_message("The %s value does not exist");
				//$this->index();
				redirect('/');
				//echo "<script language=\"javascript\">alert('O campo não pode ficar em branco.');</script>";
			} else {
				$projectId = $this->project_model->create_project(); //chechHere checar se conseguiu
				
				redirect(site_url('projects/'.$projectId));
			}
		}

		public function update_project($id) {
			$this->form_validation->set_rules('project_name', 'Project name', 'required');

			if ($this->form_validation->run() == FALSE){
				$this->load->view('templates/header');
				$this->load->view('projects/'.$id, $data);
				$this->load->view('templates/footer');		
			} else {
				$this->project_model->update_project($id);
				redirect(site_url('projects/'.$id));
			}
		}
        
        public function setLastGsId($projectId, $lastGsId) {
            $this->project_model->setLastGsId($projectId, $lastGsId);
            redirect(site_url('projects/'.$projectId.'/gsTab'));
        }
	}