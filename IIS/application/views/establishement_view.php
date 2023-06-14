<?php	require_once ('components/navbar.php'); ?>

<div class="telo">
<fieldset style="margin-left: 4vw;margin-top: 2vw;">
	<legend style="font-size: xx-large;color: #0d0d0d"><?php echo $establishement->name;?></legend>
        <legend style="font-size: larger;color: #0d0d0d"><?php echo $establishement->description;?></legend>
        
        <!-- FILTROVÁNI -->
        <form method="POST">
        <div class="form-group" id="filtering">
            <label for="foodAttributes" style="color: #0d0d0d">
                Filtrování dle atributů
                <span class="required_elements"> *</span>
            </label><br>
            <div class="custom-control custom-checkbox custom-control-inline">
                <input type="checkbox" class="custom-control-input" name="checkedAttibutes[]" id="foodAttributes1" value="1">
                <label class="custom-control-label" for="foodAttributes1">Smažené</label>
            </div>
            <div class="custom-control custom-checkbox custom-control-inline">
                <input type="checkbox" class="custom-control-input" name="checkedAttibutes[]" id="foodAttributes2" value="2">
                <label class="custom-control-label" for="foodAttributes2">RAW</label>
            </div>
            <div class="custom-control custom-checkbox custom-control-inline">
                <input type="checkbox" class="custom-control-input" name="checkedAttibutes[]" id="foodAttributes3" value="3">
                <label class="custom-control-label" for="foodAttributes3">Bio</label>
            </div>
            <div class="custom-control custom-checkbox custom-control-inline">
                <input type="checkbox" class="custom-control-input" name="checkedAttibutes[]" id="foodAttributes4" value="4">
                <label class="custom-control-label" for="foodAttributes4">Grilované</label>
            </div>
            <div class="custom-control custom-checkbox custom-control-inline">
                <input type="checkbox" class="custom-control-input" name="checkedAttibutes[]" id="foodAttributes5" value="5">
                <label class="custom-control-label" for="foodAttributes5">Bezmasé</label>
            </div>
            <button type="submit" name="filtering" style="width: 29vw;font-weight: bold;margin-bottom: 3vw;margin-top: 2vw" class="btn btn-outline-info">Filtrovat</button>
        </div>
        </form>

        <!-- DENNÍ NABÍDKA -->
        <legend style="font-size: xx-large;color: #0d0d0d;margin-top: 3vw">Denní nabídka</legend>
        <?php
            $dotaz = "SELECT * FROM food WHERE establishmentId = $establishement->establishmentId AND foodType = 'Denni'";
            if (isset($_POST['filtering']) && isset($_POST['checkedAttibutes'])) {
                $dotaz = "SELECT * FROM food JOIN foodhaveatrributes WHERE food.foodId = foodhaveatrributes.foodHaveAtrributes_foodId AND establishmentId = $establishement->establishmentId AND foodType = 'Denni'";
                $checkedAttributes = $_POST['checkedAttibutes'];
                foreach ($checkedAttributes as $attrib) {
                    $dotaz = $dotaz . " AND foodhaveatrributes.foodHaveAtrributes_atrributeId = $attrib";
                }
            }
            $products = $this->db->query($dotaz)->result();
        ?>
        <table>
            <tbody>
                <tr>
                    <td class="align-top">
                        <table class="table table-hover" style="width: 70vw;margin-top: 2vw">
                            <tbody>
                                <?php foreach($products as $product):?>
                                <tr>
                                    <td class="align-middle"><img src="<?php echo $product->foodImage;?>" class="img_enlarge" width="150px"></td>
                                    <td class="align-middle">
                                        <div>
                                            <div style="font-size: x-large">
                                                <label><?php echo $product->foodName;?></label>
                                            </div>
                                            <div style="font-size: larger">
                                                <label><?php echo $product->foodDescription;?></label>
                                            </div>
                                        </div>
                                    </td>
                                    <td class="align-middle" style="font-size: x-large"><label><?php echo $product->foodWeight . ($product->foodType == "Napoj" ? " ml" : " g"); ?></label></td>
                                    <td class="align-middle" style="font-size: x-large"><label><?php echo $product->foodPrice . " Kč";?></label></td>
                                    <form method="POST">
                                        <td class="align-middle"><button type="submit" name="removeFood" class="btn btn-danger">Přidat do košíku</button></td>
                                        <input type="hidden" name="foodID" value="<?php echo $product->foodId;?>">
                                    </form>
                                </tr>
                                <?php endforeach;?>
                            </tbody>
                        </table>
                    </td>
                </tr>
            </tbody>
        </table>
        
        <!-- STÁLÁ NABÍDKA -->
        <legend style="font-size: xx-large;color: #0d0d0d;margin-top: 3vw">Stálá nabídka</legend>
        <?php
            $dotaz = "SELECT * FROM food WHERE establishmentId = $establishement->establishmentId AND foodType = 'Stala'";
            if (isset($_POST['filtering']) && isset($_POST['checkedAttibutes'])) {
                $dotaz = "SELECT * FROM food JOIN foodhaveatrributes WHERE food.foodId = foodhaveatrributes.foodHaveAtrributes_foodId AND establishmentId = $establishement->establishmentId AND foodType = 'Stala'";
                $checkedAttributes = $_POST['checkedAttibutes'];
                foreach ($checkedAttributes as $attrib) {
                    $dotaz = $dotaz . " AND foodhaveatrributes.foodHaveAtrributes_atrributeId = $attrib";
                }
            }
            $products = $this->db->query($dotaz)->result();
        ?>
        <table>
            <tbody>
                <tr>
                    <td class="align-top">
                        <table class="table table-hover" style="width: 70vw;margin-top: 2vw">
                            <tbody>
                                <?php foreach($products as $product):?>
                                <tr>
                                    <td class="align-middle"><img src="<?php echo $product->foodImage;?>" width="150px"></td>
                                    <td class="align-middle">
                                        <div>
                                            <div style="font-size: x-large">
                                                <label><?php echo $product->foodName;?></label>
                                            </div>
                                            <div style="font-size: larger">
                                                <label><?php echo $product->foodDescription;?></label>
                                            </div>
                                        </div>
                                    </td>
                                    <td class="align-middle" style="font-size: x-large"><label><?php echo $product->foodWeight . ($product->foodType == "Napoj" ? " ml" : " g"); ?></label></td>
                                    <td class="align-middle" style="font-size: x-large"><label><?php echo $product->foodPrice . " Kč";?></label></td>
                                    <form method="POST">
                                        <td class="align-middle"><button type="submit" name="removeFood" class="btn btn-danger">Přidat do košíku</button></td>
                                        <input type="hidden" name="foodID" value="<?php echo $product->foodId;?>">
                                    </form>
                                </tr>
                                <?php endforeach;?>
                            </tbody>
                        </table>
                    </td>
                </tr>
            </tbody>
        </table>
        
        <!-- STÁLÁ NABÍDKA -->
        <legend style="font-size: xx-large;color: #0d0d0d;margin-top: 3vw">Nápoje</legend>
        <?php
            $products = $this->db->query("SELECT * FROM food WHERE establishmentId = $establishement->establishmentId AND foodType = 'Napoj'")->result();
        ?>
        <table>
            <tbody>
                <tr>
                    <td class="align-top">
                        <table class="table table-hover" style="width: 70vw;margin-top: 2vw">
                            <tbody>
                                <?php foreach($products as $product):?>
                                <tr>
                                    <td class="align-middle"><img src="<?php echo $product->foodImage;?>" width="150px"></td>
                                    <td class="align-middle">
                                        <div>
                                            <div style="font-size: x-large">
                                                <label><?php echo $product->foodName;?></label>
                                            </div>
                                            <div style="font-size: larger">
                                                <label><?php echo $product->foodDescription;?></label>
                                            </div>
                                        </div>
                                    </td>
                                    <td class="align-middle" style="font-size: x-large"><label><?php echo $product->foodWeight . ($product->foodType == "Napoj" ? " ml" : " g"); ?></label></td>
                                    <td class="align-middle" style="font-size: x-large"><label><?php echo $product->foodPrice . " Kč";?></label></td>
                                    <form method="POST">
                                        <td class="align-middle"><button type="submit" name="removeFood" class="btn btn-danger">Přidat do košíku</button></td>
                                        <input type="hidden" name="foodID" value="<?php echo $product->foodId;?>">
                                    </form>
                                </tr>
                                <?php endforeach;?>
                            </tbody>
                        </table>
                    </td>
                </tr>
            </tbody>
        </table>
</fieldset>
</div>