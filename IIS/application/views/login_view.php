<?php
require_once ('components/navbar.php');
?>
<div class="telo">
<form  action="<?php echo site_url('prihlaseni/overeni');?>" method="post">
<fieldset style="margin-left: 25vw;margin-top: 7vw;">
	<legend style="font-size: xx-large;color: #0d0d0d">Příhlášení</legend>
	<div class="form-group " style="width: 40vw">
		<label for="email" style="color: #0d0d0d">
			Email
		</label>
		<input style="width: 30vw" type="text" class="form-control" id="email" name="email" placeholder="Zadejte emailovou adresu" required/>
	</div>
	<div class="form-group" style="width: 40vw">
		<label for="password" style="color: #0d0d0d">
			Heslo
		</label>
		<input  style="width: 30vw"  type="password" class="form-control" id="password" name="password" placeholder="Zadejte heslo"  required/>
	</div>
	<button  style="margin-left: 10vw; font-weight: bold;width: 10vw" class="btn btn-outline-primary" type="submit" id="login_btn" name="login_request">Příhlásit se</button>
	<h6 style="color: red;margin-top: 1vw;margin-left: 10vw"><?php echo $this->session->flashdata('error_msg');?> </h6>
</fieldset>
</form>
</div>