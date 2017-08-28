<script src="https://code.jquery.com/ui/1.12.1/jquery-ui.js"></script>
<script>
$(function() {
	$("#sortable").sortable();
	$("#sortable").disableSelection();
});
</script>

<div class="panel panel-primary">
	<div class="panel-heading">
	    <h3 class="panel-title"><?= $project['pName'] ?></h3>
	    <span class="pull-right">
	        <ul class="nav panel-tabs">
	            <li class="active"><a data-toggle="tab" href="#project">Projeto</a></li>
				<li><a data-toggle="tab" href="#groudingSystems">Malhas de Terra</a></li>
				<li><a data-toggle="tab" href="#report">Relatório</a></li>
	        </ul>
	    </span>
	</div>
	<div class="panel-body">
	    <div class="tab-content">
			<div id="project" class="tab-pane fade in active">
		  		<div class="form-group">
				    <form>  
				      <label>Nome</label>
				      <input class="form-control" type="text"  name="project_name" value="<?= $project['pName'] ?>" style="width: 50%">
				      <br>
				      <label>Subestação</label>
				      <input class="form-control" type="text"  name="project_substation" value="">
				      <br>
				      <label>Descrição</label>
				      <textarea class="form-control" type="text" name="project_description" rows="5" cols="10"></textarea>
				      <br>
				      <label>Projetista</label>
				      <input class="form-control" type="text"  name="project_designer" value="">
				      <br>
				      <label>Data</label>
				      <input class="form-control" type="text"  name="project_date" value="<?= date('d-m-Y') ?>">
				      <br>
				    </form>
			  	</div>
		  		<a href="#" class="btn btn-success" style="float:right">Salvar</a>
			</div>
			<div id="groudingSystems" class="tab-pane fade">
			  <div class="form-group">
			    <form>  
		    	  	<label>Malha de Terra</label>
		    	  	<div class="input-group">
					    <select name="gs" id="gs" style="width: 100%" class="form-control">
							<option value="1">1</option>
							<option value="2">2</option>
		 		 		</select>
					    <span class="input-group-btn">
					        <a href="#" class="btn btn-danger">Deletar</a>
					    </span>
					</div>
      				<a href="#" class="btn btn-success" style="float:right">Adicionar nova Malha</a>
		 		 	<br>
			      	<br>
		    	  	<label>Nome</label>
		    	  	<input class="form-control" type="text"  name="gs_name" value="">					 
				 	<br>
				 	<label>Comprimento máximo do segmento do condutor</label>
			      	<input class="form-control" type="text"  name="gs_conductorMaxLength" value="">
		    	  	<br>
			      	<label>Comprimento máximo do segmento do condutor (m)</label>
			      	<input class="form-control" type="text"  name="gs_conductorMaxLength" value="">
			      	<br>
			      	<label>Número de camadas do solo</label>&nbsp;&nbsp;&nbsp;
			      	<select name="nLayers" id="numberOfLayers" style="width: 5%" class="form-control" onchange="numberOfLayersChanged(this.value)">
						<option value="1">1</option>
						<option value="2">2</option>
		 		 	</select>
		 		 	<br>
		 		 	<label>Profundidade da primeira camada do solo (m)</label>
			      	<input class="form-control" type="text"  name="gs_firstLayerDepth" value="">
			      	<br>
			      	<label>Resistividade da primeira camada do solo (&Omega;.m)</label>
			      	<input class="form-control" type="text"  name="gs_firstLayerResistivity" value="">
			      	<br>
			      	<div id="visibleIf2Layers" style="display: none"><!--Check here, verifiacar-->
						<label>Resistividade da segunda camada do solo (&Omega;.m) (se existir checkHere)</label>
						<input class="form-control" type="text"  name="gs_secondLayerResistivity" value="">
						  <br>
						  <label>Profundidade da camada de brita (m) (se existir checkHere)</label>
						<input class="form-control" type="text"  name="gs_crushedStoneLayerDepth" value="">
						<br>
					</div>
					<label>Resistividade da camada de brita (&Omega;.m)</label>
					<input class="form-control" type="text"  name="gs_crushedStoneLayerResistivity" value="">
					<br>
					<label>Corrente injetada (A)</label>
					<input class="form-control" type="text"  name="gs_secondLayerResistivity" value="">
					<br>
					<label>Arquivo DXF</label>
					<input class="" type="file"  name="gs_secondLayerResistivity" value="" >
					<br>
			    </form>
		      	<div class="panel with-nav-tabs panel-primary">
                	<div class="panel-heading">
	                    <h3 class="panel-title">Nome da malha</h3>
					    <span class="pull-right">
					        <ul class="nav panel-tabs">
					            <li class="active"><a href="#conductors" data-toggle="tab">Condutores</a></li>
		                        <li><a href="#superficialPotentialPoints" data-toggle="tab">Pontos de potencial superficial</a></li>
		                        <li><a href="#superficialPotentialProfile" data-toggle="tab">Perfil de potencial superficial</a></li>
					        </ul>
				    	</span>
                	</div>
	                <div class="panel-body">
	                    <div class="tab-content">
	                        <div class="tab-pane fade in active" id="conductors">
	                        	<table class="table table-list-search">
				                    <thead>
				                        <tr>
				                            <th>X1 (m)</th>
				                            <th>Y1 (m)</th>
				                            <th>Z1 (m)</th>
				                            <th>X2 (m)</th>
				                            <th>Y2 (m)</th>
				                            <th>Z2 (m)</th>
				                            <th>Cabo</th>
				                            <th></th>
				                        </tr>
				                    </thead>
				                    <tbody id="conductorsTable">
				                    </tbody>
				                </table> 
				                <a class="btn btn-primary" style="float:right" onclick="addConductorRow();">Adicionar</a>
	                        </div>
	                        <div class="tab-pane fade" id="superficialPotentialPoints">
	                        	<table class="table table-list-search">
				                    <thead>
				                        <tr>
				                            <th>X (m)</th>
				                            <th>Y (m)</th>
				                        </tr>
				                    </thead>
				                    <tbody id="pointsTable">
				                    </tbody>
				                </table> 
				                <a class="btn btn-primary" style="float:right" onclick="addPointRow();">Adicionar</a>
	                        </div>
	                        <div class="tab-pane fade" id="superficialPotentialProfile">
	                        	<table class="table table-list-search">
				                    <thead>
				                        <tr>
				                            <th>X1 (m)</th>
				                            <th>Y1 (m)</th>
				                            <th>X2 (m)</th>
				                            <th>Y2 (m)</th>
				                            <th>Precisão (m)</th>
				                            <th>Toque</th>
				                            <th>Passo</th>
				                        </tr>
				                    </thead>
				                    <tbody id="profileTable">
				                    </tbody>
				                </table> 
				                <a class="btn btn-primary" style="float:right" onclick="addProfileRow();">Adicionar</a>
	                        </div>
	                    </div>
	                </div>
            	</div>
		  	  </div>
			</div>
			<div id="report" class="tab-pane fade">
		  		<div class="form-group">
				    <form>
				    	<table border="0" style="width: 200px">  
				    		<tr style="width: 30%">
				      			<td><label>Exibir entrada</label></td>
				      			<td><input type="checkbox" name="showInput" checked></td>
				      		</tr>
				      		<tr>
			      				<td><label>Exibir malhas de terra</label></td>
				      			<td><input type="checkbox" name="showGS" checked></td>
				      		</tr>
				      		<tr>
				      			<td><label>Exibir condutores</label></td>
				      			<td><input type="checkbox" name="showConductors" checked></td>
			      			</tr>
			      		</table>
				    </form>
			  	</div>
		  		<a href="#" class="btn btn-success" style="float:right">Gerar Relatório</a>
			</div>
		</div>
	</div>
