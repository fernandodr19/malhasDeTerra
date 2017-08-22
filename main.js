function createProject()
{
    var newProjectName = document.getElementById('newProjectName').value;
    $.ajax({
      type: "POST",
      url: "http://localhost/tutorial/newProject.php",
      data: {pName: newProjectName, username: 'fdr'}
    }).done(function(msg) {
        document.getElementById("projectModalError").innerHTML = msg;
      alert(msg);
    }); 
}


function clearModal()
{
    document.getElementById('newProjectName').value = '';
    document.getElementById("projectModalError").innerHTML = '';
}
