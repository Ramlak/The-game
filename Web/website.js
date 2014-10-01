     function flashtext() {
    var tmpColCheck = document.getElementById( "title_text" ).style.color;

      if (tmpColCheck === '#ff0000') {
        document.getElementById( "title_text"  ).style.color = '#00ff00';
      } else {
        document.getElementById( "title_text" ).style.color = '#ff0000';
      }
    } 

    setInterval(function() {
        flashtext();
    }, 200 );