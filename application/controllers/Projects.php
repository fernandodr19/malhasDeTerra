<?php
	class Projects extends CI_Controller {
		public function index() {
			$data['title'] = 'Projects';

			$this->load->view('templates/header');
			$this->load->view('projects/index', $data);
			$this->load->view('templates/footer');
		}

		public function view($pName = '') {
			$data['project'] = $this->project_model->get_projects($pName);

			if(empty($data['project'])) {
				show_404();
			}

			//pegar todas as infos do projeto checkHere

			$this->load->view('templates/header');
			$this->load->view('projects/view', $data);
			$this->load->view('templates/footer');		
		}

		public function myfunction() {

			$data['title'] = 'aaa';

			$this->load->library('form_validation');

			$this->form_validation->set_rules('newProjectName', 'NewProjectName', 'required');

			if ($this->form_validation->run() == FALSE){
				//print_r('expression<br>expression<br>expression<br>expression<br>expression<br>');
				//$this->form_validation->set_message("The %s value does not exist");
				//$this->index();
				redirect('/');
				//echo "<script language=\"javascript\">alert('O campo não pode ficar em branco.');</script>";
			} else {
				$this->project_model->create_project();
				
				$this->load->view('templates/header');
				$this->load->view('pages/home');
				$this->load->view('templates/footer');

				//redirect('views/pages/home');
			}
		}
	}