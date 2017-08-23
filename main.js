function createProject()
{
    var newProjectName = document.getElementById('newProjectName').value;
    $.ajax({
      type: "POST",
      url: "http://localhost/tutorial/newProject.php",
      data: {pName: newProjectName, username: localStorage.getItem('email')}
    }).done(function(msg) {
        document.getElementById("projectModalError").innerHTML = msg;
    }); 
}


function clearModal()
{
    document.getElementById('newProjectName').value = '';
    document.getElementById('projectModalError').innerHTML = '';
}

function loadProjects()
{       
    $.ajax({
      type: "POST",
      url: "http://localhost/tutorial/projectsManager.php", //getProjects
      data: {email: localStorage.getItem('email')}
    }).done(function(response) {
        var projects = JSON.parse(response);
        document.getElementById('myProjects').innerHTML = '';
        var i;
        for(i = 0; i < projects.length; i++) {
            $("#myProjects").append('<li><a href="#1">' + projects[i] + '</a></li>');
        }
        $("#myProjects").append('<li><a href="#" class="add-project" data-toggle="modal" data-target="#add_project" onClick="clearModal()">Add Project</a></li>');
    });
}
