<?php
	class Settings extends CI_Controller {

		public function profile() {
			$data['title'] = 'Profile';

			$data['user'] = $this->user_model->get_user($this->session->userdata('id')); //checkHEre

			$this->load->view('templates/header', $data);
			$this->load->view('settings/index');
			$this->load->view('settings/profile');
			$this->load->view('templates/footer');	
			//echo "<script language=\"javascript\">alert('test');</script>";
		}

		public function account() {
			$data['title'] = 'Profile';

			$this->load->view('templates/header', $data);
			$this->load->view('settings/index');
			$this->load->view('settings/account');
			$this->load->view('templates/footer');	
		}

		public function notifications() {
			$data['title'] = 'Profile';

			$this->load->view('templates/header', $data);
			$this->load->view('settings/index');
			$this->load->view('settings/notifications');
			$this->load->view('templates/footer');	
		}

		public function biling() {
			$data['title'] = 'Profile';

			$this->load->view('templates/header', $data);
			$this->load->view('settings/index');
			$this->load->view('settings/biling');
			$this->load->view('templates/footer');	
		}
	}