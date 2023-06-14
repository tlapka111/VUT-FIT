<?php require_once('components/navbar.php')?>

<fieldset style="margin-top: 7vw;margin-left: 3vw">
	<legend style="color: #0d0d0d"> Editace profilu uživatele</legend>
	<form action="<?php echo base_url()?>sprava-uzivatelu/editace-uzivatele/<?php echo $user->userId?>" method="post">
		<fieldset>
			<div class="form-group " style="width: 40vw">
				<label for="firstName" style="color: #0d0d0d">
					Jméno uživatele
				</label>
				<input type="text" class="form-control" id="firstName" name="firstName" value="<?php echo $user->firstName ?>" required/>
			</div>
			<div class="form-group" style="width: 40vw">
				<label for="firstName" style="color: #0d0d0d">
					Příjmení uživatele
				</label>
				<input type="text" class="form-control" id="lastName" name="lastName" value="<?php echo $user->lastName ?>" required/>
			</div>
			<div class="form-group " style="width: 40vw">
				<label for="firstName" style="color: #0d0d0d">
					Role uživatele
				</label>
				<select class="form-control list-group-item-warning" style="width: 14vw"   id="role" name="role">
					<option <?php if($selectedValue == '1'){echo 'selected="selected"';}?>>admin</option>
					<option  <?php if($selectedValue == '2'){echo 'selected="selected"';}?>>operátor</option>
					<option  <?php if($selectedValue == '3'){echo 'selected="selected"';}?>>řidič</option>
					<option  <?php if($selectedValue == '4'){echo 'selected="selected"';}?>>běžný uživatel</option>
				</select>
			</div>
			<div class="form-group" style="width: 40vw">
				<label for="phoneNum" style="color: #0d0d0d">Telefon</label>
				<input type="tel" class="form-control" id="phoneNum" name="phoneNum" value="<?php echo $user->phoneNumber?>">
			</div>
			<div class="form-group" style="width: 40vw">
				<label style="color: #0d0d0d">Město</label>
				<input type="text" class="form-control address_parts" id="city" name="city" value="<?php echo $user->deliveryCity?>">
				<label style="color: #0d0d0d">Ulice</label>
				<input type="text" class="form-control address_parts" id="street" name="street" value="<?php echo $user->deliveryStreet?>">
				<label style="color: #0d0d0d">Číslo popisné</label>
				<input type="text" class="form-control address_parts" id="houseNumber" name="houseNumber" value="<?php echo $user->deliveryHouseNumber?>">
			</div>
		<button class="btn-danger btn"  style="text-align: center;margin-left: 15vw" type="submit"> Uložit změny</button>
			<h5 style="color: green;font-weight: bolder;margin-top: 1vw;margin-left: 10vw"><?php echo $this->session->flashdata('ok_msg');?> </h5>
		</fieldset>
	</form>
	<button class="btn btn-outline-primary btn-sm"  style="margin-bottom: 3vw" onclick="location.href='<?php echo base_url() ?>sprava-uzivatelu'">Zpět</button>
</fieldset>
