<?php
	class User_model extends CI_Model {
		public function __construct() {
			$this->load->database();
		}

		public function get_users($email = '') {
			if($email == '') {
				$query = $this->db->get('users');
				return $query->result_array();
			}

			$query = $this->db->get_where('users', array('email' => $email));
			return $query->row_array();
		}

		function validate() {
	        $this->db->where('email', $this->input->post('email')); 
	        $this->db->where('password', $this->input->post('password'));

	        $query = $this->db->get('users'); 

        	$data['success'] = false;
	        if ($query->num_rows() == 1) { 
	        	$row = $query->row();

	            $data['success'] = true;
	            $data['email'] = $row->email;
	            $data['firstName'] = $row->firstName;
	        }
	        return $data;
	    }

	    function logged() {
	        $logged = $this->session->userdata('logged');

	        if (!isset($logged) || $logged != true) {
	            echo 'Voce nao tem permissao para entrar nessa pagina. <a href="http://oficina2015/login">Efetuar Login</a>';
	            die();
	        }
	    }
	}