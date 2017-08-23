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
    document.getElementById('projectModalError').innerHTML = '';
}

function loadProjects()
{       
    console.log(localStorage.getItem('name'));
    document.getElementById('myProjects').innerHTML = '';
    $("#myProjects").append('<li><a href="#1">Project 1</a></li>');
    $("#myProjects").append('<li><a href="#" class="add-project" data-toggle="modal" data-target="#add_project" onClick="clearModal()">Add Project</a></li>');
}
