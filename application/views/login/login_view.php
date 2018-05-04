<html>
    <head>
        <meta charset="UTF-8">
        <link rel="stylesheet" href="<?php echo base_url(); ?>css/login.css">
        <style>
            #body {
            /*
                background: url('http://relaypowersystems.com/wp-content/uploads/2011/11/substation.jpg') fixed;
                            background-size: cover;
                padding: 0;
                margin: 0;
            */

            background-color: grey;
        }
        </style>
        <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
        <title>Malhas de Terra</title>
        <link rel="shortcut icon" type="image/png" href="<?php echo base_url(); ?>/assets/icons/lightning.png"/>    
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>   
    </head>
    <body id="body">
        <div class="container">
            <div class="row">
                <div class="col-md-12">
                    <div class="pr-wrap">
                        <div class="pass-reset">
                            <label>
                                Enter the email you signed up with</label>
                            <input type="email" placeholder="Email" />
                            <input type="submit" value="Submit" class="pass-reset-submit btn btn-success btn-sm" />
                        </div>
                    </div>
                    <div class="wrap">
                        <p class="form-title">
                            Sign In
                        </p>
                        <form class="login" action="<?php echo base_url(); ?>login" method="POST">
                            <input type="text" placeholder="Email" name="email">
                            <input type="password" placeholder="Senha" name="password">
                            <input type="submit" value="Sign In" class="btn btn-success btn-sm">
                            <div class="register-forgot">
                                <div class="row">
                                    <div class="col-md-6 forgot-pass-content">
                                        <a href="<?php echo base_url(); ?>login/register">Crie uma conta!</a>
                                    </div>
                                    <div class="col-md-6 forgot-pass-content">
                                        <a href="javascription:void(0)" class="forgot-pass">Esqueceu a senha?</a>
                                    </div>
                                </div>
                            </div>
                            <?php
                            if (isset($error) and !empty($error))
                                echo "<div class='alert alert-danger'>$error</div>";
                            ?>
                        </form>
                    </div>
                </div>
            </div>
        </div>  
    </body>
</html>

<script type="text/javascript">
     $(document).ready(function () {
    $('.forgot-pass').click(function(event) {
      $(".pr-wrap").toggleClass("show-pass-reset");
    }); 
    
    $('.pass-reset-submit').click(function(event) {
      $(".pr-wrap").removeClass("show-pass-reset");
    }); 
});
</script>