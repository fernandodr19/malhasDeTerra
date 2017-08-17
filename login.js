
function checkLogin(form)
{
    $.ajax({
      type: "POST",
      url: "http://localhost/tutorial/login.php",
      data: {username: form.userid.value, password: form.pswrd.value}
    }).done(function(msg) {
        document.getElementById("error").innerHTML = msg;
      //alert(msg);
    }); 
}

function checkRegister(form)
{
    var name = form.name.value
    var email = form.email.value
    var username = form.username.value
    var password = form.password.value
    var password2 = form.password2.value
    
    if(name.length == 0) {
        document.getElementById("error").innerHTML = "<font color=\"red\">Nome não informado.</font>";
        return;
    }
    
    if(email.length == 0) {
        document.getElementById("error").innerHTML = "<font color=\"red\">Email não informado.</font>";
        return;
    }
    
    if(password.length == 0 || password2.length == 0) {
        document.getElementById("error").innerHTML = "<font color=\"red\">Senha não informado.</font>";
        return;
    }
    
    if(password != password2) {
        document.getElementById("error").innerHTML = "<font color=\"red\">As senhas não conferem.</font>";
        return;
    }
        
    $.ajax({
      type: "POST",
      url: "http://localhost/tutorial/register.php",
      data: {username: email, password: password}
    }).done(function(msg) {
        document.getElementById("error").innerHTML = msg;
      //alert(msg);
    }); 

}