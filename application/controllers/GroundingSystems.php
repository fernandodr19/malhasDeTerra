<?php
	class GroundingSystems extends CI_Controller { //THIS CONTROLLER USES THE SAME VIEW AS PROJECT
		public function index() {
			$data['title'] = 'Grounding Systems';

			$this->load->view('templates/header');
			$this->load->view('projects/index', $data);
			$this->load->view('templates/footer');
		}

		public function view($pName = '', $data) {
			$data['project'] = $this->project_model->get_project($pName);

			if(empty($data['project'])) {
				show_404();
			}

			//pegar todas as infos do projeto checkHere

			$this->load->view('templates/header');
			$this->load->view('projects/view', $data);
			$this->load->view('templates/footer');		
		}
        
        public function update_gs($projectId = '') {
            //name required
            
            $this->form_validation->set_rules('gs_conductorMaxLength', "Comprimento máximo do segmento do condutor", "numeric");
            $this->form_validation->set_rules('gs_firstLayerDepth', "Profundidade da primeira camada do solo", "numeric");
            $this->form_validation->set_rules('gs_firstLayerResistivity', "Resistividade da primeira camada do solo", "numeric");
            if($this->input->post('nLayers') == 2) {
                $this->form_validation->set_rules('gs_secondLayerResistivity', "Resistividade da segunda camada do solo", "numeric");
                $this->form_validation->set_rules('gs_crushedStoneLayerDepth', "Profundidade da camada de brita", "numeric");
            }
            $this->form_validation->set_rules('gs_crushedStoneLayerResistivity', "Resistividade da camada de brita", "numeric");
            $this->form_validation->set_rules('gs_injectedCurrent', "Corrente injetada", "numeric");
            
            $this->form_validation->set_rules('conductors[x1]', "Condutores X1", "numeric");
            $this->form_validation->set_rules('conductors[y1]', "Condutores Y1", "numeric");
            $this->form_validation->set_rules('conductors[z1]', "Condutores Z1", "numeric");
            $this->form_validation->set_rules('conductors[x2]', "Condutores X2", "numeric");
            $this->form_validation->set_rules('conductors[y2]', "Condutores Y2", "numeric");
            $this->form_validation->set_rules('conductors[z2]', "Condutores Z2", "numeric");
            
            $this->form_validation->set_rules('points[x]', "Pontos de potencial superficial X", "numeric");
            $this->form_validation->set_rules('points[y]', "Pontos de potencial superficial Y", "numeric");
            
            $this->form_validation->set_rules('profiles[x1]', "Perfil de potencial superficial X1", "numeric");
            $this->form_validation->set_rules('profiles[x2]', "Perfil de potencial superficial Y1", "numeric");
            $this->form_validation->set_rules('profiles[y1]', "Perfil de potencial superficial X2", "numeric");
            $this->form_validation->set_rules('profiles[y2]', "Perfil de potencial superficial Y2", "numeric");
            $this->form_validation->set_rules('profiles[precision]', "Perfil de potencial superficial Precisão", "numeric");
            
            $data['tab'] = 'gs';
            if ($this->form_validation->run() == FALSE){
                $data['error'] = validation_errors();
            } else {
                $data['success'] = 'success';
                $gs['name'] = 'name';
                
                $gs['conductorsMaxLength'] = $this->input->post('gs_conductorMaxLength');
                print($gs['conductorsMaxLength']);
                print('<br>');
                
                $gs['nLayers'] = $this->input->post('nLayers');
                print($gs['nLayers']);
                print('<br>');
                
                $gs['firstLayerDepth'] = $this->input->post('gs_firstLayerDepth');
                print($gs['firstLayerDepth']);
                print('<br>');
                
                $gs['firstLayerResistivity'] = $this->input->post('gs_firstLayerResistivity');
                print($gs['firstLayerResistivity']);
                print('<br>');
                
                if($gs['nLayers'] == 2) {
                    $gs['secondLayerResistivity'] = $this->input->post('gs_secondLayerResistivity');
                    print($gs['secondLayerResistivity']);
                    print('<br>');
                    
                    $gs['crushedStoneLayerDepth'] = $this->input->post('gs_crushedStoneLayerDepth');
                    print($gs['crushedStoneLayerDepth']);
                    print('<br>');
                }
                
                $gs['crushedStoneLayerResistivity'] = $this->input->post('gs_crushedStoneLayerResistivity');
                print($gs['crushedStoneLayerResistivity']);
                print('<br>');
                
                $gs['injectedCurrent'] = $this->input->post('gs_injectedCurrent');
                print($gs['injectedCurrent']);
                print('<br>');
                
                $gs['file'] = $this->input->post('gs_file');
                print($gs['file']);
                print('<br>');
                
                $conductors = $this->input->post('conductors');
                $conductorCables = $this->input->post('conductorCables');
                for ($i = 0; $i < sizeof($conductors['x1']); $i++) {
                    print($conductors['x1'][$i]);
                    print(" ");
                    print($conductors['y1'][$i]);
                    print(" ");
                    print($conductors['z1'][$i]);
                    print(" ");
                    print($conductors['x2'][$i]);
                    print(" ");
                    print($conductors['y2'][$i]);
                    print(" ");
                    print($conductors['z2'][$i]);
                    print(" ");
                    print($conductors['z2'][$i]);
                    print(" ");
                    print($conductorCables[$i]);
                    print("<br>");
                }
                
                $points = $this->input->post('points');
                for ($i = 0; $i < sizeof($points['x']); $i++) {
                    print($points['x'][$i]);
                    print(" ");
                    print($points['y'][$i]);
                    print("<br>");
                }
                
                $profiles = $this->input->post('profiles');
                $touchIndex = 1;
                $stepIndex = 1;
                for ($i = 0; $i < sizeof($profiles['x1']); $i++) {
                    print($profiles['x1'][$i]);
                    print(" ");
                    print($profiles['y1'][$i]);
                    print(" ");
                    print($profiles['x2'][$i]);
                    print(" ");
                    print($profiles['y2'][$i]);
                    print(" ");
                    print($profiles['precision'][$i]);
                    print(" ");
                    if(isset($profiles['touch'][$i + $touchIndex]) and $profiles['touch'][$i + $touchIndex] == 'touch') {
                        print('touch');
                        $touchIndex = $touchIndex + 1;
                    } else {
                        print('not touch');
                    }
                    print(" ");
                    if(isset($profiles['step'][$i + $stepIndex]) and $profiles['step'][$i + $stepIndex] == 'step') {
                        print('step');
                        $stepIndex = $stepIndex + 1;
                    } else {
                        print('not step');
                    }
                    print("<br>");
                }
            }
            redirect(site_url('projects/'.$id.'/gsTab'));
        }
	}