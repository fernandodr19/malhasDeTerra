<script src="https://code.jquery.com/ui/1.12.1/jquery-ui.js"></script>
<script>
    $(function() {
        $("#sortable").sortable();
        $("#sortable").disableSelection();
    });

    $(function() {
        //initialize data    
        var sel = document.getElementById('gs');
        for(var i = 0; i < sel.options.length; i++) {
            if(sel.options[i].value == <?= $gs['id'] ?>) {
               sel.selectedIndex = i;
               break;
            }
        }
        
       
       var conductors = <?php echo json_encode($conductors) ?>;
       for(var i = 0; i < conductors.length; i++) {
            var x1 = conductors[i].x1;
            var y1 = conductors[i].y1;
            var z1 = conductors[i].z1;
            var x2 = conductors[i].x2;
            var y2 = conductors[i].y2;
            var z2 = conductors[i].z2;
            var cableId = conductors[i].cableId;
            addConductorRow(x1, y1, z1, x2, y2, z2, cableId);
        }
            
        var points = <?php echo json_encode($points) ?>;
        for(var i = 0; i < points.length; i++) {
            var x = points[i].x;
            var y = points[i].y;
            addPointRow(x, y);
        }
            
        <?php foreach ($profiles as $profile) : ?>
            var x1 = <?= $profile['x1'] ?>;
            var y1 = <?= $profile['y1'] ?>;
            var x2 = <?= $profile['x2'] ?>;
            var y2 = <?= $profile['y2'] ?>;
            var precision = <?= $profile['precision'] ?>;
            var touch = <?= $profile['touch'] ?>;
            var step = <?= $profile['step'] ?>;
            addProfileRow(x1, y1, x2, y2, precision, touch, step);
        <?php endforeach; ?> 
    });
       
    


</script>

