<!DOCTYPE html>
<?php
 //a3353084@trbvm.com
?>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
	<link rel="icon" href="fonts/favicon.ico">
	<link rel="stylesheet" type="text/css" href="style.css">
	
	
    <title>TheGame</title>

  </head>
  
  <body>
   <script> 
     function flashtext() {
    var tmpColCheck = document.getElementById( "title_text" ).style.color;
	alert(tmpColCheck===rgb(255,0,0));
      if (tmpColCheck === '#ff0000') {
        document.getElementById( "title_text"  ).style.color = '#00ff00';
      } else {
        document.getElementById( "title_text" ).style.color = '#ff0000';
      }
    } 

    setInterval(function() {
        flashtext();
    }, 200 );
 </script>
  <center>
  <br />
    <h1 style="color: #ff0000;" id="title_text">TheGame bug & request submission</h1> 
	<div id="main" align="right">
	<form id="form1" method="post" action="index.php">
		<textarea name="submission" cols="70" rows="15" id="form1">Enter text here...</textarea><br />
		Name: <input type="text" value="Anonymous"><br />
		<input type="submit" value="Submit">
	</form>
	</div>
	
	
	<?php
 for($i = 0; $i < 100; $i++)
 {
	echo " <br /> ";
 }
?>

  </body>
</html>


