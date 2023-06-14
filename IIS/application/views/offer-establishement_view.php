<?php	require_once ('components/navbar.php'); ?>

<div class="telo">
<fieldset style="margin-left: 4vw;margin-top: 2vw;">
	<legend style="font-size: xx-large;color: #0d0d0d"><?php echo $establishement->name;?></legend>
        <legend style="font-size: larger;color: #0d0d0d"><?php echo $establishement->description;?></legend>

        <?php
            $products = $this->db->query("SELECT * FROM food WHERE establishmentId = $establishement->establishmentId")->result();
        ?>
        
        <table>
            <tbody>
                <tr>
                    <td class="align-top">
                        <table class="table table-hover" style="width: 60vw;margin-top: 2vw">
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
                                        <td class="align-middle"><button type="submit" name="removeFood" class="btn btn-danger">Odstranit</button></td>
                                        <input type="hidden" name="foodID" value="<?php echo $product->foodId;?>">
                                    </form>
                                </tr>
                                <?php endforeach;?>
                            </tbody>
                        </table>
                    </td>
                        
                    <td class="align-top" style="position: fixed">
                        <fieldset style="margin-left: 1vw">
                            <form method="POST" action="" enctype="multipart/form-data">
                            <div class="form-group " style="width: 29vw">
                                <label for="foodName" style="color: #0d0d0d">
                                    Název jídla
                                    <span class="required_elements"> *</span>
                                </label>
                                <input type="text" class="form-control" id="foodName" name="foodName" required/>
                            </div>
                            <div class="form-group " style="width: 29vw">
                                <label for="foodDescription" style="color: #0d0d0d">
                                    Popis jídla
                                    <span class="required_elements"> *</span>
                                </label>
                                <input type="text" class="form-control" id="foodDescription" name="foodDescription" required/>
                            </div>
                            <div class="form-group " style="width: 29vw">
                                <label for="foodType" style="color: #0d0d0d">
                                    Typ
                                    <span class="required_elements"> *</span>
                                </label><br>
                                <!--<input type="text" class="form-control" id="foodType" name="foodType" required/>-->
                                <div class="custom-control custom-radio custom-control-inline">
                                    <input type="radio" class="custom-control-input" name="foodType" id="foodType1" value="Stala" checked>
                                    <label class="custom-control-label" for="foodType1">Stálá</label>
                                </div>
                                <div class="custom-control custom-radio custom-control-inline">
                                    <input type="radio" class="custom-control-input" name="foodType" id="foodType2" value="Denni">
                                    <label class="custom-control-label" for="foodType2">Denní</label>
                                </div>
                                <div class="custom-control custom-radio custom-control-inline">
                                    <input type="radio" class="custom-control-input" name="foodType" id="foodType3" value="Napoj">
                                    <label class="custom-control-label" for="foodType3">Nápoj</label>
                                </div>
                            </div>
                            <div class="form-group " style="width: 29vw">
                                <label for="foodAttributes" style="color: #0d0d0d">
                                    Atributy
                                    <span class="required_elements"> *</span>
                                </label><br>
                                <div class="custom-control custom-checkbox custom-control-inline">
                                    <input type="checkbox" class="custom-control-input" name="foodAttributes[]" id="foodAttributes1" value="1">
                                    <label class="custom-control-label" for="foodAttributes1">Smažené</label>
                                </div>
                                <div class="custom-control custom-checkbox custom-control-inline">
                                    <input type="checkbox" class="custom-control-input" name="foodAttributes[]" id="foodAttributes2" value="2">
                                    <label class="custom-control-label" for="foodAttributes2">RAW</label>
                                </div>
                                <div class="custom-control custom-checkbox custom-control-inline">
                                    <input type="checkbox" class="custom-control-input" name="foodAttributes[]" id="foodAttributes3" value="3">
                                    <label class="custom-control-label" for="foodAttributes3">Bio</label>
                                </div>
                                <div class="custom-control custom-checkbox custom-control-inline">
                                    <input type="checkbox" class="custom-control-input" name="foodAttributes[]" id="foodAttributes4" value="4">
                                    <label class="custom-control-label" for="foodAttributes4">Grilované</label>
                                </div>
                                <div class="custom-control custom-checkbox custom-control-inline">
                                    <input type="checkbox" class="custom-control-input" name="foodAttributes[]" id="foodAttributes5" value="5">
                                    <label class="custom-control-label" for="foodAttributes5">Bezmasé</label>
                                </div>
                            </div>
                            <div class="form-group " style="width: 29vw">
                                <label for="foodWeight" style="color: #0d0d0d">
                                    Váha
                                    <span class="required_elements"> *</span>
                                </label>
                                <input type="text" class="form-control" id="foodWeight" name="foodWeight" required/>
                            </div>
                            <div class="form-group " style="width: 29vw">
                                <label for="foodPrice" style="color: #0d0d0d">
                                    Cena
                                    <span class="required_elements"> *</span>
                                </label>
                                <input type="text" class="form-control" id="foodPrice" name="foodPrice" required/>
                            </div>
                                <label for="obr-lbl" style="color: #0d0d0d">
                                    Obrázek
                                </label><br>
                            <div id="file" class="custom-file" style="width: 29vw;">
                                <label for="foodImage" id="obr-lbl" class="custom-file-label" style="color: #0d0d0d">
                                    Vyberte obrázek
                                </label><br>
                                <input type="file" id="foodImage" class="custom-file-input" name="foodImage"/>
                            </div>
                                <button type="submit" name="newFood" style="width: 29vw;font-weight: bold;margin-bottom: 3vw;margin-top: 2vw" class="btn btn-outline-info">Přidat jídlo</button>
                        </form>
                                
                        </fieldset>
                    </td>
                </tr>
            </tbody>
        </table>
        
        <?php

        if(isset($_POST['removeFood'])) {
            $foodID = $_POST['foodID'];
            $this->db->query("DELETE FROM food WHERE foodId = $foodID");
            echo "<meta http-equiv='refresh' content='0'>";
        }

        if(isset($_POST['newFood'])) {
                $name = $_POST['foodName'];
                $description = $_POST['foodDescription'];
                $type = $_POST['foodType'];
                $weight = $_POST['foodWeight'];
                $price = $_POST['foodPrice'];
                $foodAttributes = $_POST['foodAttributes'];
                if($_FILES['foodImage']['name'] != null) {
                    $image = $_FILES['foodImage']['name'];
                    $target_dir = getcwd() . DIRECTORY_SEPARATOR . "images" . DIRECTORY_SEPARATOR;
                    $target_file = $target_dir . basename($_FILES['foodImage']['name']);
                    $target_file_for_db = "/iis/images/" . basename($_FILES['foodImage']['name']);
                    $tmp_name = $_FILES['foodImage']['tmp_name'];
                    move_uploaded_file($tmp_name, $target_file);
                } else {
                    $target_file_for_db = "/iis/images/bezObrazku.jpg";
                    
                }

                $model = array('foodName' => $name, 'foodDescription' => $description, 'foodType' => $type, 'foodWeight' => $weight, 'foodPrice' => $price, 'establishmentId' => $establishement->establishmentId, 'foodImage' => $target_file_for_db);
                $this->db->insert('food',$model);
                $lastFoodId = $this->db->insert_id();
                foreach($foodAttributes as $attribute) {
                    $model = array('foodHaveAtrributes_atrributeId' => $attribute, 'foodHaveAtrributes_foodId' => $lastFoodId);
                    $this->db->insert('foodhaveatrributes',$model);
                }
                echo "<meta http-equiv='refresh' content='0'>";
            }
        ?>
</fieldset>
</div>

<script>
// Add the following code if you want the name of the file appear on select
$(".custom-file-input").on("change", function() {
  var fileName = $(this).val().split("\\").pop();
  $(this).siblings(".custom-file-label").addClass("selected").html(fileName);
});
</script>
