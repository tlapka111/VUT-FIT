<?php require_once ('components/navbar.php')?>

<fieldset style="margin-left: 3vw;margin-top: 10vw;">
	<?php require_once ('components/user-administratory-tabs.php')?>
	<form method="post">
		<table>
			<th style="alignment: center">
			<H5  style="color: #0d0d0d; margin-right: 5vw"> Vyhledat podle emailu</H5>
			</th>
			<th style="alignment: center">
		<div class="form-group" style="width: 20vw">
			<input type="email" class="form-control" id="email" name="email" placeholder="Zadejte email" required>
		</div>
			</th>
			<th style="alignment: center">
				<button class="btn btn-info" style="margin-left: 3vw"> Vyhledat podle emailu</button>
			</th>
		</table>
	</form>
	<form method="post">
		<table>
			<th style="alignment: center">
				<H5 style="color: #0d0d0d; margin-right: 5.45vw"> Vyhledat podle jména</H5>
			</th>
			<th style="alignment: center">
				<div class="form-group" style="width: 9.8vw;  margin-right: 0.4vw">
					<input type="text" class="form-control" id="name" name="name" placeholder="Zadejte křeštní jméno" required>
				</div>
			</th>
			<th style="alignment: center">
				<div class="form-group" style="width: 9.8vw">
					<input type="text" class="form-control" id="lastName" name="lastName" placeholder="Zadejte příjmení" required>

				</div>
			</th>
			<th style="alignment: center">
				<button class="btn btn-info" style="margin-left: 3vw"> Vyhledat podle jména</button>
			</th>
		</table>
	</form>

	<form method="post">
		<table>
			<th style="alignment: center">
				<H5  style="color: #0d0d0d; margin-right: 5.6vw"> Vyhledat podle města</H5>
			</th>
			<th style="alignment: center">
				<div class="form-group" style="width: 20vw">
					<input type="text" class="form-control" id="city" name="city" placeholder="Zadejte město" required>
				</div>
			</th>
			<th style="alignment: center">
				<button class="btn btn-info" style="margin-left: 3vw"> Vyhledat podle města</button>
			</th>
		</table>
	</form>
	<?php if($noData && $shouldRender){?>
		<h1 style="color: red;margin-left: 8vw;margin-top: 10vw">Vyhledávání neodpovídá žádný uživatelský profil</h1>
	<?php }?>
	<?php  if($shouldRender && !$noData){?>
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
					<th scope="col" style="text-align: center" ><button style="background: navy;width: 10vw" class="btn btn-sm btn-info" onclick="location.href='<?php echo base_url() ?>detail-profilu/<?php echo $user->userId?> '">Detail profilu</button></th>
				</tr>
			<?php }?>
			</tbody>
	<?php }?>
</fieldset>
