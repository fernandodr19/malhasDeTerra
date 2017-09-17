<?php
	class Profile_model extends CI_Model {
		public function __construct() {
			$this->load->database();
		}

		public function get_profiles($gsId) {
            $this->db->where('gsId', $gsId);
            $this->db->from('profiles');
            $query = $this->db->get();
            return $query->result_array();
		}
        
        public function create_profiles($gsId) { 
            $profiles = $this->input->post('profiles');
                
            $touchIndex = 1;
            $stepIndex = 1;
            $touch = array();
            $step = array();
            for($i = 0; $i < sizeof($profiles['x1']); $i++) {
                if(isset($profiles['touch'][$i + $touchIndex]) and $profiles['touch'][$i + $touchIndex] == 'touch') {
                    array_push($touch, true);
                    $touchIndex = $touchIndex + 1;
                } else {
                    array_push($touch, false);
                }
                if(isset($profiles['step'][$i + $stepIndex]) and $profiles['step'][$i + $stepIndex] == 'step') {
                    array_push($step, true);
                    $stepIndex = $stepIndex + 1;
                } else {
                    array_push($step, false);
                }
            }
            
            for ($i = 0; $i < sizeof($profiles['x1']); $i++) {
                $conductorData = array(
                    'gsId' => $gsId,
                    'x1' => $profiles['x1'][$i],
                    'y1' => $profiles['y1'][$i],
                    'x2' => $profiles['x2'][$i],
                    'y2' => $profiles['y2'][$i],
                    'precision' => $profiles['precision'][$i],
                    'touch' => $touch[$i],
                    'step' => $step[$i]
                 );

                $this->db->insert('profiles', $conductorData);
            }
		}

		public function delete_profiles($gsId) {
            $this->db->where('gsId', $gsId);
			$this->db->delete('profiles'); 
		}
	}
