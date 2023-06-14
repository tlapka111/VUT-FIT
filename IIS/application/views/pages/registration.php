<?php
require_once ('components/navbar.php');
?>

<script>
    var pE = function() {
        pass = document.getElementById('password').value;
        pass2 = document.getElementById('passwordAgain').value;
        if(pass == pass2) {
            document.getElementById('message').style.color = 'green';
            document.getElementById('message').innerHTML = 'Hesla se shodují';
            document.getElementById('register').disabled = false;
        } else {
            document.getElementById('message').style.color = 'red';
            document.getElementById('message').innerHTML = 'Hesla se neshodují';
            document.getElementById('register').disabled = true;
        }
    }
</script>

<form method="POST">
	<fieldset class="registration_form">
		<legend>Registrace</legend>
		<div class="form-group " style="width: 40vw">
			<label for="firstName">
				Jméno
				<span class="required_elements"> *</span>
			</label>
                    <input type="text" class="form-control" id="firstName" name="firstName" placeholder="Zadejte jméno" required/>
		</div>
		<div class="form-group" style="width: 40vw">
			<label for="lastName">
				Příjmení
				<span class="required_elements"> *</span>
			</label>
                    <input type="text" class="form-control" id="lastName" name="lastName" placeholder="Zadejte příjmení" required>
		</div>
		<div class="form-group" style="width: 40vw">
			<label for="phoneNum">Telefon</label>
			<input type="tel" class="form-control" id="phoneNum" name="phoneNum" placeholder="Zadejte telefon">
		</div>
		<div class="form-group" style="width: 40vw">
			<label>Adresa</label>
			<input type="text" class="form-control address_parts" id="city" name="city" placeholder="Zadejte město">
			<input type="text" class="form-control address_parts" id="street" name="street" placeholder="Zadejte ulici">
			<input type="text" class="form-control address_parts" id="houseNumber" name="houseNumber" placeholder="Zadejte číslo  popisné">
		</div>
		<div class="form-group" style="width: 40vw">
			<label for="email">
				Email
				<span class="required_elements"> *</span>
			</label>
                    <input type="email" class="form-control" id="email" name="email" placeholder="Zadejte email" required>
		</div>
		<div class="form-group" style="width: 40vw">
			<label for="password">
				Heslo
				<span class="required_elements"> *</span>
			</label>
                    <input type="password" class="form-control" id="password" name="password" placeholder="Zadejte heslo" onkeyup='pE();' required>
		</div>
		<div class="form-group" style="width: 40vw">
			<label for="passwordAgain">
				Heslo znovu
				<span class="required_elements"> *</span>
			</label>
                    <input type="password" onkeyup='pE();' class="form-control" id="passwordAgain" name="passwordAgain" placeholder="Zadejte heslo znovu" required>
		</div>
                <label>
                    <span id="message"></span><br/>
                </label>
		<label>
			<span class="required_elements"> *</span>
			Označená pole jsou povinná
		</label>
		<br>
                <button class="btn pull-right btn-outline-primary registration_button" type="submit" id="register" name="register" disabled>Zaregistrovat</button>
	</fieldset>
</form>

<?php
    if(isset($_POST['register'])) {
        $jmeno = trim(htmlspecialchars($_POST['firstName']));
        $prijmeni = trim(htmlspecialchars($_POST['lastName']));
        $telefon = trim(htmlspecialchars($_POST['phoneNum']));
        $mesto = trim(htmlspecialchars($_POST['city']));
        $ulice = trim(htmlspecialchars($_POST['street']));
        $cisloPopisne = trim(htmlspecialchars($_POST['houseNumber']));
        $email = trim(htmlspecialchars($_POST['email']));
        $heslo = trim(htmlspecialchars($_POST['password']));
        
        try {
            $this->db->insert('users', array('firstName' => $jmeno, 'lastName' => $prijmeni, 'email' => $email, 'password'=>md5($heslo),
                'phoneNumber' => $telefon, 'deliveryCity' => $mesto, 'deliveryStreet' => $ulice, 'deliveryHouseNumber' => $cisloPopisne));
            echo "<script type='text/javascript'>alert('Registrace proběhla úspěsně');</script>";
        } catch (Exception $ex) {
            echo "<script type='text/javascript'>alert('Při registraci se vyskytla chyba');</script>";
        }
    }

?>
