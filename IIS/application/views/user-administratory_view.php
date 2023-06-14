<?php require_once ('components/navbar.php'); ?>
<fieldset style="margin-left: 10vw;margin-top: 3vw;">
	<legend style="font-size: xx-large;color: #0d0d0d">Správa uživatelů</legend>
	<form  action="<?php echo site_url('sprava-uzivatelu/');?>" method="post">
	<fieldset style="margin-left: 3vw;margin-top: 1vw;">
		<div class="form-group " style="width: 40vw">
			<?php require_once('components/user-administratory-tabs.php');?>
			<label for="priceCategory" style="color: #0d0d0d">
				Filtroval podle role uživatele
			</label>
			<table class="table">
			<tr>
				<th>
			<select class="form-control" style="width: 14vw" id="role" name="role">
				<option>admin</option>
				<option>operátor</option>
				<option>řidič</option>
				<option>běžný uživatel</option>
				<option>všichni</option>
			</select>
				</th>
				<th>
					<button class="btn btn-outline-primary"  style="font-weight: bold" type="submit">Filtrovat</button>
				</th>
				<tr>
			</table>
		</div>
	</fieldset>
	</form>

			<?php if (true){?>
			<table class="table" style="width: 75vw;margin-top: 2vw;margin-bottom: 5vw">
				<thead class="thead-dark">
				<tr>
					<th  style="text-align: center" scope="col">ID uživatele</th>
					<th style="text-align: center" scope="col">Jméno</th>
					<th style="text-align: center"  scope="col">Email</th>
					<th style="text-align: center" scope="col">Role</th>
					<th style="text-align: center" scope="col">Editace profilu</th>
					<th style="text-align: center" scope="col">Objednávky uživatele</th>
					<th style="text-align: center" scope="col">Detail profilu</th>
				</tr>
				</thead>
				<tbody>
				<?php foreach ($users as $user){?>
					<tr>
						<th style="text-align: center" scope="col"><?php echo $user->userId?> </th>
						<th style="text-align: center" scope="col"><?php echo $user->firstName ." " . $user->lastName?> </th>
						<th style="text-align: center" scope="col"><?php echo $user->email?> </th>
						<th style="text-align: center" scope="col"><?php if($user->roleName == 'admin'){echo "admin";}if($user->roleName == 'driver'){echo "admin";}if($user->roleName == 'operator'){echo "operátor";} if($user->roleName == 'user'){echo "běžný uživatel";}?> </th>
						<th scope="col" style="text-align: center" ><button style="width: 10vw" class="btn btn-sm btn-warning" onclick="location.href='<?php echo base_url() ?>sprava-uzivatelu/editace-uzivatele/<?php echo $user->userId?> '">Editovat</button></th>
						<th scope="col" style="text-align: center" ><button style="width: 10vw" class="btn btn-sm btn-success" onclick="location.href='<?php echo base_url() ?>#<?php echo $user->userId?> '">Objednávky uživatele</button></th>
						<th scope="col" style="text-align: center" ><button style="background: navy;width: 10vw" class="btn btn-sm btn-info" onclick="location.href='<?php echo base_url() ?>detail-profilu/<?php echo $user->userId?>'">Detail profilu</button></th>
					</tr>
				<?php }?>
				</tbody>
			</table>
		<?php }?>
</fieldset>

