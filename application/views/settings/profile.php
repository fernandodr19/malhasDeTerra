<div class="col-md-4">
  <h2>Profile</h2>
  <div class="form-group">
    <form>  
      <label>Name</label>
      <input class="form-control" type="text"  name="name" value="<?= $user['firstName'] ?>">
      <br>
      <label>Email</label>
      <input class="form-control" type="text" name="email" value="<?= $user['email'] ?>">
    </form>
  </div>
  <br/>
  <a href="#" class="btn btn-success">Update</a>
  </div>
  <div class="col-md-4">
    <br/>
    <h4>Picture</h4>
    <img class="lib-img" src="">  
  </div>
</div>