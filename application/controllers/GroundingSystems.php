<?php
	class GroundingSystems extends CI_Controller { //THIS CONTROLLER USES THE SAME VIEW AS PROJECT
		public function index() {
			$data['title'] = 'Grounding Systems';

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
        
        public function update_gs() {
            $this->form_validation->set_rules('conductors[x1]',"Condutores x1", "numeric");
            $this->form_validation->set_rules('conductors[y1]',"Condutores y1", "numeric");
            $this->form_validation->set_rules('conductors[z1]',"Condutores z1", "numeric");
            $this->form_validation->set_rules('conductors[x2]',"Condutores x2", "numeric");
            $this->form_validation->set_rules('conductors[y2]',"Condutores y2", "numeric");
            $this->form_validation->set_rules('conductors[z2]',"Condutores z2", "numeric");
            if ($this->form_validation->run() == FALSE){
                $data['error'] = validation_errors();
                print(validation_errors());
            } else {
                $conductors = $this->input->post('conductors');
                for ($i = 0; $i < sizeof($conductors['x1']); $i++) {
                    print(" ");
                    print($i);
                    print($conductors['x1'][$i]);
                }
            }
        }
	}