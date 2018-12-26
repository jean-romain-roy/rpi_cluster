
<?php
$target_dir = "uploads/";
$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
$uploadOk = 1;
$imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));

// Error Handling
ini_set('display_errors',1);
error_reporting(E_ALL);

echo "<br>";
if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
	echo "The file ". basename( $_FILES["fileToUpload"]["name"]). " has been uploaded.";
}else{
	echo "Sorry, there was an error uploading your file.";
}


echo "<br><br>";
echo "<h4>Here are our files:</h4>";
$path = "uploads/";
$dh = opendir($path);
$i=1;
while (($file = readdir($dh)) !== false) {
    if($file != "." && $file != ".."){
	echo "$i: <a href='$path/$file'>$file </a> <br>";
        $i++;
    }
}
closedir($dh);

?>

