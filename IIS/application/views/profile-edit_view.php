<?php require_once ('components/navbar.php')?>
<fieldset style="margin-top: 9vw; margin-left: 5vw">
	<legend style="color: #0d0d0d"> Editace profilu - <?php echo $user->firstName . " " . $user->lastName ?></legend>
	<fieldset>
		<div class="form-group">
			<fieldset style="width: 20vw">
			<form method="post">
				<label class="control-label" for="1">Jméno</label>
				<input class="form-control"  style="font-weight: bolder; margin-bottom: 1vw" id="firstName" name="firstName" type="text" value="<?php echo $user->firstName?>" required/>
				<label class="control-label" for="2">Příjmení</label>
				<input class="form-control"  style="font-weight: bolder; margin-bottom: 1vw" id="lastName" name="lastName" type="text" value="<?php echo $user->lastName?>" required/>
				<label class="control-label" for="3">Email</label>
				<input class="form-control"  style="font-weight: bolder; margin-bottom: 1vw" id="6" type="text" value="<?php echo $user->email?>" readonly="">
				<label class="control-label" for="3">Telefonní číslo</label>
				<input class="form-control"  style="font-weight: bolder; margin-bottom: 1vw" id="phoneNumber" name="phoneNumber" type="text" value="<?php echo $user->phoneNumber?>" required/>
				<div class="form-group" style="width: 20vw">
					<label style="color: #0d0d0d">Adresa</label>
					<input type="text" class="form-control address_parts" id="city" name="city" placeholder="Zadejte město" value="<?php echo $user->deliveryCity?>">
					<input type="text" class="form-control address_parts" id="street" name="street" placeholder="Zadejte ulici" value="<?php echo $user->deliveryStreet?>">
					<input type="text" class="form-control address_parts" id="houseNumber" name="houseNumber" placeholder="Zadejte číslo  popisné" value="<?php echo $user->deliveryHouseNumber?>">
				</div>
				<button type="submit" class="btn-danger btn" style="margin-left: 7vw"> Uložit změny</button>
			</form>
			<button class="btn btn-primary" onclick="location.href='<?php echo base_url()?>detail-profilu/<?php echo $user->userId?>'">Zpět</button>
		</fieldset>
		</div>
	</fieldset>
</fieldset>
