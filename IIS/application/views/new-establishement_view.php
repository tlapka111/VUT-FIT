<?php require_once('components/navbar.php')?>

<form  action="<?php echo site_url('sprava-provozoven-nabidek/pridatProvozovnu');?>" method="post">
	<fieldset style="margin-left: 10vw;margin-top: 3vw;">
	<legend style="font-size: xx-large;color: #0d0d0d">Přídání nové provozovny</legend>
		<div class="form-group " style="width: 40vw">
			<label for="name" style="color: #0d0d0d">
				Název provozovny
				<span class="required_elements"> *</span>
			</label>
			<input type="text" class="form-control" id="name" name="name" placeholder="Zadejte název provozovny" required/>
		</div>

		<div class="form-group " style="width: 40vw">
			<label for="description" style="color: #0d0d0d">
				Krátký popis o provozovně
			</label>
			<input type="text" class="form-control" id="description" name="description" placeholder="Zadejte popis provozovny">
		</div>
		<div class="form-group " style="width: 40vw">
			<label for="priceCategory" style="color: #0d0d0d">
				Orientační cenová kategorie provozovny:
			</label>
<!--				<select class="form-control" style="width: 6vw" id="priceCategory" name="priceCategory">
					<option>1</option>
					<option>2</option>
					<option>3</option>
					<option>4</option>
					<option>5</option>
				</select>-->
                    <div class="custom-control custom-radio custom-control-inline">
                        <input type="radio" class="custom-control-input" name="priceCategory" id="priceCategory1" value="1" checked>
                        <label class="custom-control-label" for="priceCategory1">1</label>
                    </div>
                    <div class="custom-control custom-radio custom-control-inline">
                        <input type="radio" class="custom-control-input" name="priceCategory" id="priceCategory2" value="2">
                        <label class="custom-control-label" for="priceCategory2">2</label>
                    </div>
                    <div class="custom-control custom-radio custom-control-inline">
                        <input type="radio" class="custom-control-input" name="priceCategory" id="priceCategory3" value="3">
                        <label class="custom-control-label" for="priceCategory3">3</label>
                    </div>
                    <div class="custom-control custom-radio custom-control-inline">
                        <input type="radio" class="custom-control-input" name="priceCategory" id="priceCategory4" value="4">
                        <label class="custom-control-label" for="priceCategory4">4</label>
                    </div>
                    <div class="custom-control custom-radio custom-control-inline">
                        <input type="radio" class="custom-control-input" name="priceCategory" id="priceCategory5" value="5">
                        <label class="custom-control-label" for="priceCategory5">5</label>
                    </div>
		</div>
		<div class="form-group" style="width: 40vw">
			<label style="color: #0d0d0d">Adresa
				<span class="required_elements"> *</span>
			</label>
			<input type="text" class="form-control address_parts" id="city" name="city" placeholder="Zadejte město" required/>
			<input type="text" class="form-control address_parts" id="street" name="street" placeholder="Zadejte ulici"required/>
			<input type="text" class="form-control address_parts" id="houseNumber" name="houseNumber" placeholder="Zadejte číslo  popisné"required/>
		</div>
		<div class="form-check" style="margin-bottom: 1vw">
			<label class="form-check-label">
				<input class="form-check-input" type="checkbox" id="isOpen" name="isOpen" value="true" checked="">
				Přímá už provozovna objednávky?
				<span class="required_elements"> *</span>
			</label>
		</div>
		<label>
			<span class="required_elements"> *</span>
			Označená pole jsou povinná
		</label>
		<br>
		<button  style="margin-left: 25vw; font-weight: bold;width: 10vw" class="btn btn-outline-success"  type="submit" id="login_btn" name="new-request">Přidat provozovnu</button>
		<h6 style="color: red;margin-top: 1vw;margin-left: 10vw"><?php echo $this->session->flashdata('error_msg');?> </h6>
	</fieldset>
</form>
