<div class="panel panel-primary">
	<div class="panel-heading">
	    <h3 class="panel-title"><?= $project['pName'] ?></h3>
	    <span class="pull-right">
	        <ul class="nav panel-tabs">
	            <li class="active"><a data-toggle="tab" href="#project">Projeto</a></li>
				<li><a data-toggle="tab" href="#groudingSystems">Malhas de Terra</a></li>
				<li><a data-toggle="tab" href="#report" onclick="test()">Relatório</a></li>
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
			    	  <label>Nome</label>
			    	  <input class="form-control" type="text"  name="gs_name" value="">
			    	  <select name="gs" id="gs" style="width: 50%">
						<option value="">Selecione</option>
						<option value="RJ">RJ</option>
						<option value="SC">SC</option>
			 		 </select>
					 <a href="#" class="btn btn-danger" style="float:right">Deletar</a>
					 <a href="#" class="btn btn-success" style="float:right">Adicionar</a>
					 <br><br>
					 <label>Comprimento máximo do segmento do condutor</label>
				      <input class="form-control" type="text"  name="gs_conductorMaxLength" value="">
				      <br>
				      <label>Comprimento máximo do segmento do condutor (m)</label>
				      <input class="form-control" type="text"  name="gs_conductorMaxLength" value="">
				      <br>
				      <label>Número de camadas do solo</label><br>
				      <select name="gs" id="gs" style="width: 50%">
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
				      <label>Resistividade da segunda camada do solo (&Omega;.m) (se existir checkHere)</label>
				      <input class="form-control" type="text"  name="gs_secondLayerResistivity" value="">
				      <br>
				      <label>Profundidade da camada de brita (m) (se existir checkHere)</label>
				      <input class="form-control" type="text"  name="gs_crushedStoneLayerDepth" value="">
				      <br>
				      <label>Resistividade da camada de brita (&Omega;.m)</label>
				      <input class="form-control" type="text"  name="gs_crushedStoneLayerResistivity" value="">
				      <br>
				      <label>Corrente injetada (A)</label>
				      <input class="form-control" type="text"  name="gs_secondLayerResistivity" value="">
				      <br>
				      <label>Arquivo DXF</label>
				      <input class="" type="file"  name="gs_secondLayerResistivity" value="" ><!--checkHere css-->
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
			                        </tr>
			                    </thead>
			                    <tbody id="conductorsTable">
			                    </tbody>
			                </table> 
			                <a class="btn btn-primary" style="float:right" onclick="addRow();">Adicionar</a>
                        </div>
                        <div class="tab-pane fade" id="superficialPotentialPoints">Primary 2</div>
                        <div class="tab-pane fade" id="superficialPotentialProfile">Primary 3</div>
                    </div>
                </div>
            </div>
			  	</div>
			</div>
			<div id="report" class="tab-pane fade">
		  		<div class="form-group">
				    <form>  
				      
				    </form>
			  	</div>
		  		<a href="#" class="btn btn-success" style="float:right">Gerar Relatório</a>
			</div>
		</div>
	</div>
</div>

<!--checkHere fazer css para os comboboxes-->

<script type="text/javascript"> //passar pra test.js
	function addRow() {
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
	 	'</tr>';
	}
</script>
