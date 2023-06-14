<?php require_once ('components/navbar.php');?>
<div class="bg align-top">
<div class=telo>
    <div class="tabulka">
    <h2>Seznam provozoven:</h2>
    <table class="table" style="width: 830px; margin-top: 1vw">
	<thead class="thead-dark">
            <tr>
                <th  style="text-align: center" scope="col">Název provozovny</th>
		<th style="text-align: center"  scope="col">Adresa</th>
                <th style="text-align: center" scope="col">Cenová kategorie</th>
		<th style="text-align: center" scope="col">Přijímá objednávky</th>
            </tr>
	</thead>
        <tbody id="myTable">
            <?php foreach ($establishements as $establishement):?>
		<tr>
                    <th scope="col" style="text-align: center" ><a href="<?php echo base_url() ?>provozovna/<?php echo $establishement->establishmentId?>"><font size="3"><?php echo $establishement->name?></font></a></th>
                    <th scope="col"style="text-align: center" ><?php echo $establishement->city . ", " . $establishement->street . " " . $establishement->houseNumber?></th>
                    <th scope="col"style="text-align: center" >
                        <?php
                        switch($establishement->priceCategory) {
                            case 1:
                                echo '<font color="#00bb00" size="4">$</font>';
                                break;
                            case 2:
                                echo '<font color="#acbf26" size="4">$$</font>';
                                break;
                            case 3:
                                echo '<font color="#ffc82d" size="4">$$$</font>';
                                break;
                            case 4:
                                echo '<font color="#ff962d" size="4">$$$$</font>';
                                break;
                            case 5:
                                echo '<font color="#ff452d" size="4">$$$$$</font>';
                                break;
                        }
                        ?>
                    </th>
                    <?php if (($establishement->isOpen) == 'true') { ?> 
                    <th scope="col"style="text-align: center" ><img src="/iis/images/check_square.svg" width="25px" alt="check_mark" /></th>
                    <?php }else {?>
                    <th scope="col"style="text-align: center" ><img src="/iis/images/cross.png" width="25px" alt="check_mark" /></th>
                    <?php }?>
		</tr>
            <?php endforeach;?>
	</tbody>
    </table>
    </div>
</div>
</div>

<script>
    $(document).ready(function() {
        $("#mesto").on("keyup", function() {
            var value = $(this).val().toLowerCase();
            $("#myTable tr").filter(function() {
                $(this).toggle($(this).text().toLowerCase().indexOf(value) > -1)
            });
        });
    });
</script>