</div>

<div class="modal fade" id="add_gs">
	<div class="modal-dialog">
		<div class="modal-content">
    		<div class="modal-header">
      			<h4 class="modal-title">New Project</h4>
    		</div>
    		<div class="modal-body">
      			<form action="<?php echo base_url(); ?>projects/myfunction" method="POST">
				    <label>Project name</label>
	                <input class="form-control" placeholder="Enter name" type="text" name="newProjectName" id="newProjectName">
	                <?php echo form_error('newProjectName'); ?>
			    	<div class="modal-footer">
			    		<input class="btn btn-primary" type="submit" value="Create">
		    		</div>
	  			</form>
	  		</div>
  		</div>
	</div>
</div>

<script type="text/javascript"> //passar pra test.js
	function addConductorRow() {
		 document.getElementById("conductorsTable").insertRow(-1).innerHTML = 
		 '<tr>' +
		 	'<td contenteditable=\'true\'>0.000</td>' + 
		 	'<td contenteditable=\'true\'>0.000</td>' +
		 	'<td contenteditable=\'true\'>0.000</td>' +
		 	'<td contenteditable=\'true\'>0.000</td>' + 
		 	'<td contenteditable=\'true\'>0.000</td>' +
		 	'<td contenteditable=\'true\'>0.000</td>' +
		 	'<td><select name="gs" id="gs" style="width: 50%">' +
						'<option value="">Cabos</option>' +
						'<option value="1">Cobre 70mm²</option>' +
						'<option value="2">DOTTEREL</option>' +
			 		 '</select></td>' +
 		 	'<td><button class="btn btn-xs btn-danger" onClick="deleteRow(this)"><span class="glyphicon glyphicon-remove"></span></button></td>' +
	 	'</tr>';
	}

	function addPointRow() {
		 document.getElementById("pointsTable").insertRow(-1).innerHTML = 
		 '<tr>' +
		 	'<td contenteditable=\'true\'>0.000</td>' + 
		 	'<td contenteditable=\'true\'>0.000</td>' +
		 	'<td><button class="btn btn-xs btn-danger" onClick="deleteRow(this)"><span class="glyphicon glyphicon-remove"></span></button></td>' +
	 	'</tr>';
	}

	function addProfileRow() {
		 document.getElementById("profileTable").insertRow(-1).innerHTML = 
		 '<tr>' +
		 	'<td contenteditable=\'true\'>0.000</td>' + 
		 	'<td contenteditable=\'true\'>0.000</td>' +
		 	'<td contenteditable=\'true\'>0.000</td>' + 
		 	'<td contenteditable=\'true\'>0.000</td>' +
		 	'<td contenteditable=\'true\'>0.000</td>' + 
		 	'<td><input type="checkbox" name="touch" value=1 checked></td>' +
		 	'<td><input type="checkbox" name="step" value=2></td>' +
		 	'<td><button class="btn btn-xs btn-danger" onClick="deleteRow(this)"><span class="glyphicon glyphicon-remove"></span></button></td>' +
	 	'</tr>';
	}

	function numberOfLayersChanged(value) {
		var div = document.getElementById('visibleIf2Layers');
		if(value == 1)
			div.style.display = 'none';
		else
			div.style.display = 'block';
	}

	function deleteRow(row) {
		row.closest('tr').remove();
	}

	$("#conductorsTable").sortable();
	$("#superficialPotentialPoints").sortable();
	$("#superficialPotentialProfile").sortable();
</script>
