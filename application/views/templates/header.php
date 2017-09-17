<?php 
  if(!$this->session->userdata('logged')) {
    redirect('/login', 'refresh'); 
  }
?>

<html>
  <head>
    <title>Malhas de Terra</title>
<!--    <link rel="shortcut icon" type="image/png" href="http://simpleicon.com/wp-content/uploads/lightning.png"/>-->
      <link rel="shortcut icon" type="image/png" href="<?php echo base_url(); ?>/assets/icons/lightning.png"/>

    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
    <link rel="stylesheet" href="<?php echo base_url(); ?>css/style.css">
    <link rel="stylesheet" href="<?php echo base_url(); ?>css/panel.css">
    <!--<script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.5.6/angular.min.js"></script>-->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
    <script type = 'text/javascript' src = "<?php echo base_url(); ?>js/test.js"></script> 

    <style>
      body{
          background: #8999A8;
      }
    </style>

  </head>
  <body>  
    <link href="https://maxcdn.bootstrapcdn.com/font-awesome/4.6.3/css/font-awesome.min.css" rel="stylesheet" integrity="sha384-T8Gy5hrqNKT+hzMclPo118YTQO6cYprQmhrYwIiQ/3axmI1hQomh7Ud2hPOy8SP1" crossorigin="anonymous">
    <nav class="navbar navbar-inverse navbar-fixed-top" role="navigation">
      <div class="container">
        <div class="navbar-header">
          <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">
            <span class="sr-only">Toggle navigation</span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </button>
        </div>  
        <div class="navbar-collapse collapse">
          <a class="navbar-brand" href="<?php echo base_url(); ?>">Malhas de Terra</a>
          <ul class="nav navbar-nav">
              <li class="dropdown"">
                  <a href="#" class="dropdown-toggle " data-toggle="dropdown" role="button" aria-haspopup="true" aria-expanded="false">Projetos <span class="caret"></span></a>
                  <ul class="dropdown-menu" id="myProjects">
                    <?php foreach ($this->project_model->get_projects() as $project) : ?>
                      <li><a href="<?php echo site_url('projects/'.$project['id']); ?>"><?php echo $project['name']; ?></a></li>
                    <?php endforeach; ?>
                    <li><a href="#" class="add-project" data-toggle="modal" data-target="#add_project" onClick="clearModal()">New Project</a></li>
                  </ul>
              </li>
              <li><a href="<?php echo base_url(); ?>projects">Ir para projetos </a></li>
              <li><a href="#">Page 3</a></li>
          </ul>
          <ul class="nav navbar-nav navbar-right">
              <li class="dropdown"><a href="#" class="dropdown-toggle " data-toggle="dropdown" role="button" aria-haspopup="true" aria-expanded="false" id="userFirstName"><i class="fa fa-user circle" aria-hidden="true"></i>  <?php echo $this->session->userdata('firstName') ?> <span class="caret"></span></a>
                  <ul class="dropdown-menu">
                      <li><a href="<?php echo base_url(); ?>settings/profile">Configurações</a></li>
                      <li><a href="<?php echo base_url(); ?>login/logout"">Logout</a></li>
                  </ul>
              </li>
          </ul>
        </div>
      </div>
    </nav><br><br><br><br>
    <div class="container">