<div class="panel panel-primary">
	<div class="panel-heading">
	    <h3 class="panel-title"><?= $project['name'] ?></h3>
	    <span class="pull-right">
	        <ul class="nav panel-tabs">
	            <li class="<?php echo ($tab == 'projectTab') ? 'active' : ''; ?>"><a data-toggle="tab" href="#project">Projeto</a></li>
				<li class="<?php echo ($tab == 'gsTab') ? 'active' : ''; ?>"><a data-toggle="tab" href="#groudingSystems">Malhas de Terra</a></li>
				<li class="<?php echo ($tab == 'reportTab') ? 'active' : ''; ?>"><a data-toggle="tab" href="#report">Relatório</a></li>
	        </ul>
	    </span>
	</div>
	<div class="panel-body">
	    <div class="tab-content">
			<div id="project" class="tab-pane fade <?php echo ($tab == 'projectTab') ? 'in active' : ''; ?>">
		  		<div class="form-group">
				    <form action="<?php echo base_url(); ?>projects/update_project/<?= $project['id'] ?>" method="POST">
				      <label>Nome</label>
				      <input class="form-control" type="text"  name="project_name" value="<?= $project['name'] ?>" style="width: 50%">
				      <br>
				      <label>Subestação</label>
				      <input class="form-control" type="text"  name="project_substation" value="<?= $project['substation'] ?>">
				      <br>
				      <label>Descrição</label>
				      <textarea class="form-control" type="text" name="project_description" rows="5" cols="10"><?= $project['description'] ?></textarea>
				      <br>
				      <label>Projetista</label>
				      <input class="form-control" type="text"  name="project_designer" value="<?= $project['designer'] ?>">
				      <br>
				      <label>Data</label>
				      <input class="form-control" type="text"  name="project_date" value="<?= date('d-m-Y') ?>">
				      <br>
  					  <input type="submit" value="Salvar" class="btn btn-success" style="float:right">
				    </form>
			  	</div>
			</div>
			<div id="groudingSystems" class="tab-pane fade <?php echo ($tab == 'gsTab') ? 'in active' : ''; ?>">
			  <div class="form-group">
			    <form action="<?php echo base_url(); ?>groundingSystems/update_gs/<?= $project['id'] ?>/<?= $gs['id'] ?>" method="POST">
		    	  	<label>Malha de Terra</label>
		    	  	<div class="input-group">
					    <select name="gs" id="gs" style="width: 100%" class="form-control">
							<?php foreach ($this->groundingSystem_model->get_groundingSystems($project['id']) as $gsTemp) : ?>
                                <option value="<?php echo $gsTemp['id']; ?>"><?php echo $gsTemp['name']; ?></option>
                            <?php endforeach; ?>
		 		 		</select>
					    <span class="input-group-btn">
					        <a href="#" class="btn btn-danger">Deletar</a>
                            <a href="#" class="btn btn-success" data-toggle="modal" data-target="#gs_add">Adicionar nova Malha</a>
					    </span>
					</div>
			      	<br>
		    	  	<label>Nome</label>
		    	  	<input class="form-control" type="text"  name="gs_name" value="<?php echo $gs['name']; ?>">					 
				 	<br>
				 	<label>Comprimento máximo do segmento do condutor (m)</label>
                    <div class="input-group">
                      <input class="form-control" type="text"  name="gs_conductorsMaxLength" value="<?php echo $gs['conductorsMaxLength']; ?>">
                      <span class="input-group-btn">
                          <a class="btn btn-info" onClick="showHelp('Digite a help aqui')" >?</a>
                      </span>
                    </div>
			      	<br>
			      	<label>Número de camadas do solo</label>&nbsp;&nbsp;&nbsp;
			      	<select name="gs_nLayers" id="numberOfLayers" style="width: 5%" class="form-control" onchange="numberOfLayersChanged(this.value)">
						<option value="1">1</option>
						<option value="2">2</option>
		 		 	</select>
		 		 	<br>
		 		 	<label>Profundidade da primeira camada do solo (m)</label>
			      	<input class="form-control" type="text"  name="gs_firstLayerDepth" value="<?php echo $gs['firstLayerDepth']; ?>">
			      	<br>
			      	<label>Resistividade da primeira camada do solo (&Omega;.m)</label>
			      	<input class="form-control" type="text"  name="gs_firstLayerResistivity" value="<?php echo $gs['firstLayerResistivity']; ?>">
			      	<br>
			      	<div id="visibleIf2Layers" style="display: none">
						<label>Resistividade da segunda camada do solo (&Omega;.m)</label>
						<input class="form-control" type="text"  name="gs_secondLayerResistivity" value="<?php echo $gs['secondLayerResistivity']; ?>">
						  <br>
						  <label>Profundidade da camada de brita (m)</label>
						<input class="form-control" type="text"  name="gs_crushedStoneLayerDepth" value="<?php echo $gs['crushedStoneLayerDepth']; ?>">
						<br>
					</div>
					<label>Resistividade da camada de brita (&Omega;.m)</label>
					<input class="form-control" type="text"  name="gs_crushedStoneLayerResistivity" value="<?php echo $gs['crushedStoneLayerResistivity']; ?>">
					<br>
					<label>Corrente injetada (A)</label>
					<input class="form-control" type="text"  name="gs_injectedCurrent" value="<?php echo $gs['injectedCurrent']; ?>">
					<br>
					<label>Arquivo DXF</label>
					<input class="" type="file"  name="gs_file" value="" >
					<br>
                    <div class="panel with-nav-tabs panel-primary">
                        <div class="panel-heading">
                            <h3 class="panel-title"><?php echo $gs['name']; ?></h3>
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
                                        <tbody id="conductorsTableBody">
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
                                        <tbody id="pointsTableBody">
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
                                                <th width="0px"></th>
                                                <th>Toque</th>
                                                <th width="0px"></th>
                                                <th>Passo</th>
                                            </tr>
                                        </thead>
                                        <tbody id="profileTableBody">
                                        </tbody>
                                    </table> 
                                    <a class="btn btn-primary" style="float:right" onclick="addProfileRow();">Adicionar</a>
                                </div>
                            </div>
                        </div>
                    </div>
                    <input type="submit" value="Salvar" class="btn btn-success" style="float:right">
                </form>
		  	  </div>
			</div>
			<div id="report" class="tab-pane fade <?php echo ($tab == 'reportTab') ? 'in active' : ''; ?>">
		  		<div class="form-group">
                    <form action="<?php echo base_url(); ?>groundingSystems/generate_report/<?= $project['id'] ?>/<?= $gs['id'] ?>" method="POST">
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
                        <input type="submit" value="Gerar Relatório" class="btn btn-success" style="float:right">
				    </form>
			  	</div>
<!--		  		<a href="#" class="btn btn-success" style="float:right">Gerar Relatório</a>-->
			</div>
		</div>
	</div>
</div>

<style> /*checkHere Passar pro css*/
    .tableInput {
        width: 60px;
        border: none
    }
</style>

