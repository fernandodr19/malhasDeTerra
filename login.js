
function checkLogin(form)
{
    $.ajax({
      type: "POST",
      url: "http://localhost/tutorial/add.php",
      data: {username: form.userid.value, password: form.pswrd.value}
    }).done(function(msg) {
        document.getElementById("error").innerHTML = msg;
      //alert(msg);
    }); 
}