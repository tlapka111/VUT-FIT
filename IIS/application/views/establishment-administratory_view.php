<?php	require_once ('components/navbar.php'); ?>
<div class="telo">
<fieldset style="margin-left: 4vw;margin-top: 2vw;">
	<legend style="font-size: xx-large;color: #0d0d0d">Správa provozoven</legend>




	<table class="table" style="width: 85vw;margin-top: 2vw">
		<thead class="thead-dark">
		<tr>
			<th  style="text-align: center" scope="col">Název provozovny</th>
			<th style="text-align: center" scope="col">Město</th>
			<th style="text-align: center"  scope="col">Ulice</th>
			<th style="text-align: center" scope="col">Číslo popisné</th>
			<th style="text-align: center" scope="col">Nabídky provozoven</th>
			<th style="text-align: center" scope="col">Editace provozoven</th>
			<th style="text-align: center" scope="col">Otevření/uzavření objednávek</th>
		</tr>
		</thead>
		<tbody>

			<?php foreach ($establishements as $establishement):?>
			<tr>
				<th scope="col" style="text-align: center" ><?php echo $establishement->name?></th>
				<th scope="col"style="text-align: center" ><?php echo $establishement->city?></th>
				<th scope="col"style="text-align: center" ><?php echo $establishement->street?></th>
				<th scope="col"style="text-align: center" ><?php echo $establishement->houseNumber?></th>
				<th scope="col"style="text-align: center" ><button class="btn btn-success" onclick="location.href='<?php echo base_url() ?>sprava-provozoven-nabidek/nabidkaProvozovny/<?php echo $establishement->establishmentId?>'">Nabídka provozovny</button></th>
				<th scope="col"style="text-align: center" >
                                    <button class="btn btn-info" onclick="location.href='<?php echo base_url() ?>sprava-provozoven-nabidek/editaceProvozovny/<?php echo $establishement->establishmentId?>'">Editace provozovny</button>
				</th>
				<?php if (($establishement->isOpen) == 'true') { ?>
				<th  style="text-align: center" scope="col"><button class="btn btn-danger" onclick="location.href='<?php echo base_url() ?>sprava-provozoven-nabidek/ukonceniNabidky/<?php echo $establishement->establishmentId?>'">Uzavřít objednávky</button></th>
				<?php }else {?>
					<th scope="col" style="text-align: center" ><button style="width: 10vw" class="btn btn-success" onclick="location.href='<?php echo base_url() ?>sprava-provozoven-nabidek/otevreniNabidky/<?php echo $establishement->establishmentId?>'">Otevřít objednávky</button></th>
				<?php }?>
			</tr>
			<?php endforeach;?>


		</tbody>
	</table>

	<button type="button" onclick="location.href='<?php echo base_url() ?>sprava-provozoven-nabidek/nova-provozovna'" style="margin-left: 70vw;width: 15vw;font-weight: bold;margin-bottom: 3vw" class="btn btn-outline-info">Přidání nové provozovny</button>
</fieldset>
</div>