<script type="text/javascript">

    function showHelp(help) {
        document.getElementById('helpLabel').innerHTML = help;
        $('#helpModal').modal('show');
    }
    
	function addConductorRow(x1 = '0.000', y1 = '0.000', z1 = '0.000', x2 = '0.000', y2 = '0.000', z2 = '0.000', cableId = 0) {
        var body = 
		 '<tr>' +
		 	'<td><input type="text" class="tableInput" value="' + x1 + '" name="conductors[x1][]"></td>' + 
		 	'<td><input type="text" class="tableInput" value="' + y1 + '" name="conductors[y1][]"></td>' +
		 	'<td><input type="text" class="tableInput" value="' + z1 + '" name="conductors[z1][]"></td>' +
		 	'<td><input type="text" class="tableInput" value="' + x2 + '" name="conductors[x2][]"></td>' + 
		 	'<td><input type="text" class="tableInput" value="' + y2 + '" name="conductors[y2][]"></td>' +
		 	'<td><input type="text" class="tableInput" value="' + z2 + '" name="conductors[z2][]"></td>' +
		 	'<td>' +
                '<select name="conductorCables[]" id="gs" style="width: 50%">'
                var cables = <?php echo json_encode($cables) ?>;    
                for(var i = 0; i < cables.length; i++) {
                    if(i == cableId - 1)
                        body += '<option selected="selected" value="' + cables[i].id + '">' + cables[i].code + '</option>';    
                    else
                        body += '<option value="' + cables[i].id + '">' + cables[i].code + '</option>';    
                }
                 body += '</select></td>' +
 		 	'<td><button class="btn btn-xs btn-danger" onClick="deleteRow(this)"><span class="glyphicon glyphicon-remove"></span></button></td>' +
	 	'</tr>';
        document.getElementById("conductorsTableBody").insertRow(-1).innerHTML = body;
	}

	function addPointRow(x = '0.000', y = '0.000') {
		 document.getElementById("pointsTableBody").insertRow(-1).innerHTML = 
		 '<tr>' +
		 	'<td><input type="text" class="tableInput" value="' + x + '" name="points[x][]"></td>' + 
		 	'<td><input type="text" class="tableInput" value="' + y + '" name="points[y][]"></td>' +
		 	'<td><button class="btn btn-xs btn-danger" onClick="deleteRow(this)"><span class="glyphicon glyphicon-remove"></span></button></td>' +
	 	'</tr>';
	}

	function addProfileRow(x1 = '0.000', y1 = '0.000', x2 = '0.000', y2 = '0.000', precision = '0.000', touch = true, step = true) {
		 var body =
		 '<tr>' +
		 	'<td><input type="text" class="tableInput" value="' + x1 + '" name="profiles[x1][]"></td>' + 
		 	'<td><input type="text" class="tableInput" value="' + y1 + '" name="profiles[y1][]"></td>' +
		 	'<td><input type="text" class="tableInput" value="' + x2 + '" name="profiles[x2][]"></td>' + 
		 	'<td><input type="text" class="tableInput" value="' + y2 + '" name="profiles[y2][]"></td>' +
		 	'<td><input type="text" class="tableInput" value="' + precision + '" name="profiles[precision][]"></td>' + 
            '<td><input type="hidden" value="hidden" name="profiles[touch][]"></td>';
            if(touch)
		 	    body += '<td><input type="checkbox" value="touch" name="profiles[touch][]" checked></td>';
            else
                body += '<td><input type="checkbox" value="touch" name="profiles[touch][]"></td>';
            body += '<td><input type="hidden" value="hidden" name="profiles[step][]"></td>';
            if(step)
		 	    body += '<td><input type="checkbox" value="step" name="profiles[step][]" checked></td>';
            else
                body += '<td><input type="checkbox" value="step" name="profiles[step][]"></td>';
            body +=
		 	'<td><button class="btn btn-xs btn-danger" onClick="deleteRow(this)"><span class="glyphicon glyphicon-remove"></span></button></td>' +
	 	'</tr>';
        document.getElementById("profileTableBody").insertRow(-1).innerHTML = body;
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

    $('#gs').change(function() {
        window.location.href = "<?php echo base_url(); ?>projects/setLastGsId/<?= $project['id'] ?>/" + this.value;
    });
    
	//$("#conductorsTableBody").sortable();
	// $("#pointsTableBody").sortable();
	//$("#profileTableBody").sortable();
</script>
