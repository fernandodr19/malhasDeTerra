<?php
if (!defined('BASEPATH'))
    exit('No direct script access allowed');

class Login extends CI_Controller {

    // function __construct() {
    //     parent::__construct();
    // }

    function index() {

        // VALIDATION RULES
        $this->load->library('form_validation');
        $this->form_validation->set_rules('email', 'Email', 'required');
        $this->form_validation->set_rules('password', 'Password', 'required');
        $this->form_validation->set_error_delimiters('<p class="error">', '</p>');
        $this->form_validation->set_message("email_check", "The value does not exist");

        //$this->load->model('user_model', 'users');
        $userData = $query = $this->user_model->validate();

        if ($this->form_validation->run() == FALSE) {

            $this->load->view('login/login_view');
        } else {

            if ($userData['success']) {
                $data = array(
                    'email' => $userData['email'],
                    'firstName' => $userData['firstName'],
                    'logged' => true
                );
                $this->session->set_userdata($data);
                $this->load->view('templates/header');
                $this->load->view('pages/home');
                $this->load->view('templates/footer');
            } else {
                // redirect('pages/home');
                $data['error'] ="Email ou senha invalidos.";
                $this->load->view('login/login_view', $data);

            }
        }
    }

    function logout() {
        unset($_SESSION['logged']);  
        session_destroy();
        
        redirect('login','refresh');
    }
}