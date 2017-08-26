

This is home<br>This is home<br>This is home<br>This is home<br>

<form action="<?php echo base_url(); ?>projects/myfunction" method="POST">
					    <label>Project name</label>
		                <input class="form-control" placeholder="Enter name" type="text" name="newProjectName" id="newProjectName">
		                <?php echo form_error('newProjectName'); ?>
				    	<div class="modal-footer">
				    		<input class="btn btn-primary" type="submit" value="Create">
			    		</div>
			  		</form>

	
