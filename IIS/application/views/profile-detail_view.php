<?php require_once ('components/navbar.php')?>
<fieldset style="margin-top: 9vw; margin-left: 5vw">
	<legend style="color: #0d0d0d"> Detail profilu uživatele - <?php echo $user->firstName . " " . $user->lastName ?></legend>
	<fieldset>
		<div class="form-group">
			<fieldset style="width: 20vw">
				<label class="control-label" for="1">Jméno</label>
				<input class="form-control"  style="font-weight: bolder; margin-bottom: 1vw" id="1" type="text" value="<?php echo $user->firstName?>" readonly="">
				<label class="control-label" for="2">Příjmení</label>
				<input class="form-control"  style="font-weight: bolder; margin-bottom: 1vw" id="2" type="text" value="<?php echo $user->lastName?>" readonly="">
				<label class="control-label" for="3">Email</label>
				<input class="form-control"  style="font-weight: bolder; margin-bottom: 1vw" id="6" type="text" value="<?php echo $user->email?>" readonly="">
				<label class="control-label" for="3">Telefonní číslo</label>
				<input class="form-control"  style="font-weight: bolder; margin-bottom: 1vw" id="3" type="text" value="<?php echo $user->phoneNumber?>" readonly="">
				<label class="control-label" for="4">Adresa</label>
				<input class="form-control"  style="font-weight: bolder; margin-bottom: 1vw" id="4" type="text" value="<?php echo $user->deliveryCity . ", " . $user->deliveryStreet . " " . $user->deliveryHouseNumber ?>" readonly="">
				<?php if($role=='admin'){?>
					<label class="control-label" for="5">Role</label>
					<input class="form-control"  style="background:#FBE8CD ;font-weight: bolder; margin-bottom: 1vw" id="5" type="text" value="<?php
					if ($user->roleName == 'admin'){echo 'admin';}if ($user->roleName == 'user'){echo 'běžný uživatel';}if ($user->roleName == 'operator'){echo 'operátor';}if ($user->roleName == 'driver'){echo 'řidič';}
					?>" readonly="">
				<?php }?>
			</fieldset>
		</div>
	</fieldset>
	<?php if($role == 'admin'){?>
		<button onclick="location.href='<?php echo base_url()?>sprava-uzivatelu/'" style=" width: 6vw" class="btn-primary btn"> Zpět </button>
		<button onclick="location.href='<?php echo base_url()?>/sprava-uzivatelu/editace-uzivatele/<?php echo $user->userId?>'" style="margin-left: 6.5vw" class="btn-danger btn"> Editovat profil</button>
	<?php }else{?>
		<button onclick="location.href='<?php echo base_url()?>'" style=" width: 6vw" class="btn-primary btn"> Zpět </button>
		<button onclick="location.href='<?php echo base_url()?>detail-profilu/editace/<?php echo $user->userId?>'" style="margin-left: 6.5vw" class="btn-danger btn"> Editovat profil</button>
	<?php }?>
</fieldset>
