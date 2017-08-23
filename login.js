
function checkLogin(form)
{
    $.ajax({
      type: "POST",
      url: "http://localhost/tutorial/login.php",
      data: {email: form.email.value, password: form.password.value}
    }).done(function(response) {
        var user = JSON.parse(response);
        //alert(JSON.parse(msg).success);
        if(user.success) {
            localStorage.setItem('firstName', user.firstName);
            localStorage.setItem('email', user.email);
            window.location.href = "mainWindow.html";
        } else {
            document.getElementById("error").innerHTML = 'Invalid username or password.';
        }
    }); 
}

function checkRegister(form)
{
    var name = form.name.value
    var email = form.email.value
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
      data: {name: name, email: email, password: password}
    }).done(function(msg) {
        document.getElementById("error").innerHTML = msg;
      //alert(msg);
        document.getElementById("error").innerHTML = 'done';
    }); 

}
