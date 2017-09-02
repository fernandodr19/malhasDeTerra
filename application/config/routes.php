<?php
defined('BASEPATH') OR exit('No direct script access allowed');


$route['login'] = 'login';
$route['projects/add_project'] = 'projects/add_project';
$route['projects/update_project/(:any)'] = 'projects/update_project/$1';
$route['projects/(:any)'] = 'projects/view/$1';
$route['projects'] = 'projects/index';

$route['settings'] = 'settings/index';


$route['default_controller'] = 'pages/view';
$route['(:any)'] = 'pages/view/$1';
$route['404_override'] = '';
$route['translate_uri_dashes'] = FALSE;