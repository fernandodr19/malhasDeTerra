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
        
        public function add_gs($projectId) {
			$this->form_validation->set_rules('newGSName', 'Nome da malha', 'required');

			if ($this->form_validation->run() == FALSE){//checkHere
				//print_r('expression<br>expression<br>expression<br>expression<br>expression<br>');
				//$this->form_validation->set_message("The %s value does not exist");
				//$this->index();
				redirect('/');
				//echo "<script language=\"javascript\">alert('O campo não pode ficar em branco.');</script>";
			} else {
				$gsId = $this->groundingSystem_model->create_groundingSystem($projectId);
                $this->project_model->setLastGsId($projectId, $gsId);
				redirect(site_url('projects/'.$projectId.'/gsTab'));
			}
		}
        
        public function update_gs($projectId, $gsId) {
            $this->form_validation->set_rules('gs_name', "Nome", "required");
            $this->form_validation->set_rules('gs_conductorsMaxLength', "Comprimento máximo do segmento do condutor", "numeric");
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
            
            if ($this->form_validation->run() == FALSE){
                $data['error'] = validation_errors();
            } else {
                $data['success'] = 'success'; //checkHere do i need it?
                $this->groundingSystem_model->update_groundingSystem($gsId); //save gs
                
                $gs['file'] = $this->input->post('gs_file');
                //////////////SAVE CONDUCTORS//////////////
                $this->conductor_model->delete_conductors($gsId);
                $this->conductor_model->create_conductors($gsId);
                
                //////////////SAVE POINTS//////////////
                $this->point_model->delete_points($gsId);
                $this->point_model->create_points($gsId);
                
                //////////////SAVE PROFILES//////////////
                $this->profile_model->delete_profiles($gsId);
                $this->profile_model->create_profiles($gsId);
            }
            
            redirect(site_url('projects/'.$projectId.'/gsTab'));
        }
        
        public function generate_report($projectId, $gsId) {
            $this->form_validation->set_rules('gs_name', "Nome", "required");
            $this->form_validation->set_rules('gs_conductorsMaxLength', "Comprimento máximo do segmento do condutor", "numeric");
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
                        
            //I can't run this validation because it's from another form. checkHere
//            if ($this->form_validation->run() == FALSE){
//                $data['error'] = validation_errors();
//            } else {
            $data['success'] = 'success'; //checkHere do i need it?
            //now i need to generate the file
            $currentPath = getcwd();
            $file = fopen($currentPath."/calculator/input.ftl", "w");

            $project = $this->project_model->get_project($projectId);
            fwrite($file, "[Project]\n");
            $content = $this->write_ini_file($project, $file);
            fwrite($file, $content);
            fwrite($file, "showInput=".($this->input->post('showInput') != null));
            fwrite($file, "\nshowGS=".($this->input->post('showGS') != null));
            fwrite($file, "\nshowConductors=".($this->input->post('showConductors') != null));
            fwrite($file, "\n\n");
            
            fwrite($file, "[GroundingSystems]\n");
            $gs = $this->groundingSystem_model->get_groundingSystem($gsId);
            $content = $this->write_ini_file($gs, $file, "GroundingSystem");
            fwrite($file, $content);
            fwrite($file, "\n");
            
            fwrite($file, "[Conductors]\n");
            $conductors = $this->conductor_model->get_conductors($gsId);
            foreach ($conductors as $conductor) { 
                $content = $this->write_ini_file($conductor, $file, "Conductor");
                fwrite($file, $content);
            }
            fwrite($file, "\n");
            
            fwrite($file, "[Points]\n");
            $points = $this->point_model->get_points($gsId);
            foreach ($points as $point) { 
                $content = $this->write_ini_file($point, $file, "Point");
                fwrite($file, $content);
            }
            fwrite($file, "\n");
            
            fwrite($file, "[Profiles]\n");
            $profiles = $this->profile_model->get_profiles($gsId);
            foreach ($profiles as $profile) { 
                $content = $this->write_ini_file($profile, $file, "Profile");
                fwrite($file, $content);
            }
            fwrite($file, "\n");
            
            fwrite($file, "[Cables]\n");
            $cables = $this->cable_model->get_cables($gsId);
            foreach ($cables as $cable) { 
                $content = $this->write_ini_file($cable, $file, "Cable");
                fwrite($file, $content);
            }
            fwrite($file, "\n");
            
            fclose($file);
//            }    
            
            //run C++ program
            shell_exec($currentPath."/calculator/libs/GroundingSystems");
            
            //wait until result is ready
            while (!file_exists($currentPath."/calculator/results.ftl")) sleep(1);
            
            //once calculation is done get result
            $ini_array = parse_ini_file($currentPath."/calculator/results.ftl", true);
            
            $result = array();
            foreach($ini_array as $key => $value)
            {
                $p = &$result;
                foreach(explode('.', $key) as $k)
                    $p = &$p[$k];
                $p = $value;
            }
            unset($p);

            print_r($result);
            
            $this->generateReportDoc($result);

//            redirect(site_url('projects/'.$projectId.'/reportTab'));
        }
        
        function write_ini_file($assoc_arr, $file, $prefix = "") { 
            $content = ""; 
             
            $lastId = "";
            foreach ($assoc_arr as $key=>$elem) { 
                if($key == "id")
                    $lastId = $elem;
                
                if($lastId != "" and $prefix != "")
                    $content .= $prefix.$lastId."/";

                if($elem=="") 
                    $content .= $key."=\n"; 
                else 
                    $content .= $key."=\"".$elem."\"\n"; 
            } 

            return $content; 
        }
        
        function generateReportDoc($data) {
            $this->load->library('parser');
            $doc = '';
            $template1 = '<li><a href="{name}">{voltage}</a></li>';
            $data1 = array(
                    array('title' => 'First Link', 'link' => '/first'),
                    array('title' => 'Second Link', 'link' => '/second'),
            );

            $gss = $data['GroundingSystems'];
            foreach ($gss as $gs)
            {
                $doc .= $this->parser->parse_string($template1, $gs, TRUE);
                print('<ul>'.$doc.'</ul>');
            }
        }
	}