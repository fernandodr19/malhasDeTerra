function test() 
{ 
   alert(window.location.href);
}

function createProject() 
{
	localStorage.setItem('item', 'banana');
	alert(localStorage.getItem('item'));
}

function clearModal()
{
    document.getElementById('newProjectName').value = '';
}

function updateSettingsMenu()
{
	alert('id');
}
