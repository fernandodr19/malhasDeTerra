
    <div class="modal fade" id="add_project">
            <div class="modal-dialog">
                <div class="modal-content">
                    <div class="modal-header">
                        <h4 class="modal-title">Novo projeto</h4>
                    </div>
                    <div class="modal-body">
                        <form name="newProject" action="<?php echo base_url(); ?>projects/add_project" onsubmit="return validateForm()" method="POST">
                            <label>Nome do projeto</label>
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
        <div class="modal fade" id="gs_add">
            <div class="modal-dialog">
                <div class="modal-content">
                    <div class="modal-header">
                        <h4 class="modal-title">Nova malha de terra</h4>
                    </div>
                    <div class="modal-body">
                        <form action="<?php echo base_url(); ?>groundingSystems/add_gs/<?php echo $project['id'] ?>" method="POST">
                            <label>Nome da malha de terra</label>
                            <input class="form-control" placeholder="Enter name" type="text" name="newGSName" id="newGSName">
                            <div class="modal-footer">
                                <input class="btn btn-primary" type="submit" value="Create">
                            </div>
                        </form>
                    </div>
                </div>
            </div>
        </div>
        <div class="modal fade" id="helpModal">
            <div class="modal-dialog">
                <div class="modal-content">
                    <div class="modal-header">
                        <h4 class="modal-title">Ajuda</h4>
                    </div>
                    <div class="modal-body">
                        <label id="helpLabel"></label>
                        <div class="modal-footer">
                            <input class="btn btn-primary pull-right" type="submit" data-dismiss="modal" value="Ok">
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </body>
</html> 

<script>
function validateForm() {
    var x = document.forms["newProject"]["newProjectName"].value;
    if (x == "") {
        alert("O nome não pode ser vazio."); //verificar se ja existe tambem
        return false;
    }
}
</script>     