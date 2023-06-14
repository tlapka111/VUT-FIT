<div class="nav">
	<div class="obr">
		<a href="<?php echo base_url() ?>">
			<img src="/iis/images/full_logo.png" width="200px" alt="compay_logo" />
		</a>
	</div>	
	<div class="menu">
		<ul>		
			<li>
				<div class="search">
					<form method="post" action="<?php echo site_url('restaurace');?>">
                        <input id="mesto" name="mesto" type="text" onclick="nameFocus();"  class="searchTerm" placeholder="Zadejte město, kde chcete hledat provozovny">
						<button type="submit"  class="searchButton">
						  <i class="fa fa-search"></i>
						</button>
					</form>	
				</div>
			</li>
		</ul>
	</div>
	<div class="sprava">
		<!-- admin -->
		<?php  if (($this->session->userdata('role')) == 'admin'){ ?>
			<div class="dropdown">
				<button class="dropbtn">Správa systému
      				<i class="fa fa-caret-down"></i>
				</button>
				<div class="dropdown-content">
				<!-- admin nebo operátor přiřadí objednávku řidiči a potvrdí přijetí objednávky -->
				<a href="<?php echo base_url() ?>sprava-objednavek/>">Správa objednávek</a>
				<!-- řidíč má přehled o objednávkách, které mu byly přiděleny,
				může měnit status objednávky např. (na cestě, doručeno...)-->
				<a href="<?php echo base_url() ?>rozvazky/>">Přehled rozvážek</a>
				<a href="<?php echo base_url() ?>sprava-provozoven-nabidek/">Správa provozoven a nabídek</a>
				<a href="<?php echo base_url() ?>sprava-uzivatelu/">Správa uživatelů</a>
				<a href="<?php echo base_url() ?>objednavky/<?php echo $this->session->userdata('id')?>">Moje objednávky</a>
				<a href="<?php echo base_url() ?>detail-profilu/<?php echo $this->session->userdata('id')?>">Můj profil</a>
				<a href="<?php echo base_url() ?>prihlaseni/odhlaseni">Odhlásit se</a>
				</div>
			</div>
			
		<!-- operator-->
		<?php  }else if (($this->session->userdata('role')) == 'operator'){ ?>
			<div class="dropdown">
				<button class="dropbtn">Správa
      				<i class="fa fa-caret-down"></i>
				</button>
				<div class="dropdown-content">
				<!-- admin nebo operátor přiřadí objednávku řidiči a potvrdí přijetí objednávky -->
				<a href="<?php echo base_url() ?>sprava-objednavek/>">Správa objednávek</a>
				<!-- řidíč má přehled o objednávkách, které mu byly přiděleny,
				 může měnit status objednávky např. (na cestě, doručeno...)-->
				<a href="<?php echo base_url() ?>rozvazky/>">Přehled rozvážek</a>
				<a href="<?php echo base_url() ?>sprava-provozoven-nabidek/">Správa provozoven a nabídek</a>
				<a href="<?php echo base_url() ?>objednavky/<?php echo $this->session->userdata('id')?>">Moje objednávky</a>
				<a href="<?php echo base_url() ?>detail-profilu/<?php echo $this->session->userdata('id')?>'">Můj profil</a>
				<a href="<?php echo base_url() ?>prihlaseni/odhlaseni">Odhlásit se</a>
				</div>
			</div>
					
		<?php }elseif (($this->session->userdata('role')) == 'driver'){ ?>
			<div class="dropdown">
				<button class="dropbtn">Správa
      				<i class="fa fa-caret-down"></i>
				</button>
				<div class="dropdown-content">
				<!-- řidíč má přehled o objednávkách, které mu byly přiděleny,
				může měnit status objednávky např. (na cestě, doručeno...)-->
				<a href="<?php echo base_url() ?>rozvazky/>">Přehled rozvážek</a>
				<a href="<?php echo base_url() ?>objednavky/<?php echo $this->session->userdata('id')?>">Moje objednávky</a>
				<a href="<?php echo base_url() ?>detail-profilu/<?php echo $this->session->userdata('id')?>'">Můj profil</a>
				<a href="<?php echo base_url() ?>prihlaseni/odhlaseni">Odhlásit se</a>
				</div>
			</div>
				
		<!-- Prihlaseny uzivatel. -->	
		<?php }elseif (($this->session->userdata('role')) == 'user'){ ?>
			<div class="dropdown">
				<button class="dropbtn">Správa
      				<i class="fa fa-caret-down"></i>
				</button>
				<div class="dropdown-content">
				<a href="<?php echo base_url() ?>objednavky/<?php echo $this->session->userdata('id')?>">Moje objednávky</a>
				<a href="<?php echo base_url() ?>detail-profilu/<?php echo $this->session->userdata('id')?>">Můj profil</a>
				<a href="<?php echo base_url() ?>prihlaseni/odhlaseni">Odhlásit se</a>
				</div>
			</div>
				
		<!-- NEprihlaseny uzivatel. -->	
		<?php }else { ?>
			<button class="nav_button"  onclick="location.href='<?php echo base_url() ?>prihlaseni'">
			Přihlášení
			</button>
			<button class="nav_button"  onclick="location.href='<?php echo base_url() ?>registrace'" >
			Registrace
			</button>
		<?php }?>		
	</div>
		<button style="margin-right: 1vw; outline: none;background: transparent no-repeat;border: none; align-content: center" onclick="location.href='<?php echo base_url() ?>kosik'" >
			<img src="/iis/images/kosik.svg" height="60px" width="60px" alt="kosik" />

		</button>
	<hr class="hrr"/>
</div>

<script src="<?php echo base_url().'js/jquery-3.4.1.js'?>" type="text/javascript"></script>
    <script src="<?php echo base_url().'js/bootstrap.js'?>" type="text/javascript"></script>
    <script src="<?php echo base_url().'js/jquery-ui.js'?>" type="text/javascript"></script>
    <script type="text/javascript">
        $(document).ready(function(){
            $( "#mesto" ).autocomplete({
              source: "<?php echo site_url('Home/get_autocomplete/?');?>"
            });
        });
</script>