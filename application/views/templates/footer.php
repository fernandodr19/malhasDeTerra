	</div>
	<div class="modal fade" id="add_project">
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
	</body>
</html> 