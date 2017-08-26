<?php
defined('BASEPATH') OR exit('No direct script access allowed');

$route['projects/myfunction'] = 'projects/myfunction';
$route['projects/(:any)'] = 'projects/view/$1';
$route['projects'] = 'projects/index';

$route['settings'] = 'settings/index';


$route['default_controller'] = 'pages/view';
$route['(:any)'] = 'pages/view/$1';
$route['404_override'] = '';
$route['translate_uri_dashes'] = FALSE;